Static templates are extremely contained programs that a preprocessor can use to transform an input data XML into a final XML file. In the context of `vs` these final files will be either components or apps.  
When dealing with XML the most common preprocessor is XSLT, and I briefly considered using it for this project as it is. I decided against for two main reasons:

- The rest of the project is based on `pugixml`, while the only XSLT 1.0 implementation which is decently licensed is [libxslt](https://gitlab.gnome.org/GNOME/libxslt) based on [libxml2](https://gitlab.gnome.org/GNOME/libxml2).  
  Because of that, its integration would have been quite wasteful, requiring an additional serialization and parsing.
- The scope of XSLT, even in its very first version is too wide and somewhat verbose.
- I am trying to match the features set and syntax of the equivalent dynamic components that will be offered at some point in the standard library. A custom implementation ensures they are mostly aligned.

As a result, `vs` vendors its own XSLT-ish preprocessor that can be used as a build step for any component or fragment inside the supported xml files.

## Examples

## Syntax

### Operators for elements

#### `for-range`

- `from`
- `to`
- `step`

#### `for` & `for-prop`

To iterate over elements and props of an element respectively. Aside from that they share the same interface.

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
- `item`
- `error`

### `value`

To introduce the value of an expression as text content of an element.

### `element`

To generate a new element whose type is determined by a tag expression.

### `when` & `case`

To perform conditional cut and paste in the final tree.  
Attributes for `case`:

- `continue` default is false.
- `match`

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
