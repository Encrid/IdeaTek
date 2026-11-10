# src/engine_subsystems/loader_gltf/gltf_out.c

This file holds the writer of the 2 passes, and writes all the parts of a file.

With no base, the writer only counts. The order of the parts is: the meshes, the skins, the clips, the models of the scenes, the model of the file, then the labels.
The model of the file is the default scene, else the first scene, else a model with no node.

## Functions

| Function | Result |
| --- | --- |
| `out_put` | Copies bytes when the writer has a base, and gives their offset. |
| `out_take` | Room for bytes: a pointer, or nothing in the first pass. |
| `out_align` | Moves the writer to a multiple of an alignment. |
| `write_labels` | Writes the text of each label. |
| `write_all` | Writes all the parts of a file. |
