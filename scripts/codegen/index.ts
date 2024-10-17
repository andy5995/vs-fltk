#!/bin/env bun
import pkg from "@/package.json"
await Bun.write("./include/version.hpp",
    `#pragma once
inline const char* vs_version(){return "${pkg.version}";}
`)

//import "./gen-components"
import "./gen-components.standalone"