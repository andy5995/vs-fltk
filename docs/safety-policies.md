## Risk assessment guidelines

`vs` should always be considered as an unsafe runtime.  
While there is a fair amount of effort in sandboxing and gatekeeping the underlying system to any embedded script, these are mitigations for user errors at best and not safety features.  
The actual degree of safety widely varies depending on the type of embedded script and component being used.  
For this reason it is possible to configure what is allowed and where as part of a risk mitigation strategy in the user configuration.

| Component type    | Risk level | Notes                                                                                                                     |
| ----------------- | ---------- | ------------------------------------------------------------------------------------------------------------------------- |
| Basic XML         | Lowest     | As long as pugixml and vs don't have nasty bugs it is fine                                                                |
| Preprocessed XML  | Lowest     | Same as before                                                                                                            |
| XML + Native      | Highest    | It is very easy to break the thin sandbox and run arbitrary code                                                          |
| XML + QuickJS     | Medium     | QuickJS is sandboxed, but there could be vulnerabilities in the parser leading to exploits                                |
| XML + WASM        | Low        | As long as you trust WAMR to be sound it will be fine                                                                     |
| XML + Lua         | Medium     | Like QuickJS, it is possible for Lua to have vulnerabilities in its parser or runtime                                     |
| XML + LuaJIT      | High       | Not as bad as pure native code, the sandbox of LuaJIT is not designed for safety                                          |
| Native components | Highest    | Arbitrary code doing arbitrary things, unless externally constrained                                                      |
| WASI components   | Low        | As long as WAMR holds and our interface is not holed they are much safer (and portable) alternatives to native components |

High-risk combinations should only be ever used when you fully trust your source of code and the chain to obtain it.  
It is **really** unadvisable to keep them enabled for apps/components fetched via `http`, even if the source is trusted as you might be victim of _man in the middle_ attacks.

## Definition of policies

Policies are rules which are enforced by the engine to determine which operations and which feature are allowed based on contextual information.  
They can be used, for example, to allow local applications to run scripts usually considered as less safe, and disallow the same when coming from all internet sources but few selected addresses.  

During runtime, there are three main source determining policies:
- Via coarse-grained environment variables made available. Those are used as root for derivation for every policy coming after. For example setting `VS_NETWORKING_POLICY=none` as a variable will ensure networking will always be disabled no matter what.
- Via user profiles. As part of each user's configuration, it is possible to store rules that for more or less specific path patterns and app uuid will apply specialized policies.
- Via meta information in the XML of components and applications. Those are only hints, and it is up to the engine to decide whether they should be accepted or not, at times with an explicit request to the user.

It is possible that a single component or app can have multiple viable rule sets matching their path, base id or uuid. Only one set should be applied at any time, and it is the one with the highest match. For example:
- https://** will match any request performed via https.
- https://app.example.org/** will match any request associated to a specific domain.
- `#app-id:xxxxxxx` will match a specific app id (possibly cloned) given by a vendor.
- `unique#app-id:xxxxxxx` will match the unique id determined by the app id, its location AND the secret user key.
The more specific set which applies will be the one selected.  
Rule sets can be specified as a combination of requirements like URL patterns AND unique identifier.