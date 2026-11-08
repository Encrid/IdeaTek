# src/platform_abstraction/files_mosl/files_start.c

This file checks the settings and finds the 2 storages in the first round.

## The first round

`start_files` does these steps:

1. It checks `read_bytes` against 2 to 1073741824, an even number. A wrong value writes `read_bytes: <value> is outside 2 to 1073741824, an even number`, and the module is broken: `readme.md`, *The contracts*.
2. It copies `user_folder` and `content_folder` from the arena. A text that the path rule refuses writes `<setting>: <text> is outside the path rule`, and the module is broken.
3. It takes `read_bytes` bytes of the persistent arena for the 2 halves.
4. It finds the content storage: the folder of the program, from `os_get_program_path` with its last name cut, then `content_folder`. A path that is not a folder writes `no content folder: <path>`, and the content storage is empty.
5. It finds the user storage: the folder of the data of programs of the system, then `user_folder`, which `os_folder_make` makes. A system with no variable, or a refused make, writes `no user folder: <reason>`, and the user storage is empty.

| System | The folder of the data of programs |
| --- | --- |
| Windows | `APPDATA`. |
| Linux | `XDG_DATA_HOME`, else `HOME` and `.local/share`. |
| macOS | `HOME` and `Library/Application Support`. |

A storage that is empty refuses its requests: `docs/intern/platform_abstraction/files_mosl/files_request.c.md`.

## The branches

| Case | Result |
| --- | --- |
| `read_bytes` outside its range. | Its line, and `results` gives 1 in each round. |
| A folder setting outside the path rule. | Its line, and `results` gives 1 in each round. |
| No content folder. | The line of the content folder; each request of the content storage gets `IDEATEK_FILE_STATUS_NOT_FOUND`. |
| No variable of the system, or no user folder. | The line of the user folder; each request of the user storage gets `IDEATEK_FILE_STATUS_UNSUPPORTED`. |

## Functions

| Function | Result |
| --- | --- |
| `check_bytes` | Gives `true` for an even `read_bytes` from 2 to 1073741824, else writes its line. |
| `copy_setting` | Copies the text of a folder setting, and checks it against the path rule. |
| `find_content` | Finds the content storage beside the program. |
| `find_user` | Finds and makes the user storage, from the variable of the system. |
| `start_files` | Runs the steps of the first round. Gives `false` when the module is broken. |
