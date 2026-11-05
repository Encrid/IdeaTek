# include/engine_subsystems/ideatek_sound.h

`include/engine_subsystems/ideatek_sound.h` gives the types of the sound: sounds, players, spatial sound, listeners, buses, effects, zones, materials, voices, streams, music timing and analysis.
`readme.md`, *The contracts*, tells the rules of each contract.
`ideatek:SoundFrames` has the C type of the audio part of layer 2, so the header includes it.

## The roles

- The game is the author. It writes the sounds, the players, the buses, the effects, the zones and the other requests.
- The sound module mixes the players into the main bus, and sends the main bus to the output device of the audio part. It writes the states, the events and the levels.
- The sound module spawns the main bus at `init`. It is the only writer of `IDEATEK_BUS_FLAGS_MAIN`.

## The units

A volume is a linear gain: 1 keeps the signal, and 0 mutes it. The gain in dB is `20 log10(volume)`.
A threshold and a ceiling are in dB. A time is in seconds, and a frequency in hertz.
A position in a sound and a length of a sound are in frames.
The module ramps each change of a volume, a gain or a cutoff over 1 round, so a change does not click.

## The sounds

A sound is a resource entity. Its writer owns the bytes and takes the rate, the channels and the frames from the header of the file.
Without `IDEATEK_SOUND_FLAGS_STREAM`, the module decodes the whole sound at load. With it, the module keeps the encoded bytes and decodes them while it plays.
A loop with a begin and an end of 0 is the whole sound. The module logs each sound whose format it cannot decode once.

## The players

A change from STOPPED to PLAYING starts at `start_offset`. A change from PAUSED to PLAYING goes on. A change of `cue` starts again at `start_offset`.
At the end of a sound with no loop, the module sets the state to STOPPED and spawns `ideatek:SoundEnded`.
With `IDEATEK_SOUND_PLAYER_FLAGS_DESTROY_AT_END`, it destroys the player after the event. `pitch` changes the pitch and the speed together.

A start fades in over `fade_in`. A change to STOPPED or PAUSED first fades out over `fade_out`, and `ideatek:SoundEnded` comes after the fade.

## The spatial sound

A player with `ideatek:SoundSpatial` is at the place of its `ideatek:WorldTransform`. The gain at a distance `d`, with `r` the reference distance and `f` the rolloff:

```text
INVERSE:        r / (r + f (max(d, r) - r))
INVERSE_SQUARE: (r / (r + f (max(d, r) - r))) ^ 2
LINEAR:         1 - f (min(d, m) - r) / (m - r), with m the max distance
NONE:           1
CURVE:          the gain of rolloff_curve at d
```

A record of `rolloff_curve` is 2 floats: a distance and a gain, by rising distance, with no bytes between them. The gain between 2 records is a straight blend, and the gain of the first or the last record stays beyond it.
A max distance above 0 gives a gain of 0 beyond it. The cone points along -Z. The gain is 1 inside the inner angle, and the outer gain outside the outer angle. Between them, it is a straight blend.

The Doppler shift takes the speed from `ideatek:WorldTransform` and `ideatek:PreviousWorldTransform` of the player and the listener.
`doppler` scales the shift: 0 gives none, and 1 the physical shift. The references do not agree on a default, so it is 0.
`spatial_blend` mixes the 2D sound and the spatial sound: 0 is 2D, and 1 is fully spatial. `radius` is the size of the source, for the occlusion and the spread.

The module computes each spatial player against the closest listener, and multiplies by its volume. With no listener, no spatial player is heard.

## The buses and the effects

A bus sums its players and its child buses, and multiplies by its volume. A bus with a parent of 0, and a player with a bus of 0, go to the main bus.
A cycle of parents goes to the main bus, and the module logs the bus once. A change of the volume of a bus glides over its `fade`.
A send adds the signal of its player, after the volume and the spatial gain of the player, times its own volume, to its bus. A player can have many sends.

The effects of a bus apply by rising `order`. Equal orders apply in the order of the entities. An equalizer is a chain of filters.
The filters are the biquads of the cookbook of Robert Bristow-Johnson. The reverb technique is a choice of the module.

## The binaural sound

With `IDEATEK_SPATIAL_FLAGS_BINAURAL`, the module renders the player through a head-related transfer function, for headphones.
`hrtf` of `ideatek:SoundListener` names an entity with `ideatek:SoundHrtf`. 0 means the function of the module.

## The baked sound

A bake tool of the module makes `ideatek:SoundProbes` for the reflections and the paths of a place. The bytes are in the format of the module.
The module uses each `ideatek:SoundProbes` in the world. A module that does not know the format logs it once, and simulates in real time or without paths.

## The random containers

A player whose `sound` names an entity with `ideatek:SoundRandom` picks 1 of its sounds at each start and at each change of `cue`. A record of `sounds`:

| Field | C type | Offset in the record | Value |
| --- | --- | --- | --- |
| `sound` | `uint64_t` | 0 | The entity of an `ideatek:Sound`. |
| `weight` | `float` | 8 | The part of the picks, against the sum of the weights. |

The records have no bytes between them. A play takes a pitch from `1 / pitch_scale` to `pitch_scale`, and a gain of up to `volume_offset` dB more or less.
A loop repeats the sound that the player picked.

## The ambisonic sounds

A sound with `IDEATEK_SOUND_FLAGS_AMBISONIC` is a sound field of order 1, 2 or 3, with 4, 9 or 16 channels. Its layout is AmbiX: the ACN order with SN3D.
The field has the axes of the world, or of the `ideatek:WorldTransform` of the player if it has one. The module decodes it for the listener.

## The zones

A zone is an entity with `ideatek:SoundZone` and a sensor shape. The zone of a point is the zone of the highest priority whose shape holds it.
A player in a zone sends its signal times `reverb_send` to `reverb_bus`.
When the listener is in a zone and the player is not, the player takes the exterior volume and cutoff of the zone. When the player is in it and the listener is not, it takes the interior values.
The changes fade over `fade`.

## The materials of the sound

