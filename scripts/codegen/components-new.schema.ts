import { Type as t } from "@sinclair/typebox"

//New schema to replace the old one.
//Now props, computed, setters and getters are better linked together

export const type_schema = t.Union([
    t.Literal('flag'), 
    t.Literal('enum'), 
    t.Literal('raw'), 
    t.Literal('string'), 
    t.Literal('color'), 
    t.Literal('scalar-1'), 
    t.Literal('scalar-2'), 
    t.Literal('scalar-4')
], { description: 'type', default: 'string' });

const entries_schema = t.Record(t.String(), t.Object({
    type: type_schema,
    subtype: t.Optional(t.String()),
    parse: t.Union([t.String(),t.Null()], {default:null, description:"From string to object. Default based on `type` (and `subtype`)"}),
    setter: t.Union([t.String(),t.Null()], {default:null, description:"Operations on setting. If not set, it is just a computed value."}),
    getter: t.Union([t.String(),t.Null()], {default:null, description:"Information on how to extract value from object."}),
    //`serialize` not needed as it is already implemented for each supported type.
    description: t.Optional(t.String()),
    alias: t.Optional(t.Array(t.String(), { description: "alias names" , default:[]}))
}))

export const widget_schema = t.Object({
    $schema: t.Optional(t.String()),
    ns: t.Optional(t.String()),
    name: t.Optional(t.String()),
    exposed: t.Optional(t.Boolean({default:true})), 
    description: t.Optional(t.String()),
    use_main_header: t.Union([t.Null(), t.String()], { default: null }),
    headers: t.Optional(t.Array(t.String())),
    type: t.Union([t.Literal('leaf'), t.Literal('node'), t.Literal('container'), t.Literal('slot'), t.Literal('slot-contaiener')]),
    codegen: t.Object({
        extends: t.Union([t.Null(), t.String()], { default: null }),
        props_tail: t.Optional(t.Union([t.Null(), t.String()])),
        computed_tail: t.Optional(t.Union([t.Null(), t.String()])),
    }, { additionalProperties: false }),
    extends: t.Union([t.Null(), t.String()], { default: null }),
    skip_fields: t.Optional(t.Array(t.String(),{default:[],description:"Properties to be matched but ignored"})),
    fields: entries_schema,
}, { additionalProperties: false })