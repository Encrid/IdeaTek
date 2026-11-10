# src/engine_subsystems/loader_gltf/gltf_joints.c

This file gives the ranks of the joints, the names of the nodes, and the target id of a node.

The ranks follow `docs/usage/engine_subsystems/loader_gltf.md`, *The models*. The names of the nodes follow the same section.
The target id hashes the path of names from the root, 1 name at a time.

## Functions

| Function | Result |
| --- | --- |
| `node_index` | The index of a node. |
| `rank_joints` | Gives each joint its rank, and counts the joints. |
| `name_of_node` | Writes the name of a node, and gives its length. |
| `hash_path` | Hashes the path of a node from its root. |
| `target_of` | The target id of a node. |
