# src/engine_subsystems/loader_gltf/gltf_clip.c

This file writes 1 clip: its channels, then the times and the values of each channel.

A channel with no node, or with a path that glTF 2.0 does not know, is skipped. The largest last time of the channels is the duration.

## Functions

| Function | Result |
| --- | --- |
| `is_kept` | `true` for a channel with a node and a known path. |
| `count_kept` | The kept channels, and the line of the skipped ones. |
| `write_channel` | Writes the record, the times and the values of 1 channel. |
| `last_time` | The last time of a channel, or 0. |
| `write_clip` | Writes 1 clip. |
