# src/platform_abstraction/gpu_vulkan_module/gpu_keep.c

This file keeps `dxgi.dll` loaded until the process ends, on Windows.

## The reason

The capture hook of OBS Studio is an implicit Vulkan layer. It is installed with OBS, and it stays when OBS is closed. A thread of the hook patches the graphics libraries of the process, and DXGI is one of them: https://github.com/obsproject/obs-studio/blob/master/plugins/win-capture/graphics-hook/graphics-hook.c.

Some Vulkan drivers load `dxgi.dll`. `vkDestroyInstance` unloads the driver, and with it `dxgi.dll`. The hook then runs code that is not mapped, and the process stops at exit.

A reference that the module never frees keeps `dxgi.dll` mapped. The destroy stays correct, and OBS can capture the game.

## Functions

| Function | Result |
| --- | --- |
| `keep_dxgi` | Loads `dxgi.dll` on Windows, and does nothing on other systems. |
