# src/platform_abstraction/gpu_vulkan_module/gpu_loader.c

This file loads the loader of Vulkan that the setting `loader` names.

## The forms of the setting

| Text | Loader |
| --- | --- |
| `machine` | The loader of the system, through `volkInitialize`. |
| A name with no `/` and no `\\` | The library of that name, through the search of the system. |
| A path | The file at that path. A path that does not start with `/` or with a drive starts at the folder of the module. |

`resolve` copies the text of the setting, cut at `GPU_PATH` minus 1 bytes, and makes the full path.
The folder of the module is the part of its file up to its last separator: `docs/intern/platform_abstraction/gpu_vulkan_module/gpu_library.c.md`, *The file of the module*.
The join has no branch. The length of the folder is multiplied by 1 for a path that is not absolute, else by 0. It is multiplied by 0 again when the join does not fit.

`load_vulkan` loads with `volkInitialize` for `machine`. Else it opens the full path and gives its `vkGetInstanceProcAddr` to `volkInitializeCustom`.
A loader that does not open writes the line of the loader with its reason, the machine or the path, and gives `false`.

## The branches

| Case | Result |
| --- | --- |
| `machine`, and no loader on the system. | The line of the machine. |
| A name that the system finds. | Vulkan is loaded. |
| A path with no library. | The line of the path. |

## Functions

| Function | Result |
| --- | --- |
| `folder_length` | Gives the length of a path up to its last separator. |
| `resolve` | Copies the text of the setting, and makes the full path of the loader. |
| `machine_loader` | Loads the loader of the system. Gives `false` with the line of the machine. |
| `load_vulkan` | Loads Vulkan from the setting. Gives `false` with the line of the loader. |