Only a shape with `ideatek:SoundMaterial` acts on the sound. A sensor shape with a sound material blocks the sound but not the bodies.
The 3 bands center on 400 Hz, 2.5 kHz and 15 kHz. The presets of steam-audio are:

| Preset | Absorption | Scattering | Transmission |
| --- | --- | --- | --- |
| Default | 0.10, 0.20, 0.30 | 0.05 | 0.100, 0.050, 0.030 |
| Brick | 0.03, 0.04, 0.07 | 0.05 | 0.015, 0.015, 0.015 |
| Carpet | 0.24, 0.69, 0.73 | 0.05 | 0.020, 0.005, 0.003 |
| Ceramic | 0.01, 0.01, 0.02 | 0.05 | 0.060, 0.044, 0.011 |
| Concrete | 0.05, 0.07, 0.08 | 0.05 | 0.015, 0.002, 0.001 |
| Glass | 0.06, 0.03, 0.02 | 0.05 | 0.060, 0.044, 0.011 |
| Gravel | 0.60, 0.70, 0.80 | 0.05 | 0.031, 0.012, 0.008 |
| Metal | 0.20, 0.07, 0.06 | 0.05 | 0.200, 0.025, 0.010 |
| Plaster | 0.12, 0.06, 0.04 | 0.05 | 0.056, 0.056, 0.004 |
| Rock | 0.13, 0.20, 0.24 | 0.05 | 0.015, 0.002, 0.001 |
| Wood | 0.11, 0.07, 0.06 | 0.05 | 0.070, 0.014, 0.005 |

The flags of `ideatek:SoundSpatial` tell the effects: OCCLUSION, TRANSMISSION, REFLECTIONS and PATHING. The module chooses the method.

## The voices

A group plays at most `max` players. Above it, the rule of the group chooses the player that loses. On a tie, the older player loses.
The loser fades over `fade`, then goes to STOPPED and spawns `ideatek:SoundEnded`. With PREVENT_NEW, the new player goes to STOPPED at once.
Above `max_voices`, the module mutes the quietest voices, and they go on: they are virtual.

## The generated sound

A player with `ideatek:SoundStream` and a sound of 0 plays frames from the game. The game attaches `ideatek:SoundFrames` in each round that has frames.
The rules of the audio part for `at`, `dropped`, `wanted` and `ahead` apply. An empty queue plays silence, and the state stays PLAYING.
The module converts the rate to the rate of the device.

## The music timing

A clock with a player has its beat 0 at `offset` frames of that player. A clock with a player of 0 starts at its first mixed frame. A bpm of 0 gives no beat.
A player with `ideatek:SoundSync` waits for the next beat or bar to start, to stop, or to change its cue. The change falls on the exact frame.
2 players that start and stop on the same bar make a crossfade over `fade_beats`.

## The analysis

A meter measures its bus after its effects. The module writes `ideatek:SoundLevel` in each round.
A band measures the highest magnitude between its low and high frequency. The window and the method are choices of the module.

## The settings

| Setting | Default | Range | Value |
| --- | --- | --- | --- |
| `speed_of_sound` | 343.3 | Above 0 | The speed of sound, in m / s, for the Doppler shift. |
| `max_voices` | 32 | 1 or more | The most voices that the module mixes. |
| `fft_size` | 1024 | A power of 2 | The size of the transform of the bands. |

## ideatek:ideatek_sound_format_t

`ideatek_sound_format_t` tells the encoding of a sound.
`ideatek_sound_format_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_SOUND_FORMAT_PCM_I16` | Samples of 16 bits. |
| 1 | `IDEATEK_SOUND_FORMAT_PCM_F32` | Samples of 32-bit floats. |
| 2 | `IDEATEK_SOUND_FORMAT_VORBIS` | Ogg Vorbis. |
| 3 | `IDEATEK_SOUND_FORMAT_OPUS` | Opus. |
| 4 | `IDEATEK_SOUND_FORMAT_FLAC` | FLAC. |
| 5 | `IDEATEK_SOUND_FORMAT_MP3` | MP3. |

## ideatek:ideatek_sound_flags_t

`ideatek_sound_flags_t` names the options of a sound.
`ideatek_sound_flags_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_SOUND_FLAGS_STREAM` | The module decodes while it plays. |
| 0x02 | `IDEATEK_SOUND_FLAGS_AMBISONIC` | The sound is a sound field: *The ambisonic sounds*. |

## ideatek:Sound

`ideatek:Sound` is a sound on a resource entity.
`ideatek_sound_t` takes 48 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `bytes` | `bridge_text_t` | 0 | No default | The encoded bytes. |
| `frames` | `uint64_t` | 8 | No default | The length of the sound. |
| `loop_begin` | `uint64_t` | 16 | 0 | The first frame of the loop. |
| `loop_end` | `uint64_t` | 24 | 0 | The frame after the loop. |
| `rate` | `uint32_t` | 32 | No default | The frames for each second. |
| `revision` | `uint32_t` | 36 | No default | Raised at each change. |
| `channels` | `uint8_t` | 40 | No default | The channels. |
| `format` | `ideatek_sound_format_t` | 41 | No default | The encoding. |
| `flags` | `ideatek_sound_flags_t` | 42 | 0 | The options. |

Bytes 43 to 47 hold no field.

## ideatek:SoundHrtf

`ideatek:SoundHrtf` is a head-related transfer function on a resource entity: *The binaural sound*.
`ideatek_sound_hrtf_t` takes 12 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `bytes` | `bridge_text_t` | 0 | The bytes of a SOFA file. |
| `revision` | `uint32_t` | 8 | Raised at each change. |

## ideatek:SoundProbes

`ideatek:SoundProbes` holds baked reflections and paths on a resource entity: *The baked sound*.
`ideatek_sound_probes_t` takes 12 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `bytes` | `bridge_text_t` | 0 | The baked data, in the format of the module. |
| `revision` | `uint32_t` | 8 | Raised at each change. |

## ideatek:SoundSend

