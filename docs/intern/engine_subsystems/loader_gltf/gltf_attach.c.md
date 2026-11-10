# src/engine_subsystems/loader_gltf/gltf_attach.c

This file attaches the label and the component of each part, and the model of the file.

Each attach raises the revision of its part. The model of the file takes the revision of the file.

## Functions

| Function | Result |
| --- | --- |
| `give_label` | Attaches `ideatek:ResourceLabel`, and raises the revision of the part. |
| `give_mesh` | Attaches `ideatek:Mesh` from the plan of the mesh. |
| `give_motion` | Attaches `ideatek:SkinBinds` or `ideatek:Clip`. |
| `give_model` | Attaches `ideatek:Model` with the text of its bytes. |
| `attach_all` | Attaches each part, then the model of the file. |
