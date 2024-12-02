#!/bin/bash

set -e
$WORKSPACE/docker/entry-common.sh

su builder -c ". ~/.profile && bash -c '\
  cd $WORKSPACE && \
  bun install &&
  bun run codegen &&
  bun run meson-setup.clang-release &&
  meson compile -C build/ vs:executable
  '"
