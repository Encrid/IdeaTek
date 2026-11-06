# src/platform_abstraction/input_sdl3/input_text.c

This file starts, moves and stops the text input of SDL3 from `ideatek:TextRequest`, and turns each text of SDL3 into `ideatek:TextEvent`: `docs/extern/platform_abstraction/ideatek_input_text.h.md`.

## The request

The observer `text` hears `BRIDGE_ON_ENTER`, `BRIDGE_ON_ATTACH` and `BRIDGE_ON_LEAVE` of `ideatek:TextRequest`.
It reads only the triggers of the input entity. The contract puts the request there.

For an enter or an attach, `apply_text` does these steps:

1. It reads the request with `bridge_native_find_value`.
2. It takes the window of SDL3 from `SDL_GetWindows`, because the window module owns 1 window.
3. It sets the hint `SDL_HINT_ENABLE_SCREEN_KEYBOARD` to `"1"` or `"0"`, from `screen_keyboard`.
4. It calls `SDL_SetTextInputArea` with the field and the caret of the request, divided by `SDL_GetWindowPixelDensity`. The contract gives pixels, and SDL3 takes the coordinates of the window.
5. If the text input does not run, it calls `SDL_StartTextInput`. If SDL3 refuses, it writes `no text input: <reason>`. Else `text_on` is `true`.
6. If the text input runs and `screen_keyboard` changed, it stops and starts the text input again. Then SDL3 reads the hint again.

For a leave, it calls `SDL_StopTextInput`, and `text_on` is `false`.
SDL3 reads the hint of the screen keyboard when the text input starts: https://github.com/libsdl-org/SDL/blob/release-3.4.20/src/video/SDL_video.c#L5791.

## The text

`take_text` turns the text of `SDL_EVENT_TEXT_INPUT` into code points, with `SDL_StepUTF8`, and spawns 1 `ideatek:TextEvent` for each one, with its rank.
A text needs 1 spawn and 32 bytes for each code point. `need_of` counts the code points, so a text gets all its events in 1 round, or waits for the next round.
SDL3 gives the text in UTF-8, never as null, and its text holds only whole code points.

## The branches

| Case | Result |
| --- | --- |
| A request on the input entity. | The text input runs, with the area of the request. |
| The game attaches it again with a new caret. | The area moves. The text input keeps running. |
| The game attaches it again with a new `screen_keyboard`. | The text input stops and starts again. |
| The game detaches it. | The text input stops. |
| A request on another entity. | Nothing. |
| No window. | The line of no text input. |
| A text of 3 code points, 1 of them of 4 bytes. | 3 events, in their order. |
| A text with no room in the round. | It waits for the next round. |
| A detach of a request whose text input never started. | Nothing. |

## Functions

| Function | Result |
| --- | --- |
| `apply_text` | Starts the text input, or moves its area. |
| `bridge_text` | Applies or stops the text input for each trigger of the input entity. |
| `count_points` | Gives the number of code points of a text. |
| `take_text` | Spawns 1 event for each code point of a text. |
