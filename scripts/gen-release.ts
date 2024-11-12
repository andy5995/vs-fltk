#!/bin/env bun
//TODO: To be deprecated in favour of full meson integration

import { $ } from "bun"
/**
 * Prepare a binary release for the project in ./dist.
 */


const RELEASE_TYPE: 'linux' | 'windows' | 'macos' = 'linux'

await $`meson compile -C build/ vs:executable`
await $`rm -rf ./dist/`
await $`mkdir ./dist/`
await $`cp -r  ./bindings ./dist/`
await $`cp -r  ./docs ./dist/`
await $`cp -r  ./commons  ./dist/`
await $`cp -r  ./build/vs ./dist/`
if (RELEASE_TYPE == "linux") {
    await $`find . -name '*.so' -exec rsync -Rl {} ../dist/ \;`.cwd('./build')
    await $`find . -name '*.so.*' ! -name '*.symbols' -exec rsync -Rl {} ../dist/ \;`.cwd('./build')
}
else {
    throw `Release [${RELEASE_TYPE}] not supported`
}
await $`cp -r  -p ./subprojects/libtcc/include ./dist/subprojects/libtcc/`
await $`cp -r ./include ./dist/`
await $`cp ./LICENCE.md ./dist/`
await $`cp ./README.md ./dist/`
await $`cp ./RELEASE.md ./dist/`
