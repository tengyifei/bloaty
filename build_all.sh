#!/usr/bin/env bash

set -xeuf -o pipefail

pushd "${0%/*}"

pushd third_party/rustfilt
cargo build --workspace --release
popd

rm -rf build
cmake . -DCMAKE_EXPORT_COMPILE_COMMANDS=On -DCMAKE_BUILD_TYPE=Release -Bbuild
make -C build -j
make -C build -j test

popd
