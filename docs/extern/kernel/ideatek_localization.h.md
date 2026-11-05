# include/kernel/ideatek_localization.h

`include/kernel/ideatek_localization.h` gives the types of the localization: the locale, the string tables, the keys of the texts and the localized texts.
`readme.md`, *The contracts*, tells the rules of each contract.
Its fields name entities, so its header includes `ideatek_world.h`.

## The roles

- The game writes the locale, and a key on each entity that shows a text.
- A loader makes the string tables, from files such as `.po`, `.csv` or `.json`.
- The localization module writes the localized text of each key in the locale. The UI and the game read it.

## The locale

The game writes `ideatek:Locale` on 1 entity, for example from the language of the platform. `code` is a tag such as `fr-FR`.
A key is looked up in the tables of `code`, then in the tables of `fallback`. An empty `fallback` means no fallback.

## The string tables

A string table is a resource entity with `ideatek:StringTable`. Many tables can hold the same locale, for example 1 table for each mod. `domain` names the table.
A record of `bytes`:

| Field | C type | Offset in the record | Value |
| --- | --- | --- | --- |
| `key` | `uint64_t` | 0 | The hash of the namespace and the key of the text. |
| `source_hash` | `uint64_t` | 8 | The hash of the source string that the text translates. |
| `length` | `uint32_t` | 16 | The length of the text. |
| `text` | UTF-8 | 20 | The text. |

The records have no bytes between them. For 1 locale and 1 key, a later table wins over an earlier one, so a mod can change a text.

## The syntax

`syntax` of a table tells the form of its texts: ICU MessageFormat by default, or Fluent. Both put a plural or a choice in the text itself.
The plural rules come from the locale that the text came from. So a text of the fallback uses the rules of the fallback, and a count never takes a wrong form.

## The keys of the texts

The game writes `ideatek:TextKey` with the key and the args of the text. A record of `args`:

| Field | C type | Offset in the record | Value |
| --- | --- | --- | --- |
| `name` | `uint64_t` | 0 | The hash of the name of the arg in the text. |
| `kind` | `ideatek_text_arg_t` | 8 | The kind of the value. |
| `length` | `uint32_t` | 9 | The length of the value. |
| `value` | bytes | 13 | An `int64_t`, a `double` or a text in UTF-8. |

The records have no bytes between them. A change of the locale, of a table or of the args writes `ideatek:LocalizedText` again.
A `source_hash` above 0 is the hash of the source string that the game expects. A text of another source hash is stale: the module shows it, and marks it.

## The localized assets

A path of the resources part can hold `{locale}`, and the resources module fills it from `ideatek:Locale`.
So the voice lines and the clips of the lip sync of each language are files with `{locale}` in their paths.

## Right to left and the fonts

The game sets `right_to_left` of the UI root from the locale, for the scripts that run from the right.
`fallback` of `ideatek:Font` of the UI part chains the fonts. So a Latin font can fall back to a CJK font.

## ideatek:Locale

`ideatek:Locale` is the locale of the game, on 1 entity: *The locale*.
`ideatek_locale_t` takes 16 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `code` | `bridge_text_t` | 0 | No default | The tag of the locale, for example `fr-FR`. |
| `fallback` | `bridge_text_t` | 8 | No default | The tag of the fallback. Empty means none. |

## ideatek:ideatek_text_syntax_t

`ideatek_text_syntax_t` tells the form of the texts of a table: *The syntax*.
`ideatek_text_syntax_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_TEXT_SYNTAX_ICU` | ICU MessageFormat. |
| 1 | `IDEATEK_TEXT_SYNTAX_FLUENT` | Fluent. |

## ideatek:StringTable

`ideatek:StringTable` holds the texts of 1 locale, on a resource entity.
`ideatek_string_table_t` takes 40 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `bytes` | `bridge_text_t` | 0 | No default | The records: *The string tables*. |
| `locale` | `bridge_text_t` | 8 | No default | The tag of the locale of the table. |
| `domain` | `uint64_t` | 16 | No default | The hash of the name of the table. |
| `count` | `uint32_t` | 24 | No default | The records in `bytes`. |
| `revision` | `uint32_t` | 28 | No default | Raised at each change. |
| `syntax` | `ideatek_text_syntax_t` | 32 | `IDEATEK_TEXT_SYNTAX_ICU` | The form of the texts. |

