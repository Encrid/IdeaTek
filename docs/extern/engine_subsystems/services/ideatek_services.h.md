# include/engine_subsystems/services/ideatek_services.h

`include/engine_subsystems/services/ideatek_services.h` gives the types of the services of a platform, from its users and tickets to the lobbies, the voice and the content.
`readme.md`, *The contracts*, tells the rules of each contract.
The world part gives the type of the entities of its fields.

## The roles

- The game writes the requests and the values that it publishes.
- The platform module is a native module for 1 platform: Steam, EOS, or a null module. It writes the users, the states, the results and the events.
- No type of the part names a call of a platform. A module for another platform serves the same types.
- The null module gives 1 user, signed in, with an account of 0. It refuses each ticket and keeps its states in memory only.

## The requests

A request is an entity that the game spawns. The platform module writes its result on the same entity, and the game destroys it after it reads the result.
A result with a state of PENDING is not done yet. A list of results is 1 entity for each element: a score, a friend, a member, a lobby, an item.
The module destroys the entities of a list 1 round after the game destroys the request.

## The users and the tickets

The module writes 1 entity for each local user. On a console, the OS pairs each gamepad with a user, and the module writes `ideatek:DeviceUser` on the gamepad entity of the input part.
The game uses it to give the gamepad to a player of the map of the inputs.
A client puts the bytes of its ticket into the join of the networking part. The server checks them with `ideatek:TicketCheck`, then admits the peer.
`owner` differs from `account` for a game that a friend shares.

## The achievements, the stats and the scores

The platform defines the achievements, the stats and the boards. The module mirrors each of them as an entity after the sign-in.
A progress of 1 unlocks an achievement. The module sends the stores in batches.
A board keeps the best score with KEEP_BEST, or the new score with FORCE. Its sort order is set on the platform.

## The cloud

The cloud is a backend of `IDEATEK_STORAGE_USER` of the files part: a files module for each platform writes the files there. So the saves do not change.
A write that does not fit gets the status of no space of the files part.

## The friends and the lobbies

The game publishes 1 `ideatek:Presence` with a join string. When the user accepts an invite or joins from the UI of the platform, the module spawns `ideatek:JoinRequested`. It also spawns it in the first round of a game that started to join.
A lobby holds members and values. Only the owner changes a value, with `ideatek:LobbySet`, so the module stays the only writer of `ideatek:LobbyValue`.

A server of the game puts its address in a value. The relay of a platform is a backend of the sockets part of layer 2.

## The voice

The module plays each member of a channel through a stream player of the sound part, on the bus of the channel. With a speaker entity, the voice plays at that entity, for a voice near an avatar.
`TRANSMIT` is the push to talk of the game. The detection of the voice and the codec are choices of the module.
A service module of voice uses its own transport. A module for Steam sends the frames as an EVENT of the networking part.

## The content

An entitlement and an item of the users give a `path`: their folder when they are installed. The game adds it as a source of the resources part.
Code in a mod is a module of Bridge, not an item of this part.
`ideatek:PlatformOverlay` tells each part that the overlay of the platform is open, so the game can pause.

## ideatek:ideatek_service_state_t

`ideatek_service_state_t` tells the state of a request to the platform.
`ideatek_service_state_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_SERVICE_STATE_PENDING` | Not done yet. |
| 1 | `IDEATEK_SERVICE_STATE_DONE` | Done. |
| 2 | `IDEATEK_SERVICE_STATE_FAILED` | The platform refused or failed. |

## ideatek:ideatek_platform_t

`ideatek_platform_t` names a platform.
`ideatek_platform_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_PLATFORM_NONE` | No platform: the null module. |
| 1 | `IDEATEK_PLATFORM_STEAM` | Steam. |
| 2 | `IDEATEK_PLATFORM_EOS` | Epic Online Services. |
| 255 | `IDEATEK_PLATFORM_OTHER` | Another platform. |

## ideatek:ideatek_sign_status_t

`ideatek_sign_status_t` tells the sign-in of a user.
`ideatek_sign_status_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_SIGN_STATUS_SIGNED_OUT` | The user is not signed in. |
| 1 | `IDEATEK_SIGN_STATUS_SIGNED_IN` | The user is signed in and online. |
| 2 | `IDEATEK_SIGN_STATUS_REDUCED` | The user is signed in but offline. |

## ideatek:PlatformUser

`ideatek:PlatformUser` is a local user of the platform. The module writes 1 entity for each user.
`ideatek_platform_user_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `account` | `uint64_t` | 0 | The id of the account on the platform. |
| `name` | `bridge_text_t` | 8 | The name that the platform shows. |
| `platform` | `ideatek_platform_t` | 16 | The platform. |
| `status` | `ideatek_sign_status_t` | 17 | The sign-in. |

Bytes 18 to 23 hold no field.

## ideatek:DeviceUser

`ideatek:DeviceUser` names the user that the OS pairs with a gamepad, on the gamepad entity.
`ideatek_device_user_t` takes 8 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `user` | `ideatek_entity_t` | 0 | The user. |

## ideatek:ideatek_sign_in_flags_t

`ideatek_sign_in_flags_t` names the options of a sign-in.
`ideatek_sign_in_flags_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_SIGN_IN_FLAGS_SHOW_UI` | The platform shows its own UI of the sign-in. |

## ideatek:SignInRequest

`ideatek:SignInRequest` asks for a user, for example after "press start" on a console.
`ideatek_sign_in_request_t` takes 1 bytes. Its alignment is 1.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `flags` | `ideatek_sign_in_flags_t` | 0 | The options. |

## ideatek:ideatek_ticket_purpose_t

`ideatek_ticket_purpose_t` tells who checks a ticket.
`ideatek_ticket_purpose_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_TICKET_PURPOSE_SERVER` | For a server of the game. |
| 1 | `IDEATEK_TICKET_PURPOSE_WEB` | For a web service. |

## ideatek:TicketRequest

`ideatek:TicketRequest` asks for a ticket of a user.
`ideatek_ticket_request_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `user` | `ideatek_entity_t` | 0 | The user. |
| `identity` | `bridge_text_t` | 8 | The name of the web service, for WEB. |
| `purpose` | `ideatek_ticket_purpose_t` | 16 | Who checks it. |

