#!/bin/env bun
/**
 * Automatic generations of components from their schema definition.
 */

/*
./src/ui.names.autogen.cpp
template<> const char* ui<Fl_Box>::class_name()  {return "label";}
*/

/*
./src/ui.apply_prop.autogen.cpp
$start_prop(Fl_Window){
  $end_prop(Fl_Widget);
}
*/

/*
./src/ui.get_computed.autogen.cpp
$start_computed(Fl_Box){
  $end_computed(Fl_Widget);
}
*/

/*
./src/ui.xml-widgets.autogen.cpp
mkLeafWidget(markdown,ui<Fl_Markdown>)
*/

/*
./bindings/quickjs/libvs.autogen.d.ts
Type helpers
*/

import fl_widgets from "@/schemas/components/fl_widgets.json"

const dispatchers: string[] = []

//Generate the support code to construct the temporary value to assign in the custom code.
function gen_load(_type: string) {
    const type = _type as "string" | "px[1]" | "px[2]" | "px[4]" | "color" | null;
    switch (type) {
        case "string": return '';
        case "px[1]": return `size_t v; if((ok=ui_tree::h_px(1,&v,value,that)))`;
        case "px[2]": return `size_t v[2]; if((ok = ui_tree::h_px(2,v,value,that)))`;
        case "px[4]": return `size_t v[4]; if((ok = ui_tree::h_px(4,v,value,that)))`;
        case "color": return `uint32_t v; if((ok=ui_tree::h_colour(&v,value,that)))`;
        case null: return '';
        default:
            console.error("Type not recognized", type);
            return ''
    }
}

for (const [classname, specs] of Object.entries(fl_widgets)) {
    dispatchers.push(`
$start_dispatcher(${classname}){
  if(false){}
  //To handle
  ${Object.entries(specs.props).map(([prop, def]) => { return `$eq("${prop}"){ ${gen_load(def[0])}{${def[1]}} }` }).join('\n')}
  ${specs.base != null ? `$end_dispatcher(${specs.base});` : `$end_dispatcher_b();`}
}
`)
}

const classes = Object.keys(fl_widgets).map(x => `template class ui<${x}>;`).join('\n');
const widgets = Object.entries(fl_widgets).filter(x => x[1].emit != false).map(x => x[1].leaf ? `mkLeafWidget(${x[1].name},${x[0]})` : `mkWidget(${x[1].name},${x[0]})`).join('\n');

//TODO: Add the static variable determining if it is container, node, leaf etc

//TODO: Save all these generations as c/cpp/h/hpp fragments (name.autogen.xxx) in the relevant folders.

await Bun.write('./src/ui.names.autogen.cpp', "")
await Bun.write('./src/ui.apply_prop.autogen.cpp', "")
await Bun.write('./src/ui.get_computed.autogen.cpp', "")
await Bun.write('./src/ui.xml-widgets.autogen.cpp', "")
await Bun.write('./bindings/quickjs/libvs.autogen.d.ts', "")

//TODO: XML data file to generate the full editor views for each component.

