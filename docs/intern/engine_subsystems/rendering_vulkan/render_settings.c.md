# src/engine_subsystems/rendering_vulkan/render_settings.c

This file checks the settings at the first round of `draw`, and keeps them in `render_module`.

## The checks

The table of the numbers holds each setting that is a number, as `render_number_t`. Each number is a `double`.
`check_numbers` walks the table, so 1 loop checks each number with 1 test. A value outside writes `<setting>: <value> is outside <range>`.
The range of `lod_threshold` excludes 0, and `capture_bytes` must be even, as their rows tell.

The table of the words holds each setting that is a choice of words, as `render_words_t`.
`check_words` finds the word of each setting with `find_word`, and keeps its index in `choices`. A text that is no word writes `<setting>: <text> is outside <word> or <word>`.

`shader_folder` gets no check here. The files part checks the path of each request, and a refused request keeps the shader of the module with its line.

Each check runs, so the log gets 1 line for each wrong setting. 1 wrong setting is enough to stop the module: from then on, each round of `draw` gives 1.
A good module takes `capture_bytes` of the persistent arena at its start, as 2 halves. With 0, it takes nothing.

## Functions

| Function | Result |
| --- | --- |
| `check_numbers` | Checks each number against its range. `false` with 1 line for each wrong number. |
| `find_word` | The index of a text among the 2 words of a setting, or 2. |
| `check_words` | Turns each choice of words into its index. `false` with 1 line for each wrong text. |
| `start_render` | Runs the 2 checks, keeps the settings, and takes the halves. `false` when the module is broken. |
