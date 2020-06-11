// Copyright 2020 Google Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "link_map.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_set>

#include "bloaty.h"

ABSL_ATTRIBUTE_NORETURN
static void Throw(const char* str, int line) {
  std::cerr << __FILE__ << ":" << line << ", " << str << std::endl;
  throw bloaty::Error(str, __FILE__, line);
}
#define THROW(msg) Throw(msg, __LINE__)

#define likely(x) __builtin_expect((x), 1)
#define unlikely(x) __builtin_expect((x), 0)

#define GUARD(x) if (unlikely(!(x)))

namespace bloaty_link_map {

namespace {

//     VMA      LMA     Size Align Out     In      Symbol
re2::RE2 header_regex(R"(^\s*VMA\s*LMA\s*Size\s*Align\s*Out\s*In\s*Symbol)");

//     194      194       13     1 .interp
re2::RE2 line_regex(R"(\s*[0-9a-f]+\s+([0-9a-f]+)\s+([0-9a-f]+)\s+(\d+) ( *)(.*))");

// obj/zircon/system/uapp/blobfs/blobfs.main.cc.o:(.rodata.main)
re2::RE2 level2_regex(R"(^(.*):\((.*)\))");

enum class Level { k1 = 1, k2 = 2, k3 = 3 };

struct Token {
  std::string_view line;
  uint64_t address;
  uint64_t size;
  Level level;
  std::optional<uint64_t> span;
  std::string_view tok;
};

std::string_view into_string_view(const re2::StringPiece& s) {
  return std::string_view(s.data(), s.size());
}

re2::RE2 PROMOTED_GLOBAL_NAME_DEMANGLED_PATTERN(R"( \((\.\d+)?\.llvm\.\d+\)$)");
re2::RE2 PROMOTED_GLOBAL_NAME_RAW_PATTERN(R"((\.\d+)?\.llvm\.\d+$)");

std::string StripLlvmPromotedGlobalNames(std::string_view name) {
  auto llvm_pos = name.find(".llvm.");
  if (llvm_pos == std::string_view::npos) {
    return std::string(name);
  }
  if (absl::EndsWith(name, ")")) {
    std::string name_rep(name);
    RE2::Replace(&name_rep, PROMOTED_GLOBAL_NAME_DEMANGLED_PATTERN, "");
    return name_rep;
  }
  std::string name_rep(name);
  RE2::Replace(&name_rep, PROMOTED_GLOBAL_NAME_RAW_PATTERN, "");
  return name_rep;
}

const std::string STRING_LITERAL_NAME = "string literal";

std::string_view NormalizeName(std::string_view name) {
  if (absl::StartsWith(name, ".L.str")) {
    return STRING_LITERAL_NAME;
  }
  if (absl::EndsWith(name, " (.cfi)")) {
    return name.substr(0, name.size() - 7);
  }
  return name;
}

// Decides whether a Level 3 token is an annotation.
//
// Returns:
//   A 2-tuple (is_annotation, next_thumb2_mode):
//     is_annotation: Whether |tok| is an annotation.
//     next_thumb2_mode: New |thumb2_mode| value, or None if keep old value.
std::tuple<bool, std::optional<bool>> ParseArmAnnotations(std::string_view tok) {
  // Annotations for ARM match '$t', '$d.1', but not '$_21::invoke'.
  if (absl::StartsWith(tok, "$") && (tok.size() == 2 || (tok.size() >= 3 && tok[2] == '.'))) {
    if (absl::StartsWith(tok, "$t")) {
      // Is annotation, enter Thumb2 mode.
      return std::tuple<bool, std::optional<bool>>{true, std::optional<bool>{true}};
    }
    if (absl::StartsWith(tok, "$a")) {
      // Is annotation, enter ARM32 mode.
      return std::tuple<bool, std::optional<bool>>{true, std::optional<bool>{false}};
    }
    // Is annotation, keep old |thumb2_mode| value.
    return std::tuple<bool, std::optional<bool>>{true, std::nullopt};
  }
  // Not annotation, keep old |thumb2_mode| value.
  return std::tuple<bool, std::optional<bool>>{false, std::nullopt};
}

void Tokenize(std::istringstream& lines, std::function<void(const Token&)> cb) {
  std::string one_line;
  // A Level 3 symbol can have |size == 0| in some situations (e.g., assembly
  // code symbols). To provided better size estimates in this case, the "span"
  // of a Level 3 symbol is computed as:
  //  (A) The |address| difference compared to the next Level 3 symbol.
  //  (B) If the Level 3 symbol is the last among Level 3 lines nested under a
  //      Level 2 line: The difference between the Level 3 symbol's |address|
  //      and the containing Level 2 line's end address.
  // To handle (A), |lines| is visited using a one-step lookahead, using
  // |sentinel| to handle the last line. To handle (B), |level2_end_address| is
  // computed for each Level 2 line.
  const std::string sentinel = "0 0 0 0 THE_END";
  GUARD(RE2::FullMatch(sentinel, line_regex)) { THROW("sentinel must match regex"); }
  uint64_t level2_end_address = 0;
  bool thumb2_mode = false;

  std::string line;
  std::optional<uint64_t> address = std::nullopt;
  uint64_t size = 0;
  Level level = Level::k1;
  std::string tok;

  auto process_line = [&](const std::string& next_line) {
    re2::StringPiece str_next_address;
    re2::StringPiece str_next_size;
    re2::StringPiece str_next_level;
    re2::StringPiece next_tok;
    GUARD(RE2::FullMatch(next_line, line_regex, &str_next_address, &str_next_size, (void*)NULL,
                         &str_next_level, &next_tok)) {
      return;
    }
    uint64_t next_address = std::stoul(str_next_address.as_string(), nullptr, 16);
    uint64_t next_size = std::stoul(str_next_size.as_string(), nullptr, 16);
    int int_level = (str_next_level.size() / 8) + 1;
    GUARD(int_level >= 1 && int_level <= 3) { THROW("invalid level"); }
    Level next_level = static_cast<Level>(int_level);

    if (next_level == Level::k3) {
      GUARD(level == Level::k2 || level == Level::k3) {
        THROW("Cannot jump from Level 1 to Level l3");
      }
      // Detect annotations. If found, maybe update |thumb2_mode|, then skip.
      auto [is_annotation, next_thumb2_mode] = ParseArmAnnotations(into_string_view(next_tok));
      if (is_annotation) {
        if (next_thumb2_mode.has_value()) {
          thumb2_mode = next_thumb2_mode.value();
        }
        // Skip annotations.
        return;
      }
      if (thumb2_mode) {
        // Adjust odd address to even. Alignment is not guanteed for all
        // symbols (e.g., data, or x86), so this is judiciously applied.
        next_address &= ~(1ULL);
      }
    } else {
      // Resets on leaving Level 3.
      thumb2_mode = false;
    }

    if (address.has_value()) {
      std::optional<uint64_t> span = std::nullopt;
      if (level == Level::k3) {
        span = next_level == Level::k3 ? next_address : level2_end_address;
        *span -= *address;
      } else if (level == Level::k2) {
        level2_end_address = *address + size;
      }
      cb(Token{
          .line = line,
          .address = *address,
          .size = size,
          .level = level,
          .span = span,
          .tok = tok,
      });
    }

    line = next_line;
    address = next_address;
    size = next_size;
    level = next_level;
    tok = next_tok.as_string();
  };
  while (getline(lines, one_line)) {
    process_line(one_line);
  }
  process_line(sentinel);
}

const std::string SECTION_BSS = ".bss";
const std::string SECTION_BSS_REL_RO = ".bss.rel.ro";
const std::string SECTION_DATA = ".data";
const std::string SECTION_DATA_REL_RO = ".data.rel.ro";
const std::string SECTION_DATA_REL_RO_LOCAL = ".data.rel.ro.local";
const std::string SECTION_DEX = ".dex";
const std::string SECTION_DEX_METHOD = ".dex.method";
const std::string SECTION_OTHER = ".other";
const std::string SECTION_PAK_NONTRANSLATED = ".pak.nontranslated";
const std::string SECTION_PAK_TRANSLATIONS = ".pak.translations";
const std::string SECTION_PART_END = ".part.end";
const std::string SECTION_RODATA = ".rodata";
const std::string SECTION_TEXT = ".text";
// Used by SymbolGroup when they contain a mix of sections.
const std::string SECTION_MULTIPLE = ".*";

const std::unordered_set<std::string> BSS_SECTIONS = {
    SECTION_BSS,
    SECTION_BSS_REL_RO,
    SECTION_PART_END,
};

}  // namespace

std::vector<Symbol> ParseLldLinkMap(const std::string& content) {
  std::vector<Symbol> syms;
  std::istringstream lines(content);
  std::string one_line;

  GUARD(getline(lines, one_line)) { THROW("must have at least the header line"); }
  GUARD(header_regex.ok()) { THROW("can't compile header regex"); }
  GUARD(RE2::FullMatch(one_line, header_regex)) { THROW("link map is not in lld v1 format"); }

  // Example Format:
  //    VMA      LMA     Size Align Out     In      Symbol
  //    194      194       13     1 .interp
  //    194      194       13     1         <internal>:(.interp)
  //    1a8      1a8     22d8     4 .ARM.exidx
  //    1b0      1b0        8     4         obj/sandbox/syscall.o:(.ARM.exidx)
  //    400      400   123400    64 .text
  //    600      600       14     4         ...:(.text.OUTLINED_FUNCTION_0)
  //    600      600        0     1                 $x.3
  //    600      600       14     1                 OUTLINED_FUNCTION_0
  // 123800   123800    20000   256 .rodata
  // 123800   123800       4      4         ...:o:(.rodata._ZN3fooE.llvm.1234)
  // 123800   123800       4      1                 foo (.llvm.1234)
  // 123804   123804       4      4         ...:o:(.rodata.bar.llvm.1234)
  // 123804   123804       4      1                 bar.llvm.1234
  //                                 ^       ^         ^
  //                               Level 1   ^         ^
  //                                       Level 2     ^
  //                                                  Level 3
  //
  // Level 1 (Out) specify sections or PROVIDE_HIDDEN lines.
  // Level 2 (In) specify object paths and section names within objects, or '<internal>:...'.
  // Level 3 (Symbol) specify symbol names or special names such as
  // '.L_MergeGlobals'. Annotations such as '$d', $t.42' also appear at Level 3,
  // but they are consumed by |tokenizer|, so don't appear hear.
  GUARD(line_regex.ok()) { THROW("can't compile line regex"); }
  GUARD(level2_regex.ok()) { THROW("can't compile level2 regex"); }

  std::optional<std::string> cur_section;

  bool cur_section_is_useful = false;

  uint64_t promoted_name_count = 0;

  // |is_partial| indicates that an eligible Level 3 line should be used to
  // update |syms[-1].full_name| instead of creating a new symbol.
  bool is_partial = false;

  // Assembly code can create consecutive Level 3 lines with |size == 0|. These
  // lines can represent
  //  (1) assembly functions (should form symbol), or
  //  (2) assembly labels (should NOT form symbol).
  // It seems (2) correlates with the presence of a leading Level 3 line with
  // |size > 0|. This gives rise to the following strategy: Each symbol S from
  // a Level 3 line suppresses Level 3 lines with |address| less than
  // |next_usable_address := S.address + S.size|.
  uint64_t next_usable_address = 0;

  bool in_partitions = false;
  bool in_jump_table = false;
  uint64_t jump_tables_count = 0;
  uint64_t jump_entries_count = 0;
  uint64_t mangled_start_idx = 0;

  std::string cur_obj;
  // Assembly code can create consecutive Level 3 lines with |size == 0|. These
  // lines can represent
  //  (1) assembly functions (should form symbol), or
  //  (2) assembly labels (should NOT form symbol).
  // It seems (2) correlates with the presence of a leading Level 3 line with
  // |size > 0|. This gives rise to the following strategy: Each symbol S from
  // a Level 3 line suppresses Level 3 lines with |address| less than
  // |next_usable_address := S.address + S.size|.
  Tokenize(lines, [&](const Token& token) {
    // Level 1 data match the "Out" column. They specify sections or
    // PROVIDE_HIDDEN lines.
    if (token.level == Level::k1) {
      // Ignore sections that belong to feature library partitions. Seeing a
      // partition name is an indicator that we've entered a list of feature
      // partitions. After these, a single .part.end section will follow to
      // reserve memory at runtime. Seeing the .part.end section also marks the
      // end of partition sections in the map file.
      if (absl::EndsWith(token.tok, "_partition")) {
        in_partitions = true;
      } else if (token.tok == ".part.end") {
        // Note that we want to retain .part.end section, so it's fine to
        // restart processing on this section, rather than the next one.
        in_partitions = false;
      }

      if (in_partitions) {
        // For now, completely ignore feature partitions.
        cur_section = std::nullopt;
        cur_section_is_useful = false;
      } else {
        cur_section = token.tok;
        // E.g., Want to convert "(.text._name)" -> "_name" later.
        mangled_start_idx = cur_section->size() + 1;
        cur_section_is_useful = ((BSS_SECTIONS.find(*cur_section) != BSS_SECTIONS.end()) ||
                                 SECTION_RODATA == *cur_section || SECTION_TEXT == *cur_section ||
                                 absl::StartsWith(*cur_section, SECTION_DATA));
      }
    } else if (cur_section_is_useful) {
      // Level 2 data match the "In" column. They specify object paths and
      // section names within objects, or '<internal>:...'.
      if (token.level == Level::k2) {
        std::string mangled_name;
        re2::StringPiece paren_value;
        GUARD(RE2::FullMatch(token.tok, level2_regex, &cur_obj, &paren_value)) {
          THROW("Level 2 regex did not match");
        }
        bool in_jump_table = absl::StrContains(into_string_view(paren_value), ".L.cfi.jumptable");
        if (in_jump_table) {
          // Store each CFI jump table as a Level 2 symbol, whose Level 3
          // details are discarded.
          jump_tables_count++;
          // Replaces 'lto.tmp' to prevent problem later.
          cur_obj = "";
          mangled_name = "** CFI jump table";
        } else {
          // E.g., '(.text.unlikely._name)' -> '_name'.
          mangled_name = into_string_view(paren_value.substr(mangled_start_idx));
          is_partial = true;
          // As of 2017/11 LLD does not distinguish merged strings from other
          // merged data. Feature request is filed under:
          // https://bugs.llvm.org/show_bug.cgi?id=35248
          if (cur_obj == "<internal>") {
            if (cur_section.has_value() && *cur_section == ".rodata" && mangled_name == "") {
              // Treat all <internal> sections within .rodata as as string
              // literals. Some may hold numeric constants or other data, but
              // there is currently no way to distinguish them.
              mangled_name = "** lld merge strings";
            } else {
              // e.g. <internal>:(.text.thunk)
              mangled_name = "** " + mangled_name;
            }

            is_partial = false;
            cur_obj = "";
          } else if (cur_obj == "lto.tmp" ||
                     absl::StrContains(into_string_view(cur_obj), "thinlto-cache")) {
            cur_obj = "";
          }
        }

        // Create a symbol here since there may be no ensuing Level 3 lines.
        // But if there are, then the symbol can be modified later as sym[-1].
        syms.push_back(Symbol{
            .name = std::move(mangled_name),
            .compile_unit = cur_obj,
            .section = *cur_section,
            .addr = token.address,
            .size = token.size,
        });
        // Level 3 |address| is nested under Level 2, don't add |size|.
        next_usable_address = token.address;
      } else if (token.level == Level::k3) {
        // Level 3 data match the "Symbol" column. They specify symbol names or
        // special names such as '.L_MergeGlobals'. Annotations such as '$d',
        // '$t.42' also appear at Level 3, but they are consumed by |tokenizer|,
        // so don't appear hear.

        // Handle .L.cfi.jumptable.
        if (in_jump_table) {
          // Level 3 entries in CFI jump tables are thunks with mangled names.
          // Extracting them as symbols is not worthwhile; we only store the
          // Level 2 symbol, and print the count for verbose output. For
          // counting, '__typeid_' entries are excluded since they're likely
          // just annotations.
          if (!absl::StartsWith(token.tok, "__typeid_")) {
            jump_entries_count++;
          }
          return;
        }

        // Ignore anything with '.L_MergedGlobals' prefix. This seems to only
        // happen for ARM (32-bit) builds.
        if (absl::StartsWith(token.tok, ".L_MergedGlobals")) {
          return;
        }

        // Use |span| to decide whether to use a Level 3 line for Symbols. This
        // is useful for two purposes:
        // * This is a better indicator than |size|, which can be 0 for
        //   assembly functions.
        // * If multiple Level 3 lines have the same starting address, this
        //   cause all but the last line to have |span > 0|. This dedups lines
        //   with identical symbol names (why do they exist?). Note that this
        //   also skips legitimate aliases, but that's desired because nm.py
        //   (downstream) assumes no aliases already exist.
        if (*token.span > 0) {
          std::string stripped_tok = StripLlvmPromotedGlobalNames(token.tok);
          std::string tok;
          if (token.tok.size() != stripped_tok.size()) {
            promoted_name_count++;
            tok = stripped_tok;
          } else {
            tok = token.tok;
          }
          tok = NormalizeName(tok);

          // Handle special case where a partial symbol consumes bytes before
          // the first Level 3 symbol.
          if (is_partial && syms.back().addr < token.address) {
            // Truncate the partial symbol and leave it without |full_name|.
            // The data from the current line will form a new symbol.
            syms.back().size = token.address - syms.back().addr;
            next_usable_address = token.address;
            is_partial = false;
          }

          if (is_partial) {
            syms.back().name = tok;
            syms.back().size =
                token.size > 0 ? token.size : std::min(syms.back().size, *token.span);
            next_usable_address = token.address + syms.back().size;
            is_partial = false;
          } else if (token.address >= next_usable_address) {
            uint64_t size_to_use;
            if (absl::StartsWith(tok, "__typeid_")) {
              GUARD(token.size == 1) { THROW("Token size"); }
              if (absl::EndsWith(tok, "_byte_array")) {
                // CFI byte array table: |size| is inaccurate, so use |span|.
                size_to_use = *token.span;
              } else {
                // Likely '_global_addr' or '_unique_member'. These should be:
                // * Skipped since they're in CFI tables.
                // * Suppressed (via |next_usable_address|) by another Level 3
                //   symbol.
                // Anything that makes it here would be an anomaly worthy of
                // investigation, so print warnings.
                std::cerr << "Unrecognized __typeid_ symbol at " << token.address << std::endl;
                return;
              }
            } else {
              size_to_use = token.size > 0 ? token.size : *token.span;
            }
            syms.push_back(Symbol{
                .name = std::move(tok),
                .compile_unit = cur_obj,
                .section = *cur_section,
                .addr = token.address,
                .size = size_to_use,
            });

            // Suppress symbols with overlapping |address|. This eliminates
            // labels from assembly sources.
            next_usable_address = token.address + size_to_use;
            if (!cur_obj.empty()) {
              syms.back().compile_unit = cur_obj;
            }
          }
        }
      }
    }
  });

  if (promoted_name_count > 0) {
    std::cerr << "Found " << promoted_name_count << " promoted global names" << std::endl;
  }
  if (jump_tables_count > 0) {
    std::cerr << "Found " << jump_tables_count << " CFI jump tables with " << jump_entries_count
              << "total entries" << std::endl;
  }

  return syms;
}

std::vector<Section> ParseLldLinkMapSections(const std::string& content) {
  std::vector<Section> sections;

  std::istringstream lines(content);
  std::string one_line;

  Tokenize(lines, [&](const Token& token) {
    // Level 1 data match the "Out" column. They specify sections or
    // PROVIDE_HIDDEN lines.
    if (token.level == Level::k1) {
      sections.push_back(Section{
          .name = std::string(token.tok),
          .addr = token.address,
          .size = token.size,
      });
    }
  });

  return sections;
}

namespace {

// ./exe.unstripped/component_manager.alloc-54127f36ba192482.alloc.4k1iwrm2-cgu.0.rcgu.o.rcgu.o
re2::RE2 library_crate_regex(
    R"(\/[a-zA-Z0-9_]+\.[a-zA-Z0-9_-]+\.([a-zA-Z0-9_]+)\.[a-zA-Z0-9-]+.*\.rcgu\.o$)");

// ./exe.unstripped/component_manager.component_manager.7rcbfp3g-cgu.0.rcgu.o
re2::RE2 bin_crate_regex(R"(\/[a-zA-Z0-9_-]+\.([a-zA-Z0-9_]+)\.[a-zA-Z0-9-]+.*\.rcgu\.o$)");

// foobar.rlib(libregex_syntax-579ced0738b0164d-579ced0738b0164d.regex_syntax.c02sfxfu-cgu.13.rcgu.o)
re2::RE2 rlib_crate_regex(R"(rlib\([a-zA-Z_\-0-9]+\.([a-zA-Z0-9_]+)\.[a-zA-Z0-9-]+.*\.rcgu\.o\)$)");

// /usr/local/google/home/yifeit/vg/out/default.zircon/user-arm64-clang.shlib/obj/system/ulib/c/crt1.Scrt1.cc.o
re2::RE2 zircon_lib_regex(R"(\/out\/[a-zA-Z0-9_-]+\.zircon\/.*\/obj\/system\/ulib\/(.*)\.o$)");

// obj/out/default/fidling/gen/sdk/fidl/fuchsia.hardware.block/fuchsia.hardware.block_tables.fuchsia.hardware.block.fidl.tables.c.o
re2::RE2 fidling_regex(R"(^obj\/out\/.*\/fidling\/gen\/(.*)\.o$)");

// obj/zircon/public/lib/fidl_base/libfidl_base.a(libfidl_base.decoding.cc.o)
re2::RE2 zircon_fidl_lib_regex(
    R"(^obj\/zircon\/public\/lib\/fidl_base\/libfidl_base\.a\(libfidl_base\.(.*)\.cc\.o\)$)");

// obj/zircon/system/uapp/blobfs/blobfs.main.cc.o
re2::RE2 zircon_lib_regex2(R"(^obj\/zircon\/system\/(.*)\.o$)");

}  // namespace

std::optional<std::tuple<std::string, std::optional<std::string>>> TransformCompileUnitForFuchsia(
    const std::string& compile_unit) {
  GUARD(library_crate_regex.ok()) { THROW("can't compile library crate regex"); }
  {
    std::string crate_name;
    if (RE2::PartialMatch(compile_unit, library_crate_regex, &crate_name)) {
      return std::tuple{"[crate: " + crate_name + "]", crate_name};
    }
  }

  GUARD(bin_crate_regex.ok()) { THROW("can't compile bin crate regex"); }
  {
    std::string crate_name;
    if (RE2::PartialMatch(compile_unit, bin_crate_regex, &crate_name)) {
      return std::tuple{"[crate: " + crate_name + "]", crate_name};
    }
  }

  GUARD(rlib_crate_regex.ok()) { THROW("can't compile rlib crate regex"); }
  {
    std::string crate_name;
    if (RE2::PartialMatch(compile_unit, rlib_crate_regex, &crate_name)) {
      return std::tuple{"[crate: " + crate_name + "]", crate_name};
    }
  }

  GUARD(zircon_lib_regex.ok()) { THROW("can't compile zircon lib regex"); }
  {
    std::string cc_path;
    if (RE2::PartialMatch(compile_unit, zircon_lib_regex, &cc_path)) {
      // Remove the prefix in last path component
      // c/crt1.Scrt1.cc -> c/Scrt1.cc
      static re2::RE2 prefix_regex(R"(\/[a-zA-Z0-9\-_]+\.([a-zA-Z0-9\-_]+\.(cc|c))$)");
      if (RE2::Replace(&cc_path, prefix_regex, R"(/\1)")) {
        return std::tuple{"../../zircon/system/ulib/" + cc_path, std::nullopt};
      }
    }
  }

  GUARD(fidling_regex.ok()) { THROW("can't compile fidling regex"); }
  {
    std::string cc_path;
    if (RE2::PartialMatch(compile_unit, fidling_regex, &cc_path)) {
      // A: sdk/fidl/fuchsia.hardware.block/fuchsia.hardware.block_tables.fuchsia.hardware.block.fidl.tables.c
      // B: sdk/fidl/fuchsia.hardware.block/fuchsia.hardware.block.fidl.tables.c
      // A: sdk/fidl/fuchsia.hardware.block/fuchsia/hardware/block/c/fuchsia.hardware.block_c_client.fidl.client.c
      // B: sdk/fidl/fuchsia.hardware.block/fuchsia/hardware/block/c/fidl.client.c
      // A: sdk/fidl/fuchsia.io/fuchsia/io/llcpp/fuchsia.io_llcpp.fidl.cc
      // B: sdk/fidl/fuchsia.io/fuchsia/io/llcpp/fidl.cc
      static re2::RE2 prefix_regex(
          R"([a-zA-Z0-9\-\.]+_[a-zA-Z0-9\-_]+\.([a-zA-Z0-9\-\.]+\.(c|cc))$)");
      if (RE2::Replace(&cc_path, prefix_regex, R"(\1)")) {
        return std::tuple{"fidling/gen/" + cc_path, std::nullopt};
      }
    }
  }

  GUARD(zircon_fidl_lib_regex.ok()) { THROW("can't compile zircon fidl lib regex"); }
  {
    std::string cc_name;
    if (RE2::PartialMatch(compile_unit, zircon_fidl_lib_regex, &cc_name)) {
      return std::tuple{"../../zircon/system/ulib/fidl/" + cc_name + ".cc", std::nullopt};
    }
  }

  // Rust-specific special casing...
  if (absl::StartsWith(compile_unit, "obj/third_party/rust_crates/compat/ring/libring-core.a")) {
    return std::tuple{"[crate: ring]", "ring"};
  }

  return std::nullopt;
}

}  // namespace bloaty_link_map
