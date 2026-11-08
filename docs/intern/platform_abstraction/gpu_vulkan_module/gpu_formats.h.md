# src/platform_abstraction/gpu_vulkan_module/gpu_formats.h

This header gives the picks of the swapchain that read only lists. They call no Vulkan, so a test program checks them with lists of its own.

`gpu_format_t` holds a format and a color space of Vulkan, and the same pair as values of the contract.
The functions are in `docs/intern/platform_abstraction/gpu_vulkan_module/gpu_formats.c.md`.
