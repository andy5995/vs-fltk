/*
    Alternative take on the generation process for components from specs. Each is now its own header/cpp pair instead of a messy one.
*/

import type { Static } from '@sinclair/typebox';
import { $ } from 'bun'
import { Glob } from "bun";
import { parse } from "node:path"
import { widget_schema } from './components.schema';
import { Value } from '@sinclair/typebox/value';

import { render, JSXXML } from 'jsx-xml'


function gen_cpp(data: Static<typeof widget_schema>) {
    let class_decl = ``;
    let class_impl = ``;
    let parser_selector = ``;

    return [class_decl, class_impl, parser_selector]
}

function gen_d_ts(data: Static<typeof widget_schema>) { }

function gen_xsd(data: Static<typeof widget_schema>) { }

function gen_xml_editor(data: Static<typeof widget_schema>) {
    return <component><props></props><computed></computed></component>
}


await $`rm -rf ./src/components/autogen/`
await $`mkdir -p ./src/components/autogen/`

await $`rm -rf ./include/components/autogen/`
await $`mkdir -p ./include/components/autogen/`

await $`rm -rf ./commons/schemas/components/`
await $`mkdir -p ./commons/schemas/components/`

const glob = new Glob("./schemas/components/**/*.json");


const cpp_files = []
for await (const file of glob.scan(".")) {
    const tmp = Value.Default(widget_schema, (await Bun.file(file).json()))
    if (!Value.Check(widget_schema, tmp)) {
        console.warn(`[SKIP] ${file} since its syntax is not comformant`);
        for (const i of [...Value.Errors(widget_schema, tmp)]) {
            console.error(i.path, i.message);
        }
    }
    else {
        const namepieces = parse(file).name.split(':');
        tmp.ns ??= namepieces[0]
        tmp.name ??= namepieces[1]

        //Cpp code
        cpp_files.push(`./src/components/autogen/${tmp.ns}/${tmp.name}.cpp`)
        {
            const ret = gen_cpp(tmp);
            if (tmp.use_main_header === null) {
                await $`mkdir -p ./include/components/autogen/${tmp.ns}/`
                await Bun.write(`./include/components/autogen/${tmp.ns}/${tmp.name}.hpp`, ret[0])
            }
            await $`mkdir -p ./src/components/autogen/${tmp.ns}/`
            await Bun.write(`./src/components/autogen/${tmp.ns}/${tmp.name}.cpp`, ret[1])
            await Bun.write('./src/ui.xml-widgets.autogen.cpp', ret[2])
        }

        //XML Model for the Editor
        {
            const ret = gen_xml_editor(tmp);
            await $`mkdir -p ./commons/schemas/components/${tmp.ns}/`
            await Bun.write(`./commons/schemas/components/${tmp.ns}/${tmp.name}.xml`, render(ret))
        }

        //XML Schema for aid when writing code
        //TODO:

        //Typescript types
        //TODO:

        console.log(`[DONE] ${parse(file).base}`)
    }
}

await Bun.write('./src/components/autogen/meson.build', `autogen_components = [${cpp_files.map(x => `'${x}'`)}]`)
