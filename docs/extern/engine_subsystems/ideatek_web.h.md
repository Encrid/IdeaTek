# include/engine_subsystems/ideatek_web.h

`include/engine_subsystems/ideatek_web.h` gives the types of the requests to the web: HTTP and HTTPS.
`readme.md`, *The contracts*, tells the rules of each contract.
Its fields hold only bytes and numbers, so the header includes no other part.

## The roles

- The game spawns a request entity and writes `ideatek:WebRequest` on it. After it reads the answer, the game destroys the entity.
- The web module is a native module. It sends the request and writes `ideatek:WebResponse` on the same entity.
- A module in wasm uses the web module, because it cannot open a socket or run TLS.

## The security

The web module always checks the certificate of the server against the trust store of the OS. No type of the part turns the check off.
A failed check gives the error TLS.

## The bytes

`headers` holds lines of `Name: value`, each line ended by a CR and an LF. The body is bytes, with no conversion.
A body larger than `body_bytes` gives the error TOO_BIG.

## The settings

| Setting | Default | Range | Value |
| --- | --- | --- | --- |
| `body_bytes` | 4194304 | 1 to 1073741824 | The most bytes of a body. It is the size of `load_bytes` of the resources part. |

## ideatek:ideatek_web_method_t

`ideatek_web_method_t` tells the method of HTTP of a request.
`ideatek_web_method_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_WEB_METHOD_GET` | GET. |
| 1 | `IDEATEK_WEB_METHOD_HEAD` | HEAD. |
| 2 | `IDEATEK_WEB_METHOD_POST` | POST. |
| 3 | `IDEATEK_WEB_METHOD_PUT` | PUT. |
| 4 | `IDEATEK_WEB_METHOD_DELETE` | DELETE. |
| 5 | `IDEATEK_WEB_METHOD_PATCH` | PATCH. |

## ideatek:WebRequest

`ideatek:WebRequest` is a request to the web.
`ideatek_web_request_t` takes 36 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `url` | `bridge_text_t` | 0 | No default | The URL, with `http` or `https`. |
| `headers` | `bridge_text_t` | 8 | No default | The headers: *The bytes*. |
| `body` | `bridge_text_t` | 16 | No default | The body. |
| `timeout` | `float` | 24 | 0 | The seconds before the error TIMEOUT. 0 means no limit. |
| `max_redirects` | `uint32_t` | 28 | 20 | The most redirects. |
| `method` | `ideatek_web_method_t` | 32 | No default | The method. |

Bytes 33 to 35 hold no field.

A limit of 20 redirects is the rule of the Fetch standard of WHATWG, and the limit of Chrome and Firefox.

## ideatek:ideatek_web_state_t

`ideatek_web_state_t` tells the state of a request.
`ideatek_web_state_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_WEB_STATE_PENDING` | No answer yet. |
| 1 | `IDEATEK_WEB_STATE_DONE` | The server answered. |
| 2 | `IDEATEK_WEB_STATE_FAILED` | The request failed: `error` tells why. |

## ideatek:ideatek_web_error_t

`ideatek_web_error_t` tells why a request failed.
`ideatek_web_error_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_WEB_ERROR_NONE` | No error. |
| 1 | `IDEATEK_WEB_ERROR_RESOLVE` | The name of the host was not found. |
| 2 | `IDEATEK_WEB_ERROR_CONNECT` | The connection failed. |
| 3 | `IDEATEK_WEB_ERROR_TLS` | The TLS or the certificate failed. |
| 4 | `IDEATEK_WEB_ERROR_TIMEOUT` | The time ran out. |
| 5 | `IDEATEK_WEB_ERROR_TOO_BIG` | The body is larger than `body_bytes`. |

## ideatek:WebResponse

`ideatek:WebResponse` is the answer to a request. The web module writes it.
`ideatek_web_response_t` takes 20 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `headers` | `bridge_text_t` | 0 | The headers of the answer. |
| `body` | `bridge_text_t` | 8 | The body of the answer. |
| `status` | `uint16_t` | 16 | The status code of HTTP. |
| `state` | `ideatek_web_state_t` | 18 | The state. |
| `error` | `ideatek_web_error_t` | 19 | The error. |

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:ideatek_web_method_t` | `ideatek_web_method_t` | `ideatek_web_method_t_desc` | `IDEATEK_WEB_METHOD_VALUES` |
| `ideatek:WebRequest` | `ideatek_web_request_t` | `ideatek_web_request_t_desc` | `IDEATEK_WEB_REQUEST_FIELDS` |
| `ideatek:ideatek_web_state_t` | `ideatek_web_state_t` | `ideatek_web_state_t_desc` | `IDEATEK_WEB_STATE_VALUES` |
| `ideatek:ideatek_web_error_t` | `ideatek_web_error_t` | `ideatek_web_error_t_desc` | `IDEATEK_WEB_ERROR_VALUES` |
| `ideatek:WebResponse` | `ideatek_web_response_t` | `ideatek_web_response_t_desc` | `IDEATEK_WEB_RESPONSE_FIELDS` |

```text
{ IDEATEK_WEB_METHOD_GET = 0, IDEATEK_WEB_METHOD_HEAD = 1, IDEATEK_WEB_METHOD_POST = 2, IDEATEK_WEB_METHOD_PUT = 3, IDEATEK_WEB_METHOD_DELETE = 4, IDEATEK_WEB_METHOD_PATCH = 5 }
{ bridge_text_t url; bridge_text_t headers; bridge_text_t body; float timeout; uint32_t max_redirects; ideatek_web_method_t method; }
{ IDEATEK_WEB_STATE_PENDING = 0, IDEATEK_WEB_STATE_DONE = 1, IDEATEK_WEB_STATE_FAILED = 2 }
{ IDEATEK_WEB_ERROR_NONE = 0, IDEATEK_WEB_ERROR_RESOLVE = 1, IDEATEK_WEB_ERROR_CONNECT = 2, IDEATEK_WEB_ERROR_TLS = 3, IDEATEK_WEB_ERROR_TIMEOUT = 4, IDEATEK_WEB_ERROR_TOO_BIG = 5 }
{ bridge_text_t headers; bridge_text_t body; uint16_t status; ideatek_web_state_t state; ideatek_web_error_t error; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:ideatek_web_method_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_web_state_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_web_error_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:WebRequest` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:WebResponse` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

## The grants

| Role | `write` | `read` |
| --- | --- | --- |
| The game | `ideatek:WebRequest` | The types of the fields |
| The web module | `ideatek:WebResponse` | `ideatek:WebRequest`, and the types of the fields |
