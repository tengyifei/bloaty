#!/usr/bin/env bash

set -xeuf -o pipefail

pushd "${0%/*}"

./build_rustc_demangle.sh

rm -rf build
# Mimic the LUCI environment defined in
# https://fuchsia.googlesource.com/infra/recipes/+/refs/heads/master/recipes/bloaty.py#59
export CC=`which clang`
export CXX=`which clang++`
cmake . -DCMAKE_EXPORT_COMPILE_COMMANDS=On -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXE_LINKER_FLAGS='-static-libstdc++ -ldl -lpthread' -Bbuild
make -C build -j
make -C build -j test

popd
