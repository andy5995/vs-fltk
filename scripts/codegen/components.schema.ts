import { Type as t } from "@sinclair/typebox"

const type_schema = t.Union([t.Literal('raw'), t.Literal('string'), t.Literal('color'), t.Literal('scalar-1'), t.Literal('scalar-2'), t.Literal('scalar-4')], { description: 'type', default: 'string' });

const entries_schema = t.Record(t.String(), t.Object({
    type: type_schema,
    code: t.String(),
    description: t.Optional(t.String()),
    alias: t.Array(t.String(), { description: "alias names" })
}))

export const widget_schema = t.Object({
    ns: t.Optional(t.String()),
    name: t.Optional(t.String()),
    description: t.Optional(t.String()),
    use_main_header: t.Union([t.Null(), t.String()], { default: null }),
    headers: t.Optional(t.Array(t.String())),
    type: t.Union([t.Literal('leaf'), t.Literal('node'), t.Literal('container'), t.Literal('slot'), t.Literal('slot-contaiener')]),
    public: t.Optional(t.Boolean({ default: true })),
    codegen: t.Object({
        extends: t.Union([t.Null(), t.String()], { default: null }),
        props_tail: t.Optional(t.Union([t.Null(), t.String()])),
        computed_tail: t.Optional(t.Union([t.Null(), t.String()])),
    }),
    extends: t.Union([t.Null(), t.String()], { default: null }),
    props: entries_schema,
    computed: entries_schema,
}, { additionalProperties: false })