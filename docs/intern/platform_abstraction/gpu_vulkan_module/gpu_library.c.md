# src/platform_abstraction/gpu_vulkan_module/gpu_library.c

This file holds the calls of the system on libraries. It keeps `dxgi.dll` loaded, finds the file of the module, and opens a loader of Vulkan.

## The reason of dxgi

The capture hook of OBS Studio is an implicit Vulkan layer. It is installed with OBS, and it stays when OBS is closed. A thread of the hook patches the graphics libraries of the process, and DXGI is one of them: https://github.com/obsproject/obs-studio/blob/master/plugins/win-capture/graphics-hook/graphics-hook.c.

Some Vulkan drivers load `dxgi.dll`. `vkDestroyInstance` unloads the driver, and with it `dxgi.dll`. The hook then runs code that is not mapped, and the process stops at exit.

A reference that the module never frees keeps `dxgi.dll` mapped. The destroy stays correct, and OBS can capture the game.

## The file of the module

`module_file` gives the path of the file of this module, in UTF-8. It asks the system for the library that holds the address of `MARK`, a byte of this file.
On Windows, `GetModuleHandleExW` gives the handle and `GetModuleFileNameW` the path. On the other systems, `dladdr` gives the path. The library of Linux declares it only with `_GNU_SOURCE`, which the artifact of the module defines.
A system that gives no path gives an empty text.

## The loader

`open_loader` opens a library by its text and gives its `vkGetInstanceProcAddr`.
On Windows, `LoadLibraryW` opens it, and the text follows the search of the system when it has no folder. On the other systems, `dlopen` does the same.
A library that does not open gives `nullptr`. The library stays open for the process, like the loader that volk opens.

## Functions

| Function | Result |
| --- | --- |
| `keep_dxgi` | Loads `dxgi.dll` on Windows, and does nothing on other systems. |
| `module_file` | Writes the path of the file of the module, and gives its length. |
| `open_loader` | Opens a library and gives its `vkGetInstanceProcAddr`, or `nullptr`. |
