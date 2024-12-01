## App-Class Keys

App-Class keys are the main mechanism to ensure data sandboxing across applications, even when part of the same rendered UI.

- App-class keys are meant to be unique across a class of apps which can be considered as _strongly related_.  
  For example the equivalent of web pages from the same domain would likely share the same app-class key under vs.
- App keys are not used as they appear in source, but are computed based on the parent one. For top level app-class keys, the parent key used for mixing is the secret one for each user.
- It is possible to record metadata for app-class keys, like the owner and which domains are allowed to use it. It might be a good idea to provide some form of registry shipped with vs and updated of known entries.

## Viewports

Viewports are a way to break the chain of keys, ensuring that an embedded app can be directly attached to the user root key, no matter how nested it is located. This poses some safety concerns, so there are strong restrictions in how viewports are handled:

- No information can be exchanged between its content and the parent for example.
- Some application might refuse to run within a viewport, unless all its parent up to root are restricted to some specific safety policies.

## Visibility specifiers

Visibility specifiers are the third and last way to sandbox content, and it is by limiting access to nodes in the UI tree.

- **leaf** terminal node. No named children below (and even if there are, we cannot see it; in that case slot should be used).
- **node** pass-through both up and down.
- **container** children cannot go up beyond the container.
- **slot** parents cannot get any lower, beyond the slot.
- **slot-container** children cannot go up, parents cannot go down.

Notice! There is some unintuitive behaviour based on these rules: given `node:A -> slot:B -> leaf:C` as starting from `A`, `B` can be resolved, `C` cannot. However, `B/C` is resolved as one can reach `B` from `A` and `C` from `B`. To avoid propagation, boundary elements should be kept nameless so that they cannot be later recalled in a path expression and are purged from all serializations.