Bytes 17 to 23 hold no field.

## ideatek:Ticket

`ideatek:Ticket` is the answer to a ticket request.
`ideatek_ticket_t` takes 12 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `data` | `bridge_text_t` | 0 | The bytes of the ticket. |
| `state` | `ideatek_service_state_t` | 8 | The state. |

Bytes 9 to 11 hold no field.

## ideatek:TicketCheck

`ideatek:TicketCheck` asks the platform to check a ticket, on the server.
`ideatek_ticket_check_t` takes 16 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `account` | `uint64_t` | 0 | The account that the peer claims. |
| `data` | `bridge_text_t` | 8 | The bytes of the ticket. |

## ideatek:TicketResult

`ideatek:TicketResult` is the answer to a check.
`ideatek_ticket_result_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `account` | `uint64_t` | 0 | The account of the ticket. |
| `owner` | `uint64_t` | 8 | The account that owns the game. |
| `state` | `ideatek_service_state_t` | 16 | The state. |

Bytes 17 to 23 hold no field.

## ideatek:AchievementState

`ideatek:AchievementState` is 1 achievement of a user. The module writes 1 entity for each.
`ideatek_achievement_state_t` takes 32 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `user` | `ideatek_entity_t` | 0 | The user. |
| `time` | `int64_t` | 8 | The time of the unlock, in Unix seconds. |
| `name` | `bridge_text_t` | 16 | The name of the achievement on the platform. |
| `progress` | `float` | 24 | The progress, from 0 to 1. |
| `unlocked` | `bool` | 28 | `true` if the user has it. |

Bytes 29 to 31 hold no field.

## ideatek:AchievementProgress

`ideatek:AchievementProgress` reports a progress.
`ideatek_achievement_progress_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `user` | `ideatek_entity_t` | 0 | The user. |
| `name` | `bridge_text_t` | 8 | The name of the achievement. |
| `progress` | `float` | 16 | The new progress. 1 unlocks it. |

Bytes 20 to 23 hold no field.

## ideatek:StatState

