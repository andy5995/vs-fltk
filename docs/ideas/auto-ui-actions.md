> [!WARNING]  
> None of what follows has been implemented yet. These are just specs to sketch out the idea.  
> It will probably its own separate project like `vs.templ` as it needs a tight `fltk` integration.

Automatic UI actions are sourced from a XML file, and run sequentially once the UI has been fully built.  
These actions are designed for machine usage and macros. Typical scenarios can be:
- To replace human input in automatic testing pipelines.  
- In documentation, to show users how to do something within the application.
- To allow LLM agents or crawlers to operate on a UI built with vs.

## Supported actions

- `action.sleep` wait for some time before proceeding.
- `action.click` to run the callback associated to a widget.
- `action.fill` to add some value as keyboard input or copy/cut.
- `action.tab`
- `action.focus`
- `action.wait` to wait for some condition on the ui to be true.

## Special blocks for testing
- `test` the test is resolved. 
    - `deny` the test ends and is marked as unresolved.
    - `accept` if unresolved, continue with the next operation.
    - `exit` to end the application.

## Support in documentation
Documentation for vs projects is intended to be written in Markdown.  
As such, XML code can be directly embedded in it, which means vs components are usable (please notice that based on the local policies, this functionality might be disabled).  
The special component `fl:inline-helper` can embed UI actions in it, which will run on request.

## Additional props to handle semantics
Additional explicit information should be provided to ensure the semantic of the exposed UI components is explained.  
This is done by nesting a `semantics` element to each element needing that. Elements with `semantics` specified should also have a `name` attribute.    
For example:
```
<fl:container>
<script>
    void on_new(){}
    void on_open(){}
    void on_delete(){}

    $cb(on_new)
    $cb(on_open)
    $cb(on_delete)
</script>
<semantics>
    <callback name="on_new">Explanation</callback>
    <callback name="on_open">Explanation</callback>
    <callback name="on_delete">Explanation</callback>
</semantics>
<fl:button on.callback="on_new" label="New"/>
<fl:button on.callback="on_open" label="New"/>
<fl:button on.callback="on_delete" label="New"/>
</fl:container>
```

This additional context is extremely good to automatically generate documentation, but also to ensure proper contextual information is given to autonomous agents using the UI.