Bytes 33 to 39 hold no field.

## ideatek:ideatek_text_arg_t

`ideatek_text_arg_t` tells the kind of an arg of a text.
`ideatek_text_arg_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_TEXT_ARG_INT` | An `int64_t`. |
| 1 | `IDEATEK_TEXT_ARG_FLOAT` | A `double`. |
| 2 | `IDEATEK_TEXT_ARG_TEXT` | A text in UTF-8. |

## ideatek:TextKey

`ideatek:TextKey` asks for a text in the locale, on the entity that shows it.
`ideatek_text_key_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `args` | `bridge_text_t` | 0 | No default | The records of the args: *The keys of the texts*. |
| `key` | `uint64_t` | 8 | No default | The hash of the namespace and the key. |
| `source_hash` | `uint64_t` | 16 | 0 | The hash of the expected source string. 0 means no check. |

## ideatek:LocalizedText

`ideatek:LocalizedText` is the text of a key in the locale. The localization module writes it.
`ideatek_localized_text_t` takes 20 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `text` | `bridge_text_t` | 0 | The text, formatted, in UTF-8. |
| `locale` | `bridge_text_t` | 8 | The tag of the locale that the text came from. |
| `missing` | `bool` | 16 | `true` if no table holds the key. The text is then the key in hex. |
| `stale` | `bool` | 17 | `true` if the source hash differs. |

Bytes 18 to 19 hold no field.

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:Locale` | `ideatek_locale_t` | `ideatek_locale_t_desc` | `IDEATEK_LOCALE_FIELDS` |
| `ideatek:ideatek_text_syntax_t` | `ideatek_text_syntax_t` | `ideatek_text_syntax_t_desc` | `IDEATEK_TEXT_SYNTAX_VALUES` |
| `ideatek:StringTable` | `ideatek_string_table_t` | `ideatek_string_table_t_desc` | `IDEATEK_STRING_TABLE_FIELDS` |
| `ideatek:ideatek_text_arg_t` | `ideatek_text_arg_t` | `ideatek_text_arg_t_desc` | `IDEATEK_TEXT_ARG_VALUES` |
| `ideatek:TextKey` | `ideatek_text_key_t` | `ideatek_text_key_t_desc` | `IDEATEK_TEXT_KEY_FIELDS` |
| `ideatek:LocalizedText` | `ideatek_localized_text_t` | `ideatek_localized_text_t_desc` | `IDEATEK_LOCALIZED_TEXT_FIELDS` |

```text
{ bridge_text_t code; bridge_text_t fallback; }
{ IDEATEK_TEXT_SYNTAX_ICU = 0, IDEATEK_TEXT_SYNTAX_FLUENT = 1 }
{ bridge_text_t bytes; bridge_text_t locale; uint64_t domain; uint32_t count; uint32_t revision; ideatek_text_syntax_t syntax; }
{ IDEATEK_TEXT_ARG_INT = 0, IDEATEK_TEXT_ARG_FLOAT = 1, IDEATEK_TEXT_ARG_TEXT = 2 }
{ bridge_text_t args; uint64_t key; uint64_t source_hash; }
{ bridge_text_t text; bridge_text_t locale; bool missing; bool stale; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:ideatek_text_syntax_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_text_arg_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Locale` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:StringTable` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:TextKey` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:LocalizedText` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

## The grants

| Role | `write` | `read` |
| --- | --- | --- |
| The game | `ideatek:Locale`, `ideatek:TextKey` | `ideatek:LocalizedText`, and the types of the fields |
| A loader | `ideatek:StringTable` | The types of the fields |
| The localization module | `ideatek:LocalizedText` | `ideatek:Locale`, `ideatek:StringTable`, `ideatek:TextKey`, and the types of the fields |
