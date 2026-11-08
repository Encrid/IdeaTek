# src/platform_abstraction/gpu_vulkan_module/gpu_instance.c

This file makes the instance, and the messenger that writes each message of Vulkan into the log.

## The instance

`make_instance` asks for Vulkan 1.4 and for each extension of `WANTED` that the loader has. They are the 4 surfaces, the enumeration of portability and the messages of debug.
With `validation`, it also asks for the layer `VK_LAYER_KHRONOS_validation`, and puts a messenger into the chain of the instance.
That messenger hears the messages of the loader while it makes the instance.

```pseudo
FUNCTION open_instance() RETURNS BOOLEAN
   r <- vkCreateInstance()
   IF r = LAYER_NOT_PRESENT THEN
      CALL say("no validation layer")
      layers <- 0
      r <- vkCreateInstance()
   ENDIF
   IF r <> SUCCESS THEN
      CALL say("no instance: <result>")
      RETURN false
   ENDIF
   RETURN true
ENDFUNCTION
```

With no layer on the machine, the loader writes its message through the messenger. Then the module writes the line of the layer, and makes the instance again with no layer.
With `validation`, a messenger of the instance then writes each later message of the layer or of the loader.
Each message is 1 line at the level of a warning, cut at `GPU_MAX_LOG` bytes. An error of the layer does not fail the run.

## The branches

| Case | Result |
| --- | --- |
| `validation` and no layer. | The message of the loader, the line of the layer, and an instance with no layer. |
| No `validation`. | An instance with no layer and no messenger. |
| The instance is refused. | The line of the instance. |

## Functions

| Function | Result |
| --- | --- |
| `on_message` | Writes a message of Vulkan as 1 line of the log. |
| `available` | Gives the extensions of `WANTED` that the loader has. |
| `open_instance` | Makes the instance, again with no layer when the layer is not there. Gives `false` at a refusal. |
| `make_instance` | Makes the instance and the messenger. Gives `false` at a refusal. |
