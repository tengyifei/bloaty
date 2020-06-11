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

#include <fstream>
#include <tuple>

#include "bloaty.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

TEST(TransformCompileUnitForFuchsiaTest, RustLibraryCrate1) {
  auto result = bloaty_link_map::TransformCompileUnitForFuchsia(
      "./exe.unstripped/"
      "component_manager.alloc-54127f36ba192482.alloc.4k1iwrm2-cgu.0.rcgu.o.rcgu.o");
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(std::get<0>(*result), "[crate: alloc]");
  ASSERT_EQ(*std::get<1>(*result), "alloc");
}

TEST(TransformCompileUnitForFuchsiaTest, RustLibraryCrate2) {
  auto result = bloaty_link_map::TransformCompileUnitForFuchsia(
      "./exe.unstripped/"
      "component_manager.libcomponent_manager_lib.component_manager_lib.3a1fbbbh-cgu.2.rcgu.o.rcgu."
      "o");
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(std::get<0>(*result), "[crate: component_manager_lib]");
  ASSERT_EQ(*std::get<1>(*result), "component_manager_lib");
}

TEST(TransformCompileUnitForFuchsiaTest, RustLibraryCrate3) {
  auto result = bloaty_link_map::TransformCompileUnitForFuchsia(
      "./exe.unstripped/"
      "component_manager.libcm_fidl_translator.cm_fidl_translator.3a1fbbbh-cgu.0.rcgu.o.rcgu.o");
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(std::get<0>(*result), "[crate: cm_fidl_translator]");
  ASSERT_EQ(*std::get<1>(*result), "cm_fidl_translator");
}

TEST(TransformCompileUnitForFuchsiaTest, RustLibraryCrate4) {
  auto result = bloaty_link_map::TransformCompileUnitForFuchsia(
      "./exe.unstripped/"
      "component_manager.libfidl_fuchsia_io.fidl_fuchsia_io.3a1fbbbh-cgu.0.rcgu.o.rcgu.o");
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(std::get<0>(*result), "[crate: fidl_fuchsia_io]");
  ASSERT_EQ(*std::get<1>(*result), "fidl_fuchsia_io");
}

TEST(TransformCompileUnitForFuchsiaTest, RustBinaryCrate) {
  auto result = bloaty_link_map::TransformCompileUnitForFuchsia(
      "./exe.unstripped/component_manager.component_manager.7rcbfp3g-cgu.0.rcgu.o");
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(std::get<0>(*result), "[crate: component_manager]");
  ASSERT_EQ(*std::get<1>(*result), "component_manager");
}

TEST(TransformCompileUnitForFuchsiaTest, RustRLibCrate) {
  auto result = bloaty_link_map::TransformCompileUnitForFuchsia(
      "/usr/local/google/home/yifeit/vg/out/default/obj/third_party/rust_crates/"
      "libregex_syntax-579ced0738b0164d.rlib(libregex_syntax-579ced0738b0164d-579ced0738b0164d."
      "regex_syntax.c02sfxfu-cgu.13.rcgu.o)");
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(std::get<0>(*result), "[crate: regex_syntax]");
  ASSERT_EQ(*std::get<1>(*result), "regex_syntax");
}

TEST(TransformCompileUnitForFuchsiaTest, ZirconLib) {
  auto result = bloaty_link_map::TransformCompileUnitForFuchsia(
      "/usr/local/google/home/yifeit/vg/out/default.zircon/user-arm64-clang.shlib/obj/system/ulib/"
      "c/crt1.Scrt1.cc.o");
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(std::get<0>(*result), "../../zircon/system/ulib/c/Scrt1.cc");
  ASSERT_EQ(std::get<1>(*result), std::nullopt);
}

TEST(TransformCompileUnitForFuchsiaTest, Fidling1) {
  auto result = bloaty_link_map::TransformCompileUnitForFuchsia(
      "obj/out/default/fidling/gen/sdk/fidl/fuchsia.hardware.block/"
      "fuchsia.hardware.block_tables.fuchsia.hardware.block.fidl.tables.c.o");
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(std::get<0>(*result),
            "fidling/gen/sdk/fidl/fuchsia.hardware.block/fuchsia.hardware.block.fidl.tables.c");
  ASSERT_EQ(std::get<1>(*result), std::nullopt);
}

TEST(TransformCompileUnitForFuchsiaTest, Fidling2) {
  auto result = bloaty_link_map::TransformCompileUnitForFuchsia(
      "obj/out/default/fidling/gen/sdk/fidl/fuchsia.hardware.block/fuchsia/hardware/block/c/"
      "fuchsia.hardware.block_c_client.fidl.client.c.o");
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(std::get<0>(*result),
            "fidling/gen/sdk/fidl/fuchsia.hardware.block/fuchsia/hardware/block/c/fidl.client.c");
  ASSERT_EQ(std::get<1>(*result), std::nullopt);
}

TEST(TransformCompileUnitForFuchsiaTest, Fidling3) {
  auto result = bloaty_link_map::TransformCompileUnitForFuchsia(
      "obj/out/default/fidling/gen/sdk/fidl/fuchsia.security.resource/fuchsia/security/resource/"
      "llcpp/fuchsia.security.resource_llcpp.fidl.cc.o");
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(
      std::get<0>(*result),
      "fidling/gen/sdk/fidl/fuchsia.security.resource/fuchsia/security/resource/llcpp/fidl.cc");
  ASSERT_EQ(std::get<1>(*result), std::nullopt);
}

TEST(TransformCompileUnitForFuchsiaTest, ZirconFidlLib) {
  auto result = bloaty_link_map::TransformCompileUnitForFuchsia(
      "obj/zircon/public/lib/fidl_base/libfidl_base.a(libfidl_base.decoding.cc.o)");
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(std::get<0>(*result), "../../zircon/system/ulib/fidl/decoding.cc");
  ASSERT_EQ(std::get<1>(*result), std::nullopt);
}

TEST(TransformCompileUnitForFuchsiaTest, PassthroughUnknown) {
  auto result = bloaty_link_map::TransformCompileUnitForFuchsia("foobar");
  ASSERT_FALSE(result.has_value());
}

std::string LoadLinkMapFile(const std::string& name) {
  std::ifstream infile(name);
  std::string link_map;

  // Strip comments and empty lines.
  for (std::string line; getline(infile, line);) {
    if (line.empty()) continue;
    if (line[0] == '#') continue;
    link_map += line;
    link_map += '\n';
  }

  absl::StripLeadingAsciiWhitespace(&link_map);
  absl::StripTrailingAsciiWhitespace(&link_map);

  return link_map;
}

TEST(LinkMapTest, Empty) {
  const std::string link_map = "     VMA      LMA     Size Align Out     In      Symbol\n";
  auto symbols = bloaty_link_map::ParseLldLinkMap(link_map);
  ASSERT_EQ(symbols.size(), 0);
}

