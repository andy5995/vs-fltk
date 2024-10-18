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
