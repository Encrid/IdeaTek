# src/platform_abstraction/input_sdl3/input_fingers.c

This file turns the events of the fingers of SDL3 into `ideatek:FingerEvent`, and keeps the fingers that are down: `docs/extern/platform_abstraction/ideatek_input_touch.h.md`.

## A finger

`take_finger` fills an `ideatek:FingerEvent` from the event of SDL3:

| Type of SDL3 | `kind` |
| --- | --- |
| `SDL_EVENT_FINGER_DOWN` | `IDEATEK_FINGER_EVENT_KIND_BEGAN` |
| `SDL_EVENT_FINGER_MOTION` | `IDEATEK_FINGER_EVENT_KIND_MOVED` |
| `SDL_EVENT_FINGER_UP` | `IDEATEK_FINGER_EVENT_KIND_ENDED` |
| `SDL_EVENT_FINGER_CANCELED` | `IDEATEK_FINGER_EVENT_KIND_CANCELED` |

`id` is `fingerID` of SDL3. SDL3 gives `x` and `y` from 0 to 1 in the window. The module multiplies them by the size of the window in pixels, from `SDL_GetWindowSizeInPixels`.

A motion goes into a run with the key `id`. Thus consecutive motions of 1 finger become 1 event: `docs/intern/platform_abstraction/input_sdl3/input_runs.c.md`.
Each other kind spawns its event with `spawn_event`. A finger event needs 1 spawn and 48 bytes.

## The fingers that are down

`fingers` keeps the id and the last place of each finger that is down, at most `INPUT_FINGERS`.
A finger that begins goes into the list. A finger that ends or that SDL3 cancels leaves it. A motion updates its place.
A finger that begins while the list is full still gets its events. Only a loss of the focus does not cancel it.

`cancel_fingers` spawns an event of `IDEATEK_FINGER_EVENT_KIND_CANCELED` for each finger of the list, at its last place, then empties the list.

## The branches

| Case | Result |
| --- | --- |
| A finger touches, moves 3 times, and leaves. | 3 events with `merge_motion` at `true`: began, moved, ended. |
| 2 fingers move in turns. | Each motion starts a new run, because the key changes. |
| SDL3 cancels a finger. | 1 event of `IDEATEK_FINGER_EVENT_KIND_CANCELED`. |
| The 17th finger that is down. | Its events, and no place in the list. |
| A loss of the focus with 2 fingers down. | 2 events of `IDEATEK_FINGER_EVENT_KIND_CANCELED`. |
| A finger that is not in the list ends. | Its event. The list does not change. |
| A finger with no window. | Its places of SDL3, from 0 to 1. |

## Functions

| Function | Result |
| --- | --- |
| `kind_of` | Gives the kind of the contract of a type of SDL3. |
| `keep_finger` | Adds, moves or removes a finger in the list of the fingers that are down. |
| `run_finger` | Starts a run with the motion of a finger, or joins it to the run. |
| `take_finger` | Spawns the event of a finger, or puts its motion into a run. |
| `cancel_fingers` | Spawns the cancel of each finger that is down. |
