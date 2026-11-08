# src/platform_abstraction/files_mosl/files_watch.c

This file starts, reads and stops the watches of the folders of the content storage.

## The watch

`start_watch` starts `os_watch_start` on the folder of the storage and the path of the request, down to `FILES_DEPTH` folders.
A watch that does not start, or a 17th watch, gives `IDEATEK_FILE_STATUS_FAILED`. A watch that starts gives `IDEATEK_FILE_STATUS_OK`, and the module keeps it with its entity.

In each round, `poll_watch` takes the changes of each watch with `os_watch_take`. The thread of the phases is the one thread that uses a watch:

| Result of `os_watch_take` | What the module does |
| --- | --- |
| 1 | The name goes to the lines of the change, with the path of the watch before it. |
| 0 | The take ends. |
| 2 | The path of the watch goes to the lines, and the log gets `the watch of <path> lost changes`. |
| -1 | The watch stops, the log gets `the watch of <path> stopped`, and the entity gets `ideatek:FileResult` with `IDEATEK_FILE_STATUS_NOT_FOUND`. |

The lines of a round go into the half with `put_bytes`, and `ideatek:FileChange` goes to the entity. The round before detached the change of its round.
Lines over the room left in the half wait for the next round.

## The branches

| Case | Result |
| --- | --- |
| A change in a watched folder. | A line of the change in the round. |
| The watch lost changes. | The path of the watch as the change, and its line. |
| The watched folder goes. | The line of the stop, and the status not found. |
| No change. | No `ideatek:FileChange` on the entity. |

## Functions

| Function | Result |
| --- | --- |
| `start_watch` | Starts the watch of a job, and gives its status. |
| `stop_watch` | Stops the watch of an entity, and forgets it. |
| `poll_watch` | Takes the changes of 1 watch into the half, and attaches or detaches its change. |
| `poll_watches` | Polls each watch of the module. |
