# BM-IdeaTek-Space-Bridge

The space module of IdeaTek on the observers of Bridge: the places, the lists of children and the places before a step.

## Install

The release of `BM-IdeaTek-Space-Bridge` has the tag `space_bridge-v<version>`: https://github.com/Encrid/IdeaTek/releases.
Its 2 files are `BM-IdeaTek-Space-Bridge.zip` and `sha256.txt`. `BM-IdeaTek-Space-Bridge.zip` has a folder for each target.
In `BM-IdeaTek-Space-Bridge`, each folder holds the module `space_bridge.dll`, `space_bridge.so` or `space_bridge.dylib`.
Each folder of `BM-IdeaTek-Space-Bridge.zip` also holds this readme and the licence.

The module needs only Bridge, so no library goes beside it in a deployment.
A project names the zip of `BM-IdeaTek-Space-Bridge` in a `[[source]]`: `.toolchain/docs/workflows.py.md`, *The release*.

## Usage

The identity of the module is `ideatek_space_bridge:space@0.1.0`.
Its role is the space module: `docs/extern/engine_subsystems/ideatek_space.h.md`, *The roles*.
The contract of the space part names no setting, and the module has none. Its manifest states `config` at `none`.

### The observers and the system

The module keeps nothing of its own between 2 calls: the components of the space part hold its state.
Its 5 observers run at the barriers, in this order:

1. `link` hears `ideatek:Parent` enter, change and leave. It moves the child from the list of the old parent to the list of the new one.
   `parent` of `ideatek:Sibling` names the old parent, and `order` gives the place in the new list.
2. `place` hears `ideatek:Transform` enter and change. It places each entity of its list that has no ancestor in the list, then walks the children: `docs/extern/engine_subsystems/ideatek_space.h.md`, *ideatek:WorldTransform*.
3. `relink` hears `ideatek:Sibling` enter, change and leave, and places the entity as `place` does. So a child takes its place under its new parent.
4. `previous` hears `ideatek:Interpolated` enter and leave. It attaches `ideatek:PreviousWorldTransform` at the place of now, and detaches it.
5. `gone` hears `ideatek:Transform` leave. It detaches `ideatek:WorldTransform`. On the husk of a destroyed parent, it walks the children and applies `orphan`, then detaches `ideatek:Children`.

`place` also applies `ideatek:Teleported`, as `docs/extern/engine_subsystems/ideatek_space.h.md`, *ideatek:Teleported*, tells. Then it detaches the tag.

The system `step` runs in the phase `fixed` of the module, before the movers of the fixed phase. When `tick` of `ideatek:Tick` changed, it writes `ideatek:WorldTransform` in place into `ideatek:PreviousWorldTransform` for each entity with `ideatek:Interpolated`.

A change of a parent takes 2 barriers: `link` writes the lists at the first, and `place` reads them at the second.
So a moved entity gets its `ideatek:WorldTransform` within 2 barriers. A child with a new parent gets it within 3.

The table ids are `tables` + `sparse_sets` + 1 of the budget of the world. In a world of 128 tables, the default `spans` holds about 3500 entities that move in 1 round.
A deployment with more raises `spans` by 16 bytes for each entity.

### The deployment

| Key | Default of the manifest | Rule |
| --- | --- | --- |
| `pinned` | `false` | The module asks no thread of its own. |
| `reloadable` | `false` | A reload of the module has no case yet. |
| `spawns` | `"none"` | The module makes no entity. It only destroys orphans. |
| `changes` | 262144 | The first attach of `ideatek:WorldTransform` of each new entity, and the links of a round. A moved entity takes no changes. |
| `persistent` | `"none"` | The module keeps nothing between the rounds. |
| `scratch` | 1048576 | The set of the entities of 1 call of `place`, 16 bytes for each, and the stack of the walk. |
| `log_ring` | 512 | 16 log lines fit, with 32 bytes for each line. |
| `spans` | 65536 | The cells of its 16 types and the spans of its query: 68 bytes for each table id, and 376 bytes. Also 16 bytes for each entity in the lists of its observers at 1 barrier. |

- The deployment sets `observer_passes` to 3 or more, as the document of `bridge.h` of Bridge tells.
- `fixed` of the module is the fixed phase of the deployment.
- A pair puts `ideatek_space_bridge:space:step` after `ideatek_frame_sdl3:clock:step`, and before each mover of the fixed phase.
- The module takes the grants of the space module: `docs/extern/engine_subsystems/ideatek_space.h.md`, *The grants*. It also reads `ideatek:Tick`.
- When a call of `place` has more entities than `scratch` holds, it skips the test of the ancestors for the rest.
  Then an entity can be placed 2 times.

### The messages

The module writes these lines:

| Line | Cause |
| --- | --- |
| `a transform of <entity> is refused` | The space contract refuses that `ideatek:Transform`: `docs/extern/engine_subsystems/ideatek_space.h.md`, *ideatek:Transform*. The entity gets no `ideatek:WorldTransform`. |
| `<entity> is in a cycle of parents` | The entity is its own ancestor. It gets no `ideatek:WorldTransform`. |
| `<count> entities placed without the test of the ancestors` | A call of `place` had more entities than `scratch` holds. |
| `the walk of <entity> stops: no room for its stack` | The tree under the entity is deeper or wider than `scratch` holds. The rest waits for its next change. |
| `the list of <entity> stays: no room for its children` | The children of 1 parent are more than `scratch` holds. Their list keeps its old links. |
| `<entity> keeps its transform: its new parent has no inverse` | The basis of the new parent has a determinant of 0, so no transform keeps the place in the world. |
