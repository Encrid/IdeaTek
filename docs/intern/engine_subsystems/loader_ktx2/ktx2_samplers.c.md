# src/engine_subsystems/loader_ktx2/ktx2_samplers.c

This file attaches the sampler of a load, and writes the faults of its settings.

## The faults

Each fault writes 1 line with its key: `docs/usage/engine_subsystems/loader_ktx2.md`, *The messages*.
A line has at most 127 bytes, and `snprintf` cuts a longer one.

## Functions

| Function | Result |
| --- | --- |
| `say_fault` | Writes the line of 1 fault. |
| `give_sampler` | Reads the settings of the bytes, writes the faults, and attaches `ideatek:Sampler`. |
