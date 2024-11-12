#!/bin/env bun
/*
    Single script to perform all codegen task in one single step.
*/


/*
    Export the information from the package.json into a header file to be consumed during compilation.
    The json file is considered as authoritative for all metadata which is related to this project.
*/
import pkg from "@/package.json"
await Bun.write("./include/version.hpp",
    `#pragma once
inline const char* vs_version(){return "${pkg.version}";}
`)


// Components automatic codegen
import "./gen-components"
import { $ } from "bun"

await $`bun drizzle-kit generate && bun drizzle-kit push`;