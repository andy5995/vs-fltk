> [!WARNING]  
> Revision needed as the first fragments of caching have been implemented.

- `file://` prefix for all files mounted on fs, both absolute and relative.
- `http(s)://` prefix for resources obtainable via internet (libcurl?).
- `this://` relative prefix based on the local base (only supporting file, http/s, packages, app, user).
- `packages://` prefix for where external libraries are downloaded and locally stored (CORS safety like for http/s based on the package name)
- `app://` prefix for the root where shared libraries and includes are stored (bindings included)
- `user://` location where permanent files are allocated (downloaded stuff, generated configuration)
- `sock://` virtual location to the UNIX socket (?) for external code execution
- `storage://` prefix for the persistent DB storage
- `session://` prefix for temporary resources available during the current session.

## Extensions

Some tags do have a `src` attribute:

- Optional for `script` in case inline content is not provided. If defined and the src load fails its content is used instead.
  If used with `file`, `http(s)`, `app`, `user` it will try to match the full name. In case of failure if no typical extension is matched it will try to take it (considering the alternatives due to portability factors)
  If used with `sock`, `storage`, `session` or `cache` only `store/uuids` are usable, and in most cases are just for internal usage.
- Needed for the `import` block. Same rules as for script.
- Needed for `use`. Same rules as for script.

All tags with `src` also have cache policy. Default is `session`.

- `no-cache`
- `session`
- `forever`
- Simple number Timestamp of invalidation.
- Number with `+` prefix, max time before invalidation since last aquisition.
  Keep in mind, even if invalidated already rendered objects will not re-render on their own.

All `use` (or forwarded nodes from the list of imports) allows for the definition of `policy-safety`:

- `unrestricted` is the default and lets any code to run
- `safe` only wasm and quickjs can run, but globally defined and cached components can still
- `safest` only wasm and quickjs can run.
  Children cannot be less safe of the parent. If so its original will be inherited.

Auth tokens are used to determine which application is running. Every `app` and `component` can set its own authtoken as a cryptohash of the provided uuid with the one of the user. Components cannot gather information about authtokens once computed, so the user secret is generally safe.
Setting it to an empty string will generate a random one.  
Using the original user secret as auth token (not exposed, not usable, just internal) determines shared globals. They are not trivially used by components as a failsafe