TEST(LinkMapTest, ExampleCpp) {
  std::string link_map = LoadLinkMapFile("example_cpp.map");
  absl::StripLeadingAsciiWhitespace(&link_map);
  absl::StripTrailingAsciiWhitespace(&link_map);

  auto symbols = bloaty_link_map::ParseLldLinkMap(link_map);

  struct Golden {
    uint64_t addr;
    uint64_t size;
    std::string name;
    std::string compile_unit;
    std::string section;
  };
  std::vector<Golden> goldens = {
      {0x4380, 0x14, "main", "obj/zircon/system/uapp/blobfs/blobfs.main.cc.o", ".rodata"},
      {0x4394, 0x8a18, "** lld merge strings", "", ".rodata"},
      {0xcdb0, 0xa8, "** lld merge strings", "", ".rodata"},
      {0xce58, 0xc, "HandlefifonullableTable",
       "obj/out/default/fidling/gen/sdk/fidl/fuchsia.hardware.block/"
       "fuchsia.hardware.block_tables.fuchsia.hardware.block.fidl.tables.c.o",
       ".rodata"},
      {0xce64, 0xc, "HandlevmononnullableTable",
       "obj/out/default/fidling/gen/sdk/fidl/fuchsia.hardware.block/"
       "fuchsia.hardware.block_tables.fuchsia.hardware.block.fidl.tables.c.o",
       ".rodata"},
      {0xce70, 0xc, "HandleresourcenonnullableTable",
       "obj/out/default/fidling/gen/sdk/fidl/fuchsia.security.resource/"
       "fuchsia.security.resource_tables.fuchsia.security.resource.fidl.tables.c.o",
       ".rodata"},
      {0xce7c, 0xa, "_ZN5llcpp7fuchsia2io8NodeInfo9reset_ptrEON4fidl12tracking_ptrIvEE",
       "obj/out/default/fidling/gen/sdk/fidl/fuchsia.io/fuchsia/io/llcpp/"
       "fuchsia.io_llcpp.fidl.cc.o",
       ".rodata"},
      {0xce86, 0x2b, "fidl::kErrorWriteFailed",
       "obj/out/default/fidling/gen/sdk/fidl/fuchsia.io/fuchsia/io/llcpp/"
       "fuchsia.io_llcpp.fidl.cc.o",
       ".rodata"},
      {0xceb4, 0xc, "Request15fuchsia_io_NodenonnullableTable",
       "obj/out/default/fidling/gen/sdk/fidl/fuchsia.io/"
       "fuchsia.io_tables.fuchsia.io.fidl.tables.c.o",
       ".rodata"},
      {0xcec0, 0x8, "String4096nonnullableTable",
       "obj/out/default/fidling/gen/sdk/fidl/fuchsia.io/"
       "fuchsia.io_tables.fuchsia.io.fidl.tables.c.o",
       ".rodata"},
      {0xd030, 0x100, "** lld merge strings", "", ".rodata"},
      {0xd130, 0xc, "HandleeventnonnullableTable",
       "obj/out/default/fidling/gen/sdk/fidl/fuchsia.fs/"
       "fuchsia.fs_tables.fuchsia.fs.fidl.tables.c.o",
       ".rodata"},
      {0xd13c, 0x8, "String32nonnullableTable",
       "obj/out/default/fidling/gen/sdk/fidl/fuchsia.fs/"
       "fuchsia.fs_tables.fuchsia.fs.fidl.tables.c.o",
       ".rodata"},
      {0xfb38, 0x9c, "LZ4HC_compress_generic_internal.clTable",
       "obj/zircon/public/lib/lz4/liblz4.a(liblz4.lz4hc.c.o)", ".rodata"},
      {0xfbd4, 0x10, "XXH32_finalize", "obj/zircon/public/lib/lz4/liblz4.a(liblz4.xxhash.c.o)",
       ".rodata"},
      {0xfbe4, 0x27, "kServicePath",
       "obj/zircon/system/ulib/trace-provider/"
       "libtrace-provider-with-fdio.a(libtrace-provider-with-fdio.fdio_connect.cc.o)",
       ".rodata"},
      {0x21000, 0x14, "_start",
       "/usr/local/google/home/yifeit/vg/out/default.zircon/user-arm64-clang.shlib/obj/system/ulib/"
       "c/crt1.Scrt1.cc.o",
       ".text"},
      {0x21014, 0x494, "main", "obj/zircon/system/uapp/blobfs/blobfs.main.cc.o", ".text"},
      {0x3a04c, 0x8, "non-virtual thunk to blobfs::TransactionManager::~TransactionManager()",
       "obj/zircon/system/ulib/blobfs/libblobfs.a(libblobfs.blobfs.cc.o)", ".text"},
      {0x108120, 0xdc, "trace::internal::Session::~Session()",
       "obj/zircon/system/ulib/trace-provider/"
       "libtrace-provider-with-fdio.a(libtrace-provider-with-fdio.session.cc.o)",
       ".text"},

  };

  // for (const auto& sym : symbols) {
  //   std::cout << std::hex << "{0x" << sym.addr << ", 0x" << sym.size << ", \"" << sym.name
  //             << "\", \"" << sym.compile_unit << "\", \"" << sym.section << "\"}," << std::endl;
  // }

  ASSERT_EQ(symbols.size(), goldens.size());
  for (size_t i = 0; i < symbols.size(); i++) {
    ASSERT_EQ(symbols[i].addr, goldens[i].addr);
    ASSERT_EQ(symbols[i].size, goldens[i].size);
    ASSERT_EQ(symbols[i].name, goldens[i].name);
    ASSERT_EQ(symbols[i].compile_unit, goldens[i].compile_unit);
    ASSERT_EQ(symbols[i].section, goldens[i].section);
  }
}