`ideatek:StatState` is 1 stat of a user. The module writes 1 entity for each.
`ideatek_stat_state_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `user` | `ideatek_entity_t` | 0 | The user. |
| `value` | `double` | 8 | The value. |
| `name` | `bridge_text_t` | 16 | The name of the stat. |

## ideatek:ideatek_stat_mode_t

`ideatek_stat_mode_t` tells how an update changes a stat.
`ideatek_stat_mode_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_STAT_MODE_SET` | The value replaces the stat. |
| 1 | `IDEATEK_STAT_MODE_ADD` | The value adds to the stat. |

## ideatek:StatUpdate

`ideatek:StatUpdate` changes a stat.
`ideatek_stat_update_t` takes 32 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `user` | `ideatek_entity_t` | 0 | The user. |
| `value` | `double` | 8 | The value. |
| `name` | `bridge_text_t` | 16 | The name of the stat. |
| `mode` | `ideatek_stat_mode_t` | 24 | The change. |

Bytes 25 to 31 hold no field.

## ideatek:ideatek_score_method_t

`ideatek_score_method_t` tells how a board takes a score.
`ideatek_score_method_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_SCORE_METHOD_KEEP_BEST` | The board keeps the best score of the user. |
| 1 | `IDEATEK_SCORE_METHOD_FORCE` | The board keeps the new score. |

## ideatek:ScoreUpload

`ideatek:ScoreUpload` sends a score.
`ideatek_score_upload_t` takes 32 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `user` | `ideatek_entity_t` | 0 | The user. |
| `score` | `int64_t` | 8 | The score. |
| `board` | `bridge_text_t` | 16 | The name of the board. |
| `method` | `ideatek_score_method_t` | 24 | How the board takes it. |

Bytes 25 to 31 hold no field.

## ideatek:ScoreResult

`ideatek:ScoreResult` is the answer to an upload or to a query of scores.
`ideatek_score_result_t` takes 8 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `rank` | `int32_t` | 0 | The rank of the user after the upload. |
| `state` | `ideatek_service_state_t` | 4 | The state. |

Bytes 5 to 7 hold no field.

## ideatek:ideatek_score_range_t

`ideatek_score_range_t` tells which scores a query gives.
`ideatek_score_range_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_SCORE_RANGE_GLOBAL` | The ranks from `start`. |
| 1 | `IDEATEK_SCORE_RANGE_AROUND_USER` | The ranks around the user, from `start` before it. |
| 2 | `IDEATEK_SCORE_RANGE_FRIENDS` | The friends of the user. |

## ideatek:ScoreQuery

`ideatek:ScoreQuery` asks for scores of a board.
`ideatek_score_query_t` takes 32 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `user` | `ideatek_entity_t` | 0 | The user. |
| `board` | `bridge_text_t` | 8 | The name of the board. |
| `start` | `int32_t` | 16 | The first rank, or the offset from the user. |
| `count` | `uint32_t` | 20 | The most entries. |
| `range` | `ideatek_score_range_t` | 24 | The scores. |

Bytes 25 to 31 hold no field.

## ideatek:ScoreEntry

`ideatek:ScoreEntry` is 1 score of a query.
`ideatek_score_entry_t` takes 40 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `query` | `ideatek_entity_t` | 0 | The query. |
| `account` | `uint64_t` | 8 | The account. |
| `score` | `int64_t` | 16 | The score. |
| `name` | `bridge_text_t` | 24 | The name of the user. |
| `rank` | `int32_t` | 32 | The rank. |

Bytes 36 to 39 hold no field.

## ideatek:CloudState

`ideatek:CloudState` is the state of the cloud of a user.
`ideatek_cloud_state_t` takes 32 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `user` | `ideatek_entity_t` | 0 | The user. |
| `quota_bytes` | `uint64_t` | 8 | The size of the cloud of the user. |
| `free_bytes` | `uint64_t` | 16 | The free bytes. |
| `enabled` | `bool` | 24 | `true` if the cloud is on. |

Bytes 25 to 31 hold no field.

## ideatek:ideatek_friend_status_t

`ideatek_friend_status_t` tells the status of a friend.
`ideatek_friend_status_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_FRIEND_STATUS_OFFLINE` | Not online. |
| 1 | `IDEATEK_FRIEND_STATUS_ONLINE` | Online. |
| 2 | `IDEATEK_FRIEND_STATUS_AWAY` | Away. |
| 3 | `IDEATEK_FRIEND_STATUS_BUSY` | Busy. |

## ideatek:Friend

`ideatek:Friend` is 1 friend of a user. The module writes 1 entity for each.
`ideatek_friend_t` takes 40 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `user` | `ideatek_entity_t` | 0 | The user. |
| `account` | `uint64_t` | 8 | The account of the friend. |
| `name` | `bridge_text_t` | 16 | The name. |
| `presence` | `bridge_text_t` | 24 | The text of the presence of the friend. |
| `status` | `ideatek_friend_status_t` | 32 | The status. |
| `in_game` | `bool` | 33 | `true` if the friend plays this game. |

Bytes 34 to 39 hold no field.

## ideatek:ideatek_joinable_t

`ideatek_joinable_t` tells who can join a user.
`ideatek_joinable_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_JOINABLE_PUBLIC` | Each user can join. |
| 1 | `IDEATEK_JOINABLE_FRIENDS` | The friends can join. |
| 2 | `IDEATEK_JOINABLE_INVITE` | Only an invite joins. |
| 3 | `IDEATEK_JOINABLE_PRIVATE` | Nobody joins. |

## ideatek:Presence

`ideatek:Presence` publishes what a user does.
`ideatek_presence_t` takes 32 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `user` | `ideatek_entity_t` | 0 | The user. |
| `text` | `bridge_text_t` | 8 | The text that the friends see. |
| `join` | `bridge_text_t` | 16 | The join string of the game. |
| `joinable` | `ideatek_joinable_t` | 24 | Who can join. |

Bytes 25 to 31 hold no field.

## ideatek:InviteRequest

`ideatek:InviteRequest` invites a friend.
`ideatek_invite_request_t` takes 16 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `user` | `ideatek_entity_t` | 0 | The user. |
| `account` | `uint64_t` | 8 | The friend. 0 opens the UI of the platform. |

## ideatek:JoinRequested

`ideatek:JoinRequested` tells that a user joins from the platform. It is an event of 1 round.
`ideatek_join_requested_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `user` | `ideatek_entity_t` | 0 | The user. |
| `join` | `bridge_text_t` | 8 | The join string. |
| `order` | `uint32_t` | 16 | The rank in the round. |

Bytes 20 to 23 hold no field.

## ideatek:ideatek_lobby_kind_t

`ideatek_lobby_kind_t` tells who finds a lobby.
`ideatek_lobby_kind_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_LOBBY_KIND_PRIVATE` | Only an invite joins. |
| 1 | `IDEATEK_LOBBY_KIND_FRIENDS` | The friends see it. |
| 2 | `IDEATEK_LOBBY_KIND_PUBLIC` | A search finds it. |

## ideatek:Lobby

`ideatek:Lobby` creates a lobby.
`ideatek_lobby_t` takes 16 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `user` | `ideatek_entity_t` | 0 | The user. |
| `max_members` | `uint32_t` | 8 | The most members. |
| `kind` | `ideatek_lobby_kind_t` | 12 | Who finds it. |
| `joinable` | `bool` | 13 | `true` if it takes new members. |

Bytes 14 to 15 hold no field.

## ideatek:LobbyJoin

`ideatek:LobbyJoin` joins a lobby.
`ideatek_lobby_join_t` takes 16 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `user` | `ideatek_entity_t` | 0 | The user. |
| `id` | `uint64_t` | 8 | The lobby. |

## ideatek:ideatek_lobby_state_t

`ideatek_lobby_state_t` tells the state of the user in a lobby.
`ideatek_lobby_state_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_LOBBY_STATE_PENDING` | Not joined yet. |
| 1 | `IDEATEK_LOBBY_STATE_JOINED` | The user is a member. |
| 2 | `IDEATEK_LOBBY_STATE_FAILED` | The join failed. |
| 3 | `IDEATEK_LOBBY_STATE_LEFT` | The user left. |

## ideatek:LobbyState

