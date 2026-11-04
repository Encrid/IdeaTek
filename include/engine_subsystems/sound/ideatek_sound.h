/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BC_IDEATEK_SOUND_H
    #define BC_IDEATEK_SOUND_H

    #include <stdint.h>

    #include "bridge_abi.h"
    #include "bridge_schema.h"
    #include "ideatek_world.h"
    #include "ideatek_audio.h"

    #define IDEATEK_SOUND_FORMAT_VALUES { \
        IDEATEK_SOUND_FORMAT_PCM_I16 = 0, \
        IDEATEK_SOUND_FORMAT_PCM_F32 = 1, \
        IDEATEK_SOUND_FORMAT_VORBIS = 2, \
        IDEATEK_SOUND_FORMAT_OPUS = 3, \
        IDEATEK_SOUND_FORMAT_FLAC = 4, \
        IDEATEK_SOUND_FORMAT_MP3 = 5 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_sound_format_t,
    uint8_t, IDEATEK_SOUND_FORMAT_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_sound_format_t,
    uint8_t, IDEATEK_SOUND_FORMAT_VALUES);

    #define IDEATEK_SOUND_FLAGS_VALUES { \
        IDEATEK_SOUND_FLAGS_STREAM = 0x01 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_sound_flags_t,
    uint8_t, IDEATEK_SOUND_FLAGS_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_sound_flags_t,
    uint8_t, IDEATEK_SOUND_FLAGS_VALUES);

    #define IDEATEK_SOUND_FIELDS { \
        bridge_text_t bytes; \
        uint64_t frames; \
        uint64_t loop_begin; \
        uint64_t loop_end; \
        uint32_t rate; \
        uint32_t revision; \
        uint8_t channels; \
        ideatek_sound_format_t format; \
        ideatek_sound_flags_t flags; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_sound_t, IDEATEK_SOUND_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_sound_t, IDEATEK_SOUND_FIELDS);

    #define IDEATEK_SOUND_STATE_VALUES { \
        IDEATEK_SOUND_STATE_STOPPED = 0, \
        IDEATEK_SOUND_STATE_PLAYING = 1, \
        IDEATEK_SOUND_STATE_PAUSED = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_sound_state_t,
    uint8_t, IDEATEK_SOUND_STATE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_sound_state_t,
    uint8_t, IDEATEK_SOUND_STATE_VALUES);

    #define IDEATEK_SOUND_PLAYER_FLAGS_VALUES { \
        IDEATEK_SOUND_PLAYER_FLAGS_LOOP = 0x01, \
        IDEATEK_SOUND_PLAYER_FLAGS_DESTROY_AT_END = 0x02 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_sound_player_flags_t,
    uint8_t, IDEATEK_SOUND_PLAYER_FLAGS_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_sound_player_flags_t,
    uint8_t, IDEATEK_SOUND_PLAYER_FLAGS_VALUES);

    #define IDEATEK_SOUND_PLAYER_FIELDS { \
        ideatek_entity_t sound; \
        ideatek_entity_t bus; \
        uint64_t start_offset; \
        float volume; \
        float pitch; \
        uint32_t cue; \
        ideatek_sound_state_t state; \
        ideatek_sound_player_flags_t flags; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_sound_player_t, IDEATEK_SOUND_PLAYER_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_sound_player_t, IDEATEK_SOUND_PLAYER_FIELDS);

    #define IDEATEK_SOUND_PLAYBACK_FIELDS { \
        uint64_t position; \
        ideatek_sound_state_t state; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_sound_playback_t,
    IDEATEK_SOUND_PLAYBACK_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_sound_playback_t,
    IDEATEK_SOUND_PLAYBACK_FIELDS);

    #define IDEATEK_SOUND_ENDED_FIELDS { \
        ideatek_entity_t player; \
        uint32_t order; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_sound_ended_t, IDEATEK_SOUND_ENDED_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_sound_ended_t, IDEATEK_SOUND_ENDED_FIELDS);

    #define IDEATEK_ATTENUATION_VALUES { \
        IDEATEK_ATTENUATION_INVERSE = 0, \
        IDEATEK_ATTENUATION_INVERSE_SQUARE = 1, \
        IDEATEK_ATTENUATION_LINEAR = 2, \
        IDEATEK_ATTENUATION_NONE = 3 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_attenuation_t,
    uint8_t, IDEATEK_ATTENUATION_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_attenuation_t,
    uint8_t, IDEATEK_ATTENUATION_VALUES);

    #define IDEATEK_SPATIAL_FLAGS_VALUES { \
        IDEATEK_SPATIAL_FLAGS_DOPPLER = 0x01, \
        IDEATEK_SPATIAL_FLAGS_AIR_ABSORPTION = 0x02, \
        IDEATEK_SPATIAL_FLAGS_OCCLUSION = 0x04, \
        IDEATEK_SPATIAL_FLAGS_TRANSMISSION = 0x08, \
        IDEATEK_SPATIAL_FLAGS_REFLECTIONS = 0x10, \
        IDEATEK_SPATIAL_FLAGS_PATHING = 0x20 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_spatial_flags_t,
    uint8_t, IDEATEK_SPATIAL_FLAGS_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_spatial_flags_t,
    uint8_t, IDEATEK_SPATIAL_FLAGS_VALUES);

    #define IDEATEK_SOUND_SPATIAL_FIELDS { \
        float reference_distance; \
        float max_distance; \
        float rolloff; \
        float cone_inner; \
        float cone_outer; \
        float cone_outer_gain; \
        ideatek_attenuation_t model; \
        ideatek_spatial_flags_t flags; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_sound_spatial_t,
    IDEATEK_SOUND_SPATIAL_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_sound_spatial_t,
    IDEATEK_SOUND_SPATIAL_FIELDS);

    #define IDEATEK_SOUND_LISTENER_FIELDS { \
        float volume; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_sound_listener_t,
    IDEATEK_SOUND_LISTENER_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_sound_listener_t,
    IDEATEK_SOUND_LISTENER_FIELDS);

    #define IDEATEK_BUS_FLAGS_VALUES { \
        IDEATEK_BUS_FLAGS_MUTE = 0x01, \
        IDEATEK_BUS_FLAGS_BYPASS = 0x02, \
        IDEATEK_BUS_FLAGS_MAIN = 0x04 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_bus_flags_t,
    uint8_t, IDEATEK_BUS_FLAGS_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_bus_flags_t,
    uint8_t, IDEATEK_BUS_FLAGS_VALUES);

    #define IDEATEK_SOUND_BUS_FIELDS { \
        ideatek_entity_t parent; \
        float volume; \
        ideatek_bus_flags_t flags; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_sound_bus_t, IDEATEK_SOUND_BUS_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_sound_bus_t, IDEATEK_SOUND_BUS_FIELDS);

    #define IDEATEK_EFFECT_FLAGS_VALUES { \
        IDEATEK_EFFECT_FLAGS_BYPASS = 0x01 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_effect_flags_t,
    uint8_t, IDEATEK_EFFECT_FLAGS_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_effect_flags_t,
    uint8_t, IDEATEK_EFFECT_FLAGS_VALUES);

    #define IDEATEK_SOUND_EFFECT_FIELDS { \
        ideatek_entity_t bus; \
        uint32_t order; \
        ideatek_effect_flags_t flags; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_sound_effect_t, IDEATEK_SOUND_EFFECT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_sound_effect_t, IDEATEK_SOUND_EFFECT_FIELDS);

    #define IDEATEK_FILTER_MODE_VALUES { \
        IDEATEK_FILTER_MODE_LOW_PASS = 0, \
        IDEATEK_FILTER_MODE_HIGH_PASS = 1, \
        IDEATEK_FILTER_MODE_BAND_PASS = 2, \
        IDEATEK_FILTER_MODE_NOTCH = 3, \
        IDEATEK_FILTER_MODE_PEAK = 4, \
        IDEATEK_FILTER_MODE_LOW_SHELF = 5, \
        IDEATEK_FILTER_MODE_HIGH_SHELF = 6 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_filter_mode_t,
    uint8_t, IDEATEK_FILTER_MODE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_filter_mode_t,
    uint8_t, IDEATEK_FILTER_MODE_VALUES);

    #define IDEATEK_SOUND_FILTER_FIELDS { \
        float cutoff; \
        float resonance; \
        float gain; \
        ideatek_filter_mode_t mode; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_sound_filter_t, IDEATEK_SOUND_FILTER_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_sound_filter_t, IDEATEK_SOUND_FILTER_FIELDS);

    #define IDEATEK_SOUND_COMPRESSOR_FIELDS { \
        ideatek_entity_t sidechain; \
        float threshold; \
        float ratio; \
        float attack; \
        float release; \
        float gain; \
        float mix; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_sound_compressor_t,
    IDEATEK_SOUND_COMPRESSOR_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_sound_compressor_t,
    IDEATEK_SOUND_COMPRESSOR_FIELDS);

    #define IDEATEK_SOUND_LIMITER_FIELDS { \
        float ceiling; \
        float release; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_sound_limiter_t,
    IDEATEK_SOUND_LIMITER_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_sound_limiter_t,
    IDEATEK_SOUND_LIMITER_FIELDS);

    #define IDEATEK_SOUND_REVERB_FIELDS { \
        float predelay; \
        float room_size; \
        float damping; \
        float spread; \
        float dry; \
        float wet; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_sound_reverb_t, IDEATEK_SOUND_REVERB_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_sound_reverb_t, IDEATEK_SOUND_REVERB_FIELDS);

    #define IDEATEK_SOUND_DELAY_FIELDS { \
        float delay; \
        float feedback; \
        float dry; \
        float wet; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_sound_delay_t, IDEATEK_SOUND_DELAY_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_sound_delay_t, IDEATEK_SOUND_DELAY_FIELDS);

    #define IDEATEK_SOUND_ZONE_FIELDS { \
        ideatek_entity_t reverb_bus; \
        int32_t priority; \
        float reverb_send; \
        float exterior_volume; \
        float exterior_cutoff; \
        float interior_volume; \
        float interior_cutoff; \
        float fade; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_sound_zone_t, IDEATEK_SOUND_ZONE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_sound_zone_t, IDEATEK_SOUND_ZONE_FIELDS);

    #define IDEATEK_SOUND_MATERIAL_FIELDS { \
        float absorption_low; \
        float absorption_mid; \
        float absorption_high; \
        float scattering; \
        float transmission_low; \
        float transmission_mid; \
        float transmission_high; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_sound_material_t,
    IDEATEK_SOUND_MATERIAL_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_sound_material_t,
    IDEATEK_SOUND_MATERIAL_FIELDS);

    #define IDEATEK_VOICE_RULE_VALUES { \
        IDEATEK_VOICE_RULE_PREVENT_NEW = 0, \
        IDEATEK_VOICE_RULE_STOP_OLDEST = 1, \
        IDEATEK_VOICE_RULE_STOP_FARTHEST = 2, \
        IDEATEK_VOICE_RULE_STOP_LOWEST_PRIORITY = 3, \
        IDEATEK_VOICE_RULE_STOP_QUIETEST = 4 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_voice_rule_t,
    uint8_t, IDEATEK_VOICE_RULE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_voice_rule_t,
    uint8_t, IDEATEK_VOICE_RULE_VALUES);

    #define IDEATEK_SOUND_GROUP_FIELDS { \
        float fade; \
        uint32_t max; \
        ideatek_voice_rule_t rule; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_sound_group_t, IDEATEK_SOUND_GROUP_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_sound_group_t, IDEATEK_SOUND_GROUP_FIELDS);

    #define IDEATEK_SOUND_VOICE_FIELDS { \
        ideatek_entity_t group; \
        float priority; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_sound_voice_t, IDEATEK_SOUND_VOICE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_sound_voice_t, IDEATEK_SOUND_VOICE_FIELDS);

    #define IDEATEK_SOUND_STREAM_FIELDS { \
        uint32_t rate; \
        uint32_t ahead; \
        uint8_t channels; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_sound_stream_t, IDEATEK_SOUND_STREAM_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_sound_stream_t, IDEATEK_SOUND_STREAM_FIELDS);

    #define IDEATEK_SOUND_STREAM_STATE_FIELDS { \
        uint64_t dropped; \
        uint32_t queued; \
        uint32_t wanted; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_sound_stream_state_t,
    IDEATEK_SOUND_STREAM_STATE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_sound_stream_state_t,
    IDEATEK_SOUND_STREAM_STATE_FIELDS);

    #define IDEATEK_CLOCK_FLAGS_VALUES { \
        IDEATEK_CLOCK_FLAGS_BEAT_EVENTS = 0x01 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_clock_flags_t,
    uint8_t, IDEATEK_CLOCK_FLAGS_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_clock_flags_t,
    uint8_t, IDEATEK_CLOCK_FLAGS_VALUES);

    #define IDEATEK_SOUND_CLOCK_FIELDS { \
        ideatek_entity_t player; \
        uint64_t offset; \
        float bpm; \
        uint32_t beats_per_bar; \
        ideatek_clock_flags_t flags; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_sound_clock_t, IDEATEK_SOUND_CLOCK_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_sound_clock_t, IDEATEK_SOUND_CLOCK_FIELDS);

    #define IDEATEK_QUANTUM_VALUES { \
        IDEATEK_QUANTUM_NOW = 0, \
        IDEATEK_QUANTUM_BEAT = 1, \
        IDEATEK_QUANTUM_BAR = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_quantum_t, uint8_t, IDEATEK_QUANTUM_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_quantum_t, uint8_t, IDEATEK_QUANTUM_VALUES);

    #define IDEATEK_SOUND_SYNC_FIELDS { \
        ideatek_entity_t clock; \
        float fade_beats; \
        ideatek_quantum_t quantum; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_sound_sync_t, IDEATEK_SOUND_SYNC_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_sound_sync_t, IDEATEK_SOUND_SYNC_FIELDS);

    #define IDEATEK_SOUND_BEAT_FIELDS { \
        ideatek_entity_t clock; \
        uint64_t beat; \
        uint64_t bar; \
        uint32_t order; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_sound_beat_t, IDEATEK_SOUND_BEAT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_sound_beat_t, IDEATEK_SOUND_BEAT_FIELDS);

    #define IDEATEK_SOUND_METER_FIELDS { \
        ideatek_entity_t bus; \
        float attack; \
        float release; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_sound_meter_t, IDEATEK_SOUND_METER_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_sound_meter_t, IDEATEK_SOUND_METER_FIELDS);

    #define IDEATEK_SOUND_LEVEL_FIELDS { \
        float rms; \
        float peak; \
        uint32_t clipped; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_sound_level_t, IDEATEK_SOUND_LEVEL_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_sound_level_t, IDEATEK_SOUND_LEVEL_FIELDS);

    #define IDEATEK_SOUND_BAND_FIELDS { \
        ideatek_entity_t meter; \
        float low; \
        float high; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_sound_band_t, IDEATEK_SOUND_BAND_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_sound_band_t, IDEATEK_SOUND_BAND_FIELDS);

    #define IDEATEK_SOUND_BAND_LEVEL_FIELDS { \
        float magnitude; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_sound_band_level_t,
    IDEATEK_SOUND_BAND_LEVEL_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_sound_band_level_t,
    IDEATEK_SOUND_BAND_LEVEL_FIELDS);

#endif
