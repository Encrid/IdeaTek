# src/platform_abstraction/files_mosl/files_list.c

This file lists a folder for a job. It runs on the worker thread, with the folder calls of mosl.

## The list

`do_list` opens the folder at the path of the job, below its storage, with `os_folder_open`. It takes a block of the size of a half from the heap.

`os_folder_read_entry` gives each entry, and `add_entry` writes its line. A folder gives its name and `/`. A file gives its name, then a space and its size in decimal from `os_folder_entry_size`. `put_number` writes the digits. Each line ends with `\n`.

An entry whose name holds `\n`, or has `FILES_PATH` bytes or more, gives no line. No case can make such a name on each system, so the skip is arithmetic, with no branch.

| Case | Status |
| --- | --- |
| The folder does not open. | `IDEATEK_FILE_STATUS_NOT_FOUND` |
| The lines do not fit in a half. | `IDEATEK_FILE_STATUS_TOO_BIG` |
| The heap or a size refuses. | `IDEATEK_FILE_STATUS_FAILED` |
| A path too long for `FILES_PATH` below its storage. | `IDEATEK_FILE_STATUS_DENIED` |

`os_folder_entry_size` comes with mosl v0.5.0.

## The branches

| Case | Result |
| --- | --- |
| An empty folder. | `IDEATEK_FILE_STATUS_OK` with no line. |
| The lines pass the half. | `IDEATEK_FILE_STATUS_TOO_BIG`. |

## Functions

| Function | Result |
| --- | --- |
| `put_number` | Writes a number in decimal, and gives its count of digits. |
| `add_entry` | Writes the line of an entry into the bytes of the job, and gives the status. |
| `do_list` | Lists the folder of a job, 1 line for each entry. |
