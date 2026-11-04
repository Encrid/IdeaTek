# include/engine_subsystems/ideatek_subtitles.h

`include/engine_subsystems/ideatek_subtitles.h` gives the types of the subtitles and the closed captions of the sounds.
`readme.md`, *The contracts*, tells the rules of each contract.
The header includes the world part: a caption names the entity of its sound.

## The roles

- The game loads 1 set of captions for each language, and writes the setting of the player.
- The subtitle module reads the players of the sound part. When a player plays a sound with captions, it spawns the lines at the time of each cue.
- The UI of the game reads the lines and draws them with the nodes of the UI part. The subtitle module draws nothing.

## The lines

A caption is a cue of a sound: a speech or an effect. 1 sound can have many cues, each on an entity of its own.
The module spawns a line at the time of its cue, minus `lead`. It destroys the line after its end plus `linger`.
The lines of a scene of a choreography come from its sounds, so they need no other form.
The colors of a speaker come from the spans of the text of the UI part.

## The defaults

The setting of the player is off by default, as in Source 1. Unreal turns it on, so the game chooses its own default.
The linger and the lead are 0, because only Source 1 gives values for them.

## The settings

| Setting | Default | Range | Value |
| --- | --- | --- | --- |
| `linger` | 0 | 0 or more | The seconds that a line stays after its end. |
| `lead` | 0 | 0 or more | The seconds that a line shows before its cue. |

## ideatek:ideatek_caption_kind_t

`ideatek_caption_kind_t` tells the kind of a caption.
`ideatek_caption_kind_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_CAPTION_KIND_SPEECH` | A speech. |
| 1 | `IDEATEK_CAPTION_KIND_EFFECT` | A sound effect, for the players who cannot hear it. |

## ideatek:SoundCaption

`ideatek:SoundCaption` is 1 timed cue of the text of a sound.
`ideatek_sound_caption_t` takes 40 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `sound` | `ideatek_entity_t` | 0 | No default | The sound. |
| `speaker` | `bridge_text_t` | 8 | No default | The name of the speaker. |
| `text` | `bridge_text_t` | 16 | No default | The text, in UTF-8. |
| `time` | `float` | 24 | 0 | The time of the cue in the sound. |
| `duration` | `float` | 28 | 0 | The length. 0 means up to the end of the sound. |
| `priority` | `int32_t` | 32 | 0 | The rank against other lines at the same time. |
| `kind` | `ideatek_caption_kind_t` | 36 | No default | The kind. |

Bytes 37 to 39 hold no field.

## ideatek:SubtitleLine

`ideatek:SubtitleLine` is a line to show now. The subtitle module writes it.
`ideatek_subtitle_line_t` takes 32 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `caption` | `ideatek_entity_t` | 0 | The caption. |
| `player` | `ideatek_entity_t` | 8 | The player that plays the sound. |
| `start` | `float` | 16 | The time of the start, in `elapsed` of the frame. |
| `end` | `float` | 20 | The time of the end. |
| `kind` | `ideatek_caption_kind_t` | 24 | The kind. |

Bytes 25 to 31 hold no field.

## ideatek:ideatek_subtitle_flags_t

`ideatek_subtitle_flags_t` names the lines that show.
`ideatek_subtitle_flags_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_SUBTITLE_FLAGS_SPEECH` | The lines of the speech show. |
| 0x02 | `IDEATEK_SUBTITLE_FLAGS_EFFECTS` | The lines of the effects show. |

## ideatek:SubtitleSettings

`ideatek:SubtitleSettings` is the choice of the player. The game saves it as a setting of the user.
`ideatek_subtitle_settings_t` takes 1 byte. Its alignment is 1.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `flags` | `ideatek_subtitle_flags_t` | 0 | 0 | The lines that show. |

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:ideatek_caption_kind_t` | `ideatek_caption_kind_t` | `ideatek_caption_kind_t_desc` | `IDEATEK_CAPTION_KIND_VALUES` |
| `ideatek:SoundCaption` | `ideatek_sound_caption_t` | `ideatek_sound_caption_t_desc` | `IDEATEK_SOUND_CAPTION_FIELDS` |
| `ideatek:SubtitleLine` | `ideatek_subtitle_line_t` | `ideatek_subtitle_line_t_desc` | `IDEATEK_SUBTITLE_LINE_FIELDS` |
| `ideatek:ideatek_subtitle_flags_t` | `ideatek_subtitle_flags_t` | `ideatek_subtitle_flags_t_desc` | `IDEATEK_SUBTITLE_FLAGS_VALUES` |
| `ideatek:SubtitleSettings` | `ideatek_subtitle_settings_t` | `ideatek_subtitle_settings_t_desc` | `IDEATEK_SUBTITLE_SETTINGS_FIELDS` |

```text
{ IDEATEK_CAPTION_KIND_SPEECH = 0, IDEATEK_CAPTION_KIND_EFFECT = 1 }
{ ideatek_entity_t sound; bridge_text_t speaker; bridge_text_t text; float time; float duration; int32_t priority; ideatek_caption_kind_t kind; }
{ ideatek_entity_t caption; ideatek_entity_t player; float start; float end; ideatek_caption_kind_t kind; }
{ IDEATEK_SUBTITLE_FLAGS_SPEECH = 0x01, IDEATEK_SUBTITLE_FLAGS_EFFECTS = 0x02 }
{ ideatek_subtitle_flags_t flags; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:ideatek_entity_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_caption_kind_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_subtitle_flags_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SoundCaption` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SubtitleLine` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SubtitleSettings` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

## The grants

| Role | `write` | `read` |
| --- | --- | --- |
| The game | `ideatek:SoundCaption`, `ideatek:SubtitleSettings` | The types of the fields |
| The subtitle module | `ideatek:SubtitleLine` | `ideatek:SoundCaption`, `ideatek:SubtitleSettings`, `ideatek:SoundPlayer`, and the types of the fields |