TEST(LinkMapTest, ExampleRust) {
  std::string link_map = LoadLinkMapFile("example_rust.map");
  absl::StripLeadingAsciiWhitespace(&link_map);
  absl::StripTrailingAsciiWhitespace(&link_map);

  auto symbols = bloaty_link_map::ParseLldLinkMap(link_map);

  struct Golden {
    uint64_t addr;
    uint64_t size;
    std::string name;
    std::string compile_unit;
    std::string section;
  };
  std::vector<Golden> goldens = {
      {0x2800, 0x27, ".Lanon.745d157954bc19b57d1f47812cead630.3",
       "./exe.unstripped/"
       "component_manager.alloc-54127f36ba192482.alloc.4k1iwrm2-cgu.0.rcgu.o.rcgu.o",
       ".rodata"},
      {0x2828, 0x0, ".Lanon.745d157954bc19b57d1f47812cead630.5",
       "./exe.unstripped/"
       "component_manager.alloc-54127f36ba192482.alloc.4k1iwrm2-cgu.0.rcgu.o.rcgu.o",
       ".rodata"},
      {0x2828, 0x33, ".Lanon.745d157954bc19b57d1f47812cead630.13",
       "./exe.unstripped/"
       "component_manager.alloc-54127f36ba192482.alloc.4k1iwrm2-cgu.0.rcgu.o.rcgu.o",
       ".rodata"},
      {0x2885, 0x11, "anon.745d157954bc19b57d1f47812cead630",
       "./exe.unstripped/"
       "component_manager.alloc-54127f36ba192482.alloc.4k1iwrm2-cgu.0.rcgu.o.rcgu.o",
       ".rodata"},
      {0x2896, 0x3, ".Lanon.745d157954bc19b57d1f47812cead630.23",
       "./exe.unstripped/"
       "component_manager.alloc-54127f36ba192482.alloc.4k1iwrm2-cgu.0.rcgu.o.rcgu.o",
       ".rodata"},
      {0x2899, 0x16, ".Lanon.745d157954bc19b57d1f47812cead630.26",
       "./exe.unstripped/"
       "component_manager.alloc-54127f36ba192482.alloc.4k1iwrm2-cgu.0.rcgu.o.rcgu.o",
       ".rodata"},
      {0x292a, 0xd, "anon.745d157954bc19b57d1f47812cead630",
       "./exe.unstripped/"
       "component_manager.alloc-54127f36ba192482.alloc.4k1iwrm2-cgu.0.rcgu.o.rcgu.o",
       ".rodata"},
      {0x2937, 0x5, "anon.745d157954bc19b57d1f47812cead630",
       "./exe.unstripped/"
       "component_manager.alloc-54127f36ba192482.alloc.4k1iwrm2-cgu.0.rcgu.o.rcgu.o",
       ".rodata"},
      {0x293c, 0x5, "anon.745d157954bc19b57d1f47812cead630",
       "./exe.unstripped/"
       "component_manager.alloc-54127f36ba192482.alloc.4k1iwrm2-cgu.0.rcgu.o.rcgu.o",
       ".rodata"},
      {0x2950, 0xa90, "** lld merge strings", "", ".rodata"},
      {0x33e0, 0x29, ".Lanon.256afffd819e3254289014381fd650e5.2",
       "./exe.unstripped/"
       "component_manager.backtrace-5619ad41e613872b.backtrace.e1f4vorm-cgu.0.rcgu.o.rcgu.o",
       ".rodata"},
      {0x346d, 0x0, ".Lanon.256afffd819e3254289014381fd650e5.14",
       "./exe.unstripped/"
       "component_manager.backtrace-5619ad41e613872b.backtrace.e1f4vorm-cgu.0.rcgu.o.rcgu.o",
       ".rodata"},
      {0x3470, 0x38, ".Lanon.256afffd819e3254289014381fd650e5.16",
       "./exe.unstripped/"
       "component_manager.backtrace-5619ad41e613872b.backtrace.e1f4vorm-cgu.0.rcgu.o.rcgu.o",
       ".rodata"},
      {0x34a8, 0x1b08, "** lld merge strings", "", ".rodata"},
      {0x4fb0, 0x3, ".Lanon.256afffd819e3254289014381fd650e5.18",
       "./exe.unstripped/"
       "component_manager.backtrace-5619ad41e613872b.backtrace.e1f4vorm-cgu.0.rcgu.o.rcgu.o",
       ".rodata"},
      {0x5070, 0x2780, "** lld merge strings", "", ".rodata"},
      {0x77f0, 0x6, ".Lanon.256afffd819e3254289014381fd650e5.26",
       "./exe.unstripped/"
       "component_manager.backtrace-5619ad41e613872b.backtrace.e1f4vorm-cgu.0.rcgu.o.rcgu.o",
       ".rodata"},
      {0x77f8, 0x118, ".Lanon.256afffd819e3254289014381fd650e5.28",
       "./exe.unstripped/"
       "component_manager.backtrace-5619ad41e613872b.backtrace.e1f4vorm-cgu.0.rcgu.o.rcgu.o",
       ".rodata"},
      {0x7910, 0xc, "anon.256afffd819e3254289014381fd650e5",
       "./exe.unstripped/"
       "component_manager.backtrace-5619ad41e613872b.backtrace.e1f4vorm-cgu.0.rcgu.o.rcgu.o",
       ".rodata"},
      {0x791c, 0x6, "anon.256afffd819e3254289014381fd650e5",
       "./exe.unstripped/"
       "component_manager.backtrace-5619ad41e613872b.backtrace.e1f4vorm-cgu.0.rcgu.o.rcgu.o",
       ".rodata"},
      {0x7922, 0x10,
       "_RINvNtCs6QHBvYLDz7W_4core3ptr13drop_in_placeNtCsbDqzXfLQacH_15fidl_fuchsia_"
       "io11FileRequestECs4fqI2P2rA04_17component_manager",
       "./exe.unstripped/component_manager.component_manager.7rcbfp3g-cgu.0.rcgu.o", ".rodata"},
      {0x7950, 0xb1,
       "_RNvXsc_NtCsbDqzXfLQacH_17fuchsia_component6serverINtB5_9ServiceFsINtNtB5_"
       "7service10ServiceObjuEENtNtCseXumDWplyBO_12futures_core6stream6Stream9poll_"
       "nextCs4fqI2P2rA04_17component_manager",
       "./exe.unstripped/component_manager.component_manager.7rcbfp3g-cgu.0.rcgu.o", ".rodata"},
      {0x7a08, 0x380, "** lld merge strings", "", ".rodata"},
      {0x7d88, 0x2b, ".Lanon.dda71605b8fec89c031783be146b7025.1",
       "./exe.unstripped/component_manager.component_manager.7rcbfp3g-cgu.0.rcgu.o", ".rodata"},
      {0x8cf1, 0x4e, ".Lanon.8db4583f006f833985ea18d2ab89bab7.1",
       "./exe.unstripped/component_manager.component_manager.7rcbfp3g-cgu.7.rcgu.o", ".rodata"},
      {0x8d3f, 0x1f, "anon.8db4583f006f833985ea18d2ab89bab7",
       "./exe.unstripped/component_manager.component_manager.7rcbfp3g-cgu.7.rcgu.o", ".rodata"},
      {0x8d5e, 0x25, ".Lanon.a1c52c190a58d1713a23e7a7bdb0f9c0.2",
       "./exe.unstripped/component_manager.component_manager.7rcbfp3g-cgu.8.rcgu.o", ".rodata"},
      {0x8d83, 0x7,
       "_RINvNtCs6QHBvYLDz7W_4core3ptr13drop_in_placeINtNvNtB4_6future14from_"
       "generator9GenFutureNCNvMNtNtNtCsbDqzXfLQacH_21component_manager_"
       "lib5model6events6sourceNtB1s_11EventSource3new0EECs4fqI2P2rA04_17component_manager",
       "./exe.unstripped/component_manager.component_manager.7rcbfp3g-cgu.9.rcgu.o", ".rodata"},
      {0x8d8a, 0x8,
       "_RINvNtCs6QHBvYLDz7W_4core3ptr13drop_in_placeINtNvNtB4_6future14from_"
       "generator9GenFutureNCNvMs2_NtNtCsbDqzXfLQacH_21component_manager_lib5model5realmNtB1v_"
       "5Realm19lock_resolved_state0EECs4fqI2P2rA04_17component_manager",
       "./exe.unstripped/component_manager.component_manager.7rcbfp3g-cgu.9.rcgu.o", ".rodata"},
      {0x8d92, 0x16,
       "_RINvNtCs6QHBvYLDz7W_4core3ptr13drop_in_placeINtNvNtB4_6future14from_"
       "generator9GenFutureNCNvMs_NtCsbDqzXfLQacH_21component_manager_lib19builtin_"
       "environmentNtB1u_18BuiltinEnvironment3new0EECs4fqI2P2rA04_17component_manager",
       "./exe.unstripped/component_manager.component_manager.7rcbfp3g-cgu.9.rcgu.o", ".rodata"},
      {0x8da8, 0x4,
       "_RINvNtCs6QHBvYLDz7W_4core3ptr13drop_in_placeNtCsbDqzXfLQacH_7cm_"
       "rust10ExposeDeclECs4fqI2P2rA04_17component_manager",
       "./exe.unstripped/component_manager.component_manager.7rcbfp3g-cgu.9.rcgu.o", ".rodata"},
      {0x8dac, 0x6,
       "_RINvNtCs6QHBvYLDz7W_4core3ptr13drop_in_placeNtCsbDqzXfLQacH_7cm_"
       "rust7UseDeclECs4fqI2P2rA04_17component_manager",
       "./exe.unstripped/component_manager.component_manager.7rcbfp3g-cgu.9.rcgu.o", ".rodata"},
      {0x91e3, 0x4, "_ZN4core3fmt9Formatter3pad17h9c055cdb18c92cc1E",
       "./exe.unstripped/component_manager.core-6a4b1e7af979d229.core.3ocx6h1n-cgu.0.rcgu.o.rcgu.o",
       ".rodata"},
      {0x91e7, 0x4, "_ZN4core3fmt9Formatter19pad_formatted_parts17heb4f002989281314E",
       "./exe.unstripped/component_manager.core-6a4b1e7af979d229.core.3ocx6h1n-cgu.0.rcgu.o.rcgu.o",
       ".rodata"},
      {0x9214, 0x29, "_ZN41_$LT$char$u20$as$u20$core..fmt..Debug$GT$3fmt17hb14158a7386f6b1aE",
       "./exe.unstripped/component_manager.core-6a4b1e7af979d229.core.3ocx6h1n-cgu.0.rcgu.o.rcgu.o",
       ".rodata"},
      {0x925c, 0x6f,
       "_ZN61_$LT$core..str..EscapeDebug$u20$as$u20$core..fmt..Display$GT$3fmt17hf1238c3db5de3de5E",
       "./exe.unstripped/component_manager.core-6a4b1e7af979d229.core.3ocx6h1n-cgu.0.rcgu.o.rcgu.o",
       ".rodata"},
      {0x92d0, 0x0, "anon.6741f024a682c8cb8e59ecab4fc7e9ed.12.llvm.9325873315546439775",
       "./exe.unstripped/component_manager.core-6a4b1e7af979d229.core.3ocx6h1n-cgu.0.rcgu.o.rcgu.o",
       ".rodata"},
      {0x92d0, 0x1, ".Lanon.6741f024a682c8cb8e59ecab4fc7e9ed.14",
       "./exe.unstripped/component_manager.core-6a4b1e7af979d229.core.3ocx6h1n-cgu.0.rcgu.o.rcgu.o",
       ".rodata"},
      {0x92d1, 0x1d, ".Lanon.6741f024a682c8cb8e59ecab4fc7e9ed.27",
       "./exe.unstripped/component_manager.core-6a4b1e7af979d229.core.3ocx6h1n-cgu.0.rcgu.o.rcgu.o",
       ".rodata"},
      {0x9344, 0x28, "core::num::flt2dec::strategy::dragon::POW10::he881f1848ad745b6",
       "./exe.unstripped/component_manager.core-6a4b1e7af979d229.core.3ocx6h1n-cgu.0.rcgu.o.rcgu.o",
       ".rodata"},
      {0x936c, 0x28, "core::num::flt2dec::strategy::dragon::TWOPOW10::h8ee033ba72605fde",
       "./exe.unstripped/component_manager.core-6a4b1e7af979d229.core.3ocx6h1n-cgu.0.rcgu.o.rcgu.o",
       ".rodata"},
      {0x9394, 0x8, "core::num::flt2dec::strategy::dragon::POW10TO16::hd008c9b35d67c443",
       "./exe.unstripped/component_manager.core-6a4b1e7af979d229.core.3ocx6h1n-cgu.0.rcgu.o.rcgu.o",
       ".rodata"},
      {0x939c, 0x10, "core::num::flt2dec::strategy::dragon::POW10TO32::h94bb3ddee75ca692",
       "./exe.unstripped/component_manager.core-6a4b1e7af979d229.core.3ocx6h1n-cgu.0.rcgu.o.rcgu.o",
       ".rodata"},
      {0x93ac, 0x1c, "core::num::flt2dec::strategy::dragon::POW10TO64::h2607b29da45ca92b",
       "./exe.unstripped/component_manager.core-6a4b1e7af979d229.core.3ocx6h1n-cgu.0.rcgu.o.rcgu.o",
       ".rodata"},
      {0xebf8, 0x28, ".Lanon.31de557278eced41e2a6dce0e33ed7ec.9",
       "./exe.unstripped/"
       "component_manager.libcomponent_manager_lib.component_manager_lib.3a1fbbbh-cgu.2.rcgu.o."
       "rcgu.o",
       ".rodata"},
      {0xec20, 0x72, ".Lanon.31de557278eced41e2a6dce0e33ed7ec.15",
       "./exe.unstripped/"
       "component_manager.libcomponent_manager_lib.component_manager_lib.3a1fbbbh-cgu.2.rcgu.o."
       "rcgu.o",
       ".rodata"},
      {0xec92, 0x2e, "anon.31de557278eced41e2a6dce0e33ed7ec",
       "./exe.unstripped/"
       "component_manager.libcomponent_manager_lib.component_manager_lib.3a1fbbbh-cgu.2.rcgu.o."
       "rcgu.o",
       ".rodata"},
      {0xecc0, 0x13, "anon.31de557278eced41e2a6dce0e33ed7ec",
       "./exe.unstripped/"
       "component_manager.libcomponent_manager_lib.component_manager_lib.3a1fbbbh-cgu.2.rcgu.o."
       "rcgu.o",
       ".rodata"},
      {0xecd3, 0x4a, ".Lanon.31de557278eced41e2a6dce0e33ed7ec.34",
       "./exe.unstripped/"
       "component_manager.libcomponent_manager_lib.component_manager_lib.3a1fbbbh-cgu.2.rcgu.o."
       "rcgu.o",
       ".rodata"},
      {0x2e20c, 0xc, ".Lanon.93519e6636017643a07f78e678f4f11e.3501",
       "./exe.unstripped/"
       "component_manager.libidna-5c3c4f537fb8313f-5c3c4f537fb8313f.idna.axcknxfw-cgu.15.rcgu.o."
       "rcgu.o",
       ".rodata"},
      {0x2e218, 0xc, ".Lanon.93519e6636017643a07f78e678f4f11e.3502",
       "./exe.unstripped/"
       "component_manager.libidna-5c3c4f537fb8313f-5c3c4f537fb8313f.idna.axcknxfw-cgu.15.rcgu.o."
       "rcgu.o",
       ".rodata"},
      {0x2e224, 0xc, ".Lanon.93519e6636017643a07f78e678f4f11e.3503",
       "./exe.unstripped/"
       "component_manager.libidna-5c3c4f537fb8313f-5c3c4f537fb8313f.idna.axcknxfw-cgu.15.rcgu.o."
       "rcgu.o",
       ".rodata"},
      {0x2f4ca, 0x1b, "anon.7cbed09a5c2e2961b1298b053de6d71a",
       "./exe.unstripped/component_manager.libio_util.io_util.3a1fbbbh-cgu.3.rcgu.o.rcgu.o",
       ".rodata"},
      {0x2f4e5, 0x28, ".Lanon.2dc76abe0f971e71acd588990ffc8513.0",
       "./exe.unstripped/component_manager.libio_util.io_util.3a1fbbbh-cgu.8.rcgu.o.rcgu.o",
       ".rodata"},
      {0x2f50d, 0x2b, ".Lanon.6f9bafb4adb671d7ac9f8408e86ff31a.0",
       "./exe.unstripped/"
       "component_manager.liblibrary_loader.library_loader.3a1fbbbh-cgu.1.rcgu.o.rcgu.o",
       ".rodata"},
      {0x3aacc, 0x134,
       "_ZN9libunwind17DwarfInstructionsINS_17LocalAddressSpaceENS_15Registers_"
       "arm64EE18evaluateExpressionEmRS1_RKS2_m",
       "/tmp/rustcL27BaP/libunwind-e14dabfa3d8ce62b.rlib(libunwind.o)", ".rodata"},
      {0x3ac00, 0x39,
       "_ZN9libunwind10CFI_ParserINS_17LocalAddressSpaceEE8parseCIEERS1_mPNS2_8CIE_InfoE",
       "/tmp/rustcL27BaP/libunwind-e14dabfa3d8ce62b.rlib(libunwind.o)", ".rodata"},
      {0x3ac39, 0xc, "_ZN9libunwind14EHHeaderParserINS_17LocalAddressSpaceEE17getTableEntrySizeEh",
       "/tmp/rustcL27BaP/libunwind-e14dabfa3d8ce62b.rlib(libunwind.o)", ".rodata"},
      {0xb2640, 0x14, "_start",
       "/usr/local/google/home/yifeit/vg/out/default.zircon/user-arm64-clang.shlib/obj/system/ulib/"
       "c/crt1.Scrt1.cc.o",
       ".text"},
      {0xb2654, 0x1c, "_$LT$$RF$T$u20$as$u20$core..fmt..Debug$GT$::fmt::h12b31caec1d188c4",
       "./exe.unstripped/"
       "component_manager.alloc-54127f36ba192482.alloc.4k1iwrm2-cgu.0.rcgu.o.rcgu.o",
       ".text"},
      {0xb2670, 0xb4, "_$LT$$RF$T$u20$as$u20$core..fmt..Debug$GT$::fmt::h268a568299531efb",
       "./exe.unstripped/"
       "component_manager.alloc-54127f36ba192482.alloc.4k1iwrm2-cgu.0.rcgu.o.rcgu.o",
       ".text"},
      {0xcd52c, 0x98,
       "core::num::_$LT$impl$u20$core..str..FromStr$u20$for$u20$usize$GT$::from_str::"
       "h4398f35a1c0b4be7",
       "./exe.unstripped/component_manager.core-6a4b1e7af979d229.core.3ocx6h1n-cgu.0.rcgu.o.rcgu.o",
       ".text"},
      {0xcd5c4, 0xe4,
       "_$LT$core..num..TryFromIntError$u20$as$u20$core..fmt..Debug$GT$::fmt::h10047a113409f418",
       "./exe.unstripped/component_manager.core-6a4b1e7af979d229.core.3ocx6h1n-cgu.0.rcgu.o.rcgu.o",
       ".text"},
      {0xcd6a8, 0x34,
       "core::fmt::float::_$LT$impl$u20$core..fmt..Display$u20$for$u20$f64$GT$::fmt::"
       "hac033855e28d5eb4",
       "./exe.unstripped/component_manager.core-6a4b1e7af979d229.core.3ocx6h1n-cgu.0.rcgu.o.rcgu.o",
       ".text"},
      {0xe2554, 0x10, "OUTLINED_FUNCTION_359",
       "./exe.unstripped/"
       "component_manager.libcm_fidl_translator.cm_fidl_translator.3a1fbbbh-cgu.0.rcgu.o.rcgu.o",
       ".text"},
      {0xe2564, 0x10, "OUTLINED_FUNCTION_360",
       "./exe.unstripped/"
       "component_manager.libcm_fidl_translator.cm_fidl_translator.3a1fbbbh-cgu.0.rcgu.o.rcgu.o",
       ".text"},
      {0xeb888, 0x40,
       "_RINvNtNtCs6QHBvYLDz7W_4core4iter8adapters15process_resultsINtB2_3MapINtNtCsiEMmsMe2HZG_"
       "5alloc3vec8IntoIterNtNtCsbDqzXfLQacH_7cm_json2cm8ResolverENCNvXs_CsbDqzXfLQacH_18cm_fidl_"
       "translatorINtB17_3VecB1G_EINtB2q_6CmIntoIB2Z_NtCsbDqzXfLQacH_17fidl_fuchsia_"
       "sys212ResolverDeclEE7cm_into0EB3x_NtB1K_5ErrorNCINvXsx_NtB6_6resultINtB4V_6ResultB3s_B4A_"
       "EINtNtNtB4_6traits7collect12FromIteratorIB58_B3x_B4A_EE9from_iterBU_E0B3s_EB2q_",
       "./exe.unstripped/"
       "component_manager.libcm_fidl_translator.cm_fidl_translator.3a1fbbbh-cgu.4.rcgu.o.rcgu.o",
       ".text"},
      {0x197340, 0x70,
       "_RINvNtCsevZhND7KDbE_9hashbrown3raw16calculate_layoutTjINtNtCsiEMmsMe2HZG_"
       "5alloc4sync3ArcDNtNtCsbDqzXfLQacH_13fuchsia_async8executor14PacketReceiverEL_EEEB1t_",
       "./exe.unstripped/"
       "component_manager.libfuchsia_async.fuchsia_async.3a1fbbbh-cgu.1.rcgu.o.rcgu.o",
       ".text"},
      {0x200800, 0x38,
       "std::sys_common::thread_info::THREAD_INFO::__getit::__KEY::h836843d10e59cada",
       "./exe.unstripped/component_manager.std-7a72ffd2f3a7a812.std.a0uwjzoe-cgu.0.rcgu.o.rcgu.o",
       ".text"},
      {0x200840, 0x18,
       "std::panicking::update_panic_count::PANIC_COUNT::__getit::__KEY::h69785e8fa31a63e6",
       "./exe.unstripped/component_manager.std-7a72ffd2f3a7a812.std.a0uwjzoe-cgu.0.rcgu.o.rcgu.o",
       ".text"},
      {0x1ff610, 0x30, ".Lanon.745d157954bc19b57d1f47812cead630.1",
       "./exe.unstripped/"
       "component_manager.alloc-54127f36ba192482.alloc.4k1iwrm2-cgu.0.rcgu.o.rcgu.o",
       ".data.rel.ro"},
      {0x1ff640, 0x20, ".Lanon.745d157954bc19b57d1f47812cead630.2",
       "./exe.unstripped/"
       "component_manager.alloc-54127f36ba192482.alloc.4k1iwrm2-cgu.0.rcgu.o.rcgu.o",
       ".data.rel.ro"},
      {0x1ff660, 0x18, ".Lanon.745d157954bc19b57d1f47812cead630.4",
       "./exe.unstripped/"
       "component_manager.alloc-54127f36ba192482.alloc.4k1iwrm2-cgu.0.rcgu.o.rcgu.o",
       ".data.rel.ro"},
      {0x202a88, 0x20, "anon.e57b3f2ae8b5861992cfe341e01c75d6",
       "./exe.unstripped/"
       "component_manager.libcm_fidl_validator.cm_fidl_validator.3a1fbbbh-cgu.0.rcgu.o.rcgu.o",
       ".data.rel.ro"},
      {0x2110a8, 0x18, ".Lanon.6f9bafb4adb671d7ac9f8408e86ff31a.3",
       "./exe.unstripped/"
       "component_manager.liblibrary_loader.library_loader.3a1fbbbh-cgu.1.rcgu.o.rcgu.o",
       ".data.rel.ro"},
      {0x211108, 0x10, "anon.6f9bafb4adb671d7ac9f8408e86ff31a",
       "./exe.unstripped/"
       "component_manager.liblibrary_loader.library_loader.3a1fbbbh-cgu.1.rcgu.o.rcgu.o",
       ".data.rel.ro"},
      {0x211118, 0x38, "anon.6f9bafb4adb671d7ac9f8408e86ff31a",
       "./exe.unstripped/"
       "component_manager.liblibrary_loader.library_loader.3a1fbbbh-cgu.1.rcgu.o.rcgu.o",
       ".data.rel.ro"},
      {0x211150, 0x20, "anon.54c490f3ebdb496a1657b64fb6de7217",
       "./exe.unstripped/"
       "component_manager.liblibrary_loader.library_loader.3a1fbbbh-cgu.10.rcgu.o.rcgu.o",
       ".data.rel.ro"},
      {0x211170, 0x18, ".Lanon.39cf32991562f2df00749835baa6fdc6.1",
       "./exe.unstripped/"
       "component_manager.liblibrary_loader.library_loader.3a1fbbbh-cgu.11.rcgu.o.rcgu.o",
       ".data.rel.ro"},
      {0x215018, 0x18, "anon.d8e73c4ab2af7c41d0ca8802142f134e",
       "./exe.unstripped/component_manager.std-7a72ffd2f3a7a812.std.a0uwjzoe-cgu.0.rcgu.o.rcgu.o",
       ".data.rel.ro"},
      {0x215030, 0x88,
       "vtable for libunwind::UnwindCursor<libunwind::LocalAddressSpace, "
       "libunwind::Registers_arm64>",
       "/tmp/rustcL27BaP/libunwind-e14dabfa3d8ce62b.rlib(libunwind.o)", ".data.rel.ro"},
      {0x2150b8, 0x310, ".Lswitch.table._ZN9libunwind15Registers_arm6415getRegisterNameEi",
       "/tmp/rustcL27BaP/libunwind-e14dabfa3d8ce62b.rlib(libunwind.o)", ".data.rel.ro"},
      {0x225848, 0x10, "_RNvCsi1CgODtnl2B_3log6LOGGER",
       "./exe.unstripped/"
       "component_manager.liblog-51a8548556518bd3-51a8548556518bd3.log.7ncv9mgr-cgu.0.rcgu.o.rcgu."
       "o",
       ".data"},
      {0x225858, 0x8, "std::thread::ThreadId::new::COUNTER::hb3a413a29c0f81f6",
       "./exe.unstripped/component_manager.std-7a72ffd2f3a7a812.std.a0uwjzoe-cgu.0.rcgu.o.rcgu.o",
       ".data"},
      {0x225860, 0x38, "std::io::stdio::stderr::INSTANCE::hb24941292040ef06",
       "./exe.unstripped/component_manager.std-7a72ffd2f3a7a812.std.a0uwjzoe-cgu.0.rcgu.o.rcgu.o",
       ".data"},
      {0x2258c8, 0x8, "libunwind::DwarfFDECache<libunwind::LocalAddressSpace>::_bufferEnd",
       "/tmp/rustcL27BaP/libunwind-e14dabfa3d8ce62b.rlib(libunwind.o)", ".data"},
      {0x225c68, 0x20,
       "_RNvNvNvXs15_CsbDqzXfLQacH_7cm_rustNtBa_13DATA_TYPENAMENtNtNtCs6QHBvYLDz7W_"
       "4core3ops5deref5Deref5deref11___stability4LAZY",
       "./exe.unstripped/component_manager.libcm_rust.cm_rust.3a1fbbbh-cgu.0.rcgu.o.rcgu.o",
       ".bss"},
      {0x225c88, 0x20,
       "_RNvNvNvXs17_CsbDqzXfLQacH_7cm_rustNtBa_14CACHE_TYPENAMENtNtNtCs6QHBvYLDz7W_"
       "4core3ops5deref5Deref5deref11___stability4LAZY",
       "./exe.unstripped/component_manager.libcm_rust.cm_rust.3a1fbbbh-cgu.0.rcgu.o.rcgu.o",
       ".bss"},
      {0x225ca8, 0x20,
       "_RNvNvNvXs19_CsbDqzXfLQacH_7cm_rustNtBa_13META_TYPENAMENtNtNtCs6QHBvYLDz7W_"
       "4core3ops5deref5Deref5deref11___stability4LAZY",
       "./exe.unstripped/component_manager.libcm_rust.cm_rust.3a1fbbbh-cgu.0.rcgu.o.rcgu.o",
       ".bss"},
      {0x225cc8, 0x38,
       "_RNvNvNvXs0_NtNtNtCsbDqzXfLQacH_21component_manager_lib5model6events14source_factoryNtB9_"
       "25EVENT_SOURCE_SERVICE_PATHNtNtNtCs6QHBvYLDz7W_4core3ops5deref5Deref5deref11___"
       "stability4LAZY",
       "./exe.unstripped/"
       "component_manager.libcomponent_manager_lib.component_manager_lib.3a1fbbbh-cgu.12.rcgu.o."
       "rcgu.o",
       ".bss"},
      {0x225d00, 0x38,
       "_RNvNvNvXs2_NtNtNtCsbDqzXfLQacH_21component_manager_lib5model6events14source_factoryNtB9_"
       "30EVENT_SOURCE_SYNC_SERVICE_PATHNtNtNtCs6QHBvYLDz7W_4core3ops5deref5Deref5deref11___"
       "stability4LAZY",
       "./exe.unstripped/"
       "component_manager.libcomponent_manager_lib.component_manager_lib.3a1fbbbh-cgu.12.rcgu.o."
       "rcgu.o",
       ".bss"},
      {0x2262ac, 0x8, "libunwind::DwarfFDECache<libunwind::LocalAddressSpace>::_lock",
       "/tmp/rustcL27BaP/libunwind-e14dabfa3d8ce62b.rlib(libunwind.o)", ".bss"},
      {0x2262b8, 0x800, "libunwind::DwarfFDECache<libunwind::LocalAddressSpace>::_initialBuffer",
       "/tmp/rustcL27BaP/libunwind-e14dabfa3d8ce62b.rlib(libunwind.o)", ".bss"},
      {0x226ab8, 0x1, "logAPIs::checked",
       "/tmp/rustcL27BaP/libunwind-e14dabfa3d8ce62b.rlib(libunwind.o)", ".bss"},
      {0x226abc, 0x1, "logAPIs::log",
       "/tmp/rustcL27BaP/libunwind-e14dabfa3d8ce62b.rlib(libunwind.o)", ".bss"},
      {0x226abd, 0x1, "logUnwinding::checked",
       "/tmp/rustcL27BaP/libunwind-e14dabfa3d8ce62b.rlib(libunwind.o)", ".bss"},
      {0x226ac0, 0x1, "logUnwinding::log",
       "/tmp/rustcL27BaP/libunwind-e14dabfa3d8ce62b.rlib(libunwind.o)", ".bss"},
      {0x226ac1, 0x1, "logDWARF::checked",
       "/tmp/rustcL27BaP/libunwind-e14dabfa3d8ce62b.rlib(libunwind.o)", ".bss"},
      {0x226ac4, 0x1, "logDWARF::log",
       "/tmp/rustcL27BaP/libunwind-e14dabfa3d8ce62b.rlib(libunwind.o)", ".bss"},
  };

  // for (const auto& sym : symbols) {
  //   std::cout << std::hex << "{0x" << sym.addr << ", 0x" << sym.size << ", \"" << sym.name
  //             << "\", \"" << sym.compile_unit << "\", \"" << sym.section << "\"}," << std::endl;
  // }

  ASSERT_EQ(symbols.size(), goldens.size());
  for (size_t i = 0; i < symbols.size(); i++) {
    ASSERT_EQ(symbols[i].addr, goldens[i].addr);
    ASSERT_EQ(symbols[i].size, goldens[i].size);
    ASSERT_EQ(symbols[i].name, goldens[i].name);
    ASSERT_EQ(symbols[i].compile_unit, goldens[i].compile_unit);
    ASSERT_EQ(symbols[i].section, goldens[i].section);
  }
}

