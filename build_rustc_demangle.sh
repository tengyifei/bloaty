#!/usr/bin/env bash

set -xeuf -o pipefail

pushd "${0%/*}"

pushd third_party/rustc-demangle
cargo build -p rustc-demangle-capi --release
# Rust uses libunwind, and embeds the libunwind runtime inside any static
# library produced from the build.
# Bloaty uses C++ exceptions to trigger failure. Use of C++ exceptions also
# requires libunwind, but specifically the version coming with GCC/Clang.
# Their interaction causes strange failures when compiled using GCC 9.3.
# To fix this, take out the unwind-related members from the Rust static lib.
ar d target/release/librustc_demangle.a Unwind-EHABI.o Unwind-seh.o Unwind-sjlj.o UnwindLevel1-gcc-ext.o UnwindLevel1.o UnwindRegistersRestore.o UnwindRegistersSave.o libunwind.o
popd

popd
