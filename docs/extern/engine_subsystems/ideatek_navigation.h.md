# include/engine_subsystems/ideatek_navigation.h

`include/engine_subsystems/ideatek_navigation.h` gives the types of the navigation: nav meshes, areas, links, path queries, agents and obstacles.
`readme.md`, *The contracts*, tells the rules of each contract.
The header includes the space part for the positions and the vectors of its fields.

## The roles

- The game writes the nav meshes, the areas, the marks, the links, the queries, the agents and the obstacles.
- The navigation module builds each nav mesh from the shapes of the physics. It answers the queries and steers the agents.
- A mover moves an agent. The navigation module never writes `ideatek:Transform`: it gives a velocity, as the root motion of the animation does.

## The nav meshes

A nav mesh is the walkable surface for 1 size of agent. A second size is a second entity.
The sources are the `ideatek:Shape` of each static body whose `categories & sources` is not 0.
When such a shape changes, the module builds the tiles that the shape touches again. So a destruction changes the nav mesh.
The defaults are the medians of Godot, Unreal and the demo of Recast.

## The areas and the filters

A polygon with no mark has an enter cost of 0, a travel cost of 1, and the groups 1.
A mark is a box of `size`, or the polygons over a shape. It builds the tiles that it touches again.
A query or an agent has `groups`. A polygon is open to it only if `area.groups & groups` is not 0 and the area is not BLOCKED.
The module logs each area that it cannot keep once.

## The links

A link joins 2 points that the nav mesh does not join, for example a jump, a ladder or a door. There is 1 link for each entity.
Its costs and its groups come from its area. A door turns ENABLED off and on.
A radius of 0 is the radius of the agent of the nav mesh.

## The queries

The game spawns a query entity. In a later round, the module writes `ideatek:NavPath` on it.
`bytes` of `ideatek:NavPath` holds `count` records of `ideatek_nav_point_t`, little-endian. The module owns the bytes. It smooths the path with the corridor funnel.

PROJECT gives the nearest point of the nav mesh to `start`. RAY walks straight from `start` to `end`, and gives the first hit or `end`.
To ask again, the game writes new values, and the module raises `revision`. The game destroys the entity when it is done.

## The agents

An agent has `ideatek:NavAgent`. The module writes `ideatek:NavSteer` and the `ideatek:NavPath` of the agent on the agent entity.
At a link, the module spawns `ideatek:NavLinkReached`. The mover plays the move of the link and puts the entity at its far end. Then the module goes on.

Avoidance is off by default. The method of the avoidance, its time horizons and its neighbors are choices of the module.

## The obstacles

An obstacle with `ideatek:NavObstacle` is a cylinder that the agents with AVOID steer around. It builds no tile again. The module takes its velocity from the change of its place.
To cut a hole in the nav mesh, an entity uses `ideatek:NavModifier` with a BLOCKED area instead.

## The settings

| Setting | Default | Range | Value |
| --- | --- | --- | --- |
| `edge_max_error` | 1.3 | Above 0 | The largest error of an edge of a polygon, in cells. |
| `vertices_per_polygon` | 6 | 3 to 6 | The most vertices of a polygon. |
| `detail_sample_distance` | 6 | 0 or more | The distance of the samples of the detail mesh, in cells. |
| `detail_sample_max_error` | 1 | 0 or more | The largest error of the detail mesh, in cells. |
| `region_merge_size` | 20 | 0 or more | The size of a region under which it joins a larger region, in cells. |

## ideatek:NavMesh

`ideatek:NavMesh` asks for a nav mesh around the place of its entity: *The nav meshes*.
`ideatek_nav_mesh_t` takes 48 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `sources` | `uint64_t` | 0 | All bits | The collision categories of the shapes that it reads. |
| `size` | `ideatek_vec3_t` | 8 | 0, 0, 0 | The half extents of the bounds. 0 means the whole world. |
| `agent_radius` | `float` | 20 | 0.5 | The radius of the agent. |
| `agent_height` | `float` | 24 | 1.5 | The height of the agent. |
| `max_climb` | `float` | 28 | 0.35 | The highest step. |
| `max_slope` | `float` | 32 | pi / 4 | The steepest slope. |
| `cell_size` | `float` | 36 | 0.25 | The width of a cell. |
| `cell_height` | `float` | 40 | 0.2 | The height of a cell. |
| `tile_size` | `float` | 44 | 0 | The width of a tile. 0 means 1 tile. |