`ideatek:LobbyState` is the state of a lobby, on the entity of the lobby or of the join.
`ideatek_lobby_state_record_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `id` | `uint64_t` | 0 | The lobby. |
| `owner` | `uint64_t` | 8 | The account of the owner. |
| `state` | `ideatek_lobby_state_t` | 16 | The state. |

Bytes 17 to 23 hold no field.

## ideatek:LobbyMember

`ideatek:LobbyMember` is 1 member of a lobby.
`ideatek_lobby_member_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `lobby` | `ideatek_entity_t` | 0 | The lobby. |
| `account` | `uint64_t` | 8 | The account. |
| `name` | `bridge_text_t` | 16 | The name. |

## ideatek:LobbyValue

`ideatek:LobbyValue` is 1 value of a lobby. Only the module writes it.
`ideatek_lobby_value_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `lobby` | `ideatek_entity_t` | 0 | The lobby, or the lobby that a search found. |
| `key` | `bridge_text_t` | 8 | The key. |
| `value` | `bridge_text_t` | 16 | The value. |

## ideatek:LobbySet

`ideatek:LobbySet` changes a value of a lobby that the user owns. The game spawns it as an event.
`ideatek_lobby_set_t` takes 32 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `lobby` | `ideatek_entity_t` | 0 | The lobby. |
| `key` | `bridge_text_t` | 8 | The key. |
| `value` | `bridge_text_t` | 16 | The new value. |
| `order` | `uint32_t` | 24 | The rank in the round. |

Bytes 28 to 31 hold no field.

## ideatek:LobbySearch

`ideatek:LobbySearch` searches lobbies. Its filters are entities of their own.
`ideatek_lobby_search_t` takes 16 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `user` | `ideatek_entity_t` | 0 | The user. |
| `max` | `uint32_t` | 8 | The most lobbies. |
| `free_slots` | `uint32_t` | 12 | The fewest free places. |

## ideatek:ideatek_compare_t

`ideatek_compare_t` tells a comparison of a filter.
`ideatek_compare_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_COMPARE_EQUAL` | Equal. |
| 1 | `IDEATEK_COMPARE_NOT_EQUAL` | Not equal. |
| 2 | `IDEATEK_COMPARE_LESS` | Less. |
| 3 | `IDEATEK_COMPARE_LESS_EQUAL` | Less or equal. |
| 4 | `IDEATEK_COMPARE_GREATER` | Greater. |
| 5 | `IDEATEK_COMPARE_GREATER_EQUAL` | Greater or equal. |

## ideatek:LobbyFilter

`ideatek:LobbyFilter` is 1 filter of a search.
`ideatek_lobby_filter_t` takes 32 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `search` | `ideatek_entity_t` | 0 | The search. |
| `key` | `bridge_text_t` | 8 | The key. |
| `value` | `bridge_text_t` | 16 | The value to compare with. |
| `compare` | `ideatek_compare_t` | 24 | The comparison. |
| `numeric` | `bool` | 25 | `true` if the values are numbers. |

Bytes 26 to 31 hold no field.

## ideatek:LobbyFound

`ideatek:LobbyFound` is 1 lobby that a search found, with its values.
`ideatek_lobby_found_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `search` | `ideatek_entity_t` | 0 | The search. |
| `id` | `uint64_t` | 8 | The lobby. |
| `members` | `uint32_t` | 16 | The members. |
| `max_members` | `uint32_t` | 20 | The most members. |

## ideatek:ideatek_voice_channel_flags_t

`ideatek_voice_channel_flags_t` names the options of a voice channel.
`ideatek_voice_channel_flags_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_VOICE_CHANNEL_FLAGS_TRANSMIT` | The user talks in the channel. |

## ideatek:VoiceChannel

`ideatek:VoiceChannel` joins a voice channel: *The voice*.
`ideatek_voice_channel_t` takes 32 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `user` | `ideatek_entity_t` | 0 | The user. |
| `bus` | `ideatek_entity_t` | 8 | The bus of the sound part. 0 means the main bus. |
| `name` | `bridge_text_t` | 16 | The name of the channel, for example the id of a lobby. |
| `flags` | `ideatek_voice_channel_flags_t` | 24 | The options. |

Bytes 25 to 31 hold no field.

## ideatek:VoiceMember

`ideatek:VoiceMember` is 1 member of a voice channel.
`ideatek_voice_member_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `channel` | `ideatek_entity_t` | 0 | The channel. |
| `account` | `uint64_t` | 8 | The account. |
| `talking` | `bool` | 16 | `true` while the member talks. |

Bytes 17 to 23 hold no field.

## ideatek:VoiceSettings

`ideatek:VoiceSettings` sets how a user hears a member, on the member entity.
`ideatek_voice_settings_t` takes 16 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `speaker` | `ideatek_entity_t` | 0 | 0 | The entity where the voice plays. 0 means flat. |
| `volume` | `float` | 8 | 1 | The gain. |
| `muted` | `bool` | 12 | `false` | `true` if the user does not hear the member. |

Bytes 13 to 15 hold no field.

## ideatek:Entitlement

`ideatek:Entitlement` is 1 content that a user can own, for example a DLC.
`ideatek_entitlement_t` takes 40 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `user` | `ideatek_entity_t` | 0 | The user. |
| `id` | `bridge_text_t` | 8 | The id on the platform, as text. |
| `name` | `bridge_text_t` | 16 | The name. |
| `path` | `bridge_text_t` | 24 | The folder of the content. It is empty when it is not installed. |
| `owned` | `bool` | 32 | `true` if the user owns it. |
| `installed` | `bool` | 33 | `true` if it is installed. |

Bytes 34 to 39 hold no field.

## ideatek:InstallRequest

`ideatek:InstallRequest` installs an owned content.
`ideatek_install_request_t` takes 8 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `entitlement` | `ideatek_entity_t` | 0 | The entitlement. |

## ideatek:ideatek_store_flags_t

`ideatek_store_flags_t` names the options of the store.
`ideatek_store_flags_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_STORE_FLAGS_ADD_TO_CART` | The store adds the content to the cart. |

## ideatek:StoreRequest

`ideatek:StoreRequest` opens the store of the platform.
`ideatek_store_request_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `user` | `ideatek_entity_t` | 0 | The user. |
| `id` | `bridge_text_t` | 8 | The content. Empty means the page of the game. |
| `flags` | `ideatek_store_flags_t` | 16 | The options. |

