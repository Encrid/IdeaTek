# src/platform_abstraction/sockets_gns/sockets_judge.c

This file checks the fields of `ideatek:ConnectionRequest`, and attaches the problem of a request that fails.

## The judge

`judge` checks a request before the observer `connection` runs it: `docs/intern/platform_abstraction/sockets_gns/sockets_connect.c.md`.

1. `action` is in its enum, 0 to 4. A wrong action writes its line, with the range 0 to 4.
2. With `IDEATEK_CONNECTION_ACTION_CLOSE`, `judge_close` checks `reason`, from `GNS_END_APP_MIN` to `GNS_END_APP_MAX`, 1000 to 2999. A `linger` on a symmetric connection is wrong too: GameNetworkingSockets does not linger a symmetric connection.
3. With each other action, `judge_rates` checks the 2 send rates, from 1024 to 268435456, and `send_rate_min` not above `send_rate_max`. The range of the line is `1024 to <send_rate_max>` when both rates are in their range, else `1024 to 268435456`.

A wrong field writes its line, and `judge` gives `false`: the request does nothing. Each line comes from `say_request`: `docs/intern/platform_abstraction/sockets_gns/sockets_log.c.md`.

## The problem

`attach_problem` attaches `ideatek:ConnectionState` with `IDEATEK_CONNECTION_STATUS_PROBLEM` and a text in `end_text`, for a request that GameNetworkingSockets or a bound refuses at once.

## The branches

| Case | Result |
| --- | --- |
| An action outside 0 to 4. | Its line, and `false`. |
| A close with a reason outside 1000 to 2999. | Its line, and `false`. |
| A close with `linger` on a symmetric connection. | Its line, and `false`. |
| A send rate outside its range, or `send_rate_min` above `send_rate_max`. | Its line, and `false`. |
| A good request. | `true`. |

## Functions

| Function | Result |
| --- | --- |
| `attach_problem` | Attaches a state with the problem and its text. |
| `judge_rates` | Checks the 2 send rates. Gives `false` with its line for a wrong one. |
| `judge_close` | Checks the reason and `linger` of a close. A wrong one gives `false` and its line. |
| `judge` | Checks the action, then the fields of the action. Gives `false` for a wrong field. |