## ideatek:ideatek_nav_build_t

`ideatek_nav_build_t` tells the state of the build of a nav mesh.
`ideatek_nav_build_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_NAV_BUILD_EMPTY` | No tile yet. |
| 1 | `IDEATEK_NAV_BUILD_BUILDING` | The module builds tiles. |
| 2 | `IDEATEK_NAV_BUILD_READY` | Each tile is built. |

## ideatek:NavMeshState

`ideatek:NavMeshState` is the state of a nav mesh. The navigation module writes it.
`ideatek_nav_mesh_state_t` takes 12 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `tiles` | `uint32_t` | 0 | The built tiles. |
| `revision` | `uint32_t` | 4 | Raised at each change. |
| `state` | `ideatek_nav_build_t` | 8 | The state. |

Bytes 9 to 11 hold no field.

## ideatek:ideatek_nav_area_flags_t

`ideatek_nav_area_flags_t` names the options of an area.
`ideatek_nav_area_flags_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_NAV_AREA_FLAGS_BLOCKED` | No agent goes through the area. |

## ideatek:NavArea

`ideatek:NavArea` is a kind of ground, for example mud or a road.
`ideatek_nav_area_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `groups` | `uint64_t` | 0 | 1 | The groups of the area. |
| `enter_cost` | `float` | 8 | 0 | The cost to enter. |
| `travel_cost` | `float` | 12 | 1 | The factor of the length inside. |
| `flags` | `ideatek_nav_area_flags_t` | 16 | 0 | The options. |

Bytes 17 to 23 hold no field.

The costs of Godot and Unreal agree on these defaults.

## ideatek:NavModifier

`ideatek:NavModifier` marks a part of the nav mesh with an area.
`ideatek_nav_modifier_t` takes 32 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `area` | `ideatek_entity_t` | 0 | The area. |
| `shape` | `ideatek_entity_t` | 8 | A shape. 0 means a box of `size`. |
| `size` | `ideatek_vec3_t` | 16 | The half extents of the box. |

Bytes 28 to 31 hold no field.

## ideatek:ideatek_nav_link_flags_t

`ideatek_nav_link_flags_t` names the options of a link.
`ideatek_nav_link_flags_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_NAV_LINK_FLAGS_BIDIRECTIONAL` | The link goes both ways. Without it, it goes from start to end. |
| 0x02 | `IDEATEK_NAV_LINK_FLAGS_ENABLED` | The link is open. |

## ideatek:NavLink

`ideatek:NavLink` joins 2 points for the paths: *The links*.
`ideatek_nav_link_t` takes 40 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `area` | `ideatek_entity_t` | 0 | 0 | The area of the costs. 0 means the default. |
| `start` | `ideatek_vec3_t` | 8 | No default | The start, in the space of the entity. |
| `end` | `ideatek_vec3_t` | 20 | No default | The end, in the space of the entity. |
| `radius` | `float` | 32 | 0 | The radius of the snap to the nav mesh. |
| `flags` | `ideatek_nav_link_flags_t` | 36 | BIDIRECTIONAL and ENABLED | The options. |

Bytes 37 to 39 hold no field.

## ideatek:ideatek_nav_query_kind_t

`ideatek_nav_query_kind_t` tells the kind of a query of the navigation.
`ideatek_nav_query_kind_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_NAV_QUERY_KIND_PATH` | The path from start to end. |
| 1 | `IDEATEK_NAV_QUERY_KIND_PROJECT` | The nearest point of the nav mesh. |
| 2 | `IDEATEK_NAV_QUERY_KIND_RAY` | A straight walk. |

## ideatek:ideatek_nav_query_flags_t

`ideatek_nav_query_flags_t` names the options of a query of the navigation.
`ideatek_nav_query_flags_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_NAV_QUERY_FLAGS_PARTIAL` | With no full path, the path goes to the nearest point. |

## ideatek:NavQuery

`ideatek:NavQuery` asks for a path or a point: *The queries*.
`ideatek_nav_query_t` takes 72 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `nav_mesh` | `ideatek_entity_t` | 0 | No default | The nav mesh. |
| `start` | `ideatek_position_t` | 8 | No default | The start. |
| `end` | `ideatek_position_t` | 32 | No default | The end. |
| `groups` | `uint64_t` | 56 | All bits | The filter of the areas. |
| `kind` | `ideatek_nav_query_kind_t` | 64 | No default | The kind. |
| `flags` | `ideatek_nav_query_flags_t` | 65 | PARTIAL | The options. |

Bytes 66 to 71 hold no field.

## ideatek:ideatek_nav_result_t

`ideatek_nav_result_t` tells the answer of a query of the navigation.
`ideatek_nav_result_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_NAV_RESULT_PENDING` | No answer yet. |
| 1 | `IDEATEK_NAV_RESULT_FOUND` | A full path. |
| 2 | `IDEATEK_NAV_RESULT_PARTIAL` | A path to the nearest point. |
| 3 | `IDEATEK_NAV_RESULT_NONE` | No path. |

## ideatek:ideatek_nav_point_t

`ideatek_nav_point_t` is 1 point of a path.
`ideatek_nav_point_t` takes 32 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `position` | `ideatek_position_t` | 0 | The point. |
| `link` | `ideatek_entity_t` | 24 | The link that starts at the point. 0 means none. |

## ideatek:NavPath

`ideatek:NavPath` is the answer of a query or the path of an agent. The navigation module writes it.
`ideatek_nav_path_t` takes 24 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `bytes` | `bridge_text_t` | 0 | The points: *The queries*. |
| `count` | `uint32_t` | 8 | The number of points. |
| `length` | `float` | 12 | The length of the path. |
| `revision` | `uint32_t` | 16 | Raised at each change. |
| `state` | `ideatek_nav_result_t` | 20 | The answer. |

Bytes 21 to 23 hold no field.

## ideatek:ideatek_nav_agent_flags_t

`ideatek_nav_agent_flags_t` names the options of an agent.
`ideatek_nav_agent_flags_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_NAV_AGENT_FLAGS_ACTIVE` | The agent goes to its target. |
| 0x02 | `IDEATEK_NAV_AGENT_FLAGS_AVOID` | The agent steers around the others. |

## ideatek:NavAgent

`ideatek:NavAgent` makes an entity go to a target: *The agents*.
`ideatek_nav_agent_t` takes 56 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `nav_mesh` | `ideatek_entity_t` | 0 | No default | The nav mesh. |
| `target` | `ideatek_position_t` | 8 | No default | The target. |
| `groups` | `uint64_t` | 32 | All bits | The filter of the areas. |
| `max_speed` | `float` | 40 | 0 | The highest speed, in m / s. |
| `radius` | `float` | 44 | 0 | The radius. 0 means the agent of the nav mesh. |
| `arrive_distance` | `float` | 48 | 0 | The distance of the arrival. 0 means the radius. |
| `flags` | `ideatek_nav_agent_flags_t` | 52 | ACTIVE | The options. |

Bytes 53 to 55 hold no field.

Godot and Unreal do not agree on a speed, so its default is 0 and the game sets it.

## ideatek:ideatek_nav_steer_state_t

`ideatek_nav_steer_state_t` tells the state of an agent.
`ideatek_nav_steer_state_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_NAV_STEER_STATE_IDLE` | The agent has nothing to do. |
| 1 | `IDEATEK_NAV_STEER_STATE_MOVING` | The agent goes. |
| 2 | `IDEATEK_NAV_STEER_STATE_ARRIVED` | The agent is at the target. |
| 3 | `IDEATEK_NAV_STEER_STATE_BLOCKED` | The module finds no path. |

## ideatek:NavSteer

`ideatek:NavSteer` tells a mover how to move an agent. The navigation module writes it.
`ideatek_nav_steer_t` takes 48 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `link` | `ideatek_entity_t` | 0 | The link that the agent is at. 0 means none. |
| `next` | `ideatek_position_t` | 8 | The next point of the path. |
| `velocity` | `ideatek_vec3_t` | 32 | The wanted velocity, after the avoidance. |
| `state` | `ideatek_nav_steer_state_t` | 44 | The state. |

Bytes 45 to 47 hold no field.

## ideatek:NavLinkReached

`ideatek:NavLinkReached` tells that an agent is at a link. It is an event of 1 round.
`ideatek_nav_link_reached_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `agent` | `ideatek_entity_t` | 0 | The agent. |
| `link` | `ideatek_entity_t` | 8 | The link. |
| `order` | `uint32_t` | 16 | The rank in the round. |

