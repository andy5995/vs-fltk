## Debugging

| **Name**     | **Type** | **Description**                                                                                        |
| ------------ | -------- | ------------------------------------------------------------------------------------------------------ |
| VS_DEBUG     | fs-path  | If specified, file where debug record are appended. Else, there is no side effect for those operations |
| VS_LOG_LEVEL | ...      | Globally set the baseline log level, so that some messages can be skipped if not relevant.             |

## Safety policies

## Runtime

| **Name**       | **Type**           | **Description**                                                                                                     |
| -------------- | ------------------ | ------------------------------------------------------------------------------------------------------------------- |
| VS_DB          | fs-path            | A path to a custom database for session/persistent caching & storage. If not specified the default one will be used |
| VS_HEADLESS    | boolean            | If true no UI is rendered, useful for testing purposes.                                                             |
| VS_DEBUG       | fs-path            | If specified, file where debug record are appended. Else, there is no side effect for those operations              |
| VS_PARSER_MODE | `balanced,low-mem` | It changes the type of parse used, mostly for compatibility with lower spec hardware. `balanced` by default         |
| VS_NO_CACHE    | boolean            | Disable any kind of caching & persistence. Some sort of "private" mode.                                             |