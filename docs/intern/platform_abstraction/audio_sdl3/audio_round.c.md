# src/platform_abstraction/audio_sdl3/audio_round.c

This file holds the system `round`, which runs in the first phase and does the work of a round of the audio module.

## The round

```pseudo
FUNCTION bridge_round() RETURNS I32
   IF NOT audio_module.started THEN
      CALL start_audio()
   ENDIF
   IF audio_module.broken THEN
      RETURN 1
   ENDIF
   audio_module.side <- 1 - audio_module.side
   CALL SDL_PumpEvents()
   CALL take_events()
   CALL update_defaults()
   CALL apply_stream(audio_module.streams[0])
   CALL apply_stream(audio_module.streams[1])
   CALL take_recorded()
   CALL attach_state(audio_module.streams[0])
   CALL attach_state(audio_module.streams[1])
   RETURN 0
ENDFUNCTION
```

- `start_audio` runs in the first round only: `docs/intern/platform_abstraction/audio_sdl3/audio_start.c.md`.
- `SDL_PumpEvents` brings the events of the devices into the queue: SDL3 gives them from its pump. The window module pumps too, and 2 pumps in a round change nothing.
- `take_events` takes the 3 types of the audio devices from the queue with `SDL_PeepEvents`, and leaves each other event there. `SDL_EVENT_AUDIO_DEVICE_ADDED` adds a device, and `SDL_EVENT_AUDIO_DEVICE_REMOVED` removes one: `docs/intern/platform_abstraction/audio_sdl3/audio_devices.c.md`. `SDL_EVENT_AUDIO_DEVICE_FORMAT_CHANGED` changes nothing, because `ideatek:AudioDevice` holds no format.
- `update_defaults` follows the default of each direction.
- `apply_stream` opens, changes or closes a stream whose request came, or whose device went: `docs/intern/platform_abstraction/audio_sdl3/audio_stream.c.md`.
- `take_recorded` takes the recorded frames into the half of the round: `docs/intern/platform_abstraction/audio_sdl3/audio_frames.c.md`.
- `attach_state` attaches the state of a stream that changed: `docs/intern/platform_abstraction/audio_sdl3/audio_state.c.md`.

The devices come before the streams, so a request of this round can name a device that came in this round.

## The branches

| Case | Result |
| --- | --- |
| The first round, when a setting is outside its range. | `bridge_round` gives 1. |
| An event of a device. | A device comes or goes. |
| An event of a format. | Nothing. |
| No event. | Nothing. |

## Functions

| Function | Result |
| --- | --- |
| `take_events` | Takes the events of the audio devices from the queue. |
| `bridge_round` | Does the work of a round. Gives 1 with a bad setting. |
