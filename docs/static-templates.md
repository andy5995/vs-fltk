Static templates are extremely simple programs serving as preprocessor to transform an input data XML into a derived XML file. This program is also expressed in XML. Whithin the context of `vs`, these derived files will be either components or apps.  
Dealing with XML, the most common and widespred preprocessor is XSLT. It was briefly considered for this project, but discarded for few reasons:

- The rest of the project is based on `pugixml`, while the only XSLT 1.0 implementation which is decently licensed is [libxslt](https://gitlab.gnome.org/GNOME/libxslt) based on [libxml2](https://gitlab.gnome.org/GNOME/libxml2).  
  Because of that, its integration would have been quite wasteful, requiring an additional serialization and parsing to go from one library to the next.
- The scope of XSLT, even in its very first version is too wide, verbose and not always easy to decode from its syntax.
- At some point `vs` will come with dynamic native components matching in behaviour the extended tagsused in the preprocessor program. A custom implementation ensures they can be seamlessly replaced when needed.

As a result, `vs` vendors its own XSLT-ish preprocessor. Still, nothing about its semantics or syntax is directly tied to `vs`; as such, it can be used for standalone applications as well.

## Usage
In general there are two files involved:
- the static dataset contains any data needed for the generation of the final component. For vs, its root must be `<vs:sdata>` and provide a `s:template` attribute pointing to the actual component template. The conventional extension, like normal components, is `.vs`.
- the template itself, containing a `vs` tree structure and entities in the `s` namespace as well. Its conventional extension is `.tvs`. These files shall never be used as components themselves.

When importing or using a component which requires processing, its static dataset is used.

### Examples

## Syntax


### Path expressions
Expression are used to use elements and properties in the tree structure of the static dataset from the template. Their definition and usage is purposefully restricted to prevent arbitrary code to be run.  
This is the list of all feasible value

- String, automatically cast for expressions starting with `#`
- Integers, automatically cast for expressions starting with a digit, `+`, `-` or `.`
- Local paths, `$/.../`. `$` is used to mark the nearest named spope being visited.
- Local paths leading to a property/attribute, `$/...~prop-name`
- Paths with arbitrary prefix `{var-name}/.../` where `var-name`is searched for and resolved from the symbols' stack.
- As before, but ending with a property/attribute, `{var-name}/...~prop-name`
- Absolute paths. `/.../` starting with `/`.
- Absolute paths resolving in a property/attribute `/...~prop-name`

There are also two special properties:
- Special access to the property`~!txt` to get the node text.
- Special access to the element's name via `~!tag`

No further combination or format is allowed, and if used they might lead to undefined behaviour. However the preprocessor should not result in exceptions.

### Operators for elements
Operators acting over elements will use information from the current static data subpath to further generate a parametrized version of what shown in their children on the template tree.

#### `for-range`

- `tag` is the name of the symbol where the current value will be stored. If empty the default `$` is used.
- `from` starting value.
- `to` final value.
- `step` step of increment. It can be negative. If so `to<from` must hold true.

Infinite cycles are detected before execution, in which case no cycle will run. Unlike other `for` variants, there is no header, footer or empty child. Anything inside a `for-range` is interpreted as `item`.

#### `for` & `for-props`

To iterate over children and props of an element respectively.  
Aside from that, they mostly share the same interface.

- `tag` the name of the symbol hosting the current xml node pointer. If empty, its default is `$`
- `in` must be specified and is a path expression
- `filter` ~~if set it is a quickjs formula~~ its actual definition has yet to be determined. I'd like to avoid qjs if possible.
- `sort-by` (only available for `for`) list of comma separated path expressions. Elements will be sorted giving priority from left to right
- `order-by` order preference for each field in the `sort-by` or the only one implicit for `for-props`. Each entry is a pair `type:comparator` with type either ASC, DESC or RANDOM. If not provided, comparator is assumed to be the default one. As an alternative comparator we could have a one using `.` to separate values in tokens, and order them token by token.
- `limit` maximum number of entries to be iterated. If 0 all of the will be considered, if positive that or the maximum number, if negative all but that number if possible o no contnet.
- `offset` offset from start (of the filtered and ordered list of children)

Both `for` & `for-props` support the following list of children. You can use as many instances of them as you want, in any order.

- `header` shown at the top of a not-emtpy container
- `footer` shown at the botton of a not-empty container
- `empty` shown if a container is empty
- `item` the main body
- `error` shown if it was not possible to retrieve items (because of an error in the path)

### `value`

To introduce the value of an expression as text content of an element. It accept a path expression `src` as argument. By default, it is assumed to be `$`.  
It also supports an additional `format` argument, but at this stage it has no implementation.

### `element`

To generate a new element whose type is determined by a tag expression `ns:type`. Any other property and child will be preserved.

### `when` & `is`

To perform conditional cut and paste in the final tree based on simple matches between a reference expression and some values.  
`when` accepts a single `subject` property as a path expression.  
Inside the body of `when` we have one or more `is`.  
Attributes for `is`:

- `continue` default is `false`. If `true` it continues checking and executing even after a match. Else it will break.
- `value` a path expression to compare against.

The order of `is` elements is important and determines the overall flow.

### Operators for properties

### `for.FIELD.xxx` & `for-props.FIELD.xxx`

As prop, attribute variants of `for` and `for-props`. They add attributes to the node they are defined within.

### `value.FIELD.xxx`

As prop, to introduce the value of an expression as value of a prop `xxx`.

### `prop`

To generate a new props whose name is determined by an expression.

## Old stuff to sort out

### Component file

```xml
<vs:sdata template="this://TableLike.xml">
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
</vs:sdata>
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
