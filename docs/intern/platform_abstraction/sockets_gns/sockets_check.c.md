# src/platform_abstraction/sockets_gns/sockets_check.c

This file checks the settings in the first round: the numbers against their ranges, the texts against the arena, and the words against their tables.

## The numbers

`check_numbers` walks `SOCKETS_NUMBERS`: `docs/intern/platform_abstraction/sockets_gns/sockets_config.c.md`, *The table*. A value outside its range writes `<setting>: <value> is outside <range>`.
2 rules are outside the table. `message_bytes` is an even number. `recv_buffer` is more than `recv_max_message`.
The check goes through every setting, so the log names each wrong one.

## The texts

`TEXTS` is a constant table of the 9 text settings: the offset of each field in `sockets_settings_t`, and its name. `TEXT_*` of the header gives the index of each one.
`copy_text` copies 1 text from the arena of the share into its text of `SOCKETS_TEXT` bytes, with a zero at the end. A text outside the arena, or of `SOCKETS_TEXT` bytes or more, writes `<setting>: a text of <count> bytes is outside 0 to 1023`, and gives `false`.
`copy_texts` copies the 9 texts and `debug_output`, which has its own text `debug_level`. A wrong `debug_output` leaves an empty text.

## The words

`check_word` finds the text of a word setting in its table: `docs/intern/platform_abstraction/sockets_gns/sockets_values.c.md`. A text outside the table writes `<setting>: <text> is outside <words>`, with the words of the table.
`check_words` checks `allow_without_auth`, `allow_localhost_without_auth`, `unencrypted`, `ice_implementation` and `debug_output`. Then `ice_mask` reads `ice_transports`: a wrong word writes `ice_transports: <text> is outside relay, private and public`.

## The branches

| Case | Result |
| --- | --- |
| A number outside its range. | Its line, and `false`. |
| An odd `message_bytes`, or a `recv_buffer` not above `recv_max_message`. | Its line, and `false`. |
| A text outside the arena, or too long. | Its line, and `false`. |
| A word outside its table. | Its line, and `false`. |
| A wrong word of `ice_transports`. | Its line, and `false`. |

## Functions

| Function | Result |
| --- | --- |
| `check_numbers` | Checks each number setting against its range. Gives `false` with the line of each wrong one. |
| `copy_text` | Copies 1 text setting from the arena. Gives `false` with its line when the text does not fit. |
| `copy_texts` | Copies the 9 texts and `debug_output`. Gives `false` when one does not fit. |
| `check_word` | Checks 1 word setting against its table. A wrong word gives `false` and its line. |
| `check_words` | Checks the 5 word settings and `ice_transports`. |
