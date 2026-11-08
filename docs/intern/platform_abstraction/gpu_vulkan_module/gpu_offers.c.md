# src/platform_abstraction/gpu_vulkan_module/gpu_offers.c

This file reads what an adapter offers: its features of Roadmap 2022 and its families of queues.

## The features

`query_chain` reads the structures of the features of Vulkan 1.0 to 1.3 into 1 chain.
`roadmap_lacks` counts the features of Roadmap 2022 that are off, from a table of their places in the chain.
The table holds the features of the profile `VP_KHR_roadmap_2022` of Khronos: https://github.com/KhronosGroup/Vulkan-Profiles/blob/main/profiles/Khronos/VP_KHR_roadmap.json.
An adapter with a count of 0 is usable, if it has Vulkan 1.4: `docs/extern/platform_abstraction/ideatek_gpu_vulkan.h.md`, *The features*.

## The queues

`find_queues` reads at most 32 families, and picks 3 of them:

| Queue | Family |
| --- | --- |
| Graphics | The first family with graphics. |
| Compute | The first family with compute and no graphics, else the family of graphics. |
| Transfer | The first family that only transfers, else the family of graphics. |

The pick is arithmetic, so a GPU with 1 family and a GPU with many families run the same code.

## Functions

| Function | Result |
| --- | --- |
| `query_chain` | Reads the features of an adapter into a chain. |
| `roadmap_lacks` | Gives the count of the features of Roadmap 2022 that are off. |
| `pick_family` | Gives the first family whose flags match, or the count of families. |
| `find_queues` | Picks the 3 families of the device. |
