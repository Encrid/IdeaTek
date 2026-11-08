# src/platform_abstraction/vulkan/leaks.toml

This file gives the kinds of the objects of Vulkan that a module on volk makes and destroys, so `test --leaks` counts them.
`.toolchain/config/leaks.toml` lists it in `[leaks] with` for the modules of IdeaTek.
A module of a different project lists the copy that the release of the part `vulkan` holds.

## The kinds

A call of Vulkan writes the handle of a new object into the value that an argument points to. Its destroy gets the handle as an argument.
volk calls each function through a pointer variable, which the count reaches: `.toolchain/docs/rewrite.py.md`, *The pointer variables*.

| Kind | Takes | Gives back |
| --- | --- | --- |
| `vulkan_instance` | `vkCreateInstance` at `*3` | `vkDestroyInstance` at `1` |
| `vulkan_messenger` | `vkCreateDebugUtilsMessengerEXT` at `*4` | `vkDestroyDebugUtilsMessengerEXT` at `2` |
| `vulkan_device` | `vkCreateDevice` at `*4` | `vkDestroyDevice` at `1` |
| `vulkan_semaphore` | `vkCreateSemaphore` at `*4` | `vkDestroySemaphore` at `2` |
| `vulkan_command_pool` | `vkCreateCommandPool` at `*4` | `vkDestroyCommandPool` at `2` |
| `vulkan_swapchain` | `vkCreateSwapchainKHR` at `*4` | `vkDestroySwapchainKHR` at `2` |
| `vulkan_image_view` | `vkCreateImageView` at `*4` | `vkDestroyImageView` at `2` |

The command buffers go back with their pool, so no kind counts them.
A surface comes from a call that the module loads by its name at run time, so no kind counts it.
