# src/engine_subsystems/loader_ktx2/ktx2_sampler.c

This file reads the sampler from the settings of a load: `docs/usage/engine_subsystems/loader_ktx2.md`, *The sampler*.

## The keys

`KEYS` holds the 7 keys, the list of words of each key, and the offset of its field in `ideatek_sampler_t`.
A filter takes a word of `nearest` and `linear`, and a wrap takes a word of `repeat`, `mirrored_repeat` and `clamp_to_edge`. The word is in quotes.
`max_anisotropy` takes a number from 1 to 16. `strtof` reads it from a copy of at most 31 characters, and the whole value must be the number.

## The faults

An unknown key gives `KTX2_UNKNOWN_KEY`, and a value outside its range gives `KTX2_BAD_VALUE`. The field then keeps its default.
`ktx2_read_sampler` keeps the first `KTX2_MAX_FAULTS` faults, with the offset of their key from the start of the text.

## Functions

| Function | Result |
| --- | --- |
| `read_word` | The index of the quoted word in its list. `false` when the word is not there. |
| `read_number` | The number of the value. `false` outside 1 to 16. |
| `apply_pair` | Writes 1 pair into the sampler, or gives the kind of its fault. |
| `ktx2_read_sampler` | The sampler with the defaults of the contract, the pairs of the text, and the faults. |
