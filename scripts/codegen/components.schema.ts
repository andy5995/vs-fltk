import { Type as t } from "@sinclair/typebox"

const type_schema = t.Union([t.Literal('string'), t.Literal('color'), t.Literal('scalar-1'), t.Literal('scalar-2'), t.Literal('scalar-4')], { description: 'type' });

export const widget_schema = t.Object({
    ns: t.Optional(t.String()),
    name: t.Optional(t.String()),
    description: t.Optional(t.String()),
    use_main_header: t.Union([t.Null(), t.String()], { default: null }),
    headers: t.Optional(t.Array(t.String())),
    type: t.Union([t.Literal('leaf'), t.Literal('node'), t.Literal('container'), t.Literal('slot'), t.Literal('slot-contaiener')]),
    public: t.Optional(t.Boolean({ default: true })),
    extends: t.Union([t.Null(), t.String()], { default: null }),
    props_tail: t.Union([t.Null(), t.String()], { default: null }),
    computed_tail: t.Union([t.Null(), t.String()], { default: null }),
    props: t.Record(t.String(), t.Tuple([type_schema, t.String({ description: "code" }), t.Array(t.String(), { description: "alias names" })])),
    computed: t.Record(t.String(), t.Tuple([type_schema, t.String({ description: "code" }), t.Array(t.String(), { description: "alias names" })])),
}, { additionalProperties: false })