# src/platform_abstraction/gpu_vulkan_module/gpu_gpus.c

This file reads the adapters of the machine and makes their entities.

## The first round

`turn_gpus` runs 1 time, when `started` is `false`:

1. It reads the settings. `frames_in_flight` takes 2 or 3, `image_count` takes 2 to 4, `validation` is a boolean. A value outside gives the line of a setting, and the system fails.
2. It asks Vulkan for the physical devices. The first `GPU_GPUS` get an entity with `ideatek:Gpu`.
3. It reads each adapter into the value of `ideatek:Gpu`. The name is cut at 255 bytes. The device-local heaps sum into `memory_bytes`. The kind comes from `VkPhysicalDeviceType`. The field `features` comes from the extensions of rays, of mesh shaders and from `drawIndirectCount`.
4. It marks `usable`: the version of the adapter is 1.4 or more, and it has each feature of Roadmap 2022. A single table holds the features to check, chain by chain.
5. It picks the default among the usable adapters. A discrete one wins over the rest, and the most memory wins inside a kind.
No usable adapter leaves `ideatek:DefaultGpu` off.

```pseudo
FUNCTION pick_default() RETURNS U32
   best <- NONE
   FOR i <- 0 TO 15
      IF i < gpu_count AND usable[i] THEN
         IF best = NONE THEN
            best <- i
         ENDIF
         IF memory[i] > memory[best] THEN
            best <- i
         ENDIF
         IF memory[i] = memory[best] AND kind[i] > kind[best] THEN
            best <- i
         ENDIF
      ENDIF
   NEXT i
   RETURN best
ENDFUNCTION
```

The rule of `usable` comes from the contract: Vulkan 1.4 and Roadmap 2022. The features of Roadmap 2024 need no check here, because they turn on at the device when the adapter has them.

## The branches

| Case | Result |
| --- | --- |
| More than 16 adapters. | The first 16 get an entity. |
| An adapter with Vulkan 1.3. | `usable` is `false`. |
| An adapter that lacks a feature of Roadmap 2022. | `usable` is `false`. |
| No usable adapter. | No entity carries `ideatek:DefaultGpu`. |
| A discrete and an integrated adapter with the same memory. | The discrete one is the default. |

## Functions

| Function | Result |
| --- | --- |
| `turn_gpus` | Reads the settings and the adapters in the first round, and attaches the values. |
| `read_gpu` | Reads the properties of 1 adapter into a value of `ideatek:Gpu`. |
| `gpu_usable` | Tells if an adapter has Vulkan 1.4 and each feature of Roadmap 2022. |
| `pick_default` | Gives the index of the default adapter, or no index. |