TEST(LinkMapTest, SuperSizeGoldens) {
  std::ifstream infile("test_lld-lto_v1.map");
  std::string link_map;
  for (std::string line; getline(infile, line);) {
    if (line.empty()) continue;
    if (line[0] == '#') continue;
    link_map += line;
    link_map += '\n';
  }

  absl::StripLeadingAsciiWhitespace(&link_map);
  absl::StripTrailingAsciiWhitespace(&link_map);

  struct Golden {
    uint64_t addr;
    uint64_t size;
    std::string name;
    std::string compile_unit;
    std::string section;
  };
  std::vector<Golden> goldens = {
      {0x213200, 0x4, "v8_Default_embedded_blob_size_", "obj/v8/v8_external_snapshot/embedded.o",
       ".rodata"},
      {0x213210, 0x10, "pmmp",
       "obj/third_party/ffmpeg/libffmpeg_internal.a(ffmpeg_internal/fft_neon.o)", ".rodata"},
      {0x213220, 0x10, "mppm",
       "obj/third_party/ffmpeg/libffmpeg_internal.a(ffmpeg_internal/fft_neon.o)", ".rodata"},
      {0x213230, 0xc0, "std::__ndk1::(anonymous namespace)::small_primes",
       "../../third_party/android_ndk/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/"
       "libc++_static.a(hash.o)",
       ".rodata"},
      {0x2132f0, 0xc0, "std::__ndk1::(anonymous namespace)::indices",
       "../../third_party/android_ndk/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/"
       "libc++_static.a(hash.o)",
       ".rodata"},
      {0x2133b0, 0x4, "std::__ndk1::ios_base::boolalpha",
       "../../third_party/android_ndk/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/"
       "libc++_static.a(ios.o)",
       ".rodata"},
      {0x2133b4, 0x4, "std::__ndk1::ios_base::dec",
       "../../third_party/android_ndk/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/"
       "libc++_static.a(ios.o)",
       ".rodata"},
      {0x2133b8, 0x4, "std::__ndk1::ios_base::fixed",
       "../../third_party/android_ndk/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/"
       "libc++_static.a(ios.o)",
       ".rodata"},
      {0x2133bc, 0x4, "std::__ndk1::ios_base::hex",
       "../../third_party/android_ndk/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/"
       "libc++_static.a(ios.o)",
       ".rodata"},
      {0x2133c0, 0x4, "std::__ndk1::ios_base::internal",
       "../../third_party/android_ndk/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/"
       "libc++_static.a(ios.o)",
       ".rodata"},
      {0x2133c4, 0x4, "std::__ndk1::ios_base::left",
       "../../third_party/android_ndk/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/"
       "libc++_static.a(ios.o)",
       ".rodata"},
      {0x2133c8, 0x4, "std::__ndk1::ios_base::oct",
       "../../third_party/android_ndk/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/"
       "libc++_static.a(ios.o)",
       ".rodata"},
      {0x2133cc, 0x4, "std::__ndk1::ios_base::right",
       "../../third_party/android_ndk/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/"
       "libc++_static.a(ios.o)",
       ".rodata"},
      {0x2133d0, 0x4, "std::__ndk1::ios_base::scientific",
       "../../third_party/android_ndk/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/"
       "libc++_static.a(ios.o)",
       ".rodata"},
      {0x2133d4, 0x4, "std::__ndk1::ios_base::showbase",
       "../../third_party/android_ndk/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/"
       "libc++_static.a(ios.o)",
       ".rodata"},
      {0x2133d8, 0x4, "std::__ndk1::ios_base::showpoint",
       "../../third_party/android_ndk/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/"
       "libc++_static.a(ios.o)",
       ".rodata"},
      {0x2133dc, 0x4, "std::__ndk1::ios_base::showpos",
       "../../third_party/android_ndk/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/"
       "libc++_static.a(ios.o)",
       ".rodata"},
      {0x2133e0, 0x4, "std::__ndk1::ios_base::skipws",
       "../../third_party/android_ndk/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/"
       "libc++_static.a(ios.o)",
       ".rodata"},
      {0x2133e4, 0x4, "std::__ndk1::ios_base::unitbuf",
       "../../third_party/android_ndk/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/"
       "libc++_static.a(ios.o)",
       ".rodata"},
      {0x2133e8, 0x4, "std::__ndk1::ios_base::uppercase",
       "../../third_party/android_ndk/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/"
       "libc++_static.a(ios.o)",
       ".rodata"},
      {0x2133ec, 0x4, "std::__ndk1::ios_base::adjustfield",
       "../../third_party/android_ndk/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/"
       "libc++_static.a(ios.o)",
       ".rodata"},
      {0x2133f0, 0x4, "std::__ndk1::ios_base::basefield",
       "../../third_party/android_ndk/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/"
       "libc++_static.a(ios.o)",
       ".rodata"},
      {0x2133f4, 0x4, "std::__ndk1::ios_base::floatfield",
       "../../third_party/android_ndk/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/"
       "libc++_static.a(ios.o)",
       ".rodata"},
      {0x2133f8, 0x4, "std::__ndk1::ios_base::badbit",
       "../../third_party/android_ndk/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/"
       "libc++_static.a(ios.o)",
       ".rodata"},
      {0x2133fc, 0x4, "std::__ndk1::ios_base::eofbit",
       "../../third_party/android_ndk/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/"
       "libc++_static.a(ios.o)",
       ".rodata"},
      {0x213400, 0x4, "std::__ndk1::ios_base::failbit",
       "../../third_party/android_ndk/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/"
       "libc++_static.a(ios.o)",
       ".rodata"},
      {0x213404, 0x4, "std::__ndk1::ios_base::goodbit",
       "../../third_party/android_ndk/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/"
       "libc++_static.a(ios.o)",
       ".rodata"},
      {0x213408, 0x4, "std::__ndk1::ios_base::app",
       "../../third_party/android_ndk/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/"
       "libc++_static.a(ios.o)",
       ".rodata"},
      {0x21340c, 0x4, "std::__ndk1::ios_base::ate",
       "../../third_party/android_ndk/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/"
       "libc++_static.a(ios.o)",
       ".rodata"},
      {0x213410, 0x4, "std::__ndk1::ios_base::binary",
       "../../third_party/android_ndk/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/"
       "libc++_static.a(ios.o)",
       ".rodata"},
      {0x213414, 0x4, "std::__ndk1::ios_base::in",
       "../../third_party/android_ndk/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/"
       "libc++_static.a(ios.o)",
       ".rodata"},
      {0x213418, 0x4, "std::__ndk1::ios_base::out",
       "../../third_party/android_ndk/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/"
       "libc++_static.a(ios.o)",
       ".rodata"},
      {0x21341c, 0x4, "std::__ndk1::ios_base::trunc",
       "../../third_party/android_ndk/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/"
       "libc++_static.a(ios.o)",
       ".rodata"},
      {0x213420, 0x1d, "typeinfo name for std::__ndk1::ios_base::failure",
       "../../third_party/android_ndk/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/"
       "libc++_static.a(ios.o)",
       ".rodata"},
      {0x213440, 0x15, "typeinfo name for std::__ndk1::ios_base",
       "../../third_party/android_ndk/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/"
       "libc++_static.a(ios.o)",
       ".rodata"},
      {0x213460, 0x21, "typeinfo name for std::__ndk1::__iostream_category",
       "../../third_party/android_ndk/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/"
       "libc++_static.a(ios.o)",
       ".rodata"},
      {0x213490, 0x2d, "_ZTSNSt6__ndk19basic_iosIcNS_11char_traitsIcEEEE",
       "../../third_party/android_ndk/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/"
       "libc++_static.a(ios.o)",
       ".rodata"},
      {0x21368b, 0x2c378e, "** lld merge strings", "", ".rodata"},
      {0x4d6e20, 0x1b, "typeinfo name for std::__ndk1::__stdinbuf<wchar_t>",
       "../../third_party/android_ndk/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/"
       "libc++_static.a(iostream.o)",
       ".rodata"},
      {0x4d7920, 0x266c, "** lld merge strings", "", ".rodata"},
      {0x4d9f90, 0x18, "typeinfo name for std::__ndk1::ctype_base",
       "../../third_party/android_ndk/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/"
       "libc++_static.a(locale.o)",
       ".rodata"},
      {0x4d9fb0, 0x1a, "typeinfo name for std::__ndk1::codecvt_base",
       "../../third_party/android_ndk/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/"
       "libc++_static.a(locale.o)",
       ".rodata"},
      {0x503eb3, 0x1, "network::mojom::CookieManagerProxy_SetCanonicalCookie_Message::kMessageTag",
       "", ".rodata"},
      {0x503eb4, 0x1,
       "network::mojom::CookieManagerProxy_DeleteCanonicalCookie_Message::kMessageTag", "",
       ".rodata"},
      {0x82f000, 0x40, "", "obj/third_party/boringssl/boringssl_asm/chacha-armv4.o", ".text"},
      {0x82f040, 0x3f0, "ChaCha20_ctr32", "obj/third_party/boringssl/boringssl_asm/chacha-armv4.o",
       ".text"},
      {0x82f440, 0x89c, "ChaCha20_neon", "obj/third_party/boringssl/boringssl_asm/chacha-armv4.o",
       ".text"},
      {0x82fce0, 0x540, "AES_Te", "obj/third_party/boringssl/boringssl_asm/aes-armv4.o", ".text"},
      {0x830220, 0x60, "aes_nohw_encrypt", "obj/third_party/boringssl/boringssl_asm/aes-armv4.o",
       ".text"},
      {0x830280, 0x1d8, "_armv4_AES_encrypt", "obj/third_party/boringssl/boringssl_asm/aes-armv4.o",
       ".text"},
      {0x830460, 0x2a0, "aes_nohw_set_encrypt_key",
       "obj/third_party/boringssl/boringssl_asm/aes-armv4.o", ".text"},
      {0x830700, 0x20, "aes_nohw_set_decrypt_key",
       "obj/third_party/boringssl/boringssl_asm/aes-armv4.o", ".text"},
      {0x830720, 0x124, "AES_set_enc2dec_key",
       "obj/third_party/boringssl/boringssl_asm/aes-armv4.o", ".text"},
      {0x830860, 0x500, "AES_Td", "obj/third_party/boringssl/boringssl_asm/aes-armv4.o", ".text"},
      {0x830d60, 0x60, "aes_nohw_decrypt", "obj/third_party/boringssl/boringssl_asm/aes-armv4.o",
       ".text"},
      {0x830dc0, 0x1f8, "_armv4_AES_decrypt", "obj/third_party/boringssl/boringssl_asm/aes-armv4.o",
       ".text"},
      {0x831000, 0x40, "", "obj/third_party/boringssl/boringssl_asm/aesv8-armx32.o", ".text"},
      {0x831040, 0x218, "aes_hw_set_encrypt_key",
       "obj/third_party/boringssl/boringssl_asm/aesv8-armx32.o", ".text"},
      {0x831260, 0x60, "aes_hw_set_decrypt_key",
       "obj/third_party/boringssl/boringssl_asm/aesv8-armx32.o", ".text"},
      {0x8312c0, 0x50, "aes_hw_encrypt", "obj/third_party/boringssl/boringssl_asm/aesv8-armx32.o",
       ".text"},
      {0x831320, 0x50, "aes_hw_decrypt", "obj/third_party/boringssl/boringssl_asm/aesv8-armx32.o",
       ".text"},
      {0x83c640, 0x2e60, "v8_Default_embedded_blob_data_", "obj/v8/v8_external_snapshot/embedded.o",
       ".text"},
      {0x83f4a0, 0x320, "Builtins_RecordWrite", "obj/v8/v8_external_snapshot/embedded.o", ".text"},
      {0x83f7c0, 0x40, "Builtins_AdaptorWithExitFrame", "obj/v8/v8_external_snapshot/embedded.o",
       ".text"},
      {0x83f800, 0x40, "Builtins_AdaptorWithBuiltinExitFrame",
       "obj/v8/v8_external_snapshot/embedded.o", ".text"},
      {0x83f840, 0x100, "Builtins_ArgumentsAdaptorTrampoline",
       "obj/v8/v8_external_snapshot/embedded.o", ".text"},
      {0x83f940, 0x100, "Builtins_CallFunction_ReceiverIsNullOrUndefined",
       "obj/v8/v8_external_snapshot/embedded.o", ".text"},
      {0x83fa40, 0x140, "Builtins_CallFunction_ReceiverIsNotNullOrUndefined",
       "obj/v8/v8_external_snapshot/embedded.o", ".text"},
      {0x83fb80, 0x180, "Builtins_CallFunction_ReceiverIsAny",
       "obj/v8/v8_external_snapshot/embedded.o", ".text"},
      {0x83fd00, 0x137820, "Builtins_CallBoundFunction", "obj/v8/v8_external_snapshot/embedded.o",
       ".text"},
      {0x977520, 0x18, "PushAllRegisters",
       "obj/third_party/blink/renderer/platform/heap/asm/asm/SaveRegisters_arm.o", ".text"},
      {0x97c834, 0x4dc, "vpx_convolve8_avg_horiz_filter_type1_neon",
       "obj/third_party/libvpx/libvpx_assembly_arm.a(libvpx_assembly_arm/"
       "vpx_convolve8_avg_horiz_filter_type1_neon.asm.o)",
       ".text"},
      {0x99cad8, 0xfc, "sinh",
       "../../third_party/android_ndk/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/"
       "libandroid_support.a(e_sinh.o)",
       ".text"},
      {0x1401788, 0xa4, "$_21::operator()(FamilyData*, char const*, char const**) const", "",
       ".text"},
      {0x1401820, 0x10, "UnlikelyFunc", "", ".text"},
      {0x1401850, 0x10, "StartUpFunc", "", ".text"},
      {0x2d4c000, 0x4, "v8_Default_embedded_blob_", "obj/v8/v8_external_snapshot/embedded.o",
       ".data"},
      {0x2d4c004, 0x4, "__dso_handle",
       "../../third_party/android_ndk/platforms/android-16/arch-arm/usr/lib/crtbegin_so.o",
       ".data"},
      {0x2d69000, 0x3c, "fft_tab_vfp",
       "obj/third_party/ffmpeg/libffmpeg_internal.a(ffmpeg_internal/fft_vfp.o)", ".data.rel.ro"},
      {0x2d6903c, 0x3c, "fft_tab_neon",
       "obj/third_party/ffmpeg/libffmpeg_internal.a(ffmpeg_internal/fft_neon.o)", ".data.rel.ro"},
      {0x2d69078, 0x40,
       "vtable for std::__ndk1::basic_streambuf<wchar_t, std::__ndk1::char_traits<wchar_t> >",
       "../../third_party/android_ndk/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/"
       "libc++_static.a(ios.o)",
       ".data.rel.ro"},
      {0x2d690b8, 0x28,
       "vtable for std::__ndk1::basic_istream<char, std::__ndk1::char_traits<char> >",
       "../../third_party/android_ndk/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/"
       "libc++_static.a(ios.o)",
       ".data.rel.ro"},
      {0x2d690e0, 0x28,
       "vtable for std::__ndk1::basic_istream<wchar_t, std::__ndk1::char_traits<wchar_t> >",
       "../../third_party/android_ndk/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/"
       "libc++_static.a(ios.o)",
       ".data.rel.ro"},
      {0x2d69108, 0x28,
       "vtable for std::__ndk1::basic_ostream<char, std::__ndk1::char_traits<char> >",
       "../../third_party/android_ndk/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/"
       "libc++_static.a(ios.o)",
       ".data.rel.ro"},
      {0x2d69130, 0x28,
       "vtable for std::__ndk1::basic_ostream<wchar_t, std::__ndk1::char_traits<wchar_t> >",
       "../../third_party/android_ndk/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/"
       "libc++_static.a(ios.o)",
       ".data.rel.ro"},
      {0x2d69158, 0x24, "vtable for std::__ndk1::__iostream_category",
       "../../third_party/android_ndk/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/"
       "libc++_static.a(ios.o)",
       ".data.rel.ro"},
      {0x2d6917c, 0x14, "vtable for std::__ndk1::ios_base::failure",
       "../../third_party/android_ndk/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/"
       "libc++_static.a(ios.o)",
       ".data.rel.ro"},
      {0x2d69190, 0x10, "vtable for std::__ndk1::ios_base",
       "../../third_party/android_ndk/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/"
       "libc++_static.a(ios.o)",
       ".data.rel.ro"},
      {0x2d691a0, 0xc, "typeinfo for std::__ndk1::ios_base::failure",
       "../../third_party/android_ndk/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/"
       "libc++_static.a(ios.o)",
       ".data.rel.ro"},
      {0x2d691ac, 0x8, "typeinfo for std::__ndk1::ios_base",
       "../../third_party/android_ndk/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/"
       "libc++_static.a(ios.o)",
       ".data.rel.ro"},
      {0x2d691b4, 0xc, "typeinfo for std::__ndk1::__iostream_category",
       "../../third_party/android_ndk/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/"
       "libc++_static.a(ios.o)",
       ".data.rel.ro"},
      {0x2f56000, 0x4, "WebRtcSpl_CrossCorrelation", "", ".bss"},
      {0x2f56004, 0x4, "WebRtcSpl_DownsampleFast", "", ".bss"},
      {0x2f56008, 0x4, "WebRtcSpl_MaxAbsValueW16", "", ".bss"},
      {0x0, 0x1000, "** ", "", ".part.end"},
  };

  auto symbols = bloaty_link_map::ParseLldLinkMap(link_map);
  // for (const auto& sym : symbols) {
  //   std::cout << std::hex << "{0x" << sym.addr << ", 0x" << sym.size << ", \"" << sym.name
  //             << "\", \"" << sym.compile_unit << "\", \"" << sym.section << "\"}," << std::endl;
  // }

  ASSERT_EQ(symbols.size(), goldens.size());
  for (size_t i = 0; i < symbols.size(); i++) {
    ASSERT_EQ(symbols[i].addr, goldens[i].addr);
    ASSERT_EQ(symbols[i].size, goldens[i].size);
    ASSERT_EQ(symbols[i].name, goldens[i].name);
    ASSERT_EQ(symbols[i].compile_unit, goldens[i].compile_unit);
    ASSERT_EQ(symbols[i].section, goldens[i].section);
  }
}