`ideatek:SoundSend` sends the signal of a player to a bus: *The buses and the effects*.
`ideatek_sound_send_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `player` | `ideatek_entity_t` | 0 | No default | The player. |
| `bus` | `ideatek_entity_t` | 8 | No default | The bus that takes the signal. |
| `volume` | `float` | 16 | 1 | The gain of the send. |

Bytes 20 to 23 hold no field.

## ideatek:ideatek_random_mode_t

`ideatek_random_mode_t` tells how a random container picks.
`ideatek_random_mode_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_RANDOM_MODE_RANDOM_NO_REPEATS` | A random sound, never the last one twice. |
| 1 | `IDEATEK_RANDOM_MODE_RANDOM` | A random sound. |
| 2 | `IDEATEK_RANDOM_MODE_SEQUENTIAL` | The next sound in the order of the records. |

## ideatek:SoundRandom

`ideatek:SoundRandom` picks a sound at each start, on a resource entity: *The random containers*.
`ideatek_sound_random_t` takes 24 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `sounds` | `bridge_text_t` | 0 | No default | The records of the sounds: *The random containers*. |
| `count` | `uint32_t` | 8 | No default | The records in `sounds`. |
| `pitch_scale` | `float` | 12 | 1 | The largest factor of the pitch, 1 or more. |
| `volume_offset` | `float` | 16 | 0 | The most dB that a play adds or takes away. |
| `mode` | `ideatek_random_mode_t` | 20 | `IDEATEK_RANDOM_MODE_RANDOM_NO_REPEATS` | How it picks. |

Bytes 21 to 23 hold no field.

## ideatek:ideatek_sound_state_t

`ideatek_sound_state_t` tells the state of a player.
`ideatek_sound_state_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_SOUND_STATE_STOPPED` | The player is silent. |
| 1 | `IDEATEK_SOUND_STATE_PLAYING` | The player plays. |
| 2 | `IDEATEK_SOUND_STATE_PAUSED` | The player waits at its position. |

## ideatek:ideatek_sound_player_flags_t

`ideatek_sound_player_flags_t` names the options of a player.
`ideatek_sound_player_flags_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_SOUND_PLAYER_FLAGS_LOOP` | The player loops. |
| 0x02 | `IDEATEK_SOUND_PLAYER_FLAGS_DESTROY_AT_END` | The module destroys the player at the end. |

## ideatek:SoundPlayer

`ideatek:SoundPlayer` plays a sound.
`ideatek_sound_player_t` takes 48 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `sound` | `ideatek_entity_t` | 0 | No default | The sound, or a random container. |
| `bus` | `ideatek_entity_t` | 8 | 0 | The bus. 0 means the main bus. |
| `start_offset` | `uint64_t` | 16 | 0 | The frame of the start. |
| `volume` | `float` | 24 | 1 | The gain. |
| `pitch` | `float` | 28 | 1 | The factor of the speed. |
| `fade_in` | `float` | 32 | 0 | The time of the fade at a start. |
| `fade_out` | `float` | 36 | 0 | The time of the fade at a stop or a pause. |
| `cue` | `uint32_t` | 40 | 0 | A number that starts the sound again when it changes. |
| `state` | `ideatek_sound_state_t` | 44 | `IDEATEK_SOUND_STATE_STOPPED` | The state. |
| `flags` | `ideatek_sound_player_flags_t` | 45 | 0 | The options. |

Bytes 46 to 47 hold no field.

## ideatek:SoundPlayback

`ideatek:SoundPlayback` tells where a player is. The sound module writes it.
`ideatek_sound_playback_t` takes 16 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `position` | `uint64_t` | 0 | The frame that plays now. |
| `state` | `ideatek_sound_state_t` | 8 | The state that the module plays. |

Bytes 9 to 15 hold no field.

## ideatek:SoundEnded

`ideatek:SoundEnded` tells that a player ended. It is an event of 1 round.
`ideatek_sound_ended_t` takes 16 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `player` | `ideatek_entity_t` | 0 | The player. |
| `order` | `uint32_t` | 8 | The rank in the round. |

Bytes 12 to 15 hold no field.

## ideatek:ideatek_attenuation_t

`ideatek_attenuation_t` tells how the gain falls with the distance.
`ideatek_attenuation_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_ATTENUATION_INVERSE` | The inverse distance. |
| 1 | `IDEATEK_ATTENUATION_INVERSE_SQUARE` | The inverse square of the distance. |
| 2 | `IDEATEK_ATTENUATION_LINEAR` | A straight fall to the max distance. |
| 3 | `IDEATEK_ATTENUATION_NONE` | No fall. |
| 4 | `IDEATEK_ATTENUATION_CURVE` | The gains of `rolloff_curve`. |

## ideatek:ideatek_spatial_flags_t

`ideatek_spatial_flags_t` names the effects of the space on a player.
`ideatek_spatial_flags_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_SPATIAL_FLAGS_AIR_ABSORPTION` | The loss of high frequencies in the air. |
| 0x02 | `IDEATEK_SPATIAL_FLAGS_OCCLUSION` | The shapes between block the sound. |
| 0x04 | `IDEATEK_SPATIAL_FLAGS_TRANSMISSION` | The sound passes through the shapes, by band. It needs OCCLUSION. |
| 0x08 | `IDEATEK_SPATIAL_FLAGS_REFLECTIONS` | The echoes of the shapes. |
| 0x10 | `IDEATEK_SPATIAL_FLAGS_PATHING` | The sound goes around the shapes. |
| 0x20 | `IDEATEK_SPATIAL_FLAGS_BINAURAL` | The module renders through an HRTF: *The binaural sound*. |

## ideatek:SoundSpatial

`ideatek:SoundSpatial` places a player in the world: *The spatial sound*.
`ideatek_sound_spatial_t` takes 48 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `rolloff_curve` | `bridge_text_t` | 0 | No default | The records of `IDEATEK_ATTENUATION_CURVE`: *The spatial sound*. |
| `reference_distance` | `float` | 8 | 1 | The distance of the full gain. |
| `max_distance` | `float` | 12 | 0 | The distance of a gain of 0. 0 means no limit. |
| `rolloff` | `float` | 16 | 1 | The speed of the fall. |
| `cone_inner` | `float` | 20 | 2 pi | The inner angle of the cone. |
| `cone_outer` | `float` | 24 | 2 pi | The outer angle of the cone. |
| `cone_outer_gain` | `float` | 28 | 0 | The gain outside the cone. |
| `radius` | `float` | 32 | 0 | The size of the source, in m. 0 means a point. |
| `spatial_blend` | `float` | 36 | 1 | The part of spatial sound, from 0 to 1. |
| `doppler` | `float` | 40 | 0 | The factor of the Doppler shift. |
| `model` | `ideatek_attenuation_t` | 44 | `IDEATEK_ATTENUATION_INVERSE` | The fall. |
| `flags` | `ideatek_spatial_flags_t` | 45 | 0 | The effects. |

Bytes 46 to 47 hold no field.

A cone of 2 pi is no cone. A player with no `ideatek:SoundSpatial` is not spatial.

## ideatek:SoundListener

`ideatek:SoundListener` hears at the place of its entity. Its forward is -Z, its right +X, and its up +Y.
`ideatek_sound_listener_t` takes 16 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `hrtf` | `ideatek_entity_t` | 0 | 0 | The HRTF. 0 means that of the module. |
| `volume` | `float` | 8 | 1 | The gain of what the listener hears. |

Bytes 12 to 15 hold no field.

## ideatek:ideatek_bus_flags_t

`ideatek_bus_flags_t` names the options of a bus.
`ideatek_bus_flags_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_BUS_FLAGS_MUTE` | The bus is silent. |
| 0x02 | `IDEATEK_BUS_FLAGS_BYPASS` | The effects of the bus do not apply. |
| 0x04 | `IDEATEK_BUS_FLAGS_MAIN` | The main bus. Only the sound module sets it. |

## ideatek:SoundBus

`ideatek:SoundBus` mixes players and buses: *The buses and the effects*.
`ideatek_sound_bus_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `parent` | `ideatek_entity_t` | 0 | 0 | The bus that it goes to. 0 means the main bus. |
| `volume` | `float` | 8 | 1 | The gain. |
| `fade` | `float` | 12 | 0 | The time of a change of the volume. |
| `flags` | `ideatek_bus_flags_t` | 16 | 0 | The options. |