Bytes 17 to 23 hold no field.

## ideatek:PlatformOverlay

`ideatek:PlatformOverlay` tells that the overlay of the platform is open. The module writes it on 1 entity.
`ideatek_platform_overlay_t` takes 1 bytes. Its alignment is 1.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `active` | `bool` | 0 | `true` while the overlay is open. |

## ideatek:ideatek_ugc_state_t

`ideatek_ugc_state_t` names the state of an item of the users.
`ideatek_ugc_state_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_UGC_STATE_SUBSCRIBED` | The user subscribes. |
| 0x02 | `IDEATEK_UGC_STATE_INSTALLED` | The item is installed. |
| 0x04 | `IDEATEK_UGC_STATE_NEEDS_UPDATE` | A newer version exists. |
| 0x08 | `IDEATEK_UGC_STATE_DOWNLOADING` | The item downloads. |

## ideatek:UgcItem

`ideatek:UgcItem` is 1 item that a user subscribes to, for example a mod.
`ideatek_ugc_item_t` takes 48 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `user` | `ideatek_entity_t` | 0 | The user. |
| `bytes` | `uint64_t` | 8 | The size. |
| `id` | `bridge_text_t` | 16 | The id of the item. |
| `title` | `bridge_text_t` | 24 | The title. |
| `path` | `bridge_text_t` | 32 | The folder of the item. |
| `state` | `ideatek_ugc_state_t` | 40 | The state. |

Bytes 41 to 47 hold no field.

## ideatek:UgcSubscribe

