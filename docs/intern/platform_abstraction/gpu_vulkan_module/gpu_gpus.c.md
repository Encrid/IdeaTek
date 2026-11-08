# src/platform_abstraction/gpu_vulkan_module/gpu_gpus.c

This file reads the settings in the first round, and loads Vulkan.

## The first round

`start_gpus` does these steps:

1. It checks `frames_in_flight` against 2 to 3, and `image_count` against 2 to 4. It checks both, so each wrong setting writes its line.
2. A setting outside its range marks the module as broken. Then the system `begin` gives 1 in each round: `readme.md`, *The contracts*.
3. It loads the loader that the setting `loader` names: `docs/intern/platform_abstraction/gpu_vulkan_module/gpu_loader.c.md`. No loader writes `no loader: <reason>`.
4. It makes the instance: `docs/intern/platform_abstraction/gpu_vulkan_module/gpu_instance.c.md`.
5. It lists the adapters: `docs/intern/platform_abstraction/gpu_vulkan_module/gpu_adapters.c.md`.

## The branches

| Case | Result |
| --- | --- |
| A setting of 1, or a setting above its most. | The line of the setting, and `begin` fails. |
| No loader. | The line of the loader, and no adapter. |
| The instance is refused. | No adapter. |

## Functions

| Function | Result |
| --- | --- |
| `in_range` | Gives `true` for a value from 2 to its most, else writes the line of the setting. |
| `start_gpus` | Reads the settings, loads Vulkan and lists the adapters. A wrong setting gives `false`. |
