# src/kernel/frame_sdl3/frame_request.c

This file applies `ideatek:TimeRequest` at the start of each round.

## The request

`apply_request` reads the singleton. A value that is the last value that it read changes nothing.
Else it copies each field that the module does not refuse into `rule`, or `max_rate` into its own field:

| Field | Refused when | Line |
| --- | --- | --- |
| `scale` | It is not finite, or it is below 0. | `scale <value> is refused` |
| `rate` | It is 0, or more than 1000. | `rate <value> is refused` |
| `max_rate` | It is more than 1000. | `max_rate <value> is refused` |
| `paused` | It is not 0 or 1. | `paused <value> is refused` |

A refused field keeps the last value. Each refused field writes its line 1 time, when the request changes.

## The branches

| Case | Result |
| --- | --- |
| The same request as in the last round. | No change. |
| A field in its range. | The field goes into `rule`. |
| A field outside its range. | The line of the field, and the last value stays. |

## Functions

| Function | Result |
| --- | --- |
| `refuse` | Writes the line of a refused field. |
| `copy_fields` | Copies each field that passed into `rule`, or into `max_rate`. |
| `refuse_fields` | Writes the line of each field that did not pass. |
| `apply_request` | Checks the 4 fields of a new request, and copies the ones that pass. |