`ideatek:UgcSubscribe` subscribes to an item.
`ideatek_ugc_subscribe_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `user` | `ideatek_entity_t` | 0 | The user. |
| `id` | `bridge_text_t` | 8 | The item. |
| `subscribe` | `bool` | 16 | `true` to subscribe, `false` to stop. |

Bytes 17 to 23 hold no field.

## ideatek:UgcPublish

`ideatek:UgcPublish` uploads an item.
`ideatek_ugc_publish_t` takes 40 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `user` | `ideatek_entity_t` | 0 | The user. |
| `id` | `bridge_text_t` | 8 | The item. Empty means a new item. |
| `title` | `bridge_text_t` | 16 | The title. |
| `folder` | `bridge_text_t` | 24 | The folder of the user storage to upload. |
| `note` | `bridge_text_t` | 32 | The note of the change. |

## ideatek:UgcResult

`ideatek:UgcResult` is the answer to a publish.
`ideatek_ugc_result_t` takes 12 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `id` | `bridge_text_t` | 0 | The item. |
| `state` | `ideatek_service_state_t` | 8 | The state. |

Bytes 9 to 11 hold no field.

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:ideatek_service_state_t` | `ideatek_service_state_t` | `ideatek_service_state_t_desc` | `IDEATEK_SERVICE_STATE_VALUES` |
| `ideatek:ideatek_platform_t` | `ideatek_platform_t` | `ideatek_platform_t_desc` | `IDEATEK_PLATFORM_VALUES` |
| `ideatek:ideatek_sign_status_t` | `ideatek_sign_status_t` | `ideatek_sign_status_t_desc` | `IDEATEK_SIGN_STATUS_VALUES` |
| `ideatek:PlatformUser` | `ideatek_platform_user_t` | `ideatek_platform_user_t_desc` | `IDEATEK_PLATFORM_USER_FIELDS` |
| `ideatek:DeviceUser` | `ideatek_device_user_t` | `ideatek_device_user_t_desc` | `IDEATEK_DEVICE_USER_FIELDS` |
| `ideatek:ideatek_sign_in_flags_t` | `ideatek_sign_in_flags_t` | `ideatek_sign_in_flags_t_desc` | `IDEATEK_SIGN_IN_FLAGS_VALUES` |
| `ideatek:SignInRequest` | `ideatek_sign_in_request_t` | `ideatek_sign_in_request_t_desc` | `IDEATEK_SIGN_IN_REQUEST_FIELDS` |
| `ideatek:ideatek_ticket_purpose_t` | `ideatek_ticket_purpose_t` | `ideatek_ticket_purpose_t_desc` | `IDEATEK_TICKET_PURPOSE_VALUES` |
| `ideatek:TicketRequest` | `ideatek_ticket_request_t` | `ideatek_ticket_request_t_desc` | `IDEATEK_TICKET_REQUEST_FIELDS` |
| `ideatek:Ticket` | `ideatek_ticket_t` | `ideatek_ticket_t_desc` | `IDEATEK_TICKET_FIELDS` |
| `ideatek:TicketCheck` | `ideatek_ticket_check_t` | `ideatek_ticket_check_t_desc` | `IDEATEK_TICKET_CHECK_FIELDS` |
| `ideatek:TicketResult` | `ideatek_ticket_result_t` | `ideatek_ticket_result_t_desc` | `IDEATEK_TICKET_RESULT_FIELDS` |
| `ideatek:AchievementState` | `ideatek_achievement_state_t` | `ideatek_achievement_state_t_desc` | `IDEATEK_ACHIEVEMENT_STATE_FIELDS` |
| `ideatek:AchievementProgress` | `ideatek_achievement_progress_t` | `ideatek_achievement_progress_t_desc` | `IDEATEK_ACHIEVEMENT_PROGRESS_FIELDS` |
| `ideatek:StatState` | `ideatek_stat_state_t` | `ideatek_stat_state_t_desc` | `IDEATEK_STAT_STATE_FIELDS` |
| `ideatek:ideatek_stat_mode_t` | `ideatek_stat_mode_t` | `ideatek_stat_mode_t_desc` | `IDEATEK_STAT_MODE_VALUES` |
| `ideatek:StatUpdate` | `ideatek_stat_update_t` | `ideatek_stat_update_t_desc` | `IDEATEK_STAT_UPDATE_FIELDS` |
| `ideatek:ideatek_score_method_t` | `ideatek_score_method_t` | `ideatek_score_method_t_desc` | `IDEATEK_SCORE_METHOD_VALUES` |
| `ideatek:ScoreUpload` | `ideatek_score_upload_t` | `ideatek_score_upload_t_desc` | `IDEATEK_SCORE_UPLOAD_FIELDS` |
| `ideatek:ScoreResult` | `ideatek_score_result_t` | `ideatek_score_result_t_desc` | `IDEATEK_SCORE_RESULT_FIELDS` |
| `ideatek:ideatek_score_range_t` | `ideatek_score_range_t` | `ideatek_score_range_t_desc` | `IDEATEK_SCORE_RANGE_VALUES` |
| `ideatek:ScoreQuery` | `ideatek_score_query_t` | `ideatek_score_query_t_desc` | `IDEATEK_SCORE_QUERY_FIELDS` |
| `ideatek:ScoreEntry` | `ideatek_score_entry_t` | `ideatek_score_entry_t_desc` | `IDEATEK_SCORE_ENTRY_FIELDS` |
| `ideatek:CloudState` | `ideatek_cloud_state_t` | `ideatek_cloud_state_t_desc` | `IDEATEK_CLOUD_STATE_FIELDS` |
| `ideatek:ideatek_friend_status_t` | `ideatek_friend_status_t` | `ideatek_friend_status_t_desc` | `IDEATEK_FRIEND_STATUS_VALUES` |
| `ideatek:Friend` | `ideatek_friend_t` | `ideatek_friend_t_desc` | `IDEATEK_FRIEND_FIELDS` |
| `ideatek:ideatek_joinable_t` | `ideatek_joinable_t` | `ideatek_joinable_t_desc` | `IDEATEK_JOINABLE_VALUES` |
| `ideatek:Presence` | `ideatek_presence_t` | `ideatek_presence_t_desc` | `IDEATEK_PRESENCE_FIELDS` |
| `ideatek:InviteRequest` | `ideatek_invite_request_t` | `ideatek_invite_request_t_desc` | `IDEATEK_INVITE_REQUEST_FIELDS` |
| `ideatek:JoinRequested` | `ideatek_join_requested_t` | `ideatek_join_requested_t_desc` | `IDEATEK_JOIN_REQUESTED_FIELDS` |
| `ideatek:ideatek_lobby_kind_t` | `ideatek_lobby_kind_t` | `ideatek_lobby_kind_t_desc` | `IDEATEK_LOBBY_KIND_VALUES` |
| `ideatek:Lobby` | `ideatek_lobby_t` | `ideatek_lobby_t_desc` | `IDEATEK_LOBBY_FIELDS` |
| `ideatek:LobbyJoin` | `ideatek_lobby_join_t` | `ideatek_lobby_join_t_desc` | `IDEATEK_LOBBY_JOIN_FIELDS` |
| `ideatek:ideatek_lobby_state_t` | `ideatek_lobby_state_t` | `ideatek_lobby_state_t_desc` | `IDEATEK_LOBBY_STATE_VALUES` |
| `ideatek:LobbyState` | `ideatek_lobby_state_record_t` | `ideatek_lobby_state_record_t_desc` | `IDEATEK_LOBBY_STATE_RECORD_FIELDS` |
| `ideatek:LobbyMember` | `ideatek_lobby_member_t` | `ideatek_lobby_member_t_desc` | `IDEATEK_LOBBY_MEMBER_FIELDS` |
| `ideatek:LobbyValue` | `ideatek_lobby_value_t` | `ideatek_lobby_value_t_desc` | `IDEATEK_LOBBY_VALUE_FIELDS` |
| `ideatek:LobbySet` | `ideatek_lobby_set_t` | `ideatek_lobby_set_t_desc` | `IDEATEK_LOBBY_SET_FIELDS` |
| `ideatek:LobbySearch` | `ideatek_lobby_search_t` | `ideatek_lobby_search_t_desc` | `IDEATEK_LOBBY_SEARCH_FIELDS` |
| `ideatek:ideatek_compare_t` | `ideatek_compare_t` | `ideatek_compare_t_desc` | `IDEATEK_COMPARE_VALUES` |
| `ideatek:LobbyFilter` | `ideatek_lobby_filter_t` | `ideatek_lobby_filter_t_desc` | `IDEATEK_LOBBY_FILTER_FIELDS` |
| `ideatek:LobbyFound` | `ideatek_lobby_found_t` | `ideatek_lobby_found_t_desc` | `IDEATEK_LOBBY_FOUND_FIELDS` |
| `ideatek:ideatek_voice_channel_flags_t` | `ideatek_voice_channel_flags_t` | `ideatek_voice_channel_flags_t_desc` | `IDEATEK_VOICE_CHANNEL_FLAGS_VALUES` |
| `ideatek:VoiceChannel` | `ideatek_voice_channel_t` | `ideatek_voice_channel_t_desc` | `IDEATEK_VOICE_CHANNEL_FIELDS` |
| `ideatek:VoiceMember` | `ideatek_voice_member_t` | `ideatek_voice_member_t_desc` | `IDEATEK_VOICE_MEMBER_FIELDS` |
| `ideatek:VoiceSettings` | `ideatek_voice_settings_t` | `ideatek_voice_settings_t_desc` | `IDEATEK_VOICE_SETTINGS_FIELDS` |
| `ideatek:Entitlement` | `ideatek_entitlement_t` | `ideatek_entitlement_t_desc` | `IDEATEK_ENTITLEMENT_FIELDS` |
| `ideatek:InstallRequest` | `ideatek_install_request_t` | `ideatek_install_request_t_desc` | `IDEATEK_INSTALL_REQUEST_FIELDS` |
| `ideatek:ideatek_store_flags_t` | `ideatek_store_flags_t` | `ideatek_store_flags_t_desc` | `IDEATEK_STORE_FLAGS_VALUES` |
| `ideatek:StoreRequest` | `ideatek_store_request_t` | `ideatek_store_request_t_desc` | `IDEATEK_STORE_REQUEST_FIELDS` |
| `ideatek:PlatformOverlay` | `ideatek_platform_overlay_t` | `ideatek_platform_overlay_t_desc` | `IDEATEK_PLATFORM_OVERLAY_FIELDS` |
| `ideatek:ideatek_ugc_state_t` | `ideatek_ugc_state_t` | `ideatek_ugc_state_t_desc` | `IDEATEK_UGC_STATE_VALUES` |
| `ideatek:UgcItem` | `ideatek_ugc_item_t` | `ideatek_ugc_item_t_desc` | `IDEATEK_UGC_ITEM_FIELDS` |
| `ideatek:UgcSubscribe` | `ideatek_ugc_subscribe_t` | `ideatek_ugc_subscribe_t_desc` | `IDEATEK_UGC_SUBSCRIBE_FIELDS` |
| `ideatek:UgcPublish` | `ideatek_ugc_publish_t` | `ideatek_ugc_publish_t_desc` | `IDEATEK_UGC_PUBLISH_FIELDS` |
| `ideatek:UgcResult` | `ideatek_ugc_result_t` | `ideatek_ugc_result_t_desc` | `IDEATEK_UGC_RESULT_FIELDS` |

