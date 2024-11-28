> [!IMPORTANT]  
> This project is very much in its infancy.
> These specs will provide information about features which are yet to be implemented.  
> Broadly speaking, they target **at least** `v0.5.x` from the [milestone page](./milestones.md)

# Quick overview

`vs` is a lightweight runtime to design and run custom user interfaces based on [fltk](https://www.fltk.org/).  
It aims to be fast, to fit on systems with limited resources available, and to run on virtually any platform.  
The `vs` runtime ships with several built-in features, tools and libraries:

- A self-hosted workspace/explorer to quickly access local and remote apps, to update `vs`, and to visually customize user settings.
- A self-hosted UI builder/IDE to visually inspect, debug or write custom components and applications.
- C bindings to expose most features and allow custom native components from third parties.
- A language server & extension to directly handle `.vs` and `.tvs` files in vscode and compatible editors.

## Structure of an application

- An entry-point file referred as **app**, whose root element is `app`.
- **Components**, whose root element is `component`, either used from the entry-point file or other components.
- **Data sources**, usually files with a specific format or endpoints with a well-defined interface.
- **Assets** like images.
- An optional **external application** usually as a dynamic library linked with the **app**, or indirectly used via a Unix socket domain.

Furthermore, there are additional elements which are not controlled by those writing the app, but on the user side:

- A storage where secrets and persistent data can be saved.
- A profile with clear permissions and safety policies. An application can provide one as asset suggesting the user to adopt it, but it has no control over what the user decides.

Both **apps** and **components** can be either specified as final `vs` xml files, or they can be the result of some static data and a template compiled down into "final" xml files.

# Basic VS elements

## app

### attributes

- `token`
- `link-with`

## component

## namespace

## import

## use

## data

### attributes

- `src`
- `filter`
- `sort`
- `order`

## debug

### attributes

- `key`
- `value`

## viewport

### attributes

- `src`

## elements with a frame

#### attributes

- `name`
- `mixins`
- `frame.mode`
- `frame.acces`
- `frame.type`

### script

#### attributes

- `lang`
- `src`
- `type`
- `compact`

### mixins

#### attributes

### semantics

# Using components

There are two ways to use other components inside others or within your app:

- Via `vs:use`, suggested for cases where it is only needed once.
- Adding a `vs:import` directive in the app/component, and using it as an element under the specified name.

There is also `vs:viewport` which does something similar, but in this case a new root is rendered within your document, with no event or symbol being passed top/down or bottom/up.

### Examples

```xml
<vs:component tag.suggested="component-a">
    <vs:import src="this://component-b.vs" tag="component-b"/>

    <component-b prop-1="value-1"/>
</vs:component>
```

```xml
<vs:component tag.suggested="component-a">
    <vs:use vs:src="this://component-b.vs" prop-1="value-1"/>
</vs:component>
```

## Names

Names are unique identifiers within a namespace for components.  
Adding a name to a component instance also implies that a vs frame is created for it. The converse is also true, and if no explicit name is assigned, one will be automatically generated.  
Related to names there are two undefined behaviours:

- Giving the same name to two or more elements within the same namespace.
- Accessing elements for which the name has not been manually defined.

### Restrictions on names

At this time, no restriction on names has been defined nor implemented.

## Scope of visibility

The scope within names are visible is called _namespace_. A namespace is not strictly speaking equivalent to the UI tree structure, and all elements within a namespace, regardless of their child/parent ordering, are mutually visible.

### Transparent nodes

Some elements will have their associated frame and name, but their mode is set to `transparent`.  
In this case any child will be part of the same namespace as their parent, ignoring their tree dependency.

### Limitations on visibility

Elements for which their mode is set to `container` or `slot-container` will prevent name resolution upwards from children.  
Elements for which their mode is set to `slot` or `slot-container` will prevent name resolution downwards to its children from nodes above it.  
A node, will always be able to access up and down, regardless of the mode set. Limitations are only for others going through it.

## Props

## Computed values

## Symbols

### Callbacks

### Unknowns

### Setter/Getters

### Dispatcher

### Draw

## Event propagation

## Scripts

Embedded scripts are

### Single user scripts

### Module scripts

## Mixins

### Special mixins

- `*`
- `+tagName`

# Embedded interfaces

## Native

## QuickJS

## LUA

## WASM

## External

# Custom components

# Templating

Templating is based on a `vs.templ`, a custom XML preprocessor loosely inspired by [xslt]().  
Its documentation can be found on its separate repository [here](https://github.com/karurochori/vs-templ).  
Within the context of `vs.fltk`, using templates involves two files:

- the static dataset which contains any structured needed by the final generation. For vs, its root must be `<vs:static-data>` and provide a `template` attribute pointing to the actual component template. The suggested extension is `.vs`, the same as normal components.
- the template itself, containing a `vs` tree structure and templating commands as well.  
  Its conventional extension is `.tvs`. These files should never be used as components on their own.

When importing or using a component which requires processing, it is compiled and replaces the original dataset before the building process in the UI continues.  
You can find an example for a [dataset](../examples/test-templ.vs) & its associated [template](../examples/test-templ.tvs).

# Data

## General structure

## Subtypes

### CSV

### FS

### JSON

### SQLITE

### XML
