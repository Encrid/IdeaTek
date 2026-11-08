# src/kernel/frame_sdl3/frame_settings.c

This file checks the 5 settings in the first round.

## The first round

`start_clock` reads `ideatek_frame_sdl3:Settings` and checks each setting:

| Setting | Range | Line |
| --- | --- | --- |
| `most_steps` | 1 to 100 | `most_steps: <value> is outside 1 to 100` |
| `fixed_round` | more than 0 | `fixed_round: <value> is outside more than 0` |
| `time_source` | `os` or `fixed` | `time_source: <text> is outside os or fixed` |
| `sleeper` | `clock` or `events` | `sleeper: <text> is outside clock or events` |
| `smoothing` | `none` or `refresh` | `smoothing: <text> is outside none or refresh` |

It checks all 5, so each wrong setting writes its line. A wrong setting marks the module as broken, and `round` gives 1 in each round: `readme.md`, *The contracts*.
A text of a setting shows at most `FRAME_TEXT` bytes in its line.

## The branches

| Case | Result |
| --- | --- |
| A count outside its range. | The line of the count, and a broken module. |
| A text that is not one of its 2 values. | The line of the text, and a broken module. |
| Each setting in its range. | The module starts. |

## Functions

| Function | Result |
| --- | --- |
| `text_is` | Gives `true` when the text of a setting is a word. |
| `pick_choice` | Gives 0 for the first value of a text, 1 for the second, and -1 with the line for another text. |
| `check_count` | Gives `true` for a count from 1 to its most, else writes its line. |
| `check_time` | Gives `true` for a time above 0, else writes its line. |
| `start_clock` | Reads the settings. Gives `false` when a setting is outside its range. |