Bytes 17 to 23 hold no field.

## ideatek:ideatek_effect_flags_t

`ideatek_effect_flags_t` names the options of an effect.
`ideatek_effect_flags_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_EFFECT_FLAGS_BYPASS` | The effect does not apply. |

## ideatek:SoundEffect

`ideatek:SoundEffect` puts an effect on a bus. 1 type of a kind on the same entity tells the effect.
`ideatek_sound_effect_t` takes 16 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `bus` | `ideatek_entity_t` | 0 | The bus. |
| `order` | `uint32_t` | 8 | The rank on the bus. |
| `flags` | `ideatek_effect_flags_t` | 12 | The options. |

Bytes 13 to 15 hold no field.

## ideatek:ideatek_filter_mode_t

`ideatek_filter_mode_t` tells the kind of a filter.
`ideatek_filter_mode_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_FILTER_MODE_LOW_PASS` | Keeps the frequencies below the cutoff. |
| 1 | `IDEATEK_FILTER_MODE_HIGH_PASS` | Keeps the frequencies above it. |
| 2 | `IDEATEK_FILTER_MODE_BAND_PASS` | Keeps a band about it. |
| 3 | `IDEATEK_FILTER_MODE_NOTCH` | Removes a band about it. |
| 4 | `IDEATEK_FILTER_MODE_PEAK` | Changes a band by `gain`. |
| 5 | `IDEATEK_FILTER_MODE_LOW_SHELF` | Changes the frequencies below by `gain`. |
| 6 | `IDEATEK_FILTER_MODE_HIGH_SHELF` | Changes the frequencies above by `gain`. |

## ideatek:SoundFilter

`ideatek:SoundFilter` is a biquad filter.
`ideatek_sound_filter_t` takes 16 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `cutoff` | `float` | 0 | 2000 | The frequency. |
| `resonance` | `float` | 4 | 0.5 | The Q of the filter. |
| `gain` | `float` | 8 | 1 | The gain of a peak or a shelf. |
| `mode` | `ideatek_filter_mode_t` | 12 | `IDEATEK_FILTER_MODE_LOW_PASS` | The kind. |

Bytes 13 to 15 hold no field.

## ideatek:SoundCompressor

`ideatek:SoundCompressor` lowers the loud parts.
`ideatek_sound_compressor_t` takes 32 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `sidechain` | `ideatek_entity_t` | 0 | 0 | The bus that drives it. 0 means its own bus. |
| `threshold` | `float` | 8 | 0 | The level where it starts, in dB. |
| `ratio` | `float` | 12 | 4 | The ratio above the threshold. |
| `attack` | `float` | 16 | 0.00002 | The time to act. |
| `release` | `float` | 20 | 0.25 | The time to let go. |
| `gain` | `float` | 24 | 1 | The gain after it. |
| `mix` | `float` | 28 | 1 | The part of the compressed signal. |

## ideatek:SoundLimiter

`ideatek:SoundLimiter` keeps the signal under a ceiling.
`ideatek_sound_limiter_t` takes 8 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `ceiling` | `float` | 0 | -0.3 | The highest level, in dB. |
| `release` | `float` | 4 | 0.1 | The time to let go. |

## ideatek:SoundReverb

`ideatek:SoundReverb` gives the echoes of a room.
`ideatek_sound_reverb_t` takes 24 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `predelay` | `float` | 0 | 0.15 | The time before the reverb. |
| `room_size` | `float` | 4 | 0.8 | The size of the room, from 0 to 1. |
| `damping` | `float` | 8 | 0.5 | The loss of high frequencies. |
| `spread` | `float` | 12 | 1 | The stereo width. |
| `dry` | `float` | 16 | 1 | The gain of the direct signal. |
| `wet` | `float` | 20 | 0.5 | The gain of the reverb. |

## ideatek:SoundDelay

