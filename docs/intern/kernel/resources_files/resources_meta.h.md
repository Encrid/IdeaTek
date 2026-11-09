# src/kernel/resources_files/resources_meta.h

This header gives the result of the read of a meta file.

## The types

`res_meta_t` is what a meta file says. It holds the action (`META_LOAD`, `META_PROCESS` or `META_IGNORE`), and the name of the loader with its length, 0 for none. It holds the settings text with its length, 0 for none. It holds the number of the line that the reader refused, 0 when the file is good.
`res_action_t` is the enum of the 3 actions.
