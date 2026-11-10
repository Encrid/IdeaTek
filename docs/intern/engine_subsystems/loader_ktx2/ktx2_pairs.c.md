# src/engine_subsystems/loader_ktx2/ktx2_pairs.c

This file splits the settings of a load into lines of `key = value`.

## The lines

A line ends at a line feed, or at the end of the text. The spaces, the tabs and a carriage return at the 2 ends of a line go.
An empty line, and a line that starts with `#`, give no pair.
The first `=` splits the line. A line with no `=` gives a key with an empty value.

## Functions

| Function | Result |
| --- | --- |
| `trim` | The text with no space at its 2 ends. |
| `take_line` | The next line, and moves the rest after its line feed. |
| `split` | The key and the value of a line. |
| `ktx2_next_pair` | The next pair of the text. `false` at the end. |
