# src/platform_abstraction/gpu_vulkan_module/gpu_main.c

This file loads Vulkan and stops it.

## The load

`init` does these steps:

1. It calls `volkInitialize`. A failure writes `no loader: <reason>` and gives 0. The module stays loaded, with no entity of `ideatek:Gpu`.
2. It makes the instance with the version 1.4 and, when `validation` is on, the layer of Khronos. The extension `VK_EXT_debug_utils` comes with the layer.
3. A refusal of the instance writes `no instance: <reason>` and gives 0.
4. With the layer on, it makes the messenger. Every message of the layer lands in the log of the module as 1 line, cut at `GPU_MAX_LOG` bytes.

The settings are read in the first round, like the input module: https://github.com/Encrid/Bridge/blob/v0.5.0/docs/extern/bridge.h.md#bridge_load.
A setting outside its range gives its line, and the systems fail in each round.

```pseudo
FUNCTION init() RETURNS U32
   r <- volkInitialize()
   IF r <> SUCCESS THEN
      CALL say("no loader: <reason>")
      RETURN 0
   ENDIF
   instance <- make_instance()
   IF instance = 0 THEN
      RETURN 0
   ENDIF
   CALL volkLoadInstance(instance)
   IF validation THEN
      CALL make_messenger()
   ENDIF
   RETURN 0
ENDFUNCTION
```

`shutdown` waits for the device with `vkDeviceWaitIdle`, destroys the objects that the module made, in the reverse order of their making, and calls `volkFinalize`.
A device that was never made skips its part.

## The messenger

The callback of the layer writes `<type> <message>` with `bridge_native_say`: the type maps `VK_DEBUG_UTILS_MESSAGE_SEVERITY_*` to the 4 levels of Bridge.

## The branches

| Case | Result |
| --- | --- |
| The machine has no loader of Vulkan. | The line of the loader, and no adapter entity. |
| The instance is refused. | The line of the instance, and no adapter entity. |
| `validation` is on, and the layer is not there. | The instance is made with no layer, and `no validation layer` comes. |
| The layer gives a message. | 1 line of the log of the module. |
| The device was made. | `shutdown` waits for it before the destroys. |

## Functions

| Function | Result |
| --- | --- |
| `init` | Loads Vulkan, makes the instance and the messenger. Always gives 0. |
| `shutdown` | Destroys the objects of the module and closes Vulkan. |
| `make_instance` | Makes the instance of version 1.4, with the layer of validation when it is asked. Gives 0 at a refusal. |
| `make_messenger` | Makes the messenger of the layer, or writes the line of the layer. |
