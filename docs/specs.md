> [!IMPORTANT]  
> This project is very much in its infancy.
> These specs will provide information about features which are yet to be implemented.  
> Broadly speaking, they target **at least** `v0.5.x` from the [milestone page](./milestones.md)

# Quick overview
`vs` is a lightweight runtime to create and run custom user interfaces based on [fltk].  
It aims to be fast, to fit on systems with limited resources, and to run on virtually any platform.  
The `vs` runtime is distributed alongside several tools and libraries:
- A workspace/explorer (self-hosted) to quickly access local and remote apps, handle updates and setup safety policies.
- A UI builder/IDE (self-hosted), to visually inspect or write custom components and applications.
- C bindings to write custom native components, while leveraging most features internally offered by `vs`.
- A langugage server & extension to handle `.vs` and `.tvs` files directly in vscode or compatible editors.

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
The scope within names are visible is called *namespace*. A namespace is not strictly speaking equivalent to the UI tree structure, and all elements within a namespace, regardless of their child/parent ordering, are mutually visible.

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