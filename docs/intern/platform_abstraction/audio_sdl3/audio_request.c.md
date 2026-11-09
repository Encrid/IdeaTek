# src/platform_abstraction/audio_sdl3/audio_request.c

This file holds the observer of `ideatek:AudioRequest`, named `request`.

## The observer

`bridge_request` hears `BRIDGE_ON_ENTER`, `BRIDGE_ON_ATTACH` and `BRIDGE_ON_LEAVE` of `ideatek:AudioRequest`. For each trigger:

1. A leave finds the stream of the entity. The stream closes, and its entity is 0: `docs/intern/platform_abstraction/audio_sdl3/audio_stream.c.md`.
2. An enter or an attach reads the request with `bridge_native_find_value`, and takes the stream of `direction`. A direction outside the enum takes the playback stream, whose check refuses the request later. An entity that was on the other stream leaves it first.
3. If the stream has another entity, the log gets the line of the second stream.
4. Else the stream takes the entity and is `pending`: the round applies the request.

The observer keeps the request for the round, and opens nothing itself. So each open of SDL3 happens in `round`, in 1 place.

## The read of a request

`read_request` reads the pending request of a stream with `bridge_native_find_value`, and checks it with `check_request`.
A fault writes the line of the request, and `read_request` gives `false`. The round calls it: `docs/intern/platform_abstraction/audio_sdl3/audio_stream.c.md`.

## The branches

| Case | Result |
| --- | --- |
| A first request of a direction. | The stream is pending. |
| A request again on the same entity. | The stream is pending again. |
| A request of the other direction on the same entity. | The old stream closes, and the other stream is pending. |
| A request of a direction that has a stream, on another entity. | The line of the second stream. |
| A field outside its range. | The line of the request, and `false`. |
| The entity of a stream leaves. | The stream closes. |
| Another entity leaves. | Nothing. |

## Functions

| Function | Result |
| --- | --- |
| `take_request` | Marks the stream of a request, or writes the line of the second stream. |
| `bridge_request` | Reads each trigger. |
| `read_request` | Reads and checks the pending request of a stream. Gives `false` with a fault. |
