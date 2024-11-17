## App Keys
App keys are the main mechanism to ensure data sandboxing across applications, even when part of the same UI.

- App keys are meant to be unique across a class of apps which can be considered as strongly related.  
  For example the equivalent of web pages under vs should share the same app key.
- App keys are linked in a chain of encryption when apps are nested inside others. 
- The only exception are viewports which are always using the basic user key but prevent any form of information leaking (but malicious native code on parent components **surely can**).
  This is a huge hole in security. I need to think a way to avoid this situation.  
  The easiest for now is to prevent viewports in all unsafe environments (considering it from the root).
- It is possible to record metadata for app keys, like the owner and which domains are allowed to use it. It might be a good idea to provide some form of registry shipped with vs and updated of known appkeys.