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

#include "bloaty.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

TEST(LinkMapTest, Empty) {
  const std::string link_map = "     VMA      LMA     Size Align Out     In      Symbol\n";
  auto symbols = bloaty_link_map::ParseLldLinkMap(link_map);
  ASSERT_EQ(symbols.size(), 0);
}

TEST(LinkMapTest, ExampleCpp) {
  std::string link_map = R"(
             VMA              LMA     Size Align Out     In      Symbol
             2a8              2a8        8     1 .interp
             2a8              2a8        8     1         <internal>:(.interp)
             2b0              2b0       18     4 .note.gnu.build-id
             2b0              2b0       18     4         <internal>:(.note.gnu.build-id)
             2c8              2c8     12f0     8 .dynsym
             2c8              2c8     12f0     8         <internal>:(.dynsym)
            15b8             15b8       1c     8 .gnu.hash
            15b8             15b8       1c     8         <internal>:(.gnu.hash)
            15d8             15d8      1e0     8 .dynamic
            15d8             15d8      1e0     8         <internal>:(.dynamic)
            17b8             17b8     143c     1 .dynstr
            17b8             17b8     143c     1         <internal>:(.dynstr)
            2bf8             2bf8      540     8 .rela.dyn
            2bf8             2bf8      540     8         <internal>:(.rela.dyn)
            3138             3138      150     8 .relr.dyn
            3138             3138      150     8         <internal>:(.relr.dyn)
            3288             3288     10f8     8 .rela.plt
            3288             3288     10f8     8         <internal>:(.rela.plt)
            4380             4380     b88b    16 .rodata
            4380             4380       14     1         obj/zircon/system/uapp/blobfs/blobfs.main.cc.o:(.rodata.main)
            4380             4380        0     1                 $d.1
            4394             4394     8a18     1         <internal>:(.rodata)
            cdb0             cdb0       a8     8         <internal>:(.rodata)
            ce58             ce58        c     4         obj/out/default/fidling/gen/sdk/fidl/fuchsia.hardware.block/fuchsia.hardware.block_tables.fuchsia.hardware.block.fidl.tables.c.o:(.rodata.HandlefifonullableTable)
            ce58             ce58        0     1                 $d.45
            ce58             ce58        c     1                 HandlefifonullableTable
            ce64             ce64        c     4         obj/out/default/fidling/gen/sdk/fidl/fuchsia.hardware.block/fuchsia.hardware.block_tables.fuchsia.hardware.block.fidl.tables.c.o:(.rodata.HandlevmononnullableTable)
            ce64             ce64        0     1                 $d.46
            ce64             ce64        c     1                 HandlevmononnullableTable
            ce70             ce70        c     4         obj/out/default/fidling/gen/sdk/fidl/fuchsia.security.resource/fuchsia.security.resource_tables.fuchsia.security.resource.fidl.tables.c.o:(.rodata.HandleresourcenonnullableTable)
            ce70             ce70        0     1                 $d.5
            ce70             ce70        c     1                 HandleresourcenonnullableTable
            ce7c             ce7c        a     1         obj/out/default/fidling/gen/sdk/fidl/fuchsia.io/fuchsia/io/llcpp/fuchsia.io_llcpp.fidl.cc.o:(.rodata._ZN5llcpp7fuchsia2io8NodeInfo9reset_ptrEON4fidl12tracking_ptrIvEE)
            ce7c             ce7c        0     1                 $d.1175
            ce86             ce86       2b     1         obj/out/default/fidling/gen/sdk/fidl/fuchsia.io/fuchsia/io/llcpp/fuchsia.io_llcpp.fidl.cc.o:(.rodata._ZN4fidlL17kErrorWriteFailedE)
            ce86             ce86        0     1                 $d.1404
            ce86             ce86       2b     1                 fidl::kErrorWriteFailed
            ceb4             ceb4        c     4         obj/out/default/fidling/gen/sdk/fidl/fuchsia.io/fuchsia.io_tables.fuchsia.io.fidl.tables.c.o:(.rodata.Request15fuchsia_io_NodenonnullableTable)
            ceb4             ceb4        0     1                 $d.251
            ceb4             ceb4        c     1                 Request15fuchsia_io_NodenonnullableTable
            cec0             cec0        8     4         obj/out/default/fidling/gen/sdk/fidl/fuchsia.io/fuchsia.io_tables.fuchsia.io.fidl.tables.c.o:(.rodata.String4096nonnullableTable)
            cec0             cec0        0     1                 $d.253
            cec0             cec0        8     1                 String4096nonnullableTable
            d030             d030      100    16         <internal>:(.rodata)
            d130             d130        c     4         obj/out/default/fidling/gen/sdk/fidl/fuchsia.fs/fuchsia.fs_tables.fuchsia.fs.fidl.tables.c.o:(.rodata.HandleeventnonnullableTable)
            d130             d130        0     1                 $d.22
            d130             d130        c     1                 HandleeventnonnullableTable
            d13c             d13c        8     4         obj/out/default/fidling/gen/sdk/fidl/fuchsia.fs/fuchsia.fs_tables.fuchsia.fs.fidl.tables.c.o:(.rodata.String32nonnullableTable)
            d13c             d13c        0     1                 $d.23
            d13c             d13c        8     1                 String32nonnullableTable
            fb38             fb38       9c     4         obj/zircon/public/lib/lz4/liblz4.a(liblz4.lz4hc.c.o):(.rodata.LZ4HC_compress_generic_internal.clTable)
            fb38             fb38        0     1                 $d.39
            fb38             fb38       9c     1                 LZ4HC_compress_generic_internal.clTable
            fbd4             fbd4       10     1         obj/zircon/public/lib/lz4/liblz4.a(liblz4.xxhash.c.o):(.rodata.XXH32_finalize)
            fbd4             fbd4        0     1                 $d.22
            fbe4             fbe4       27     1         obj/zircon/system/ulib/trace-provider/libtrace-provider-with-fdio.a(libtrace-provider-with-fdio.fdio_connect.cc.o):(.rodata._ZL12kServicePath)
            fbe4             fbe4        0     1                 $d.1
            fbe4             fbe4       27     1                 kServicePath
            fc0c             fc0c     361c     4 .eh_frame_hdr
            fc0c             fc0c     361c     4         <internal>:(.eh_frame_hdr)
           13228            13228     d63c     8 .eh_frame
           13228            13228       14     1         /usr/local/google/home/yifeit/vg/out/default.zircon/user-arm64-clang.shlib/obj/system/ulib/c/crt1.Scrt1.cc.o:(.eh_frame+0x0)
           13260            13260       54     1         obj/zircon/system/uapp/blobfs/blobfs.main.cc.o:(.eh_frame+0x14)
           13320            13320       2c     1         obj/out/default/fidling/gen/sdk/fidl/fuchsia.security.resource/fuchsia/security/resource/llcpp/fuchsia.security.resource_llcpp.fidl.cc.o:(.eh_frame+0x34)
           207d8            207d8       1c     1         obj/zircon/system/ulib/trace-provider/libtrace-provider-with-fdio.a(libtrace-provider-with-fdio.utils.cc.o):(.eh_frame+0x14)
           20818            20818       14     1         ../../out/default.zircon/user-arm64-clang.shlib/obj/system/ulib/sync/libsync.a(sync._sources.completion.c.o):(.eh_frame+0x54)
           20848            20848       14     1         ../../prebuilt/third_party/clang/linux-x64/lib/clang/11.0.0/lib/aarch64-unknown-fuchsia/libclang_rt.builtins.a(udivmodti4.c.obj):(.eh_frame+0x14)
           21000            21000    e84f0     8 .text
           21000            21000       14     8         /usr/local/google/home/yifeit/vg/out/default.zircon/user-arm64-clang.shlib/obj/system/ulib/c/crt1.Scrt1.cc.o:(.text._start)
           21000            21000        0     1                 $x.0
           21000            21000       14     1                 _start
           21014            21014      494     4         obj/zircon/system/uapp/blobfs/blobfs.main.cc.o:(.text.main)
           21014            21014        0     1                 $x.0
           21014            21014      494     1                 main
           3a04c            3a04c        8     4         obj/zircon/system/ulib/blobfs/libblobfs.a(libblobfs.blobfs.cc.o):(.text._ZThn8_N6blobfs18TransactionManagerD1Ev)
           3a04c            3a04c        0     1                 $x.83
           3a04c            3a04c        8     1                 non-virtual thunk to blobfs::TransactionManager::~TransactionManager()
          108120           108120       dc     4         obj/zircon/system/ulib/trace-provider/libtrace-provider-with-fdio.a(libtrace-provider-with-fdio.session.cc.o):(.text._ZN5trace8internal7SessionD2Ev)
          108120           108120        0     1                 $x.1
          108120           108120       dc     1                 trace::internal::Session::~Session()
               0                0   d41f13     1 .debug_loc
               0                0       39     1         /usr/local/google/home/yifeit/vg/out/default.zircon/user-arm64-clang.shlib/obj/system/ulib/c/crt1.Scrt1.cc.o:(.debug_loc)
               0                0        0     1                 $d.1         108120           108120       dc     1                 trace::internal::Session::~Session()
          d411ac           d411ac      458     1         ../../out/default.zircon/user-arm64-clang.shlib/obj/system/ulib/sync/libsync.a(sync._sources.completion.c.o):(.debug_loc)
          d411ac           d411ac        0     1                 $d.6
          d41604           d41604       50     1         ../../prebuilt/third_party/clang/linux-x64/lib/clang/11.0.0/lib/aarch64-unknown-fuchsia/libclang_rt.builtins.a(udivti3.c.obj):(.debug_loc)
          d41604           d41604        0     1                 $d.1
          d41654           d41654      8bf     1         ../../prebuilt/third_party/clang/linux-x64/lib/clang/11.0.0/lib/aarch64-unknown-fuchsia/libclang_rt.builtins.a(udivmodti4.c.obj):(.debug_loc)
          d41654           d41654        0     1                 $d.1
               0                0    52a34     1 .debug_abbrev
               0                0       6d     1         /usr/local/google/home/yifeit/vg/out/default.zircon/user-arm64-clang.shlib/obj/system/ulib/c/crt1.Scrt1.cc.o:(.debug_abbrev)
               0                0        0     1                 $d.2
             7fa              7fa      93f     1         obj/out/default/fidling/gen/sdk/fidl/fuchsia.security.resource/fuchsia/security/resource/llcpp/fuchsia.security.resource_llcpp.fidl.cc.o:(.debug_abbrev)
             7fa              7fa        0     1                 $d.40
          3323a0           3323a0       30     1         ../../prebuilt/third_party/clang/linux-x64/lib/clang/11.0.0/lib/aarch64-unknown-fuchsia/libclang_rt.builtins.a(udivmodti4.c.obj):(.debug_ranges)
          3323a0           3323a0        0     1                 $d.4
               0                0    2ed88     8 .symtab
               0                0    2ed88     8         <internal>:(.symtab)
               0                0      127     1 .shstrtab
               0                0      127     1         <internal>:(.shstrtab)
               0                0    3b51c     1 .strtab
               0                0    3b51c     1         <internal>:(.strtab)
)";
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
  std::string link_map = R"(
             VMA              LMA     Size Align Out     In      Symbol
             2e0              2e0        8     1 .interp
             2e0              2e0        8     1         <internal>:(.interp)
             2e8              2e8       18     4 .note.gnu.build-id
             2e8              2e8       18     4         <internal>:(.note.gnu.build-id)
             300              300      ac8     8 .dynsym
             300              300      ac8     8         <internal>:(.dynsym)
             dc8              dc8       1c     8 .gnu.hash
             dc8              dc8       1c     8         <internal>:(.gnu.hash)
             de8              de8      170     8 .dynamic
             de8              de8      170     8         <internal>:(.dynamic)
             f58              f58      821     1 .dynstr
             f58              f58      821     1         <internal>:(.dynstr)
            1780             1780       48     8 .rela.dyn
            1780             1780       48     8         <internal>:(.rela.dyn)
            17c8             17c8      5b8     8 .relr.dyn
            17c8             17c8      5b8     8         <internal>:(.relr.dyn)
            1d80             1d80      a80     8 .rela.plt
            1d80             1d80      a80     8         <internal>:(.rela.plt)
            2800             2800    38445    16 .rodata
            2800             2800       27     1         ./exe.unstripped/component_manager.alloc-54127f36ba192482.alloc.4k1iwrm2-cgu.0.rcgu.o.rcgu.o:(.rodata..Lanon.745d157954bc19b57d1f47812cead630.3)
            2800             2800        0     1                 $d.27
            2828             2828        0     8         ./exe.unstripped/component_manager.alloc-54127f36ba192482.alloc.4k1iwrm2-cgu.0.rcgu.o.rcgu.o:(.rodata..Lanon.745d157954bc19b57d1f47812cead630.5)
            2828             2828       33     1         ./exe.unstripped/component_manager.alloc-54127f36ba192482.alloc.4k1iwrm2-cgu.0.rcgu.o.rcgu.o:(.rodata..Lanon.745d157954bc19b57d1f47812cead630.13)
            2828             2828        0     1                 $d.30
            2885             2885       11     1         ./exe.unstripped/component_manager.alloc-54127f36ba192482.alloc.4k1iwrm2-cgu.0.rcgu.o.rcgu.o:(.rodata.anon.745d157954bc19b57d1f47812cead630.19.llvm.1460520995096272372)
            2885             2885        0     1                 $d.34
            2885             2885       11     1                 anon.745d157954bc19b57d1f47812cead630.19.llvm.1460520995096272372
            2896             2896        3     1         ./exe.unstripped/component_manager.alloc-54127f36ba192482.alloc.4k1iwrm2-cgu.0.rcgu.o.rcgu.o:(.rodata..Lanon.745d157954bc19b57d1f47812cead630.23)
            2896             2896        0     1                 $d.36
            2899             2899       16     1         ./exe.unstripped/component_manager.alloc-54127f36ba192482.alloc.4k1iwrm2-cgu.0.rcgu.o.rcgu.o:(.rodata..Lanon.745d157954bc19b57d1f47812cead630.26)
            2899             2899        0     1                 $d.37
            292a             292a        d     1         ./exe.unstripped/component_manager.alloc-54127f36ba192482.alloc.4k1iwrm2-cgu.0.rcgu.o.rcgu.o:(.rodata.anon.745d157954bc19b57d1f47812cead630.56.llvm.1460520995096272372)
            292a             292a        0     1                 $d.57
            292a             292a        d     1                 anon.745d157954bc19b57d1f47812cead630.56.llvm.1460520995096272372
            2937             2937        5     1         ./exe.unstripped/component_manager.alloc-54127f36ba192482.alloc.4k1iwrm2-cgu.0.rcgu.o.rcgu.o:(.rodata.anon.745d157954bc19b57d1f47812cead630.57.llvm.1460520995096272372)
            2937             2937        0     1                 $d.58
            2937             2937        5     1                 anon.745d157954bc19b57d1f47812cead630.57.llvm.1460520995096272372
            293c             293c        5     1         ./exe.unstripped/component_manager.alloc-54127f36ba192482.alloc.4k1iwrm2-cgu.0.rcgu.o.rcgu.o:(.rodata.anon.745d157954bc19b57d1f47812cead630.59.llvm.1460520995096272372)
            293c             293c        0     1                 $d.60
            293c             293c        5     1                 anon.745d157954bc19b57d1f47812cead630.59.llvm.1460520995096272372
            2950             2950      a90    16         <internal>:(.rodata)
            33e0             33e0       29     1         ./exe.unstripped/component_manager.backtrace-5619ad41e613872b.backtrace.e1f4vorm-cgu.0.rcgu.o.rcgu.o:(.rodata..Lanon.256afffd819e3254289014381fd650e5.2)
            33e0             33e0        0     1                 $d.20
            346d             346d        0     1         ./exe.unstripped/component_manager.backtrace-5619ad41e613872b.backtrace.e1f4vorm-cgu.0.rcgu.o.rcgu.o:(.rodata..Lanon.256afffd819e3254289014381fd650e5.14)
            3470             3470       38     8         ./exe.unstripped/component_manager.backtrace-5619ad41e613872b.backtrace.e1f4vorm-cgu.0.rcgu.o.rcgu.o:(.rodata..Lanon.256afffd819e3254289014381fd650e5.16)
            3470             3470        0     1                 $d.25
            34a8             34a8     1b08     4         <internal>:(.rodata)
            4fb0             4fb0        3     1         ./exe.unstripped/component_manager.backtrace-5619ad41e613872b.backtrace.e1f4vorm-cgu.0.rcgu.o.rcgu.o:(.rodata..Lanon.256afffd819e3254289014381fd650e5.18)
            4fb0             4fb0        0     1                 $d.27
            5070             5070     2780     8         <internal>:(.rodata)
            77f0             77f0        6     1         ./exe.unstripped/component_manager.backtrace-5619ad41e613872b.backtrace.e1f4vorm-cgu.0.rcgu.o.rcgu.o:(.rodata..Lanon.256afffd819e3254289014381fd650e5.26)
            77f0             77f0        0     1                 $d.34
            77f8             77f8      118     8         ./exe.unstripped/component_manager.backtrace-5619ad41e613872b.backtrace.e1f4vorm-cgu.0.rcgu.o.rcgu.o:(.rodata..Lanon.256afffd819e3254289014381fd650e5.28)
            77f8             77f8        0     1                 $d.36
            7910             7910        c     1         ./exe.unstripped/component_manager.backtrace-5619ad41e613872b.backtrace.e1f4vorm-cgu.0.rcgu.o.rcgu.o:(.rodata.anon.256afffd819e3254289014381fd650e5.29.llvm.13894642946385378479)
            7910             7910        0     1                 $d.37
            7910             7910        c     1                 anon.256afffd819e3254289014381fd650e5.29.llvm.13894642946385378479
            791c             791c        6     1         ./exe.unstripped/component_manager.backtrace-5619ad41e613872b.backtrace.e1f4vorm-cgu.0.rcgu.o.rcgu.o:(.rodata.anon.256afffd819e3254289014381fd650e5.30.llvm.13894642946385378479)
            791c             791c        0     1                 $d.38
            791c             791c        6     1                 anon.256afffd819e3254289014381fd650e5.30.llvm.13894642946385378479
            7922             7922       10     1         ./exe.unstripped/component_manager.component_manager.7rcbfp3g-cgu.0.rcgu.o:(.rodata._RINvNtCs6QHBvYLDz7W_4core3ptr13drop_in_placeNtCsbDqzXfLQacH_15fidl_fuchsia_io11FileRequestECs4fqI2P2rA04_17component_manager)
            7922             7922        0     1                 $d.13
            7950             7950       b1     2         ./exe.unstripped/component_manager.component_manager.7rcbfp3g-cgu.0.rcgu.o:(.rodata._RNvXsc_NtCsbDqzXfLQacH_17fuchsia_component6serverINtB5_9ServiceFsINtNtB5_7service10ServiceObjuEENtNtCseXumDWplyBO_12futures_core6stream6Stream9poll_nextCs4fqI2P2rA04_17component_manager)
            7950             7950        0     1                 $d.44
            7a08             7a08      380     8         <internal>:(.rodata)
            7d88             7d88       2b     1         ./exe.unstripped/component_manager.component_manager.7rcbfp3g-cgu.0.rcgu.o:(.rodata..Lanon.dda71605b8fec89c031783be146b7025.1)
            7d88             7d88        0     1                 $d.101
            8cf1             8cf1       4e     1         ./exe.unstripped/component_manager.component_manager.7rcbfp3g-cgu.7.rcgu.o:(.rodata..Lanon.8db4583f006f833985ea18d2ab89bab7.1)
            8cf1             8cf1        0     1                 $d.47
            8d3f             8d3f       1f     1         ./exe.unstripped/component_manager.component_manager.7rcbfp3g-cgu.7.rcgu.o:(.rodata.anon.8db4583f006f833985ea18d2ab89bab7.4.llvm.7664684806704912226)
            8d3f             8d3f        0     1                 $d.50
            8d3f             8d3f       1f     1                 anon.8db4583f006f833985ea18d2ab89bab7.4.llvm.7664684806704912226
            8d5e             8d5e       25     1         ./exe.unstripped/component_manager.component_manager.7rcbfp3g-cgu.8.rcgu.o:(.rodata..Lanon.a1c52c190a58d1713a23e7a7bdb0f9c0.2)
            8d5e             8d5e        0     1                 $d.17
            8d83             8d83        7     1         ./exe.unstripped/component_manager.component_manager.7rcbfp3g-cgu.9.rcgu.o:(.rodata._RINvNtCs6QHBvYLDz7W_4core3ptr13drop_in_placeINtNvNtB4_6future14from_generator9GenFutureNCNvMNtNtNtCsbDqzXfLQacH_21component_manager_lib5model6events6sourceNtB1s_11EventSource3new0EECs4fqI2P2rA04_17component_manager)
            8d83             8d83        0     1                 $d.56
            8d8a             8d8a        8     1         ./exe.unstripped/component_manager.component_manager.7rcbfp3g-cgu.9.rcgu.o:(.rodata._RINvNtCs6QHBvYLDz7W_4core3ptr13drop_in_placeINtNvNtB4_6future14from_generator9GenFutureNCNvMs2_NtNtCsbDqzXfLQacH_21component_manager_lib5model5realmNtB1v_5Realm19lock_resolved_state0EECs4fqI2P2rA04_17component_manager)
            8d8a             8d8a        0     1                 $d.59
            8d92             8d92       16     1         ./exe.unstripped/component_manager.component_manager.7rcbfp3g-cgu.9.rcgu.o:(.rodata._RINvNtCs6QHBvYLDz7W_4core3ptr13drop_in_placeINtNvNtB4_6future14from_generator9GenFutureNCNvMs_NtCsbDqzXfLQacH_21component_manager_lib19builtin_environmentNtB1u_18BuiltinEnvironment3new0EECs4fqI2P2rA04_17component_manager)
            8d92             8d92        0     1                 $d.64
            8da8             8da8        4     1         ./exe.unstripped/component_manager.component_manager.7rcbfp3g-cgu.9.rcgu.o:(.rodata._RINvNtCs6QHBvYLDz7W_4core3ptr13drop_in_placeNtCsbDqzXfLQacH_7cm_rust10ExposeDeclECs4fqI2P2rA04_17component_manager)
            8da8             8da8        0     1                 $d.74
            8dac             8dac        6     1         ./exe.unstripped/component_manager.component_manager.7rcbfp3g-cgu.9.rcgu.o:(.rodata._RINvNtCs6QHBvYLDz7W_4core3ptr13drop_in_placeNtCsbDqzXfLQacH_7cm_rust7UseDeclECs4fqI2P2rA04_17component_manager)
            8dac             8dac        0     1                 $d.81
            91e3             91e3        4     1         ./exe.unstripped/component_manager.core-6a4b1e7af979d229.core.3ocx6h1n-cgu.0.rcgu.o.rcgu.o:(.rodata._ZN4core3fmt9Formatter3pad17h9c055cdb18c92cc1E)
            91e3             91e3        0     1                 $d.54
            91e7             91e7        4     1         ./exe.unstripped/component_manager.core-6a4b1e7af979d229.core.3ocx6h1n-cgu.0.rcgu.o.rcgu.o:(.rodata._ZN4core3fmt9Formatter19pad_formatted_parts17heb4f002989281314E)
            91e7             91e7        0     1                 $d.56
            9214             9214       29     1         ./exe.unstripped/component_manager.core-6a4b1e7af979d229.core.3ocx6h1n-cgu.0.rcgu.o.rcgu.o:(.rodata._ZN41_$LT$char$u20$as$u20$core..fmt..Debug$GT$3fmt17hb14158a7386f6b1aE)
            9214             9214        0     1                 $d.73
            925c             925c       6f     1         ./exe.unstripped/component_manager.core-6a4b1e7af979d229.core.3ocx6h1n-cgu.0.rcgu.o.rcgu.o:(.rodata._ZN61_$LT$core..str..EscapeDebug$u20$as$u20$core..fmt..Display$GT$3fmt17hf1238c3db5de3de5E)
            925c             925c        0     1                 $d.130
            92d0             92d0        0     8         ./exe.unstripped/component_manager.core-6a4b1e7af979d229.core.3ocx6h1n-cgu.0.rcgu.o.rcgu.o:(.rodata.anon.6741f024a682c8cb8e59ecab4fc7e9ed.12.llvm.9325873315546439775)
            92d0             92d0        0     1                 anon.6741f024a682c8cb8e59ecab4fc7e9ed.12.llvm.9325873315546439775
            92d0             92d0        1     1         ./exe.unstripped/component_manager.core-6a4b1e7af979d229.core.3ocx6h1n-cgu.0.rcgu.o.rcgu.o:(.rodata..Lanon.6741f024a682c8cb8e59ecab4fc7e9ed.14)
            92d0             92d0        0     1                 $d.133
            92d1             92d1       1d     1         ./exe.unstripped/component_manager.core-6a4b1e7af979d229.core.3ocx6h1n-cgu.0.rcgu.o.rcgu.o:(.rodata..Lanon.6741f024a682c8cb8e59ecab4fc7e9ed.27)
            92d1             92d1        0     1                 $d.134
            9344             9344       28     4         ./exe.unstripped/component_manager.core-6a4b1e7af979d229.core.3ocx6h1n-cgu.0.rcgu.o.rcgu.o:(.rodata._ZN4core3num7flt2dec8strategy6dragon5POW1017he881f1848ad745b6E)
            9344             9344        0     1                 $d.142
            9344             9344       28     1                 core::num::flt2dec::strategy::dragon::POW10::he881f1848ad745b6
            936c             936c       28     4         ./exe.unstripped/component_manager.core-6a4b1e7af979d229.core.3ocx6h1n-cgu.0.rcgu.o.rcgu.o:(.rodata._ZN4core3num7flt2dec8strategy6dragon8TWOPOW1017h8ee033ba72605fdeE)
            936c             936c        0     1                 $d.143
            936c             936c       28     1                 core::num::flt2dec::strategy::dragon::TWOPOW10::h8ee033ba72605fde
            9394             9394        8     4         ./exe.unstripped/component_manager.core-6a4b1e7af979d229.core.3ocx6h1n-cgu.0.rcgu.o.rcgu.o:(.rodata._ZN4core3num7flt2dec8strategy6dragon9POW10TO1617hd008c9b35d67c443E)
            9394             9394        0     1                 $d.144
            9394             9394        8     1                 core::num::flt2dec::strategy::dragon::POW10TO16::hd008c9b35d67c443
            939c             939c       10     4         ./exe.unstripped/component_manager.core-6a4b1e7af979d229.core.3ocx6h1n-cgu.0.rcgu.o.rcgu.o:(.rodata._ZN4core3num7flt2dec8strategy6dragon9POW10TO3217h94bb3ddee75ca692E)
            939c             939c        0     1                 $d.145
            939c             939c       10     1                 core::num::flt2dec::strategy::dragon::POW10TO32::h94bb3ddee75ca692
            93ac             93ac       1c     4         ./exe.unstripped/component_manager.core-6a4b1e7af979d229.core.3ocx6h1n-cgu.0.rcgu.o.rcgu.o:(.rodata._ZN4core3num7flt2dec8strategy6dragon9POW10TO6417h2607b29da45ca92bE)
            93ac             93ac        0     1                 $d.146
            93ac             93ac       1c     1                 core::num::flt2dec::strategy::dragon::POW10TO64::h2607b29da45ca92b
            ebf8             ebf8       28     1         ./exe.unstripped/component_manager.libcomponent_manager_lib.component_manager_lib.3a1fbbbh-cgu.2.rcgu.o.rcgu.o:(.rodata..Lanon.31de557278eced41e2a6dce0e33ed7ec.9)
            ebf8             ebf8        0     1                 $d.392
            ec20             ec20       1c     1         ./exe.unstripped/component_manager.libcomponent_manager_lib.component_manager_lib.3a1fbbbh-cgu.2.rcgu.o.rcgu.o:(.rodata..Lanon.31de557278eced41e2a6dce0e33ed7ec.15)
            ec20             ec20        0     1                 $d.393
            ec92             ec92       2e     1                 anon.31de557278eced41e2a6dce0e33ed7ec.20.llvm.11955293494338827311
            ecc0             ecc0       13     1         ./exe.unstripped/component_manager.libcomponent_manager_lib.component_manager_lib.3a1fbbbh-cgu.2.rcgu.o.rcgu.o:(.rodata.anon.31de557278eced41e2a6dce0e33ed7ec.33.llvm.13709288095524858756)
            ecc0             ecc0        0     1                 $d.400
            ecc0             ecc0       13     1                 anon.31de557278eced41e2a6dce0e33ed7ec.33.llvm.13709288095524858756
            ecd3             ecd3       4a     1         ./exe.unstripped/component_manager.libcomponent_manager_lib.component_manager_lib.3a1fbbbh-cgu.2.rcgu.o.rcgu.o:(.rodata..Lanon.31de557278eced41e2a6dce0e33ed7ec.34)
            ecd3             ecd3        0     1                 $d.401
           2e20c            2e20c        c     4         ./exe.unstripped/component_manager.libidna-5c3c4f537fb8313f-5c3c4f537fb8313f.idna.axcknxfw-cgu.15.rcgu.o.rcgu.o:(.rodata..Lanon.93519e6636017643a07f78e678f4f11e.3501)
           2e20c            2e20c        0     1                 $d.656
           2e218            2e218        c     4         ./exe.unstripped/component_manager.libidna-5c3c4f537fb8313f-5c3c4f537fb8313f.idna.axcknxfw-cgu.15.rcgu.o.rcgu.o:(.rodata..Lanon.93519e6636017643a07f78e678f4f11e.3502)
           2e218            2e218        0     1                 $d.657
           2e224            2e224        c     4         ./exe.unstripped/component_manager.libidna-5c3c4f537fb8313f-5c3c4f537fb8313f.idna.axcknxfw-cgu.15.rcgu.o.rcgu.o:(.rodata..Lanon.93519e6636017643a07f78e678f4f11e.3503)
           2e224            2e224        0     1                 $d.658
           2f4ca            2f4ca       1b     1         ./exe.unstripped/component_manager.libio_util.io_util.3a1fbbbh-cgu.3.rcgu.o.rcgu.o:(.rodata.anon.7cbed09a5c2e2961b1298b053de6d71a.14.llvm.4649089991547526960)
           2f4ca            2f4ca        0     1                 $d.33
           2f4ca            2f4ca       1b     1                 anon.7cbed09a5c2e2961b1298b053de6d71a.14.llvm.4649089991547526960
           2f4e5            2f4e5       28     1         ./exe.unstripped/component_manager.libio_util.io_util.3a1fbbbh-cgu.8.rcgu.o.rcgu.o:(.rodata..Lanon.2dc76abe0f971e71acd588990ffc8513.0)
           2f4e5            2f4e5        0     1                 $d.16
           2f50d            2f50d       2b     1         ./exe.unstripped/component_manager.liblibrary_loader.library_loader.3a1fbbbh-cgu.1.rcgu.o.rcgu.o:(.rodata..Lanon.6f9bafb4adb671d7ac9f8408e86ff31a.0)
           2f50d            2f50d        0     1                 $d.8
           3aacc            3aacc      134     2         /tmp/rustcL27BaP/libunwind-e14dabfa3d8ce62b.rlib(libunwind.o):(.rodata._ZN9libunwind17DwarfInstructionsINS_17LocalAddressSpaceENS_15Registers_arm64EE18evaluateExpressionEmRS1_RKS2_m)
           3aacc            3aacc        0     1                 $d.44
           3ac00            3ac00       39     1         /tmp/rustcL27BaP/libunwind-e14dabfa3d8ce62b.rlib(libunwind.o):(.rodata._ZN9libunwind10CFI_ParserINS_17LocalAddressSpaceEE8parseCIEERS1_mPNS2_8CIE_InfoE)
           3ac00            3ac00        0     1                 $d.51
           3ac39            3ac39        c     1         /tmp/rustcL27BaP/libunwind-e14dabfa3d8ce62b.rlib(libunwind.o):(.rodata._ZN9libunwind14EHHeaderParserINS_17LocalAddressSpaceEE17getTableEntrySizeEh)
           3ac39            3ac39        0     1                 $d.53
           3ac45            3ac45       22     1 .debug_gdb_scripts
           3ac45            3ac45       22     1         <internal>:(.debug_gdb_scripts)
           3ac68            3ac68    17624     4 .eh_frame_hdr
           3ac68            3ac68    17624     4         <internal>:(.eh_frame_hdr)
           52290            52290    503ac     8 .eh_frame
           52290            52290       14     1         /usr/local/google/home/yifeit/vg/out/default.zircon/user-arm64-clang.shlib/obj/system/ulib/c/crt1.Scrt1.cc.o:(.eh_frame+0x0)
           522a8            522a8       1c     1         /usr/local/google/home/yifeit/vg/out/default.zircon/user-arm64-clang.shlib/obj/system/ulib/c/crt1.Scrt1.cc.o:(.eh_frame+0x14)
           522c8            522c8       14     1         ./exe.unstripped/component_manager.alloc-54127f36ba192482.alloc.4k1iwrm2-cgu.0.rcgu.o.rcgu.o:(.eh_frame+0x14)
           522e0            522e0       34     1         ./exe.unstripped/component_manager.alloc-54127f36ba192482.alloc.4k1iwrm2-cgu.0.rcgu.o.rcgu.o:(.eh_frame+0x28)
           a2620            a2620       14     1         /tmp/rustcL27BaP/libunwind-e14dabfa3d8ce62b.rlib(libunwind.o):(.eh_frame+0x5ec)
           b2640            b2640   13c78c     8 .text
           b2640            b2640       14     8         /usr/local/google/home/yifeit/vg/out/default.zircon/user-arm64-clang.shlib/obj/system/ulib/c/crt1.Scrt1.cc.o:(.text._start)
           b2640            b2640        0     1                 $x.0
           b2640            b2640       14     1                 _start
           b2654            b2654       1c     4         ./exe.unstripped/component_manager.alloc-54127f36ba192482.alloc.4k1iwrm2-cgu.0.rcgu.o.rcgu.o:(.text._ZN42_$LT$$RF$T$u20$as$u20$core..fmt..Debug$GT$3fmt17h12b31caec1d188c4E)
           b2654            b2654        0     1                 $x.0
           b2654            b2654       1c     1                 _$LT$$RF$T$u20$as$u20$core..fmt..Debug$GT$::fmt::h12b31caec1d188c4
           b2670            b2670       b4     4         ./exe.unstripped/component_manager.alloc-54127f36ba192482.alloc.4k1iwrm2-cgu.0.rcgu.o.rcgu.o:(.text._ZN42_$LT$$RF$T$u20$as$u20$core..fmt..Debug$GT$3fmt17h268a568299531efbE)
           b2670            b2670        0     1                 $x.1
           b2670            b2670       b4     1                 _$LT$$RF$T$u20$as$u20$core..fmt..Debug$GT$::fmt::h268a568299531efb
           cd52c            cd52c       98     4         ./exe.unstripped/component_manager.core-6a4b1e7af979d229.core.3ocx6h1n-cgu.0.rcgu.o.rcgu.o:(.text._ZN4core3num52_$LT$impl$u20$core..str..FromStr$u20$for$u20$u64$GT$8from_str17he51bc12ed2f79990E)
           cd52c            cd52c        0     1                 $x.102
           cd52c            cd52c       98     1                 core::num::_$LT$impl$u20$core..str..FromStr$u20$for$u20$u64$GT$::from_str::he51bc12ed2f79990
           cd52c            cd52c       98     1                 core::num::_$LT$impl$u20$core..str..FromStr$u20$for$u20$usize$GT$::from_str::h4398f35a1c0b4be7
           cd5c4            cd5c4       e4     4         ./exe.unstripped/component_manager.core-6a4b1e7af979d229.core.3ocx6h1n-cgu.0.rcgu.o.rcgu.o:(.text._ZN63_$LT$core..num..TryFromIntError$u20$as$u20$core..fmt..Debug$GT$3fmt17h10047a113409f418E)
           cd5c4            cd5c4        0     1                 $x.103
           cd5c4            cd5c4       e4     1                 _$LT$core..num..TryFromIntError$u20$as$u20$core..fmt..Debug$GT$::fmt::h10047a113409f418
           cd6a8            cd6a8       34     4         ./exe.unstripped/component_manager.core-6a4b1e7af979d229.core.3ocx6h1n-cgu.0.rcgu.o.rcgu.o:(.text._ZN4core3fmt5float52_$LT$impl$u20$core..fmt..Display$u20$for$u20$f64$GT$3fmt17hac033855e28d5eb4E)
           cd6a8            cd6a8        0     1                 $x.104
           cd6a8            cd6a8       34     1                 core::fmt::float::_$LT$impl$u20$core..fmt..Display$u20$for$u20$f64$GT$::fmt::hac033855e28d5eb4
           e2554            e2554       10     4         ./exe.unstripped/component_manager.libcm_fidl_translator.cm_fidl_translator.3a1fbbbh-cgu.0.rcgu.o.rcgu.o:(.text.OUTLINED_FUNCTION_359)
           e2554            e2554        0     1                 $x.752
           e2554            e2554       10     1                 OUTLINED_FUNCTION_359
           e2564            e2564       10     4         ./exe.unstripped/component_manager.libcm_fidl_translator.cm_fidl_translator.3a1fbbbh-cgu.0.rcgu.o.rcgu.o:(.text.OUTLINED_FUNCTION_360)
           e2564            e2564        0     1                 $x.753
           e2564            e2564       10     1                 OUTLINED_FUNCTION_360
           eb888            eb888       40     4         ./exe.unstripped/component_manager.libcm_fidl_translator.cm_fidl_translator.3a1fbbbh-cgu.4.rcgu.o.rcgu.o:(.text._RINvNtNtCs6QHBvYLDz7W_4core4iter8adapters15process_resultsINtB2_3MapINtNtCsiEMmsMe2HZG_5alloc3vec8IntoIterNtNtCsbDqzXfLQacH_7cm_json2cm8ResolverENCNvXs_CsbDqzXfLQacH_18cm_fidl_translatorINtB17_3VecB1G_EINtB2q_6CmIntoIB2Z_NtCsbDqzXfLQacH_17fidl_fuchsia_sys212ResolverDeclEE7cm_into0EB3x_NtB1K_5ErrorNCINvXsx_NtB6_6resultINtB4V_6ResultB3s_B4A_EINtNtNtB4_6traits7collect12FromIteratorIB58_B3x_B4A_EE9from_iterBU_E0B3s_EB2q_)
           eb888            eb888        0     1                 $x.15
           eb888            eb888       40     1                 _RINvNtNtCs6QHBvYLDz7W_4core4iter8adapters15process_resultsINtB2_3MapINtNtCsiEMmsMe2HZG_5alloc3vec8IntoIterNtNtCsbDqzXfLQacH_7cm_json2cm8ResolverENCNvXs_CsbDqzXfLQacH_18cm_fidl_translatorINtB17_3VecB1G_EINtB2q_6CmIntoIB2Z_NtCsbDqzXfLQacH_17fidl_fuchsia_sys212ResolverDeclEE7cm_into0EB3x_NtB1K_5ErrorNCINvXsx_NtB6_6resultINtB4V_6ResultB3s_B4A_EINtNtNtB4_6traits7collect12FromIteratorIB58_B3x_B4A_EE9from_iterBU_E0B3s_EB2q_
          197340           197340       70     4         ./exe.unstripped/component_manager.libfuchsia_async.fuchsia_async.3a1fbbbh-cgu.1.rcgu.o.rcgu.o:(.text._RINvNtCsevZhND7KDbE_9hashbrown3raw16calculate_layoutTjINtNtCsiEMmsMe2HZG_5alloc4sync3ArcDNtNtCsbDqzXfLQacH_13fuchsia_async8executor14PacketReceiverEL_EEEB1t_)
          197340           197340        0     1                 $x.6
          197340           197340       70     1                 _RINvNtCsevZhND7KDbE_9hashbrown3raw16calculate_layoutTjINtNtCsiEMmsMe2HZG_5alloc4sync3ArcDNtNtCsbDqzXfLQacH_13fuchsia_async8executor14PacketReceiverEL_EEEB1t_
          200800           200800       38    32         ./exe.unstripped/component_manager.std-7a72ffd2f3a7a812.std.a0uwjzoe-cgu.0.rcgu.o.rcgu.o:(.tbss._ZN3std10sys_common11thread_info11THREAD_INFO7__getit5__KEY17h836843d10e59cadaE.llvm.8019338592007173243)
            1300             1300       38     1                 std::sys_common::thread_info::THREAD_INFO::__getit::__KEY::h836843d10e59cada (.llvm.8019338592007173243)
          200800           200800        0     1                 $d.517
          200840           200840       18    16         ./exe.unstripped/component_manager.std-7a72ffd2f3a7a812.std.a0uwjzoe-cgu.0.rcgu.o.rcgu.o:(.tbss._ZN3std9panicking18update_panic_count11PANIC_COUNT7__getit5__KEY17h69785e8fa31a63e6E.llvm.8019338592007173243)
            1340             1340       18     1                 std::panicking::update_panic_count::PANIC_COUNT::__getit::__KEY::h69785e8fa31a63e6 (.llvm.8019338592007173243)
          200840           200840        0     1                 $d.557
          1ff610           1ff610    15db8     8 .data.rel.ro
          1ff610           1ff610       30     8         ./exe.unstripped/component_manager.alloc-54127f36ba192482.alloc.4k1iwrm2-cgu.0.rcgu.o.rcgu.o:(.data.rel.ro..Lanon.745d157954bc19b57d1f47812cead630.1)
          1ff610           1ff610        0     1                 $d.25
          1ff640           1ff640       20     8         ./exe.unstripped/component_manager.alloc-54127f36ba192482.alloc.4k1iwrm2-cgu.0.rcgu.o.rcgu.o:(.data.rel.ro..Lanon.745d157954bc19b57d1f47812cead630.2)
          1ff640           1ff640        0     1                 $d.26
          1ff660           1ff660       18     8         ./exe.unstripped/component_manager.alloc-54127f36ba192482.alloc.4k1iwrm2-cgu.0.rcgu.o.rcgu.o:(.data.rel.ro..Lanon.745d157954bc19b57d1f47812cead630.4)
          1ff660           1ff660        0     1                 $d.28
          202a88           202a88       20     8         ./exe.unstripped/component_manager.libcm_fidl_validator.cm_fidl_validator.3a1fbbbh-cgu.0.rcgu.o.rcgu.o:(.data.rel.ro.anon.e57b3f2ae8b5861992cfe341e01c75d6.1.llvm.9882057474999025597)
          202a88           202a88        0     1                 $d.287
          202a88           202a88       20     1                 anon.e57b3f2ae8b5861992cfe341e01c75d6.1.llvm.9882057474999025597
          2110a8           2110a8       18     8         ./exe.unstripped/component_manager.liblibrary_loader.library_loader.3a1fbbbh-cgu.1.rcgu.o.rcgu.o:(.data.rel.ro..Lanon.6f9bafb4adb671d7ac9f8408e86ff31a.3)
          2110a8           2110a8        0     1                 $d.10
          211108           211108       10     8         ./exe.unstripped/component_manager.liblibrary_loader.library_loader.3a1fbbbh-cgu.1.rcgu.o.rcgu.o:(.data.rel.ro.anon.6f9bafb4adb671d7ac9f8408e86ff31a.9.llvm.14732610969729869220)
          211108           211108        0     1                 $d.16
          211108           211108       10     1                 anon.6f9bafb4adb671d7ac9f8408e86ff31a.9.llvm.14732610969729869220
          211118           211118       38     8         ./exe.unstripped/component_manager.liblibrary_loader.library_loader.3a1fbbbh-cgu.1.rcgu.o.rcgu.o:(.data.rel.ro.anon.6f9bafb4adb671d7ac9f8408e86ff31a.12.llvm.14732610969729869220)
          211118           211118        0     1                 $d.19
          211118           211118       38     1                 anon.6f9bafb4adb671d7ac9f8408e86ff31a.12.llvm.14732610969729869220
          211150           211150       20     8         ./exe.unstripped/component_manager.liblibrary_loader.library_loader.3a1fbbbh-cgu.10.rcgu.o.rcgu.o:(.data.rel.ro.anon.54c490f3ebdb496a1657b64fb6de7217.0.llvm.16392173952262690328)
          211150           211150        0     1                 $d.7
          211150           211150       20     1                 anon.54c490f3ebdb496a1657b64fb6de7217.0.llvm.16392173952262690328
          211170           211170       18     8         ./exe.unstripped/component_manager.liblibrary_loader.library_loader.3a1fbbbh-cgu.11.rcgu.o.rcgu.o:(.data.rel.ro..Lanon.39cf32991562f2df00749835baa6fdc6.1)
          211170           211170        0     1                 $d.19
          215018           215018       18     8         ./exe.unstripped/component_manager.std-7a72ffd2f3a7a812.std.a0uwjzoe-cgu.0.rcgu.o.rcgu.o:(.data.rel.ro.anon.d8e73c4ab2af7c41d0ca8802142f134e.759.llvm.8019338592007173243)
          215018           215018        0     1                 $d.556
          215018           215018       18     1                 anon.d8e73c4ab2af7c41d0ca8802142f134e.759.llvm.8019338592007173243
          215030           215030       88     8         /tmp/rustcL27BaP/libunwind-e14dabfa3d8ce62b.rlib(libunwind.o):(.data.rel.ro._ZTVN9libunwind12UnwindCursorINS_17LocalAddressSpaceENS_15Registers_arm64EEE)
          215030           215030        0     1                 $d.58
          215030           215030       88     1                 vtable for libunwind::UnwindCursor<libunwind::LocalAddressSpace, libunwind::Registers_arm64>
          2150b8           2150b8      310     8         /tmp/rustcL27BaP/libunwind-e14dabfa3d8ce62b.rlib(libunwind.o):(.data.rel.ro..Lswitch.table._ZN9libunwind15Registers_arm6415getRegisterNameEi)
          2150b8           2150b8        0     1                 $d.64
          2153c8           2153c8      480     8 .got
          2153c8           2153c8      480     8         <internal>:(.got)
          225848           225848       88     8 .data
          225848           225848       10     8         ./exe.unstripped/component_manager.liblog-51a8548556518bd3-51a8548556518bd3.log.7ncv9mgr-cgu.0.rcgu.o.rcgu.o:(.data._RNvCsi1CgODtnl2B_3log6LOGGER.llvm.13428694738827280366)
          225848           225848        0     1                 $d.13
          225848           225848       10     1                 _RNvCsi1CgODtnl2B_3log6LOGGER.llvm.13428694738827280366
          225858           225858        8     8         ./exe.unstripped/component_manager.std-7a72ffd2f3a7a812.std.a0uwjzoe-cgu.0.rcgu.o.rcgu.o:(.data._ZN3std6thread8ThreadId3new7COUNTER17hb3a413a29c0f81f6E)
          225858           225858        0     1                 $d.306
          225858           225858        8     1                 std::thread::ThreadId::new::COUNTER::hb3a413a29c0f81f6
          225860           225860       38     8         ./exe.unstripped/component_manager.std-7a72ffd2f3a7a812.std.a0uwjzoe-cgu.0.rcgu.o.rcgu.o:(.data._ZN3std2io5stdio6stderr8INSTANCE17hb24941292040ef06E)
          225860           225860        0     1                 $d.363
          225860           225860       38     1                 std::io::stdio::stderr::INSTANCE::hb24941292040ef06
          2258c8           2258c8        8     8         /tmp/rustcL27BaP/libunwind-e14dabfa3d8ce62b.rlib(libunwind.o):(.data._ZN9libunwind13DwarfFDECacheINS_17LocalAddressSpaceEE10_bufferEndE)
          2258c8           2258c8        0     1                 $d.63
          2258c8           2258c8        8     1                 libunwind::DwarfFDECache<libunwind::LocalAddressSpace>::_bufferEnd
          2258d0           2258d0      398     8 .got.plt
          2258d0           2258d0      398     8         <internal>:(.got.plt)
          225c68           225c68      e5d     8 .bss
          225c68           225c68       60     8         ./exe.unstripped/component_manager.libcm_rust.cm_rust.3a1fbbbh-cgu.0.rcgu.o.rcgu.o:(.bss..L_MergedGlobals)
          225c68           225c68        0     1                 $d.378
          225c68           225c68       20     1                 _RNvNvNvXs15_CsbDqzXfLQacH_7cm_rustNtBa_13DATA_TYPENAMENtNtNtCs6QHBvYLDz7W_4core3ops5deref5Deref5deref11___stability4LAZY.llvm.2893117974861896834
          225c88           225c88       20     1                 _RNvNvNvXs17_CsbDqzXfLQacH_7cm_rustNtBa_14CACHE_TYPENAMENtNtNtCs6QHBvYLDz7W_4core3ops5deref5Deref5deref11___stability4LAZY.llvm.2893117974861896834
          225ca8           225ca8       20     1                 _RNvNvNvXs19_CsbDqzXfLQacH_7cm_rustNtBa_13META_TYPENAMENtNtNtCs6QHBvYLDz7W_4core3ops5deref5Deref5deref11___stability4LAZY.llvm.2893117974861896834
          225cc8           225cc8       38     8         ./exe.unstripped/component_manager.libcomponent_manager_lib.component_manager_lib.3a1fbbbh-cgu.12.rcgu.o.rcgu.o:(.bss._RNvNvNvXs0_NtNtNtCsbDqzXfLQacH_21component_manager_lib5model6events14source_factoryNtB9_25EVENT_SOURCE_SERVICE_PATHNtNtNtCs6QHBvYLDz7W_4core3ops5deref5Deref5deref11___stability4LAZY.llvm.16599889146946869799)
          225cc8           225cc8        0     1                 $d.347
          225cc8           225cc8       38     1                 _RNvNvNvXs0_NtNtNtCsbDqzXfLQacH_21component_manager_lib5model6events14source_factoryNtB9_25EVENT_SOURCE_SERVICE_PATHNtNtNtCs6QHBvYLDz7W_4core3ops5deref5Deref5deref11___stability4LAZY.llvm.16599889146946869799
          225d00           225d00       38     8         ./exe.unstripped/component_manager.libcomponent_manager_lib.component_manager_lib.3a1fbbbh-cgu.12.rcgu.o.rcgu.o:(.bss._RNvNvNvXs2_NtNtNtCsbDqzXfLQacH_21component_manager_lib5model6events14source_factoryNtB9_30EVENT_SOURCE_SYNC_SERVICE_PATHNtNtNtCs6QHBvYLDz7W_4core3ops5deref5Deref5deref11___stability4LAZY.llvm.16599889146946869799)
          225d00           225d00        0     1                 $d.348
          225d00           225d00       38     1                 _RNvNvNvXs2_NtNtNtCsbDqzXfLQacH_21component_manager_lib5model6events14source_factoryNtB9_30EVENT_SOURCE_SYNC_SERVICE_PATHNtNtNtCs6QHBvYLDz7W_4core3ops5deref5Deref5deref11___stability4LAZY.llvm.16599889146946869799
          2262ac           2262ac        8     4         /tmp/rustcL27BaP/libunwind-e14dabfa3d8ce62b.rlib(libunwind.o):(.bss._ZN9libunwind13DwarfFDECacheINS_17LocalAddressSpaceEE5_lockE)
          2262ac           2262ac        0     1                 $d.59
          2262ac           2262ac        8     1                 libunwind::DwarfFDECache<libunwind::LocalAddressSpace>::_lock
          2262b8           2262b8      800     8         /tmp/rustcL27BaP/libunwind-e14dabfa3d8ce62b.rlib(libunwind.o):(.bss._ZN9libunwind13DwarfFDECacheINS_17LocalAddressSpaceEE14_initialBufferE)
          2262b8           2262b8        0     1                 $d.62
          2262b8           2262b8      800     1                 libunwind::DwarfFDECache<libunwind::LocalAddressSpace>::_initialBuffer
          226ab8           226ab8        d     4         /tmp/rustcL27BaP/libunwind-e14dabfa3d8ce62b.rlib(libunwind.o):(.bss..L_MergedGlobals)
          226ab8           226ab8        0     1                 $d.65
          226ab8           226ab8        1     1                 logAPIs::checked
          226abc           226abc        1     1                 logAPIs::log
          226abd           226abd        1     1                 logUnwinding::checked
          226ac0           226ac0        1     1                 logUnwinding::log
          226ac1           226ac1        1     1                 logDWARF::checked
          226ac4           226ac4        1     1                 logDWARF::log
               0                0   48c441     1 .debug_loc
               0                0       39     1         /usr/local/google/home/yifeit/vg/out/default.zircon/user-arm64-clang.shlib/obj/system/ulib/c/crt1.Scrt1.cc.o:(.debug_loc)
               0                0        0     1                 $d.1
              39               39     6d97     1         ./exe.unstripped/component_manager.alloc-54127f36ba192482.alloc.4k1iwrm2-cgu.0.rcgu.o.rcgu.o:(.debug_loc)
              39               39        0     1                 $d.63
            6dd0             6dd0     280c     1         ./exe.unstripped/component_manager.backtrace-5619ad41e613872b.backtrace.e1f4vorm-cgu.0.rcgu.o.rcgu.o:(.debug_loc)
            6dd0             6dd0        0     1                 $d.41
)";
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
      {0xe2554, 0x10, "** outlined function",
       "./exe.unstripped/"
       "component_manager.libcm_fidl_translator.cm_fidl_translator.3a1fbbbh-cgu.0.rcgu.o.rcgu.o",
       ".text"},
      {0xe2564, 0x10, "** outlined function",
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
