# src/platform_abstraction/files_mosl/files_watch.c

This file starts, reads and stops the watches of the folders of the content storage.

## The watch

`start_watch` starts `os_watch_start` at the path of the request, below the content storage, down to `FILES_DEPTH` folders.
A watch that does not start, a 17th watch, or a path that does not fit with its storage gives `IDEATEK_FILE_STATUS_FAILED`. A watch that starts gives `IDEATEK_FILE_STATUS_OK`, and the module keeps it with its entity.
`stop_watch` stops the watch of an entity and forgets it.

In each round, `poll_watch` takes the changes of each watch with `take_lines`. The thread of the phases is the one thread that uses a watch:

| Result of `os_watch_take` | What the module does |
| --- | --- |
| 1 | The name goes to the lines of the change, with the path of the watch before it. |
| 0 | The take ends. |
| 2 | The path of the watch goes to the lines, so that the game reads the whole folder again. No line, because no case can make the system lose changes, and the choice of the text is arithmetic. |
| -1 | The watch stops, the log gets `the watch of <path> stopped`, and the entity gets `ideatek:FileResult` with `IDEATEK_FILE_STATUS_NOT_FOUND`. |

`take_lines` takes a change only when the half has `FILES_PATH` free bytes, so a line always fits. The lines over that room wait for the next round.
The lines of a round go into the half with `put_bytes`, and `ideatek:FileChange` goes to the entity. The change of the round before is detached first.

## The branches

| Case | Result |
| --- | --- |
| A change in a watched folder. | A line of the change in the round. |
| The watch lost changes. | The path of the watch as the change. |
| The watched folder goes. | The line of the stop, and the status not found. |
| No change. | No `ideatek:FileChange` on the entity. |
| The half has fewer than `FILES_PATH` free bytes. | The changes wait for the next round. |

## Functions

| Function | Result |
| --- | --- |
| `start_watch` | Starts the watch of a request, and gives its status. |
| `stop_watch` | Stops the watch of an entity, and forgets it. |
| `take_lines` | Takes the changes of 1 watch into the half while a line fits, and gives the last result of `os_watch_take`. |
| `poll_watch` | Attaches or detaches the change of 1 watch, and ends a watch that stopped. Gives `false` for a watch that stopped. |
| `poll_watches` | Polls each watch of the module, and forgets the ones that stopped. |
