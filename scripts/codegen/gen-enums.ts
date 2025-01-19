/*
    Code generator for code and derived specs out of the json schemas representing enumerations.
    Use `quick` as optional arg to avoid generating the meson file again.  
    This will speed up compilation considerably and is fine as long as you have not added or removed files in the schema folder.
*/

import type { Static } from '@sinclair/typebox';
import { $ } from 'bun'
import { Glob } from "bun";
import { parse } from "node:path"
import { enum_schema } from './schemas/enum.schema';
import { Value } from '@sinclair/typebox/value';

import { render, JSXXML } from 'jsx-xml'

await Bun.write('./commons/schemas/enum.schema.json', JSON.stringify(enum_schema, null, 4))