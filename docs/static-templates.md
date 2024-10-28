Static templates are extremely contained programs that a preprocessor can use to transform an input data XML into a final XML file. In the context of `vs` these final files will be either components or apps.  
When dealing with XML the most common preprocessor is XSLT, and I briefly considered using it for this project as it is. I decided against for two main reasons:

- The rest of the project is based on `pugixml`, while the only XSLT 1.0 implementation which is decently licensed is [libxslt](https://gitlab.gnome.org/GNOME/libxslt) based on [libxml2](https://gitlab.gnome.org/GNOME/libxml2).  
  Because of that, its integration would have been quite wasteful, requiring an additional serialization and parsing.
- The scope of XSLT, even in its very first version is too wide and somewhat verbose.
- I am trying to match the features set and syntax of the equivalent dynamic components that will be offered at some point in the standard library. A custom implementation ensures they are mostly aligned.

As a result, `vs` vendors its own XSLT-ish preprocessor that can be used as a build step for any component or fragment inside the supported xml files.

## Examples

## Syntax

### Path expressions

- String, for expressions starting with `#`
- Integers, for expressions starting with a digit, `+`, `-` or `.`
- Local paths, `$/.../`
- Local paths ending with a property, `$/...~prop-name`
- Paths with arbitrary prefix `{var-name}/.../` where `var-name`is searched for in symbols
- As before, but ending with a property, `{var-name}/...~prop-name`
- Absolute paths. `/.../`
- Absolute paths resolving in a property `/...~prop-name`
- Access to the text of an element: `...~!txt`
- Access to the name of an element: `...~!tag`

  No other combination or format is allowed.

### Operators for elements

#### `for-range`

- `tag` is the name of the symbol where the current value will be stored. If empty the default `$` is used.
- `from` starting value.
- `to` final value.
- `step` step of increment. It can be negative. If so `to<from` must hold true.

Infinite cycles are detected before execution, in which case no cycle will run.

#### `for` & `for-prop`

To iterate over elements and props of an element respectively.  
Aside from that, they share the same interface.

- `tag` the name of the symbol hosting the current xml node pointer. If empty, its default is `$`
- `in` must be specified and is a path expression
- `filter` if set it is a quickjs formula
- `sort-by` (only available for `for`) list of comma separated path expressions. Elements will be sorted giving priority from left to right
- `order-by` order preference for each field in the `sort-by` or the only default one for `for-prop`, comma separated. Each entry is a pair `type:comparator` with type either ASC, DESC or RANDOM. The comparator can be skipped, and it will be assumed default to be the default one.
- `limit` maximum number of entries to be iterated
- `offset` offset from start (of the filtered and ordered list of children)

Both `for` & `for-prop` support the following list of children. You can use as many as you want with any order, they will be rearranged automatically.

- `header` shown at the top of a not-emtpy container
- `footer` shown at the botton of a not-empty container
- `empty` shown if a container is empty
- `item` the main body
- `error` shown if it was not possible to retrieve items because of an error in the path

### `value`

To introduce the value of an expression as text content of an element. It accept a path expression `src` as argument. By default, it is assumed to be `$`. It also supports an additional `format` argument, but at this stage it has no implementation.

### `element`

To generate a new element whose type is determined by a tag expression `ns:type`. Any other property and child will be preserved.

### `when` & `is`

To perform conditional cut and paste in the final tree based on simple matches between a reference expression and some values.  
`when` accepts a single `subject` property as a path expression.  
Inside the body of `when` we have one or more `is`.  
Attributes for `is`:

- `continue` default is `false`. If `true` it continues checking and executing even after a match. Else it will break.
- `value` a path expression to compare against.

The order of `case` is important and determines the overall flow.

### Operators for properties

### `for.FIELD.xxx` & `for-prop.FIELD.xxx`

As prop, attribute variants of `for` and `for-props`. They add attributes to the node they are defined within.

### `value.FIELD.xxx`

As prop, to introduce the value of an expression as value of a prop `xxx`.

### `prop`

To generate a new props whose name is determined by an expression.

## Old stuff to sort out

### Component file

```xml
<vs:component template="this://TableLike.xml">
    <format>
        <field name="field-a" format="id"/>
        <field name="field-b" format="currency"/>
    </format>
    <data>
        <entry>
            <field-a>404</field-a>
            <field-b>22255.75</field-b>
            <field-c>Hello</field-c>
        </entry>
        <entry>
            <field-a>322</field-a>
            <field-b>17122.66</field-b>
            <field-c>World</field-c>
        </entry>
    </data>
</vs:component>
```

### Template file

```xml
<vs:template>
    <html:table>
    <s:for tag="entry" in="/data/">
        <html:tr>
        <s:for tag="column" in="/format/">
            <html:td class=":{column.format}"><s:value src=":{entry}/:{column.name}"/></html:td>
        </s:for>
        </html:tr>
    </s:for>
    </html:table>
</vs:template>

```
