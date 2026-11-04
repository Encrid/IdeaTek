/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "bridge_native.h"
#include "ideatek_space.h"
#include "ideatek_physics.h"
#include "ideatek_sound.h"

[[gnu::section(MANIFEST_SECTION), gnu::used, gnu::retain]]
static const char WHO[] = MANIFEST;

#define STRUCT(api, name, T) BRIDGE_NATIVE_COMPONENT(api, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define ENUM(api, name, T) BRIDGE_NATIVE_DECLARE(api, BRIDGE_ENUM, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define MASK(api, name, T) BRIDGE_NATIVE_DECLARE(api, BRIDGE_BITMASK, name, \
    T, BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define TAG(api, name) BRIDGE_NATIVE_DECLARE_TAG(api, name, BRIDGE_STRIP, \
    BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)

[[maybe_unused]] static void declare_fields(const bridge_api_t *api)
{
    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    ENUM(api, "ideatek:ideatek_sound_format_t", ideatek_sound_format_t);
    MASK(api, "ideatek:ideatek_sound_flags_t", ideatek_sound_flags_t);
    ENUM(api, "ideatek:ideatek_sound_state_t", ideatek_sound_state_t);
    MASK(api, "ideatek:ideatek_sound_player_flags_t",
        ideatek_sound_player_flags_t);
    ENUM(api, "ideatek:ideatek_attenuation_t", ideatek_attenuation_t);
    MASK(api, "ideatek:ideatek_spatial_flags_t", ideatek_spatial_flags_t);
    MASK(api, "ideatek:ideatek_bus_flags_t", ideatek_bus_flags_t);
    MASK(api, "ideatek:ideatek_effect_flags_t", ideatek_effect_flags_t);
    ENUM(api, "ideatek:ideatek_filter_mode_t", ideatek_filter_mode_t);
    ENUM(api, "ideatek:ideatek_voice_rule_t", ideatek_voice_rule_t);
    MASK(api, "ideatek:ideatek_clock_flags_t", ideatek_clock_flags_t);
    ENUM(api, "ideatek:ideatek_quantum_t", ideatek_quantum_t);
}

[[maybe_unused]] static void declare_late(const bridge_api_t *api)
{
    STRUCT(api, "ideatek:Sound", ideatek_sound_t);
    declare_fields(api);
}

[[maybe_unused]] static void declare_all(const bridge_api_t *api)
{
    declare_fields(api);
    STRUCT(api, "ideatek:Sound", ideatek_sound_t);
    STRUCT(api, "ideatek:SoundPlayer", ideatek_sound_player_t);
    STRUCT(api, "ideatek:SoundPlayback", ideatek_sound_playback_t);
    STRUCT(api, "ideatek:SoundEnded", ideatek_sound_ended_t);
    STRUCT(api, "ideatek:SoundSpatial", ideatek_sound_spatial_t);
    STRUCT(api, "ideatek:SoundListener", ideatek_sound_listener_t);
    STRUCT(api, "ideatek:SoundBus", ideatek_sound_bus_t);
    STRUCT(api, "ideatek:SoundEffect", ideatek_sound_effect_t);
    STRUCT(api, "ideatek:SoundFilter", ideatek_sound_filter_t);
    STRUCT(api, "ideatek:SoundCompressor", ideatek_sound_compressor_t);
    STRUCT(api, "ideatek:SoundLimiter", ideatek_sound_limiter_t);
    STRUCT(api, "ideatek:SoundReverb", ideatek_sound_reverb_t);
    STRUCT(api, "ideatek:SoundDelay", ideatek_sound_delay_t);
    STRUCT(api, "ideatek:SoundZone", ideatek_sound_zone_t);
    STRUCT(api, "ideatek:SoundMaterial", ideatek_sound_material_t);
    STRUCT(api, "ideatek:SoundGroup", ideatek_sound_group_t);
    STRUCT(api, "ideatek:SoundVoice", ideatek_sound_voice_t);
    STRUCT(api, "ideatek:SoundStream", ideatek_sound_stream_t);
    STRUCT(api, "ideatek:SoundFrames", ideatek_audio_frames_t);
    STRUCT(api, "ideatek:SoundStreamState", ideatek_sound_stream_state_t);
    STRUCT(api, "ideatek:SoundClock", ideatek_sound_clock_t);
    STRUCT(api, "ideatek:SoundSync", ideatek_sound_sync_t);
    STRUCT(api, "ideatek:SoundBeat", ideatek_sound_beat_t);
    STRUCT(api, "ideatek:SoundMeter", ideatek_sound_meter_t);
    STRUCT(api, "ideatek:SoundLevel", ideatek_sound_level_t);
    STRUCT(api, "ideatek:SoundBand", ideatek_sound_band_t);
    STRUCT(api, "ideatek:SoundBandLevel", ideatek_sound_band_level_t);
}

