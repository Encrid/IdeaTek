# src/platform_abstraction/window_sdl3/window_drops.c

This file takes the events of the drops from the queue of SDL3, and spawns an entity of `ideatek:Drop` for each item.
The rules of the drops are in `docs/extern/platform_abstraction/ideatek_window.h.md`, *The drops*.

## The take

`take_drops` takes the events of the drops 1 at a time, while the round has room for 1 more item:

```pseudo
PROCEDURE take_drops()
   DECLARE e : SDL_Event
   WHILE has_room(1, 72) AND SDL_PeepEvents(e, 1, SDL_GETEVENT, SDL_EVENT_DROP_FILE, SDL_EVENT_DROP_POSITION) = 1
      CALL take_drop(e)
   ENDWHILE
ENDPROCEDURE
```

An item takes 1 spawn and 1 attach of `ideatek:Drop`, whose bytes of `changes` `CHANGE_BYTES` gives.
Without the room, the other events of the drops stay in the queue, in their order, and the next round takes them.
SDL3 keeps the text of an event only until the next pump, so `take_drop` copies it in the same round.

## The events

`take_drop` does the action of the type of the event:

| Type | Action |
| --- | --- |
| `SDL_EVENT_DROP_BEGIN` | Adds 1 to `set`, sets `index` to 0, `drop_over` to `true`, and `drop_x` and `drop_y` to 0. |
| `SDL_EVENT_DROP_POSITION` | Sets `drop_x` and `drop_y` to the point of the event. |
| `SDL_EVENT_DROP_FILE` | Calls `spawn_drop` with `IDEATEK_DROP_FILE`. |
| `SDL_EVENT_DROP_TEXT` | Calls `spawn_drop` with `IDEATEK_DROP_TEXT`. |
| `SDL_EVENT_DROP_COMPLETE` | Sets `drop_over` to `false`. |

Each action also sets `dirty` to `true`, so `attach_state` reads the drag again.
SDL3 sends `SDL_EVENT_DROP_BEGIN` before the first item of each drag: https://github.com/libsdl-org/SDL/blob/release-3.4.20/src/events/SDL_dropevents.c#L39.
Thus the first drag has the `set` 1.

## An item

`spawn_drop` fills a value of `ideatek:Drop`, and spawns its entity with `spawn_item`:

```pseudo
PROCEDURE spawn_drop(e : SDL_Event, kind : ideatek_drop_kind_t)
   DECLARE drop : ideatek_drop_t
   DECLARE need : U32
   drop.window <- 0
   IF e.drop.windowID <> 0 THEN
      drop.window <- window_module.entity
   ENDIF
   drop.set <- window_module.set
   drop.index <- window_module.index
   drop.x <- e.drop.x
   drop.y <- e.drop.y
   drop.kind <- kind
   need <- LENGTH(e.drop.data) + LENGTH(e.drop.source)
   IF need <= window_module.drops.size - window_module.drops.used THEN
      drop.data <- put_bytes(window_module.drops, e.drop.data)
      drop.source <- put_bytes(window_module.drops, e.drop.source)
      CALL spawn_item(DROP, drop)
   ELSE
      CALL bridge_native_say(BRIDGE_LOG_WARN, "the drop <set>.<index> needs <count> bytes, and the half has <count> free bytes")
   ENDIF
   window_module.index <- window_module.index + 1
ENDPROCEDURE
```

The length of a null text is 0, and `put_bytes` gives a text with a `len` of 0 for it. Thus a drop with no source has a `source` of length 0.
The test of the free bytes comes before the 2 copies, so an item that does not fit takes no byte of the half.

An item that does not fit keeps its place in the drag. Thus `index` still tells the place of each item that gets an entity.
SDL3 gives a `windowID` of 0 for an item that comes with no drag, for example on macOS. Its `window` is 0: `docs/extern/platform_abstraction/ideatek_window.h.md`, *ideatek:Drop*.

## The branches

| Case | Result |
| --- | --- |
| A drag over the window. | `drop_over` is `true`, and `drop_x` and `drop_y` follow the point. |
| The end of a drag. | `drop_over` is `false`. |
| A drop of 3 files. | 3 entities with the same `set`, and the `index` 0, 1 and 2. |
| A drop of a text. | 1 entity of `IDEATEK_DROP_TEXT`. |
| A drop with a source, and a drop with none. | A `source` with its name, and a `source` of length 0. |
| An item larger than the free bytes of the half. | The line of the drop, and no entity. |
| More items than the room of a round. | The next round spawns the rest. |
| A drop with no window. | An entity with a `window` of 0. |

## Functions

| Function | Result |
| --- | --- |
| `take_drops` | Takes the events of the drops while the round has room. |
| `take_drop` | Does the action of the type of 1 event of a drop. |
| `spawn_drop` | Spawns the entity of a dropped item, or writes the line of an item that does not fit. |
