WIP

## Debugging

| **Name**      | **Type**                     | **Description**                                                                                        |
| ------------- | ---------------------------- | ------------------------------------------------------------------------------------------------------ |
| VS_DEBUG_FILE | fs-path                      | If specified, file where debug record are appended. Else, there is no side effect for those operations |
| VS_LOG_FILE   | fs-path                      | If specified, file where log records are appended. Else, std out                                       |
| VS_LOG_LEVEL  | `silent,basic,verbose,debug` | Globally set the baseline log level, so that some messages can be skipped if not relevant.             |

## Safety policies

Policy level:

- **all**
- **none**
- **trusted**
- **normal**
- **safe**

| **Name**                    | **Type** | **Description** |
| --------------------------- | -------- | --------------- |
| VS_GLOBAL_POLICY            |          |                 |
| VS_NETWORKING_POLICY        |          |                 |
| VS_SCRIPTS_POLICY           |          |                 |
| VS_NATIVE_COMPONENTS_POLICY |          |                 |

## Runtime

| **Name**       | **Type**           | **Description**                                                                                                     |
| -------------- | ------------------ | ------------------------------------------------------------------------------------------------------------------- |
| VS_DB          | fs-path            | A path to a custom database for session/persistent caching & storage. If not specified the default one will be used |
| VS_HEADLESS    | boolean            | If true no UI is rendered, useful for testing purposes.                                                             |
| VS_PARSER_MODE | `balanced,low-mem` | It changes the type of parse used, mostly for compatibility with lower spec hardware. `balanced` by default         |
| VS_NO_CACHE    | boolean            | Disable any kind of caching & persistence. Some sort of "private" mode.                                             |
| VS_NO_THEME    | boolean            | Disable any kind of custom theming                                                                                  |
| VS_PREFIX_DIR  | fs-path            | Define a base root not in `/`. Used to run vs when installed as portable on a system. Also good for testing.        |