`ideatek:SoundDelay` gives an echo.
`ideatek_sound_delay_t` takes 16 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `delay` | `float` | 0 | 0.25 | The time of the echo. |
| `feedback` | `float` | 4 | 0 | The part that comes back. |
| `dry` | `float` | 8 | 1 | The gain of the direct signal. |
| `wet` | `float` | 12 | 0.5 | The gain of the echo. |

## ideatek:SoundZone

`ideatek:SoundZone` changes the sound of a room: *The zones*.
`ideatek_sound_zone_t` takes 40 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `reverb_bus` | `ideatek_entity_t` | 0 | 0 | The bus of the reverb. 0 means no send. |
| `priority` | `int32_t` | 8 | 0 | The zone of the highest priority wins. |
| `reverb_send` | `float` | 12 | 0 | The gain of the send. |
| `exterior_volume` | `float` | 16 | 1 | The gain of a player outside, for a listener inside. |
| `exterior_cutoff` | `float` | 20 | 20000 | The low-pass of that player. |
| `interior_volume` | `float` | 24 | 1 | The gain of a player inside, for a listener outside. |
| `interior_cutoff` | `float` | 28 | 20000 | The low-pass of that player. |
| `fade` | `float` | 32 | 0.5 | The time of a change. |

Bytes 36 to 39 hold no field.

## ideatek:SoundMaterial

`ideatek:SoundMaterial` is the sound of the matter of a shape, on the entity of the shape.
`ideatek_sound_material_t` takes 28 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `absorption_low` | `float` | 0 | 0.10 | The absorption of the low band. |
| `absorption_mid` | `float` | 4 | 0.20 | The absorption of the middle band. |
| `absorption_high` | `float` | 8 | 0.30 | The absorption of the high band. |
| `scattering` | `float` | 12 | 0.05 | The part that scatters. |
| `transmission_low` | `float` | 16 | 0.100 | The transmission of the low band. |
| `transmission_mid` | `float` | 20 | 0.050 | The transmission of the middle band. |
| `transmission_high` | `float` | 24 | 0.030 | The transmission of the high band. |

The defaults are the Default preset of steam-audio.

## ideatek:ideatek_voice_rule_t

`ideatek_voice_rule_t` tells which player of a full group loses.
`ideatek_voice_rule_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_VOICE_RULE_PREVENT_NEW` | The new player loses. |
| 1 | `IDEATEK_VOICE_RULE_STOP_OLDEST` | The oldest player loses. |
| 2 | `IDEATEK_VOICE_RULE_STOP_FARTHEST` | The player farthest from the closest listener loses. |
| 3 | `IDEATEK_VOICE_RULE_STOP_LOWEST_PRIORITY` | The player of the lowest priority loses. |
| 4 | `IDEATEK_VOICE_RULE_STOP_QUIETEST` | The quietest player loses. |

## ideatek:SoundGroup

`ideatek:SoundGroup` limits the players of a group: *The voices*.
`ideatek_sound_group_t` takes 12 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `fade` | `float` | 0 | 0 | The fade of a lost player. |
| `max` | `uint32_t` | 4 | 16 | The most players of the group. |
| `rule` | `ideatek_voice_rule_t` | 8 | `IDEATEK_VOICE_RULE_STOP_FARTHEST` | The loser. |

Bytes 9 to 11 hold no field.

## ideatek:SoundVoice

`ideatek:SoundVoice` puts a player in a group.
`ideatek_sound_voice_t` takes 16 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `group` | `ideatek_entity_t` | 0 | 0 | The group. 0 means none. |
| `priority` | `float` | 8 | 1 | The priority of the player. |

Bytes 12 to 15 hold no field.

A player with no `ideatek:SoundVoice` has a priority of 1 and no group.

## ideatek:SoundStream

`ideatek:SoundStream` makes a player play frames from the game: *The generated sound*.
`ideatek_sound_stream_t` takes 12 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `rate` | `uint32_t` | 0 | 44100 | The frames for each second. |
| `ahead` | `uint32_t` | 4 | 22050 | The frames that the module keeps queued. |
| `channels` | `uint8_t` | 8 | 2 | The channels. |

Bytes 9 to 11 hold no field.

A player with a sound and a stream plays nothing, and the module logs it once.

## ideatek:SoundFrames

`ideatek:SoundFrames` holds a batch of frames for a stream player. Its C type is `ideatek_audio_frames_t` of the audio part of layer 2, with frames of `float`, interleaved.

## ideatek:SoundStreamState

`ideatek:SoundStreamState` is the state of a stream player. The sound module writes it.
`ideatek_sound_stream_state_t` takes 16 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `dropped` | `uint64_t` | 0 | The frames that the module dropped. |
| `queued` | `uint32_t` | 8 | The frames in the queue. |
| `wanted` | `uint32_t` | 12 | The frames that fill the queue to `ahead`. |

## ideatek:ideatek_clock_flags_t

`ideatek_clock_flags_t` names the options of a clock.
`ideatek_clock_flags_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_CLOCK_FLAGS_BEAT_EVENTS` | The clock spawns `ideatek:SoundBeat`. |

## ideatek:SoundClock

`ideatek:SoundClock` counts the beats and the bars of a music.
`ideatek_sound_clock_t` takes 32 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `player` | `ideatek_entity_t` | 0 | 0 | The player of the music. 0 means none. |
| `offset` | `uint64_t` | 8 | 0 | The frame of beat 0 in that player. |
| `bpm` | `float` | 16 | 0 | The beats for each minute. 0 means no beat. |
| `beats_per_bar` | `uint32_t` | 20 | 4 | The beats of a bar. |
| `flags` | `ideatek_clock_flags_t` | 24 | 0 | The options. |

Bytes 25 to 31 hold no field.

## ideatek:ideatek_quantum_t

`ideatek_quantum_t` tells when a synced change falls.
`ideatek_quantum_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_QUANTUM_NOW` | At once. |
| 1 | `IDEATEK_QUANTUM_BEAT` | At the next beat. |
| 2 | `IDEATEK_QUANTUM_BAR` | At the next bar. |

## ideatek:SoundSync