```text
{ IDEATEK_SERVICE_STATE_PENDING = 0, IDEATEK_SERVICE_STATE_DONE = 1, IDEATEK_SERVICE_STATE_FAILED = 2 }
{ IDEATEK_PLATFORM_NONE = 0, IDEATEK_PLATFORM_STEAM = 1, IDEATEK_PLATFORM_EOS = 2, IDEATEK_PLATFORM_OTHER = 255 }
{ IDEATEK_SIGN_STATUS_SIGNED_OUT = 0, IDEATEK_SIGN_STATUS_SIGNED_IN = 1, IDEATEK_SIGN_STATUS_REDUCED = 2 }
{ uint64_t account; bridge_text_t name; ideatek_platform_t platform; ideatek_sign_status_t status; }
{ ideatek_entity_t user; }
{ IDEATEK_SIGN_IN_FLAGS_SHOW_UI = 0x01 }
{ ideatek_sign_in_flags_t flags; }
{ IDEATEK_TICKET_PURPOSE_SERVER = 0, IDEATEK_TICKET_PURPOSE_WEB = 1 }
{ ideatek_entity_t user; bridge_text_t identity; ideatek_ticket_purpose_t purpose; }
{ bridge_text_t data; ideatek_service_state_t state; }
{ uint64_t account; bridge_text_t data; }
{ uint64_t account; uint64_t owner; ideatek_service_state_t state; }
{ ideatek_entity_t user; int64_t time; bridge_text_t name; float progress; bool unlocked; }
{ ideatek_entity_t user; bridge_text_t name; float progress; }
{ ideatek_entity_t user; double value; bridge_text_t name; }
{ IDEATEK_STAT_MODE_SET = 0, IDEATEK_STAT_MODE_ADD = 1 }
{ ideatek_entity_t user; double value; bridge_text_t name; ideatek_stat_mode_t mode; }
{ IDEATEK_SCORE_METHOD_KEEP_BEST = 0, IDEATEK_SCORE_METHOD_FORCE = 1 }
{ ideatek_entity_t user; int64_t score; bridge_text_t board; ideatek_score_method_t method; }
{ int32_t rank; ideatek_service_state_t state; }
{ IDEATEK_SCORE_RANGE_GLOBAL = 0, IDEATEK_SCORE_RANGE_AROUND_USER = 1, IDEATEK_SCORE_RANGE_FRIENDS = 2 }
{ ideatek_entity_t user; bridge_text_t board; int32_t start; uint32_t count; ideatek_score_range_t range; }
{ ideatek_entity_t query; uint64_t account; int64_t score; bridge_text_t name; int32_t rank; }
{ ideatek_entity_t user; uint64_t quota_bytes; uint64_t free_bytes; bool enabled; }
{ IDEATEK_FRIEND_STATUS_OFFLINE = 0, IDEATEK_FRIEND_STATUS_ONLINE = 1, IDEATEK_FRIEND_STATUS_AWAY = 2, IDEATEK_FRIEND_STATUS_BUSY = 3 }
{ ideatek_entity_t user; uint64_t account; bridge_text_t name; bridge_text_t presence; ideatek_friend_status_t status; bool in_game; }
{ IDEATEK_JOINABLE_PUBLIC = 0, IDEATEK_JOINABLE_FRIENDS = 1, IDEATEK_JOINABLE_INVITE = 2, IDEATEK_JOINABLE_PRIVATE = 3 }
{ ideatek_entity_t user; bridge_text_t text; bridge_text_t join; ideatek_joinable_t joinable; }
{ ideatek_entity_t user; uint64_t account; }
{ ideatek_entity_t user; bridge_text_t join; uint32_t order; }
{ IDEATEK_LOBBY_KIND_PRIVATE = 0, IDEATEK_LOBBY_KIND_FRIENDS = 1, IDEATEK_LOBBY_KIND_PUBLIC = 2 }
{ ideatek_entity_t user; uint32_t max_members; ideatek_lobby_kind_t kind; bool joinable; }
{ ideatek_entity_t user; uint64_t id; }
{ IDEATEK_LOBBY_STATE_PENDING = 0, IDEATEK_LOBBY_STATE_JOINED = 1, IDEATEK_LOBBY_STATE_FAILED = 2, IDEATEK_LOBBY_STATE_LEFT = 3 }
{ uint64_t id; uint64_t owner; ideatek_lobby_state_t state; }
{ ideatek_entity_t lobby; uint64_t account; bridge_text_t name; }
{ ideatek_entity_t lobby; bridge_text_t key; bridge_text_t value; }
{ ideatek_entity_t lobby; bridge_text_t key; bridge_text_t value; uint32_t order; }
{ ideatek_entity_t user; uint32_t max; uint32_t free_slots; }
{ IDEATEK_COMPARE_EQUAL = 0, IDEATEK_COMPARE_NOT_EQUAL = 1, IDEATEK_COMPARE_LESS = 2, IDEATEK_COMPARE_LESS_EQUAL = 3, IDEATEK_COMPARE_GREATER = 4, IDEATEK_COMPARE_GREATER_EQUAL = 5 }
{ ideatek_entity_t search; bridge_text_t key; bridge_text_t value; ideatek_compare_t compare; bool numeric; }
{ ideatek_entity_t search; uint64_t id; uint32_t members; uint32_t max_members; }
{ IDEATEK_VOICE_CHANNEL_FLAGS_TRANSMIT = 0x01 }
{ ideatek_entity_t user; ideatek_entity_t bus; bridge_text_t name; ideatek_voice_channel_flags_t flags; }
{ ideatek_entity_t channel; uint64_t account; bool talking; }
{ ideatek_entity_t speaker; float volume; bool muted; }
{ ideatek_entity_t user; bridge_text_t id; bridge_text_t name; bridge_text_t path; bool owned; bool installed; }
{ ideatek_entity_t entitlement; }
{ IDEATEK_STORE_FLAGS_ADD_TO_CART = 0x01 }
{ ideatek_entity_t user; bridge_text_t id; ideatek_store_flags_t flags; }
{ bool active; }
{ IDEATEK_UGC_STATE_SUBSCRIBED = 0x01, IDEATEK_UGC_STATE_INSTALLED = 0x02, IDEATEK_UGC_STATE_NEEDS_UPDATE = 0x04, IDEATEK_UGC_STATE_DOWNLOADING = 0x08 }
{ ideatek_entity_t user; uint64_t bytes; bridge_text_t id; bridge_text_t title; bridge_text_t path; ideatek_ugc_state_t state; }
{ ideatek_entity_t user; bridge_text_t id; bool subscribe; }
{ ideatek_entity_t user; bridge_text_t id; bridge_text_t title; bridge_text_t folder; bridge_text_t note; }
{ bridge_text_t id; ideatek_service_state_t state; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:ideatek_entity_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_service_state_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_platform_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_sign_status_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_sign_in_flags_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_ticket_purpose_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_stat_mode_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_score_method_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_score_range_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_friend_status_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_joinable_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_lobby_kind_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_lobby_state_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_compare_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_voice_channel_flags_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_store_flags_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_ugc_state_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:PlatformUser` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:DeviceUser` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SignInRequest` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:TicketRequest` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Ticket` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:TicketCheck` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:TicketResult` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:AchievementState` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:AchievementProgress` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:StatState` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:StatUpdate` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ScoreUpload` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ScoreResult` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ScoreQuery` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ScoreEntry` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:CloudState` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Friend` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Presence` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:InviteRequest` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:JoinRequested` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Lobby` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:LobbyJoin` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:LobbyState` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:LobbyMember` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:LobbyValue` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:LobbySet` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:LobbySearch` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:LobbyFilter` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:LobbyFound` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:VoiceChannel` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:VoiceMember` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:VoiceSettings` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Entitlement` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:InstallRequest` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:StoreRequest` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:PlatformOverlay` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:UgcItem` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:UgcSubscribe` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:UgcPublish` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:UgcResult` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

## The grants

| Role | `write` | `read` |
| --- | --- | --- |
| The game | The requests, the presence, the sets of lobby values, the filters, the voice channels and their settings | The types of the fields |
| The platform module | The users, the states, the results, the lists and the events | Each request, and the types of the fields |