Bytes 20 to 23 hold no field.

## ideatek:NavObstacle

`ideatek:NavObstacle` makes a moving entity an obstacle for the avoidance.
`ideatek_nav_obstacle_t` takes 8 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `radius` | `float` | 0 | 0 | The radius of the cylinder. |
| `height` | `float` | 4 | 0 | The height of the cylinder. |

Only Godot gives a height, so both defaults are 0 and the game sets them.

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:NavMesh` | `ideatek_nav_mesh_t` | `ideatek_nav_mesh_t_desc` | `IDEATEK_NAV_MESH_FIELDS` |
| `ideatek:ideatek_nav_build_t` | `ideatek_nav_build_t` | `ideatek_nav_build_t_desc` | `IDEATEK_NAV_BUILD_VALUES` |
| `ideatek:NavMeshState` | `ideatek_nav_mesh_state_t` | `ideatek_nav_mesh_state_t_desc` | `IDEATEK_NAV_MESH_STATE_FIELDS` |
| `ideatek:ideatek_nav_area_flags_t` | `ideatek_nav_area_flags_t` | `ideatek_nav_area_flags_t_desc` | `IDEATEK_NAV_AREA_FLAGS_VALUES` |
| `ideatek:NavArea` | `ideatek_nav_area_t` | `ideatek_nav_area_t_desc` | `IDEATEK_NAV_AREA_FIELDS` |
| `ideatek:NavModifier` | `ideatek_nav_modifier_t` | `ideatek_nav_modifier_t_desc` | `IDEATEK_NAV_MODIFIER_FIELDS` |
| `ideatek:ideatek_nav_link_flags_t` | `ideatek_nav_link_flags_t` | `ideatek_nav_link_flags_t_desc` | `IDEATEK_NAV_LINK_FLAGS_VALUES` |
| `ideatek:NavLink` | `ideatek_nav_link_t` | `ideatek_nav_link_t_desc` | `IDEATEK_NAV_LINK_FIELDS` |
| `ideatek:ideatek_nav_query_kind_t` | `ideatek_nav_query_kind_t` | `ideatek_nav_query_kind_t_desc` | `IDEATEK_NAV_QUERY_KIND_VALUES` |
| `ideatek:ideatek_nav_query_flags_t` | `ideatek_nav_query_flags_t` | `ideatek_nav_query_flags_t_desc` | `IDEATEK_NAV_QUERY_FLAGS_VALUES` |
| `ideatek:NavQuery` | `ideatek_nav_query_t` | `ideatek_nav_query_t_desc` | `IDEATEK_NAV_QUERY_FIELDS` |
| `ideatek:ideatek_nav_result_t` | `ideatek_nav_result_t` | `ideatek_nav_result_t_desc` | `IDEATEK_NAV_RESULT_VALUES` |
| `ideatek:ideatek_nav_point_t` | `ideatek_nav_point_t` | `ideatek_nav_point_t_desc` | `IDEATEK_NAV_POINT_FIELDS` |
| `ideatek:NavPath` | `ideatek_nav_path_t` | `ideatek_nav_path_t_desc` | `IDEATEK_NAV_PATH_FIELDS` |
| `ideatek:ideatek_nav_agent_flags_t` | `ideatek_nav_agent_flags_t` | `ideatek_nav_agent_flags_t_desc` | `IDEATEK_NAV_AGENT_FLAGS_VALUES` |
| `ideatek:NavAgent` | `ideatek_nav_agent_t` | `ideatek_nav_agent_t_desc` | `IDEATEK_NAV_AGENT_FIELDS` |
| `ideatek:ideatek_nav_steer_state_t` | `ideatek_nav_steer_state_t` | `ideatek_nav_steer_state_t_desc` | `IDEATEK_NAV_STEER_STATE_VALUES` |
| `ideatek:NavSteer` | `ideatek_nav_steer_t` | `ideatek_nav_steer_t_desc` | `IDEATEK_NAV_STEER_FIELDS` |
| `ideatek:NavLinkReached` | `ideatek_nav_link_reached_t` | `ideatek_nav_link_reached_t_desc` | `IDEATEK_NAV_LINK_REACHED_FIELDS` |
| `ideatek:NavObstacle` | `ideatek_nav_obstacle_t` | `ideatek_nav_obstacle_t_desc` | `IDEATEK_NAV_OBSTACLE_FIELDS` |

```text
{ uint64_t sources; ideatek_vec3_t size; float agent_radius; float agent_height; float max_climb; float max_slope; float cell_size; float cell_height; float tile_size; }
{ IDEATEK_NAV_BUILD_EMPTY = 0, IDEATEK_NAV_BUILD_BUILDING = 1, IDEATEK_NAV_BUILD_READY = 2 }
{ uint32_t tiles; uint32_t revision; ideatek_nav_build_t state; }
{ IDEATEK_NAV_AREA_FLAGS_BLOCKED = 0x01 }
{ uint64_t groups; float enter_cost; float travel_cost; ideatek_nav_area_flags_t flags; }
{ ideatek_entity_t area; ideatek_entity_t shape; ideatek_vec3_t size; }
{ IDEATEK_NAV_LINK_FLAGS_BIDIRECTIONAL = 0x01, IDEATEK_NAV_LINK_FLAGS_ENABLED = 0x02 }
{ ideatek_entity_t area; ideatek_vec3_t start; ideatek_vec3_t end; float radius; ideatek_nav_link_flags_t flags; }
{ IDEATEK_NAV_QUERY_KIND_PATH = 0, IDEATEK_NAV_QUERY_KIND_PROJECT = 1, IDEATEK_NAV_QUERY_KIND_RAY = 2 }
{ IDEATEK_NAV_QUERY_FLAGS_PARTIAL = 0x01 }
{ ideatek_entity_t nav_mesh; ideatek_position_t start; ideatek_position_t end; uint64_t groups; ideatek_nav_query_kind_t kind; ideatek_nav_query_flags_t flags; }
{ IDEATEK_NAV_RESULT_PENDING = 0, IDEATEK_NAV_RESULT_FOUND = 1, IDEATEK_NAV_RESULT_PARTIAL = 2, IDEATEK_NAV_RESULT_NONE = 3 }
{ ideatek_position_t position; ideatek_entity_t link; }
{ bridge_text_t bytes; uint32_t count; float length; uint32_t revision; ideatek_nav_result_t state; }
{ IDEATEK_NAV_AGENT_FLAGS_ACTIVE = 0x01, IDEATEK_NAV_AGENT_FLAGS_AVOID = 0x02 }
{ ideatek_entity_t nav_mesh; ideatek_position_t target; uint64_t groups; float max_speed; float radius; float arrive_distance; ideatek_nav_agent_flags_t flags; }
{ IDEATEK_NAV_STEER_STATE_IDLE = 0, IDEATEK_NAV_STEER_STATE_MOVING = 1, IDEATEK_NAV_STEER_STATE_ARRIVED = 2, IDEATEK_NAV_STEER_STATE_BLOCKED = 3 }
{ ideatek_entity_t link; ideatek_position_t next; ideatek_vec3_t velocity; ideatek_nav_steer_state_t state; }
{ ideatek_entity_t agent; ideatek_entity_t link; uint32_t order; }
{ float radius; float height; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:ideatek_entity_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_vec3_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_position_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_nav_build_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_nav_area_flags_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_nav_link_flags_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_nav_query_kind_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_nav_query_flags_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_nav_result_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_nav_point_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_nav_agent_flags_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_nav_steer_state_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:NavMesh` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:NavMeshState` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:NavArea` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:NavModifier` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:NavLink` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:NavQuery` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:NavPath` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:NavAgent` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:NavSteer` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:NavLinkReached` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:NavObstacle` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

## The grants

| Role | `write` | `read` |
| --- | --- | --- |
| The game | `ideatek:NavMesh`, `ideatek:NavArea`, `ideatek:NavModifier`, `ideatek:NavLink`, `ideatek:NavQuery`, `ideatek:NavAgent`, `ideatek:NavObstacle` | The types of the fields |
| The navigation module | `ideatek:NavMeshState`, `ideatek:NavPath`, `ideatek:NavSteer`, `ideatek:NavLinkReached` | The types of the game, `ideatek:Shape`, `ideatek:Body`, `ideatek:WorldTransform`, and the types of the fields |
