# src/platform_abstraction/files_mosl/files_actions.c

This file does the 4 actions of a job with mosl, on the worker thread.

## The actions

| Action | Steps | Status |
| --- | --- | --- |
| Read | `path_split` gives the folder and the name. `os_folder_open` opens the folder of the storage and the folder part. `os_folder_read_file` reads the whole file. The bytes from `offset` for `length` go to the heap; `length` 0 takes them to the end. `size` is the length of the whole file. | `IDEATEK_FILE_STATUS_NOT_FOUND` when the folder does not open or the file is not there. `IDEATEK_FILE_STATUS_TOO_BIG` when the bytes do not fit in a half. `IDEATEK_FILE_STATUS_FAILED` for another refusal. |
| Write | `os_folder_make` makes the folders of the path. `os_file_write` writes the bytes to the path with `.part` at its end, and `os_path_move` gives the file its path. | `IDEATEK_FILE_STATUS_FAILED` when a call refuses; the old file stays. |
| List | `os_folder_open` opens the folder, and `os_folder_read_entry` gives each entry. A folder entry gives its name and `/`. A file entry gives its name, a space and the decimal size from `os_folder_entry_size`. Each line ends with `\n`. An entry whose name holds `\n` is skipped. | `IDEATEK_FILE_STATUS_NOT_FOUND` when the folder does not open. `IDEATEK_FILE_STATUS_TOO_BIG` when the lines do not fit in a half. `IDEATEK_FILE_STATUS_FAILED` when a size refuses. |
| Remove | `os_path_entry` tells what is at the path. A folder must have no entry. `os_path_remove` removes it. | `IDEATEK_FILE_STATUS_NOT_FOUND` for nothing at the path. `IDEATEK_FILE_STATUS_DENIED` for a folder with entries. `IDEATEK_FILE_STATUS_FAILED` when the removal refuses. |

The bytes of a read and the lines of a list go to the heap with `malloc`. `results` frees them after their copy into the half.
`os_folder_entry_size` comes with mosl v0.5.0.

## The branches

| Case | Result |
| --- | --- |
| A file that is not there. | `IDEATEK_FILE_STATUS_NOT_FOUND`. |
| A read past the half. | `IDEATEK_FILE_STATUS_TOO_BIG`. |
| A write to a new folder. | The folders are made, and the file is written. |
| A removal of a folder with entries. | `IDEATEK_FILE_STATUS_DENIED`. |

## Functions

| Function | Result |
| --- | --- |
| `do_read` | Reads the file of a job, and keeps the bytes that the request asks for. |
| `do_write` | Writes the bytes of a job to a temporary file, then gives it its path. |
| `do_list` | Lists the folder of a job, 1 line for each entry. |
| `do_remove` | Removes the file or the empty folder of a job. |
