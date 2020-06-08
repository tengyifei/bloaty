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
#include <string>
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

}  // namespace bloaty_link_map

#endif  // BLOATY_LINK_MAP_H_
