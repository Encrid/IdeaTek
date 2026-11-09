# src/platform_abstraction/sockets_gns/sockets_config.c

This file gives the settings of the contract to GameNetworkingSockets as its global config values.

## The table

`SOCKETS_NUMBERS` is a constant table of the number settings. A row holds the offset of the field in `sockets_settings_t`, its range, its name, and its config value.
`check_numbers` of `docs/intern/platform_abstraction/sockets_gns/sockets_start.c.md` and `apply_numbers` both walk it, so a setting has its range and its config value in 1 place.

| Setting | Range | Config value |
| --- | --- | --- |
| `message_bytes` | 1048576 to 1073741824, an even number | None: the half. |
| `timeout_initial`, `timeout_connected` | 1 to 2147483647 | `TimeoutInitial`, `TimeoutConnected`. |
| `send_buffer` | 4096 to 268435456 | `SendBufferSize`. |
| `recv_buffer` | 4096 to 268435456, more than `recv_max_message` | `RecvBufferSize`. |
| `recv_buffer_messages` | 2 to 268435456 | `RecvBufferMessages`. |
| `recv_max_message` | 64 to 268435456 | `RecvMaxMessageSize`. |
| `nagle_time` | 0 to 20000 | `NagleTime`. |
| `mtu` | 200 to 1300 | `MTU_PacketSize`. |
| `ice_penalty` | 0 to 2147483647 | `P2P_Transport_ICE_Penalty`. |
| `fake_loss_send`, `fake_loss_recv` | 0 to 100 | `FakePacketLoss_Send`, `FakePacketLoss_Recv`, as floats. |
| `fake_lag_send`, `fake_lag_recv` | 0 to 5000 | `FakePacketLag_Send`, `FakePacketLag_Recv`. |
| `fake_reorder_send`, `fake_reorder_recv` | 0 to 100 | `FakePacketReorder_Send`, `FakePacketReorder_Recv`, as floats. |
| `fake_reorder_time` | 0 to 5000 | `FakePacketReorder_Time`. |
| `fake_duplicate_send`, `fake_duplicate_recv` | 0 to 100 | `FakePacketDup_Send`, `FakePacketDup_Recv`, as floats. |
| `fake_duplicate_time_max` | 0 to 5000 | `FakePacketDup_TimeMax`. |
| `fake_jitter_send_avg`, `fake_jitter_recv_avg` | 0 to 2000 | `FakePacketJitter_Send_Avg`, `FakePacketJitter_Recv_Avg`, as floats. |
| `fake_jitter_send_max`, `fake_jitter_recv_max` | 0 to 5000 | `FakePacketJitter_Send_Max`, `FakePacketJitter_Recv_Max`, as floats. |
| `fake_jitter_send_pct`, `fake_jitter_recv_pct` | 0 to 100 | `FakePacketJitter_Send_Pct`, `FakePacketJitter_Recv_Pct`, as floats. |
| `fake_rate_send`, `fake_rate_recv` | 0 to 1073741824 | `FakeRateLimit_Send_Rate`, `FakeRateLimit_Recv_Rate`. |
| `fake_burst_send`, `fake_burst_recv` | 0 to 1048576 | `FakeRateLimit_Send_Burst`, `FakeRateLimit_Recv_Burst`. |
| `max_connections` | 1 to 4096 | None: the table of the connections. |
| `max_listen_sockets` | 1 to 256 | None: the table of the listen sockets. |
| `max_addresses` | 1 to 1024 | None: the table of the names. |
| `session_channels` | 1 to 1024 | None: the channels that `docs/intern/platform_abstraction/sockets_gns/sockets_receive.c.md` reads. |

The range of `recv_buffer` has 2 parts. `check_numbers` checks the second part after the table.

## The words and the texts

`apply_words` gives the number of each word setting: `allow_without_auth` and `allow_localhost_without_auth` as `IP_AllowWithoutAuth` and `IPLocalHost_AllowWithoutAuth`, `unencrypted` as `Unencrypted`, `ice_transports` as `P2P_Transport_ICE_Enable`, and `ice_implementation` as `P2P_Transport_ICE_Implementation`.
`docs/intern/platform_abstraction/sockets_gns/sockets_values.c.md` turns each word into its number.
`apply_texts` gives `stun_servers`, `turn_servers`, `turn_users` and `turn_passwords` as `P2P_STUN_ServerList`, `P2P_TURN_ServerList`, `P2P_TURN_UserList` and `P2P_TURN_PassList`. GameNetworkingSockets copies a text.

## The callbacks

`apply_callbacks` gives the 3 callbacks as pointer values: `Callback_MessagesSessionRequest` and `Callback_MessagesSessionFailed` of `docs/intern/platform_abstraction/sockets_gns/sockets_sessions.c.md`, and `Callback_CreateConnectionSignaling` of `docs/intern/platform_abstraction/sockets_gns/sockets_signals.c.md`.
The callback of the status of the connections is set the same way: `Callback_ConnectionStatusChanged` of `docs/intern/platform_abstraction/sockets_gns/sockets_status.c.md`.

## The branches

| Case | Result |
| --- | --- |
| A setting of the table with no config value. | `apply_numbers` skips it. |
| A setting that GameNetworkingSockets refuses. | The line `<setting>: refused by GameNetworkingSockets`. Only a failed call of the tests reaches it: a value in its range is never refused. |

## Functions

| Function | Result |
| --- | --- |
| `apply_numbers` | Gives each number setting with a config value to GameNetworkingSockets, as an integer or a float. |
| `apply_words` | Gives the 5 word settings as their numbers. |
| `apply_texts` | Gives the 4 text settings. |
| `apply_callbacks` | Gives the 4 callbacks. |
| `apply_settings` | Runs the 4 functions above. |