[[maybe_unused]] static void role_author(const bridge_api_t *api)
{
    uint64_t id = api->spawn(api);

    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    ENUM(api, "ideatek:ideatek_sound_format_t", ideatek_sound_format_t);
    MASK(api, "ideatek:ideatek_sound_flags_t", ideatek_sound_flags_t);
    ENUM(api, "ideatek:ideatek_sound_state_t", ideatek_sound_state_t);
    MASK(api, "ideatek:ideatek_sound_player_flags_t",
        ideatek_sound_player_flags_t);
    ENUM(api, "ideatek:ideatek_attenuation_t", ideatek_attenuation_t);
    MASK(api, "ideatek:ideatek_spatial_flags_t", ideatek_spatial_flags_t);
    MASK(api, "ideatek:ideatek_bus_flags_t", ideatek_bus_flags_t);
    MASK(api, "ideatek:ideatek_effect_flags_t", ideatek_effect_flags_t);
    ENUM(api, "ideatek:ideatek_filter_mode_t", ideatek_filter_mode_t);
    ENUM(api, "ideatek:ideatek_voice_rule_t", ideatek_voice_rule_t);
    MASK(api, "ideatek:ideatek_clock_flags_t", ideatek_clock_flags_t);
    ENUM(api, "ideatek:ideatek_quantum_t", ideatek_quantum_t);
    api->attach(api, id, STRUCT(api, "ideatek:Sound", ideatek_sound_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:SoundPlayer",
        ideatek_sound_player_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:SoundSpatial",
        ideatek_sound_spatial_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:SoundListener",
        ideatek_sound_listener_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:SoundBus", ideatek_sound_bus_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:SoundEffect",
        ideatek_sound_effect_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:SoundFilter",
        ideatek_sound_filter_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:SoundCompressor",
        ideatek_sound_compressor_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:SoundLimiter",
        ideatek_sound_limiter_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:SoundReverb",
        ideatek_sound_reverb_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:SoundDelay",
        ideatek_sound_delay_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:SoundZone", ideatek_sound_zone_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:SoundMaterial",
        ideatek_sound_material_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:SoundGroup",
        ideatek_sound_group_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:SoundVoice",
        ideatek_sound_voice_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:SoundStream",
        ideatek_sound_stream_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:SoundFrames",
        ideatek_audio_frames_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:SoundClock",
        ideatek_sound_clock_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:SoundSync", ideatek_sound_sync_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:SoundMeter",
        ideatek_sound_meter_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:SoundBand", ideatek_sound_band_t),
        BRIDGE_ZEROED);
}

[[maybe_unused]] static void role_mixer(const bridge_api_t *api)
{
    uint64_t id = api->spawn(api);

    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    STRUCT(api, "ideatek:ideatek_vec3_t", ideatek_vec3_t);
    STRUCT(api, "ideatek:ideatek_position_t", ideatek_position_t);
    ENUM(api, "ideatek:ideatek_shape_kind_t", ideatek_shape_kind_t);
    MASK(api, "ideatek:ideatek_shape_flags_t", ideatek_shape_flags_t);
    ENUM(api, "ideatek:ideatek_sound_format_t", ideatek_sound_format_t);
    MASK(api, "ideatek:ideatek_sound_flags_t", ideatek_sound_flags_t);
    ENUM(api, "ideatek:ideatek_sound_state_t", ideatek_sound_state_t);
    MASK(api, "ideatek:ideatek_sound_player_flags_t",
        ideatek_sound_player_flags_t);
    ENUM(api, "ideatek:ideatek_attenuation_t", ideatek_attenuation_t);
    MASK(api, "ideatek:ideatek_spatial_flags_t", ideatek_spatial_flags_t);
    MASK(api, "ideatek:ideatek_bus_flags_t", ideatek_bus_flags_t);
    MASK(api, "ideatek:ideatek_effect_flags_t", ideatek_effect_flags_t);
    ENUM(api, "ideatek:ideatek_filter_mode_t", ideatek_filter_mode_t);
    ENUM(api, "ideatek:ideatek_voice_rule_t", ideatek_voice_rule_t);
    MASK(api, "ideatek:ideatek_clock_flags_t", ideatek_clock_flags_t);
    ENUM(api, "ideatek:ideatek_quantum_t", ideatek_quantum_t);
    STRUCT(api, "ideatek:Sound", ideatek_sound_t);
    STRUCT(api, "ideatek:SoundPlayer", ideatek_sound_player_t);
    STRUCT(api, "ideatek:SoundSpatial", ideatek_sound_spatial_t);
    STRUCT(api, "ideatek:SoundListener", ideatek_sound_listener_t);
    STRUCT(api, "ideatek:SoundEffect", ideatek_sound_effect_t);
    STRUCT(api, "ideatek:SoundFilter", ideatek_sound_filter_t);
    STRUCT(api, "ideatek:SoundCompressor", ideatek_sound_compressor_t);
    STRUCT(api, "ideatek:SoundLimiter", ideatek_sound_limiter_t);
    STRUCT(api, "ideatek:SoundReverb", ideatek_sound_reverb_t);
    STRUCT(api, "ideatek:SoundDelay", ideatek_sound_delay_t);
    STRUCT(api, "ideatek:SoundZone", ideatek_sound_zone_t);
    STRUCT(api, "ideatek:SoundMaterial", ideatek_sound_material_t);
    STRUCT(api, "ideatek:SoundGroup", ideatek_sound_group_t);
    STRUCT(api, "ideatek:SoundVoice", ideatek_sound_voice_t);
    STRUCT(api, "ideatek:SoundStream", ideatek_sound_stream_t);
    STRUCT(api, "ideatek:SoundFrames", ideatek_audio_frames_t);
    STRUCT(api, "ideatek:SoundClock", ideatek_sound_clock_t);
    STRUCT(api, "ideatek:SoundSync", ideatek_sound_sync_t);
    STRUCT(api, "ideatek:SoundMeter", ideatek_sound_meter_t);
    STRUCT(api, "ideatek:SoundBand", ideatek_sound_band_t);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:WorldTransform",
        ideatek_world_transform_t, BRIDGE_STRIP, BRIDGE_TABLE,
        BRIDGE_ON_DESTROY_REMOVE);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:Shape", ideatek_shape_t, BRIDGE_STRIP,
        BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE);
    api->attach(api, id, STRUCT(api, "ideatek:SoundPlayback",
        ideatek_sound_playback_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:SoundEnded",
        ideatek_sound_ended_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:SoundBus", ideatek_sound_bus_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:SoundStreamState",
        ideatek_sound_stream_state_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:SoundBeat", ideatek_sound_beat_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:SoundLevel",
        ideatek_sound_level_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:SoundBandLevel",
        ideatek_sound_band_level_t), BRIDGE_ZEROED);
}

BRIDGE_NATIVE(init)
{
#if defined(ROLE_LATE)
    declare_late(api);
#elif defined(ROLE_AUTHOR)
    role_author(api);
#elif defined(ROLE_MIXER)
    role_mixer(api);
#else
    declare_all(api);
#endif
    return (0);
}

BRIDGE_NATIVE(shutdown)
{
    return (0);
}

BRIDGE_NATIVE(reload)
{
    return (1);
}
