# src/platform_abstraction/audio_sdl3/audio_devices.c

This file gives each audio device its entity, and follows the default device of each direction.

## A new device

`add_device` does these steps for a listed device and for `SDL_EVENT_AUDIO_DEVICE_ADDED`:

1. If the module holds `AUDIO_DEVICES` devices, it writes the line of the 33rd device, and stops.
2. It copies the name of `SDL_GetAudioDeviceName` with `SDL_utf8strlcpy`, which keeps whole code points in 127 bytes. A cut name writes the line of the long name.
3. It spawns the entity, and attaches `ideatek:AudioDevice` with the name, the direction and `is_default`.

A device without a name gets an empty name. A device that SDL3 lists twice gets 1 entity: `find_device` finds it first.

## The end of a device

`remove_device` does these steps for `SDL_EVENT_AUDIO_DEVICE_REMOVED`:

1. It finds the device by its id. A device with no entity changes nothing.
2. It destroys its entity, and removes the device from `devices`.
3. If a stream opened this device, the stream is `lost`: the round opens it again on the default: `docs/intern/platform_abstraction/audio_sdl3/audio_stream.c.md`.

An observer of `BRIDGE_ON_LEAVE` tells a reader of the destroy.

## The default

`update_defaults` asks `SDL_GetDefaultAudioDeviceID` for each direction. When the id differs from `defaults`:

1. The old default, if it has an entity, gets `ideatek:AudioDevice` again with `is_default` at `false`.
2. The new default, if it has an entity, gets `ideatek:AudioDevice` again with `is_default` at `true`.
3. `defaults` keeps the new id.

A stream opened on the default id follows the new default inside SDL3. Its state names the new device entity in the same round: `docs/intern/platform_abstraction/audio_sdl3/audio_state.c.md`.

## The branches

| Case | Result |
| --- | --- |
| A device comes. | An entity with its name and its direction. |
| A 33rd device. | The line of the devices, and no entity. |
| A name of more than 127 bytes. | Its first code points, after the line of the long name. |
| A device goes. | Its entity goes. |
| A device with no entity goes. | Nothing. |
| The device of a stream goes. | The stream is lost, and opens again on the default. |
| The default changes. | The 2 devices get `ideatek:AudioDevice` again. |
| The default does not change. | No attach. |

## Functions

| Function | Result |
| --- | --- |
| `find_device` | Gives the index of the device of an id, or `AUDIO_DEVICES`. |
| `attach_device` | Attaches `ideatek:AudioDevice` of a device, with its `is_default`. |
| `add_device` | Gives a device its entity, or writes the line of the devices. |
| `remove_device` | Destroys the entity of a device that went, and marks its stream lost. |
| `update_defaults` | Follows the default device of each direction. |
