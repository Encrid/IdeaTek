# include/input/ideatek_sensor.h

`include/input/ideatek_sensor.h` gives the types of the sensors.
`docs/extern/input/ideatek_input.h.md` tells the common rules.

## The sensors

A sensor is a device entity with `ideatek:Sensor` and `ideatek:SensorState`. A sensor of a gamepad names the gamepad.
The game attaches `ideatek:SensorRequest` to start or to stop a sensor. A sensor gives no data until it starts.

For the user who holds the device, x goes to the right, y goes to the top and z comes to the user.
An accelerometer gives meters per second squared, with the gravity. A gyroscope gives radians per second.
The data of an unknown sensor is the data of the system.

## ideatek:Sensor

`ideatek:Sensor` tells the kind of a sensor.
`ideatek_sensor_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `gamepad` | `ideatek_entity_t` | 0 | The gamepad of the sensor, or 0. |
| `raw_type` | `int32_t` | 8 | The kind that the system gives. |
| `rate` | `float` | 12 | The rate of the data, in hertz, or 0. |
| `type` | `ideatek_sensor_type_t` | 16 | The kind of the sensor. |

Bytes 17 to 23 hold no field.

### ideatek:ideatek_sensor_type_t

`ideatek_sensor_type_t` is an enum of `uint8_t`. Its size and its alignment are 1.

| Constant | Value | The sensor |
| --- | --- | --- |
| `IDEATEK_SENSOR_TYPE_UNKNOWN` | 0 | Not known. |
| `IDEATEK_SENSOR_TYPE_ACCEL` | 1 | An accelerometer. |
| `IDEATEK_SENSOR_TYPE_GYRO` | 2 | A gyroscope. |
| `IDEATEK_SENSOR_TYPE_ACCEL_L` | 3 | The accelerometer of a left controller. |
| `IDEATEK_SENSOR_TYPE_GYRO_L` | 4 | The gyroscope of a left controller. |
| `IDEATEK_SENSOR_TYPE_ACCEL_R` | 5 | The accelerometer of a right controller. |
| `IDEATEK_SENSOR_TYPE_GYRO_R` | 6 | The gyroscope of a right controller. |

## ideatek:SensorRequest

`ideatek:SensorRequest` starts or stops a sensor.
`ideatek_sensor_request_t` takes 1 bytes. Its alignment is 1.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `enabled` | `bool` | 0 | `true` starts the sensor. `false` stops it. |

## ideatek:SensorState

`ideatek:SensorState` holds the last data of a sensor.
`ideatek_sensor_state_t` takes 32 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `time` | `uint64_t` | 0 | The time of the data, in nanoseconds of the clock of the system. |
| `data` | `float[6]` | 8 | The data. An accelerometer and a gyroscope use x, y and z. |

## ideatek:SensorEvent

`ideatek:SensorEvent` gives new data of a sensor.
`ideatek_sensor_event_t` takes 48 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `time` | `uint64_t` | 0 | The time of the data, in nanoseconds. |
| `sensor` | `ideatek_entity_t` | 8 | The sensor entity. |
| `data` | `float[6]` | 16 | The new data. |
| `order` | `uint32_t` | 40 | The rank of the event: `docs/extern/input/ideatek_input.h.md`, *The events*. |

Bytes 44 to 47 hold no field.

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:ideatek_sensor_type_t` | `ideatek_sensor_type_t` | `ideatek_sensor_type_t_desc` | `IDEATEK_SENSOR_TYPE_VALUES` |
| `ideatek:Sensor` | `ideatek_sensor_t` | `ideatek_sensor_t_desc` | `IDEATEK_SENSOR_FIELDS` |
| `ideatek:SensorRequest` | `ideatek_sensor_request_t` | `ideatek_sensor_request_t_desc` | `IDEATEK_SENSOR_REQUEST_FIELDS` |
| `ideatek:SensorState` | `ideatek_sensor_state_t` | `ideatek_sensor_state_t_desc` | `IDEATEK_SENSOR_STATE_FIELDS` |
| `ideatek:SensorEvent` | `ideatek_sensor_event_t` | `ideatek_sensor_event_t_desc` | `IDEATEK_SENSOR_EVENT_FIELDS` |

```text
{ IDEATEK_SENSOR_TYPE_UNKNOWN = 0, IDEATEK_SENSOR_TYPE_ACCEL = 1, IDEATEK_SENSOR_TYPE_GYRO = 2, IDEATEK_SENSOR_TYPE_ACCEL_L = 3, IDEATEK_SENSOR_TYPE_GYRO_L = 4, IDEATEK_SENSOR_TYPE_ACCEL_R = 5, IDEATEK_SENSOR_TYPE_GYRO_R = 6 }
{ ideatek_entity_t gamepad; int32_t raw_type; float rate; ideatek_sensor_type_t type; }
{ bool enabled; }
{ uint64_t time; float data[6]; }
{ uint64_t time; ideatek_entity_t sensor; float data[6]; uint32_t order; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:ideatek_entity_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_sensor_type_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Sensor` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SensorRequest` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SensorState` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SensorEvent` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

`docs/extern/input/ideatek_input.h.md`, *The declarations*, tells the order of the declarations.

## The grants

| Role | `write` | `read` |
| --- | --- | --- |
| The game | `ideatek:SensorRequest` | The field types |
| The input module | `ideatek:Sensor`, `ideatek:SensorState` and `ideatek:SensorEvent` | `ideatek:SensorRequest` and the field types |
| A reader | No type. | Its types and the field types |
