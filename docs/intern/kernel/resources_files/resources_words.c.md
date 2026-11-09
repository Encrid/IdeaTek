# src/kernel/resources_files/resources_words.c

This file reads the words of a line of a meta file. It is pure, and the reader of the meta files calls it.

## The words

`trim` takes the spaces and the tabs off the ends of a line, and a `\r` at its end.
`find_key` tells which key starts a line, `action` or `loader`, and takes the key off the line.
`read_quoted` reads a word between 2 quotes, which must end the line.

`read_word` reads `key = "word"`: the key, the `=` with spaces around it, and the quoted word. It gives the index of the key, or -1 for a line outside that form.
`read_action` turns a word into an action: `load`, `process` or `ignore`. Another word gives `false`.

## Functions

| Function | Result |
| --- | --- |
| `trim` | A line without the spaces at its ends. |
| `find_key` | The index of the key of a line, or -1. |
| `read_quoted` | The word between quotes. `false` when the quotes are wrong. |
| `read_word` | The key and the word of a line, or -1. |
| `read_action` | The action of a word. `false` for a word outside the 3. |
