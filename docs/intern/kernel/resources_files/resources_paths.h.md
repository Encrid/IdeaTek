# src/kernel/resources_files/resources_paths.h

This header gives the parts of a path of a resource, and the prototypes that split and join them.

## The types

`res_path_t` is a path of the contract, `source://path#label`, split into its 3 parts. Each part is a pointer and a length: the source, the path and the label. A part that the text does not hold has a length of 0.
`RES_NAME` is 64: the most bytes of the name of a source or of a variable.