`ideatek:SoundSync` makes the changes of a player wait for the clock: *The music timing*.
`ideatek_sound_sync_t` takes 16 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `clock` | `ideatek_entity_t` | 0 | No default | The clock. |
| `fade_beats` | `float` | 8 | 0 | The beats of the fade in and out. |
| `quantum` | `ideatek_quantum_t` | 12 | `IDEATEK_QUANTUM_NOW` | When a change falls. |

Bytes 13 to 15 hold no field.

## ideatek:SoundBeat

`ideatek:SoundBeat` tells that a beat came. It is an event of 1 round.
`ideatek_sound_beat_t` takes 32 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `clock` | `ideatek_entity_t` | 0 | The clock. |
| `beat` | `uint64_t` | 8 | The beat, from 0. |
| `bar` | `uint64_t` | 16 | The bar, from 0. |
| `order` | `uint32_t` | 24 | The rank in the round. |

Bytes 28 to 31 hold no field.

## ideatek:SoundMeter

`ideatek:SoundMeter` measures a bus.
`ideatek_sound_meter_t` takes 16 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `bus` | `ideatek_entity_t` | 0 | 0 | The bus. 0 means the main bus. |
| `attack` | `float` | 8 | 0.3 | The time of a rise of the level. |
| `release` | `float` | 12 | 0.3 | The time of a fall. |

## ideatek:SoundLevel

`ideatek:SoundLevel` is the level of a meter. The sound module writes it.
`ideatek_sound_level_t` takes 12 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `rms` | `float` | 0 | The smoothed root mean square. |
| `peak` | `float` | 4 | The highest sample of the round. |
| `clipped` | `uint32_t` | 8 | The samples above 1. |

## ideatek:SoundBand

`ideatek:SoundBand` measures a band of the bus of a meter.
`ideatek_sound_band_t` takes 16 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `meter` | `ideatek_entity_t` | 0 | The meter. |
| `low` | `float` | 8 | The lowest frequency. |
| `high` | `float` | 12 | The highest frequency. |

## ideatek:SoundBandLevel

