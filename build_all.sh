#!/usr/bin/env bash

set -xeuf -o pipefail

pushd third_party/rustfilt
cargo build --workspace --release
popd

cmake . -DCMAKE_EXPORT_COMPILE_COMMANDS=On -DCMAKE_BUILD_TYPE=RelWithDebInfo -Bbuild
make -C build -j
make -C build -j test

