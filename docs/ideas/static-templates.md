Static templates are generic structures for components which will be statically generated from a static dataset.  
A typical XML technology to achieve that is XSLT.  
However, using it has two main issues:

- This project is based on `pugixml`, while the only XSLT 1.0 decently licensed implementation is `libxslt` based on `libxml2`.  
  Integrating it would be very wasteful.
- We are going to have similar primitives offered in a dynamic version, and I would like to avoid too much divergence between the two.

As a result, an XSLT-ish preprocessor will be implemented for the generation of the final XML component.

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

## Supported tags

### `for-range`

- `from`
- `to`
- `step`

### `for` & `for-prop`

To iterate over elements and props of an element respectively.

- `tag` if left empty its default is `$`
- `in` must be specified
- `filter` if set it is a quickjs formula
- `sort-by` (only `for`) list of relevant fields, comma separated
- `order-by` order preference for each field, comma separated. Each entry is ASC, DESC, RANDOM followed by a comparator name, empty if default. Default order is how it is in the source xml.
- `limit` maximum number of entries
- `offset` offset from start

We can also have sub-children

- `header`
- `footer`
- `empty`
  Anything outside is assumed to be in `s-item`

### `for.FIELD.xxx` & `for-prop.FIELD.xxx`

As prop, attribute variants of `for` and `for-props`. They add attributes to the node they are defined within.

### `value`

To introduce the value of an expression as text content of an element.

### `value.FIELD.xxx` & `value.xxx`

As prop, to introduce the value of an expression as value of a prop `xxx`.

### `element`

To generate a new element whose type is determined by a tag expression.

### `when` & `case`

To perform conditional cut and paste in the final tree.  
Attributes for `case`:

- `continue` default is false.
- `match`
