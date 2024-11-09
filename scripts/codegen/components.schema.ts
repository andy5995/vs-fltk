import { Type as t } from "@sinclair/typebox"

export const type_schema = t.Union([t.TemplateLiteral('flag'), t.TemplateLiteral('enum'), t.Literal('raw'), t.Literal('string'), t.Literal('color'), t.Literal('scalar-1'), t.Literal('scalar-2'), t.Literal('scalar-4')], { description: 'type', default: 'string' });

const entries_schema = t.Record(t.String(), t.Object({
    type: type_schema,
    subtype: t.Optional(t.String()),
    code: t.Union([t.String(),t.Null()], {default:null}),
    description: t.Optional(t.String()),
    alias: t.Optional(t.Array(t.String(), { description: "alias names" , default:[]}))
}))

export const widget_schema = t.Object({
    $schema: t.Optional(t.String()),
    ns: t.Optional(t.String()),
    name: t.Optional(t.String()),
    usable: t.Optional(t.Boolean({default:true})), 
    description: t.Optional(t.String()),
    use_main_header: t.Union([t.Null(), t.String()], { default: null }),
    headers: t.Optional(t.Array(t.String())),
    type: t.Union([t.Literal('leaf'), t.Literal('node'), t.Literal('container'), t.Literal('slot'), t.Literal('slot-contaiener')]),
    //public: t.Optional(t.Boolean({ default: true })), //probably replaced by usable
    codegen: t.Object({
        extends: t.Union([t.Null(), t.String()], { default: null }),
        props_tail: t.Optional(t.Union([t.Null(), t.String()])),
        computed_tail: t.Optional(t.Union([t.Null(), t.String()])),
    }, { additionalProperties: false }),
    extends: t.Union([t.Null(), t.String()], { default: null }),
    skip_props: t.Optional(t.Array(t.String(),{default:[],description:"Properties to be matched but ignored"})),
    props: entries_schema,
    computed: entries_schema,
}, { additionalProperties: false })