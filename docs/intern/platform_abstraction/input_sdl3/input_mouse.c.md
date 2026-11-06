# src/platform_abstraction/input_sdl3/input_mouse.c

This file turns the events of the mouse of SDL3 into `ideatek:MouseMotion`, `ideatek:MouseButton` and `ideatek:MouseWheel`, and keeps `ideatek:Mouse`: `docs/extern/platform_abstraction/ideatek_input_mouse.h.md`.

## The places

SDL3 gives the place of the mouse in the coordinates of the window. The contract gives it in pixels.
`to_pixels` multiplies a place by `SDL_GetWindowPixelDensity` of the window of the event: https://wiki.libsdl.org/SDL3/SDL_GetWindowPixelDensity.
An event with no window keeps its place.
The same factor goes on `rel_x` and `rel_y`.

## The motion

`take_motion` fills an `ideatek:MouseMotion` for `SDL_EVENT_MOUSE_MOTION`, with the places in pixels and the mask of the buttons.
If the motion joins the pending run, `x` and `y` of the run take the new place, and `rel_x` and `rel_y` add the new motion.
Else `start_run` starts a run with the motion, and the key of the run is the mask: `docs/intern/platform_abstraction/input_sdl3/input_runs.c.md`.
Then it sets `x`, `y` and `buttons` of `next_mouse`.

A motion that starts a run needs 1 spawn and 48 bytes. A motion that joins a run needs none.

## The buttons

`take_button` spawns an `ideatek:MouseButton` for `SDL_EVENT_MOUSE_BUTTON_DOWN` and `SDL_EVENT_MOUSE_BUTTON_UP`.
`button` and `clicks` come from the event. SDL3 gives 1 for the left button, 2 for the middle button, 3 for the right button, and 4 and 5 for the side buttons.
It sets or clears the bit `button - 1` of `next_mouse.buttons`, and sets `x` and `y` of `next_mouse`.
A button event needs 1 spawn and 40 bytes.

## The wheel

`take_wheel` spawns an `ideatek:MouseWheel` with `ticks_x` and `ticks_y` of the event.
With `SDL_MOUSEWHEEL_FLIPPED`, it negates the 2 values, so a positive `ticks_y` always goes away from the user: https://wiki.libsdl.org/SDL3/SDL_MouseWheelDirection.
A wheel event needs 1 spawn and 36 bytes.

## The branches

| Case | Result |
| --- | --- |
| 3 motions in a row, with `merge_motion` at `true`. | 1 event, with the place of the last motion and the sum of the 3 motions. |
| 3 motions in a row, with `merge_motion` at `false`. | 3 events. |
| 2 motions with a button event between them. | 2 events of motion, and the button event between them in the ranks. |
| A motion with a new mask of buttons. | It starts a new run. |
| A window with a pixel density of 2. | The places and the motions in pixels, 2 times the coordinates of SDL3. |
| An event with no window. | The places of SDL3. |
| A button goes down, then up. | 2 events, and the bit of the button at 1, then at 0. |
| A double click. | `clicks` of 2. |
| A wheel turns away from the user. | A positive `ticks_y`. |
| A flipped wheel turns away from the user. | A positive `ticks_y`. |
| A loss of the focus while 1 mouse button is down. | 1 event with `down` at `false`, and a mask of 0. |

## Functions

| Function | Result |
| --- | --- |
| `to_pixels` | Gives a place of SDL3 in the pixels of the window of the event. |
| `take_motion` | Starts a run of motions, or joins the motion to the run. |
| `take_button` | Spawns a mouse button event, and sets its bit in the mask. |
| `take_wheel` | Spawns the event of a wheel. |