`ideatek:SoundBandLevel` is the level of a band. The sound module writes it.
`ideatek_sound_band_level_t` takes 4 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `magnitude` | `float` | 0 | The highest linear magnitude in the band. |

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:ideatek_sound_format_t` | `ideatek_sound_format_t` | `ideatek_sound_format_t_desc` | `IDEATEK_SOUND_FORMAT_VALUES` |
| `ideatek:ideatek_sound_flags_t` | `ideatek_sound_flags_t` | `ideatek_sound_flags_t_desc` | `IDEATEK_SOUND_FLAGS_VALUES` |
| `ideatek:Sound` | `ideatek_sound_t` | `ideatek_sound_t_desc` | `IDEATEK_SOUND_FIELDS` |
| `ideatek:SoundHrtf` | `ideatek_sound_hrtf_t` | `ideatek_sound_hrtf_t_desc` | `IDEATEK_SOUND_HRTF_FIELDS` |
| `ideatek:SoundProbes` | `ideatek_sound_probes_t` | `ideatek_sound_probes_t_desc` | `IDEATEK_SOUND_PROBES_FIELDS` |
| `ideatek:SoundSend` | `ideatek_sound_send_t` | `ideatek_sound_send_t_desc` | `IDEATEK_SOUND_SEND_FIELDS` |
| `ideatek:ideatek_random_mode_t` | `ideatek_random_mode_t` | `ideatek_random_mode_t_desc` | `IDEATEK_RANDOM_MODE_VALUES` |
| `ideatek:SoundRandom` | `ideatek_sound_random_t` | `ideatek_sound_random_t_desc` | `IDEATEK_SOUND_RANDOM_FIELDS` |
| `ideatek:ideatek_sound_state_t` | `ideatek_sound_state_t` | `ideatek_sound_state_t_desc` | `IDEATEK_SOUND_STATE_VALUES` |
| `ideatek:ideatek_sound_player_flags_t` | `ideatek_sound_player_flags_t` | `ideatek_sound_player_flags_t_desc` | `IDEATEK_SOUND_PLAYER_FLAGS_VALUES` |
| `ideatek:SoundPlayer` | `ideatek_sound_player_t` | `ideatek_sound_player_t_desc` | `IDEATEK_SOUND_PLAYER_FIELDS` |
| `ideatek:SoundPlayback` | `ideatek_sound_playback_t` | `ideatek_sound_playback_t_desc` | `IDEATEK_SOUND_PLAYBACK_FIELDS` |
| `ideatek:SoundEnded` | `ideatek_sound_ended_t` | `ideatek_sound_ended_t_desc` | `IDEATEK_SOUND_ENDED_FIELDS` |
| `ideatek:ideatek_attenuation_t` | `ideatek_attenuation_t` | `ideatek_attenuation_t_desc` | `IDEATEK_ATTENUATION_VALUES` |
| `ideatek:ideatek_spatial_flags_t` | `ideatek_spatial_flags_t` | `ideatek_spatial_flags_t_desc` | `IDEATEK_SPATIAL_FLAGS_VALUES` |
| `ideatek:SoundSpatial` | `ideatek_sound_spatial_t` | `ideatek_sound_spatial_t_desc` | `IDEATEK_SOUND_SPATIAL_FIELDS` |
| `ideatek:SoundListener` | `ideatek_sound_listener_t` | `ideatek_sound_listener_t_desc` | `IDEATEK_SOUND_LISTENER_FIELDS` |
| `ideatek:ideatek_bus_flags_t` | `ideatek_bus_flags_t` | `ideatek_bus_flags_t_desc` | `IDEATEK_BUS_FLAGS_VALUES` |
| `ideatek:SoundBus` | `ideatek_sound_bus_t` | `ideatek_sound_bus_t_desc` | `IDEATEK_SOUND_BUS_FIELDS` |
| `ideatek:ideatek_effect_flags_t` | `ideatek_effect_flags_t` | `ideatek_effect_flags_t_desc` | `IDEATEK_EFFECT_FLAGS_VALUES` |
| `ideatek:SoundEffect` | `ideatek_sound_effect_t` | `ideatek_sound_effect_t_desc` | `IDEATEK_SOUND_EFFECT_FIELDS` |
| `ideatek:ideatek_filter_mode_t` | `ideatek_filter_mode_t` | `ideatek_filter_mode_t_desc` | `IDEATEK_FILTER_MODE_VALUES` |
| `ideatek:SoundFilter` | `ideatek_sound_filter_t` | `ideatek_sound_filter_t_desc` | `IDEATEK_SOUND_FILTER_FIELDS` |
| `ideatek:SoundCompressor` | `ideatek_sound_compressor_t` | `ideatek_sound_compressor_t_desc` | `IDEATEK_SOUND_COMPRESSOR_FIELDS` |
| `ideatek:SoundLimiter` | `ideatek_sound_limiter_t` | `ideatek_sound_limiter_t_desc` | `IDEATEK_SOUND_LIMITER_FIELDS` |
| `ideatek:SoundReverb` | `ideatek_sound_reverb_t` | `ideatek_sound_reverb_t_desc` | `IDEATEK_SOUND_REVERB_FIELDS` |
| `ideatek:SoundDelay` | `ideatek_sound_delay_t` | `ideatek_sound_delay_t_desc` | `IDEATEK_SOUND_DELAY_FIELDS` |
| `ideatek:SoundZone` | `ideatek_sound_zone_t` | `ideatek_sound_zone_t_desc` | `IDEATEK_SOUND_ZONE_FIELDS` |
| `ideatek:SoundMaterial` | `ideatek_sound_material_t` | `ideatek_sound_material_t_desc` | `IDEATEK_SOUND_MATERIAL_FIELDS` |
| `ideatek:ideatek_voice_rule_t` | `ideatek_voice_rule_t` | `ideatek_voice_rule_t_desc` | `IDEATEK_VOICE_RULE_VALUES` |
| `ideatek:SoundGroup` | `ideatek_sound_group_t` | `ideatek_sound_group_t_desc` | `IDEATEK_SOUND_GROUP_FIELDS` |
| `ideatek:SoundVoice` | `ideatek_sound_voice_t` | `ideatek_sound_voice_t_desc` | `IDEATEK_SOUND_VOICE_FIELDS` |
| `ideatek:SoundStream` | `ideatek_sound_stream_t` | `ideatek_sound_stream_t_desc` | `IDEATEK_SOUND_STREAM_FIELDS` |
| `ideatek:SoundFrames` | `ideatek_audio_frames_t` | `ideatek_audio_frames_t_desc` | `IDEATEK_AUDIO_FRAMES_FIELDS` |
| `ideatek:SoundStreamState` | `ideatek_sound_stream_state_t` | `ideatek_sound_stream_state_t_desc` | `IDEATEK_SOUND_STREAM_STATE_FIELDS` |
| `ideatek:ideatek_clock_flags_t` | `ideatek_clock_flags_t` | `ideatek_clock_flags_t_desc` | `IDEATEK_CLOCK_FLAGS_VALUES` |
| `ideatek:SoundClock` | `ideatek_sound_clock_t` | `ideatek_sound_clock_t_desc` | `IDEATEK_SOUND_CLOCK_FIELDS` |
| `ideatek:ideatek_quantum_t` | `ideatek_quantum_t` | `ideatek_quantum_t_desc` | `IDEATEK_QUANTUM_VALUES` |
| `ideatek:SoundSync` | `ideatek_sound_sync_t` | `ideatek_sound_sync_t_desc` | `IDEATEK_SOUND_SYNC_FIELDS` |
| `ideatek:SoundBeat` | `ideatek_sound_beat_t` | `ideatek_sound_beat_t_desc` | `IDEATEK_SOUND_BEAT_FIELDS` |
| `ideatek:SoundMeter` | `ideatek_sound_meter_t` | `ideatek_sound_meter_t_desc` | `IDEATEK_SOUND_METER_FIELDS` |
| `ideatek:SoundLevel` | `ideatek_sound_level_t` | `ideatek_sound_level_t_desc` | `IDEATEK_SOUND_LEVEL_FIELDS` |
| `ideatek:SoundBand` | `ideatek_sound_band_t` | `ideatek_sound_band_t_desc` | `IDEATEK_SOUND_BAND_FIELDS` |
| `ideatek:SoundBandLevel` | `ideatek_sound_band_level_t` | `ideatek_sound_band_level_t_desc` | `IDEATEK_SOUND_BAND_LEVEL_FIELDS` |

```text
{ IDEATEK_SOUND_FORMAT_PCM_I16 = 0, IDEATEK_SOUND_FORMAT_PCM_F32 = 1, IDEATEK_SOUND_FORMAT_VORBIS = 2, IDEATEK_SOUND_FORMAT_OPUS = 3, IDEATEK_SOUND_FORMAT_FLAC = 4, IDEATEK_SOUND_FORMAT_MP3 = 5 }
{ IDEATEK_SOUND_FLAGS_STREAM = 0x01, IDEATEK_SOUND_FLAGS_AMBISONIC = 0x02 }
{ bridge_text_t bytes; uint64_t frames; uint64_t loop_begin; uint64_t loop_end; uint32_t rate; uint32_t revision; uint8_t channels; ideatek_sound_format_t format; ideatek_sound_flags_t flags; }
{ bridge_text_t bytes; uint32_t revision; }
{ bridge_text_t bytes; uint32_t revision; }
{ ideatek_entity_t player; ideatek_entity_t bus; float volume; }
{ IDEATEK_RANDOM_MODE_RANDOM_NO_REPEATS = 0, IDEATEK_RANDOM_MODE_RANDOM = 1, IDEATEK_RANDOM_MODE_SEQUENTIAL = 2 }
{ bridge_text_t sounds; uint32_t count; float pitch_scale; float volume_offset; ideatek_random_mode_t mode; }
{ IDEATEK_SOUND_STATE_STOPPED = 0, IDEATEK_SOUND_STATE_PLAYING = 1, IDEATEK_SOUND_STATE_PAUSED = 2 }
{ IDEATEK_SOUND_PLAYER_FLAGS_LOOP = 0x01, IDEATEK_SOUND_PLAYER_FLAGS_DESTROY_AT_END = 0x02 }
{ ideatek_entity_t sound; ideatek_entity_t bus; uint64_t start_offset; float volume; float pitch; float fade_in; float fade_out; uint32_t cue; ideatek_sound_state_t state; ideatek_sound_player_flags_t flags; }
{ uint64_t position; ideatek_sound_state_t state; }
{ ideatek_entity_t player; uint32_t order; }
{ IDEATEK_ATTENUATION_INVERSE = 0, IDEATEK_ATTENUATION_INVERSE_SQUARE = 1, IDEATEK_ATTENUATION_LINEAR = 2, IDEATEK_ATTENUATION_NONE = 3, IDEATEK_ATTENUATION_CURVE = 4 }
{ IDEATEK_SPATIAL_FLAGS_AIR_ABSORPTION = 0x01, IDEATEK_SPATIAL_FLAGS_OCCLUSION = 0x02, IDEATEK_SPATIAL_FLAGS_TRANSMISSION = 0x04, IDEATEK_SPATIAL_FLAGS_REFLECTIONS = 0x08, IDEATEK_SPATIAL_FLAGS_PATHING = 0x10, IDEATEK_SPATIAL_FLAGS_BINAURAL = 0x20 }
{ bridge_text_t rolloff_curve; float reference_distance; float max_distance; float rolloff; float cone_inner; float cone_outer; float cone_outer_gain; float radius; float spatial_blend; float doppler; ideatek_attenuation_t model; ideatek_spatial_flags_t flags; }
{ ideatek_entity_t hrtf; float volume; }
{ IDEATEK_BUS_FLAGS_MUTE = 0x01, IDEATEK_BUS_FLAGS_BYPASS = 0x02, IDEATEK_BUS_FLAGS_MAIN = 0x04 }
{ ideatek_entity_t parent; float volume; float fade; ideatek_bus_flags_t flags; }
{ IDEATEK_EFFECT_FLAGS_BYPASS = 0x01 }
{ ideatek_entity_t bus; uint32_t order; ideatek_effect_flags_t flags; }
{ IDEATEK_FILTER_MODE_LOW_PASS = 0, IDEATEK_FILTER_MODE_HIGH_PASS = 1, IDEATEK_FILTER_MODE_BAND_PASS = 2, IDEATEK_FILTER_MODE_NOTCH = 3, IDEATEK_FILTER_MODE_PEAK = 4, IDEATEK_FILTER_MODE_LOW_SHELF = 5, IDEATEK_FILTER_MODE_HIGH_SHELF = 6 }
{ float cutoff; float resonance; float gain; ideatek_filter_mode_t mode; }
{ ideatek_entity_t sidechain; float threshold; float ratio; float attack; float release; float gain; float mix; }
{ float ceiling; float release; }
{ float predelay; float room_size; float damping; float spread; float dry; float wet; }
{ float delay; float feedback; float dry; float wet; }
{ ideatek_entity_t reverb_bus; int32_t priority; float reverb_send; float exterior_volume; float exterior_cutoff; float interior_volume; float interior_cutoff; float fade; }
{ float absorption_low; float absorption_mid; float absorption_high; float scattering; float transmission_low; float transmission_mid; float transmission_high; }
{ IDEATEK_VOICE_RULE_PREVENT_NEW = 0, IDEATEK_VOICE_RULE_STOP_OLDEST = 1, IDEATEK_VOICE_RULE_STOP_FARTHEST = 2, IDEATEK_VOICE_RULE_STOP_LOWEST_PRIORITY = 3, IDEATEK_VOICE_RULE_STOP_QUIETEST = 4 }
{ float fade; uint32_t max; ideatek_voice_rule_t rule; }
{ ideatek_entity_t group; float priority; }
{ uint32_t rate; uint32_t ahead; uint8_t channels; }
{ uint64_t dropped; uint32_t queued; uint32_t wanted; }
{ IDEATEK_CLOCK_FLAGS_BEAT_EVENTS = 0x01 }
{ ideatek_entity_t player; uint64_t offset; float bpm; uint32_t beats_per_bar; ideatek_clock_flags_t flags; }
{ IDEATEK_QUANTUM_NOW = 0, IDEATEK_QUANTUM_BEAT = 1, IDEATEK_QUANTUM_BAR = 2 }
{ ideatek_entity_t clock; float fade_beats; ideatek_quantum_t quantum; }
{ ideatek_entity_t clock; uint64_t beat; uint64_t bar; uint32_t order; }
{ ideatek_entity_t bus; float attack; float release; }
{ float rms; float peak; uint32_t clipped; }
{ ideatek_entity_t meter; float low; float high; }
{ float magnitude; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:ideatek_entity_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_sound_format_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_sound_flags_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_random_mode_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_sound_state_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_sound_player_flags_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_attenuation_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_spatial_flags_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_bus_flags_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_effect_flags_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_filter_mode_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_voice_rule_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_clock_flags_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_quantum_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Sound` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SoundHrtf` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SoundProbes` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SoundSend` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SoundRandom` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SoundPlayer` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SoundPlayback` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SoundEnded` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SoundSpatial` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SoundListener` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SoundBus` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SoundEffect` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SoundFilter` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SoundCompressor` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SoundLimiter` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SoundReverb` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SoundDelay` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SoundZone` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SoundMaterial` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SoundGroup` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SoundVoice` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SoundStream` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SoundFrames` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SoundStreamState` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SoundClock` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SoundSync` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SoundBeat` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SoundMeter` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SoundLevel` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SoundBand` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SoundBandLevel` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

## The grants

| Role | `write` | `read` |
| --- | --- | --- |
| The game | The sounds, the players, the buses, the effects, the zones, the materials, the streams, the clocks, the meters and the other requests | The types of the fields |
| The sound module | `ideatek:SoundPlayback`, `ideatek:SoundEnded`, `ideatek:SoundBus`, `ideatek:SoundStreamState`, `ideatek:SoundBeat`, `ideatek:SoundLevel` and `ideatek:SoundBandLevel` | Each type that the game writes, `ideatek:WorldTransform`, `ideatek:Shape`, and the types of the fields |
