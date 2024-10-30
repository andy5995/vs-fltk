> [!IMPORTANT]  
> This project is very much in its infancy.
> These specs will provide information about features which are yet to be implemented.  
> Broadly speaking, they target **at least** `v0.5.x` from the [milestone page](./milestones.md)

# Quick overview
`vs` is a lightweight runtime to render custom user interfaces.  
It can be used to provide a visual frontend to headless libraries, or to replicate many of the possibilities offered by web-based experiences.  
The `vs` runtime is distributed alongside several tools and libraries:
- A workspace/explorer (selfhosted) to quickly access local and remote apps, handle updates and setup safety policies.
- The UI builder/IDE (selfhosted), to visually inspect or write your own applications and components.
- C bindings to write custom native components leveraging the internal `vs` infrastructure.
- An extension to handle `.vs` and `.tvs` files in vscode.

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

## Scope of visibility

## Props

## Event propagation

## Computed values

## Symbols

### Callbacks

### Unknowns

### Setter/Getters

### Dispatcher

### Draw

## Scripts

### Single user scripts

### Module scripts

## Mixins

### Special mixins

- `*`
- `+tagName`

# Embedded interfaces

## Native

## Quickjs

## WASM

## External

# Custom components

# Templating

Templating is based on a `vs` custom XML preprocessor. Details about its syntax can be found [here](./static-templates.md)
