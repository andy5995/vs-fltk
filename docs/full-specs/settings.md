## Order of evaluation for policies

Directives on policies are applied base on the following priority (highest to lowest)

- built-in compiled profile (vs can be built with embedded policies)
- command line environment variables
- `runtime/base-policy`
- `domain` if not a recognized app. Else `app` and its domain policies are ignored.
- Any locally defined policy within the app or components.

## Structure of a settings profile

This is the tree structure of any settings XML file:

- `vs-profile`
  - `locale`
    - `user`: dictionary of user data, mostly used for forms auto fill-in.
    - `languages`: dictionary of supported languages. They are ordered by preference (lower number higher preference) and when ambiguous by order in the XML file.
    - `theme`: base user theme. It supports some attributes:
      - `src` the location of a theme xml file to be used as parent.
      - `scheme` pick up a color scheme if internally defined (default allowed).
      - `mode` determines if using dark, light, high contrast etc. or if this choice should be left to the OS if `inherit` (default).
    - `initial-scene`: path for the scene to show up when starting `vs`
  - `runtime`
    - `logging`
    - `parsing`
    - `base-policy` same structure as one entry in the policies, but this is applied as a global override to fully shut of features of `vs`.
  - `policies`
    - `template` reusable entries, used to avoid copy and paste. Their content is expanded in `domain` and `app` extending from them.
      - `name` unique name in the config file
      - `extends` comma separated list of sources to copy from.
    - `domain`
      - `extends` comma separated list of sources to copy from.
    - `app`
      - `extends` comma separated list of sources to copy from.

### User dictionary

Entries in the user dictionary are arbitrary and publicly exposed as `user://` virtual paths. Apps cannot mutate them, this is only possible via settings.  
They are not a replacement for secrets!

User data should only include things like user's nickname, public email, daytime zone, etc.

### Language codes

Language codes are expressed as a pair `language`, `country`. Country is optional, which means any satisfying the language itself can be picked.

### Policy structure

#### Flag entries

Most entries are flags supporting one of three values:

- `yes`: use the upstream value from the parent, but if that is `no` report a warning/error.
- `no`: revoke this permission.
- `inherit` (default): use the upstream value from the parent.
