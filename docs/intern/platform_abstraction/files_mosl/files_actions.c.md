# src/platform_abstraction/files_mosl/files_actions.c

This file does the read, the write and the removal of a job with mosl, on the worker thread. The list is in `docs/intern/platform_abstraction/files_mosl/files_list.c.md`.

## The actions

| Action | Steps | Status |
| --- | --- | --- |
| Read | `path_split` gives the folder and the name. `os_folder_open` opens the folder of the storage and the folder part. `os_folder_read_file` reads the whole file. `keep_slice` copies the bytes from `offset` for `length` to the heap. `length` 0 takes them to the end, and an `offset` past the end takes nothing. `size` is the length of the whole file. | `IDEATEK_FILE_STATUS_NOT_FOUND` when the folder does not open or the file is not there. `IDEATEK_FILE_STATUS_TOO_BIG` when the bytes do not fit in a half. `IDEATEK_FILE_STATUS_FAILED` when the heap refuses. |
| Write | `os_folder_make` makes the folders of the path. `os_file_write` writes the bytes to the path with `.part` at its end, and `os_path_move` gives the file its path. | `IDEATEK_FILE_STATUS_FAILED` when a call refuses; the old file stays. |
| Remove | `os_path_entry` tells what is at the path. `folder_status` refuses a folder with an entry. `os_path_remove` removes it. | `IDEATEK_FILE_STATUS_NOT_FOUND` for nothing at the path. `IDEATEK_FILE_STATUS_DENIED` for a folder with entries. `IDEATEK_FILE_STATUS_FAILED` when a call refuses. |

A path that does not fit in `FILES_PATH` with its storage gets `IDEATEK_FILE_STATUS_DENIED`.
The bytes of a read go to the heap with `malloc`. `results` frees them after their copy into the half.

## The branches

| Case | Result |
| --- | --- |
| A file that is not there. | `IDEATEK_FILE_STATUS_NOT_FOUND`. |
| A read past the half. | `IDEATEK_FILE_STATUS_TOO_BIG`. |
| A write to a new folder. | The folders are made, and the file is written. |
| A removal of a folder with entries. | `IDEATEK_FILE_STATUS_DENIED`. |
| A removal of an empty folder. | The folder goes. |

## Functions

| Function | Result |
| --- | --- |
| `keep_slice` | Copies the bytes that the request asks for to the heap, and gives the status of the read. |
| `do_read` | Reads the file of a job. |
| `do_write` | Writes the bytes of a job to a temporary file, then gives it its path. |
| `folder_status` | Gives `IDEATEK_FILE_STATUS_DENIED` for a folder with an entry, `IDEATEK_FILE_STATUS_FAILED` for a folder that does not open, else `IDEATEK_FILE_STATUS_OK`. |
| `do_remove` | Removes the file or the empty folder of a job. |
