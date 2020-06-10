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

#ifndef BLOATY_LINK_MAP_H_
#define BLOATY_LINK_MAP_H_

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <tuple>
#include <vector>

namespace bloaty_link_map {

struct Symbol {
  std::string name;
  std::string compile_unit;
  std::string section;
  uint64_t addr;
  uint64_t size;
};

// Parses a linker map file in lld v1 format.
//
// This function is adapted from
// https://chromium.googlesource.com/chromium/src.git/+/master/tools/binary_size/libsupersize/linker_map_parser.py
//
// Args:
//     content: Contents of the link map, the first line of which is the header.
//
// Returns:
//     A vector of symbols.
//
// Example format:
//     VMA      LMA     Size Align Out     In      Symbol
//     194      194       13     1 .interp
//     194      194       13     1         <internal>:(.interp)
//     1a8      1a8     22d8     4 .ARM.exidx
//     1b0      1b0        8     4         obj/sandbox/syscall.o:(.ARM.exidx)
//     400      400   123400    64 .text
//     600      600       14     4         ...:(.text.OUTLINED_FUNCTION_0)
//     600      600        0     1                 $x.3
//     600      600       14     1                 OUTLINED_FUNCTION_0
//  123800   123800    20000   256 .rodata
//  123800   123800       4      4         ...:o:(.rodata._ZN3fooE.llvm.1234)
//  123800   123800       4      1                 foo (.llvm.1234)
//  123804   123804       4      4         ...:o:(.rodata.bar.llvm.1234)
//  123804   123804       4      1                 bar.llvm.1234
//
// Notes:
// - `VMA` and `LMA` are mostly identical.
// - In a position-independent executable, the base address is zero.
//   Hence the minimum value of the addresses is quite small.
// - Stripping of the binary will not relocate symbols, hence the addresses
//   will be the same between a stripped and unstripped binary.
std::vector<Symbol> ParseLldLinkMap(const std::string& content);

struct Section {
  std::string name;
  uint64_t addr;
  uint64_t size;
};

std::vector<Section> ParseLldLinkMapSections(const std::string& content);

// Transform the compile unit path parsed from link maps to a format
// easier to organize, specialized to a Fuchsia build.
// If the compile unit could not be understood by the transformer, it will
// return `std::nullopt`. Since bloaty can still recover some compile unit
// information from DWARF, we do not have to cover 100% here. For Rust however,
// this allows setting a reasonable fallback compile unit at the crate level.
// Therefore, the output of this transform should be inserted after the
// usual DWARF-based compile unit data source.
//
// Examples:
//
// `./exe.unstripped/component_manager.alloc-54127f36ba192482.alloc.4k1iwrm2-cgu.0.rcgu.o.rcgu.o`
// becomes `[crate: alloc]`.
//
// `./exe.unstripped/component_manager.libcomponent_manager_lib.component_manager_lib.3a1fbbbh-cgu.2.rcgu.o.rcgu.o`
// becomes `[crate: component_manager_lib]`.
//
// `./exe.unstripped/component_manager.libcm_fidl_translator.cm_fidl_translator.3a1fbbbh-cgu.0.rcgu.o.rcgu.o`
// becomes `[crate: cm_fidl_translator]`.
//
// `./exe.unstripped/component_manager.component_manager.7rcbfp3g-cgu.0.rcgu.o`
// becomes `[crate: component_manager]`.
//
// `./exe.unstripped/component_manager.libfidl_fuchsia_io.fidl_fuchsia_io.3a1fbbbh-cgu.0.rcgu.o.rcgu.o`
// becomes `[crate: fidl_fuchsia_io]`.
//
// `/usr/local/google/home/yifeit/vg/out/default/obj/third_party/rust_crates/libregex_syntax-579ced0738b0164d.rlib(libregex_syntax-579ced0738b0164d-579ced0738b0164d.regex_syntax.c02sfxfu-cgu.13.rcgu.o)`
// becomes `[crate: regex_syntax]`.
//
// `/usr/local/google/home/yifeit/vg/out/default.zircon/user-arm64-clang.shlib/obj/system/ulib/c/crt1.Scrt1.cc.o`
// becomes `../../zircon/system/ulib/c/Scrt1.cc`.
//
// `obj/out/default/fidling/gen/sdk/fidl/fuchsia.hardware.block/fuchsia.hardware.block_tables.fuchsia.hardware.block.fidl.tables.c.o`
// becomes `fidling/gen/sdk/fidl/fuchsia.hardware.block/fuchsia.hardware.block.fidl.tables.c`.
//
// `obj/zircon/public/lib/fidl_base/libfidl_base.a(libfidl_base.decoding.cc.o)`
// becomes `../../zircon/system/ulib/fidl/decoding.cc`
//
// `obj/zircon/system/uapp/blobfs/blobfs.main.cc.o`
// becomes `../../zircon/system/uapp/blobfs/main.cc`
//
// `obj/zircon/system/ulib/trace-provider/libtrace-provider-with-fdio.a(libtrace-provider-with-fdio.fdio_connect.cc.o)`
// is not suppported.
//
// `obj/zircon/public/lib/lz4/liblz4.a(liblz4.lz4hc.c.o)` is not supported.
std::optional<std::tuple<std::string, std::optional<std::string>>> TransformCompileUnitForFuchsia(
    const std::string& compile_unit);

}  // namespace bloaty_link_map

#endif  // BLOATY_LINK_MAP_H_
