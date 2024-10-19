/*
    Alternative take on the generation process for components from specs. Each is now its own header/cpp pair instead of a messy one.
    Use quick as optional arg to avoid generating the meson file again, speeding up compilation. Useful when no additions of new components is made, and you want to test only changes in code.
*/

import type { Static } from '@sinclair/typebox';
import { $ } from 'bun'
import { Glob } from "bun";
import { parse } from "node:path"
import { widget_schema } from './components.schema';
import { Value } from '@sinclair/typebox/value';

import { render, JSXXML } from 'jsx-xml'
import Description from '@/subprojects/wamr/tests/fuzz/wasm-mutator-fuzz/portal/src/Descrpition';


function gen_cpp(data: Static<typeof widget_schema>) {
    let class_decl = data.use_main_header === null ? `
#pragma once

#include <ui.hpp>
${data.headers ? data.headers.map(x => `#include <${x}>\n`) : ``}

namespace vs{

class ${data.ns}_${data.name} : public ${data.extends}{
    public:
        template<typename... Args>
        ${data.ns}_${data.name}(ui_base* p,Args ...w):${data.extends}(p,w...){}
        virtual frame_type_t default_frame_type() override {return frame_type_t::LEAF;}
        virtual const char* class_name() override{return "${data.ns}:${data.name}";}

        virtual ~${data.ns}_${data.name}(){}

        virtual int apply_prop(const char* prop, const char* value) override {return ${data.ns}_${data.name}::_apply_prop(this,prop,value);}
        virtual int get_computed(const char* prop, const char ** value) override {return ${data.ns}_${data.name}::_get_computed(this,prop,value);};

    protected:
        static int _apply_prop(${data.ns}_${data.name}* that, const char* prop, const char* value);
        static int _get_computed(${data.ns}_${data.name}* that, const char* prop, const char** value);
};

}
    `:
        `#pragma once
#include <${data.use_main_header}>
`;

    let class_impl = `
#include <components/autogen/${data.ns}/${data.name}.hpp>

namespace vs{

int ${data.ns}_${data.name}::_apply_prop(${data.ns}_${data.name}* that, const char* prop, const char* value){
    auto& w = that->widget();
    bool ok = true;
    if(false){}
    ${Object.entries(data.props).map(x => {
        x[1].alias.push(x[0]);
        return x[1].alias.map(y => `
    ${x[1].description ? `//${x[1].description}` : ``}
    else if(strcmp(prop,"${y}")==0){
        ${x[1].code}
    }`).join('\n')
    }
    ).join('\n')
        }
    ${data.props_tail === null ? `else {return 2;}\nreturn ok?0:1;` : `if(!ok) return 1; else return ${data.props_tail ?? data.extends}::_apply_prop((${data.props_tail ?? data.extends}*)that,prop,value);`}
}

int ${data.ns}_${data.name}:: _get_computed(${data.ns}_${data.name} * that, const char* prop, const char** value) {
    auto& w = that->widget();
    bool ok = true;
    if(false){}
    ${Object.entries(data.computed).map(x => {
            x[1].alias.push(x[0]);
            return x[1].alias.map(y => `
    ${x[1].description ? `//${x[1].description}` : ``}
    else if(strcmp(prop,"${y}")==0){
        ${x[1].code}
    }`).join('\n')
        }
        ).join('\n')
        }
    ${data.props_tail === null ? `else {return 2;}\nreturn ok?0:1;` : `if(!ok) return 1; else return ${data.props_tail ?? data.extends}::_get_computed((${data.props_tail ?? data.extends}*)that,prop,value);`}
}
    
}
`;

    let parser_selector =
        data.type === 'leaf' ? `mkLeafWidget(${data.ns}, ${data.name}, ${data.ns}_${data.name})` :
            data.type === 'node' ? `mkNodeWidget(${data.ns}, ${data.name}, ${data.ns}_${data.name}); ` :
                data.type === 'container' ? `mkContainerWidget(${data.ns}, ${data.name}, ${data.ns}_${data.name})` :
                    data.type === 'slot' ? `mkSlotWidget(${data.ns}, ${data.name}, ${data.ns}_${data.name})` :
                        data.type === 'slot-contaiener' ? `mkSlotContainerWidget(${data.ns}, ${data.name}, ${data.ns}_${data.name})` : `sss`;

    return [class_decl, class_impl, parser_selector]
}


function gen_d_ts(data: Static<typeof widget_schema>) { }

function gen_xsd(data: Static<typeof widget_schema>) { }

function gen_xml_editor(data: Static<typeof widget_schema>) {
    return <component ns={data.ns} name={data.name} extends={data.extends} props-tail={data.props_tail} computed-tail={data.computed_tail}>
        <description>{data.description ?? "No description provided"}</description>
        <props>{Object.entries(data.props).map(x => <item name={x[0]} alias={x[1].alias} type={x[1].type}>{x[1].description ?? 'No description provided'}</item>)}</props>
        <computed>{Object.entries(data.computed).map(x => <item name={x[0]} alias={x[1].alias} type={x[1].type}>{x[1].description ?? 'No description provided'}</item>)}</computed>
    </component>
}

if (process.argv[2] != 'quick') {
    await $`rm -rf ./src/components/autogen/`
}
await $`mkdir -p ./src/components/autogen/`

await $`rm -rf ./include/components/autogen/`
await $`mkdir -p ./include/components/autogen/`

await $`rm -rf ./commons/schemas/components/`
await $`mkdir -p ./commons/schemas/components/`

const glob = new Glob("./schemas/components/**/*.json");


const cpp_files = []
const h_files = []
const parser_entries = []

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
        h_files.push(`components/autogen/${tmp.ns}/${tmp.name}.hpp`)
        {
            const ret = gen_cpp(tmp);

            await $`mkdir -p ./include/components/autogen/${tmp.ns}/`
            await Bun.write(`./include/components/autogen/${tmp.ns}/${tmp.name}.hpp`, ret[0])

            await $`mkdir -p ./src/components/autogen/${tmp.ns}/`
            await Bun.write(`./src/components/autogen/${tmp.ns}/${tmp.name}.cpp`, ret[1])
            parser_entries.push(ret[2])
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

await Bun.write('./include/components/autogen/index.hpp', h_files.map(x => `#include <${x}>`).join('\n'))
await Bun.write('./src/ui.xml-widgets.autogen.cpp', parser_entries.join('\n'))

if (process.argv[2] != 'quick') {
    await Bun.write('./src/components/autogen/meson.build', `autogen_components = [${cpp_files.map(x => `'${x}'`)}]`)
}