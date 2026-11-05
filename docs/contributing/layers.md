# Layers

This document gives the 9 layers of an engine, the layers that the project holds, and the rules
that place a type on a layer. Each part of the project is on 1 layer.

## The layers

| Layer | Name | Content |
| --- | --- | --- |
| 0 | Platform | The hardware, the operating system and the drivers. |
| 1 | Foundation | Memory, containers, math, threads, logs, configuration, reflection, serialization, compression and cryptography. |
| 2 | Platform abstraction | Windows, displays, the clipboard, input devices, audio devices, the GPU device, files, sockets, the time source and the life cycle. |
| 3 | Kernel | The main loop, the time steps, the scheduler, the world model, queries, observers, the load of modules, the states of the application and the resources. |
| 4 | Engine subsystems | Rendering, the scene, animation, physics, audio, the map of inputs, UI, networking, navigation, effects, scripting, media, XR, persistence and the services of a platform. |
| 5 | Gameplay foundations | Prefabs, the flow of a game, triggers, abilities, inventory, dialogue, the behavior of non-player characters, camera rigs and the movement of a character. |
| 6 | Game systems | The code of 1 game. |
| 7 | Game | The content and the data of 1 game. |
| 8 | Tools | The editor, the pipeline of assets and the profilers. Each tool is a program of its own. It is not a part of the engine. |

## The layers of the project

The project holds the contracts of layers 2, 3 and 4.

## The parts

The folder of each layer has the name of the layer in snake_case, and holds the headers of
its parts. A part has no folder of its own:

```text
platform_abstraction/
    ideatek_audio.h
    ideatek_files.h
    ideatek_gpu.h
    ideatek_gpu_vulkan.h
    ideatek_input.h
    ideatek_input_gamepad.h
    ideatek_input_keyboard.h
    ideatek_input_mouse.h
    ideatek_input_text.h
    ideatek_input_touch.h
    ideatek_sockets.h
    ideatek_window.h
    ideatek_window_clipboard.h
    ideatek_world.h
kernel/
    ideatek_changes.h
    ideatek_frame.h
    ideatek_localization.h
    ideatek_resources.h
    ideatek_states.h
engine_subsystems/
    ideatek_animation.h
    ideatek_debug_draw.h
    ideatek_effects.h
    ideatek_input_mapping.h
    ideatek_navigation.h
    ideatek_networking.h
    ideatek_persistence.h
    ideatek_physics.h
    ideatek_rendering.h
    ideatek_services.h
    ideatek_sound.h
    ideatek_space.h
    ideatek_terrain.h
    ideatek_ui.h
```

The world part is on layer 2: it holds only the type that names an entity, and Bridge holds the
world model.

The time source of layer 2 is the read and the wait of the OS that the clock module makes. The
wait is precise and does not busy wait.

## The rules

- A type becomes a contract only when 2 modules of different authors exchange it. If not, the module or the game that uses the type owns it.
- A part names in `uses` only parts of its layer or of a lower layer.
- The contracts of layer 5 are in a different project.
- No part of the project is for a tool.
