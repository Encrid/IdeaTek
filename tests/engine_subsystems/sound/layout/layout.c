/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <ideatek_sound.h>

#define SIZE(type) printf("size %zu\nalignment %zu\n", sizeof(type), \
    alignof(type))
#define AT(type, field) printf("%s %zu\n", #field, offsetof(type, field))
#define TEXT_OF(...) BRIDGE_SCHEMA_TEXT(__VA_ARGS__)

typedef struct {
    const char *name;
    void (*show)(void);
} show_mode_t;

static void show_sound_format(void)
{
    SIZE(ideatek_sound_format_t);
}

static void show_sound_flags(void)
{
    SIZE(ideatek_sound_flags_t);
}

static void show_sound(void)
{
    SIZE(ideatek_sound_t);
    AT(ideatek_sound_t, bytes);
    AT(ideatek_sound_t, frames);
    AT(ideatek_sound_t, loop_begin);
    AT(ideatek_sound_t, loop_end);
    AT(ideatek_sound_t, rate);
    AT(ideatek_sound_t, revision);
    AT(ideatek_sound_t, channels);
    AT(ideatek_sound_t, format);
    AT(ideatek_sound_t, flags);
}

static void show_sound_state(void)
{
    SIZE(ideatek_sound_state_t);
}

static void show_sound_player_flags(void)
{
    SIZE(ideatek_sound_player_flags_t);
}

static void show_sound_player(void)
{
    SIZE(ideatek_sound_player_t);
    AT(ideatek_sound_player_t, sound);
    AT(ideatek_sound_player_t, bus);
    AT(ideatek_sound_player_t, start_offset);
    AT(ideatek_sound_player_t, volume);
    AT(ideatek_sound_player_t, pitch);
    AT(ideatek_sound_player_t, cue);
    AT(ideatek_sound_player_t, state);
    AT(ideatek_sound_player_t, flags);
}

static void show_sound_playback(void)
{
    SIZE(ideatek_sound_playback_t);
    AT(ideatek_sound_playback_t, position);
    AT(ideatek_sound_playback_t, state);
}

static void show_sound_ended(void)
{
    SIZE(ideatek_sound_ended_t);
    AT(ideatek_sound_ended_t, player);
    AT(ideatek_sound_ended_t, order);
}

static void show_attenuation(void)
{
    SIZE(ideatek_attenuation_t);
}

static void show_spatial_flags(void)
{
    SIZE(ideatek_spatial_flags_t);
}

static void show_sound_spatial(void)
{
    SIZE(ideatek_sound_spatial_t);
    AT(ideatek_sound_spatial_t, reference_distance);
    AT(ideatek_sound_spatial_t, max_distance);
    AT(ideatek_sound_spatial_t, rolloff);
    AT(ideatek_sound_spatial_t, cone_inner);
    AT(ideatek_sound_spatial_t, cone_outer);
    AT(ideatek_sound_spatial_t, cone_outer_gain);
    AT(ideatek_sound_spatial_t, model);
    AT(ideatek_sound_spatial_t, flags);
}

static void show_sound_listener(void)
{
    SIZE(ideatek_sound_listener_t);
    AT(ideatek_sound_listener_t, volume);
}

static void show_bus_flags(void)
{
    SIZE(ideatek_bus_flags_t);
}

static void show_sound_bus(void)
{
    SIZE(ideatek_sound_bus_t);
    AT(ideatek_sound_bus_t, parent);
    AT(ideatek_sound_bus_t, volume);
    AT(ideatek_sound_bus_t, flags);
}

static void show_effect_flags(void)
{
    SIZE(ideatek_effect_flags_t);
}

static void show_sound_effect(void)
{
    SIZE(ideatek_sound_effect_t);
    AT(ideatek_sound_effect_t, bus);
    AT(ideatek_sound_effect_t, order);
    AT(ideatek_sound_effect_t, flags);
}

static void show_filter_mode(void)
{
    SIZE(ideatek_filter_mode_t);
}

static void show_sound_filter(void)
{
    SIZE(ideatek_sound_filter_t);
    AT(ideatek_sound_filter_t, cutoff);
    AT(ideatek_sound_filter_t, resonance);
    AT(ideatek_sound_filter_t, gain);
    AT(ideatek_sound_filter_t, mode);
}

static void show_sound_compressor(void)
{
    SIZE(ideatek_sound_compressor_t);
    AT(ideatek_sound_compressor_t, sidechain);
    AT(ideatek_sound_compressor_t, threshold);
    AT(ideatek_sound_compressor_t, ratio);
    AT(ideatek_sound_compressor_t, attack);
    AT(ideatek_sound_compressor_t, release);
    AT(ideatek_sound_compressor_t, gain);
    AT(ideatek_sound_compressor_t, mix);
}

static void show_sound_limiter(void)
{
    SIZE(ideatek_sound_limiter_t);
    AT(ideatek_sound_limiter_t, ceiling);
    AT(ideatek_sound_limiter_t, release);
}

static void show_sound_reverb(void)
{
    SIZE(ideatek_sound_reverb_t);
    AT(ideatek_sound_reverb_t, predelay);
    AT(ideatek_sound_reverb_t, room_size);
    AT(ideatek_sound_reverb_t, damping);
    AT(ideatek_sound_reverb_t, spread);
    AT(ideatek_sound_reverb_t, dry);
    AT(ideatek_sound_reverb_t, wet);
}

static void show_sound_delay(void)
{
    SIZE(ideatek_sound_delay_t);
    AT(ideatek_sound_delay_t, delay);
    AT(ideatek_sound_delay_t, feedback);
    AT(ideatek_sound_delay_t, dry);
    AT(ideatek_sound_delay_t, wet);
}

static void show_sound_zone(void)
{
    SIZE(ideatek_sound_zone_t);
    AT(ideatek_sound_zone_t, reverb_bus);
    AT(ideatek_sound_zone_t, priority);
    AT(ideatek_sound_zone_t, reverb_send);
    AT(ideatek_sound_zone_t, exterior_volume);
    AT(ideatek_sound_zone_t, exterior_cutoff);
    AT(ideatek_sound_zone_t, interior_volume);
    AT(ideatek_sound_zone_t, interior_cutoff);
    AT(ideatek_sound_zone_t, fade);
}

static void show_sound_material(void)
{
    SIZE(ideatek_sound_material_t);
    AT(ideatek_sound_material_t, absorption_low);
    AT(ideatek_sound_material_t, absorption_mid);
    AT(ideatek_sound_material_t, absorption_high);
    AT(ideatek_sound_material_t, scattering);
    AT(ideatek_sound_material_t, transmission_low);
    AT(ideatek_sound_material_t, transmission_mid);
    AT(ideatek_sound_material_t, transmission_high);
}

static void show_voice_rule(void)
{
    SIZE(ideatek_voice_rule_t);
}

static void show_sound_group(void)
{
    SIZE(ideatek_sound_group_t);
    AT(ideatek_sound_group_t, fade);
    AT(ideatek_sound_group_t, max);
    AT(ideatek_sound_group_t, rule);
}

static void show_sound_voice(void)
{
    SIZE(ideatek_sound_voice_t);
    AT(ideatek_sound_voice_t, group);
    AT(ideatek_sound_voice_t, priority);
}

static void show_sound_stream(void)
{
    SIZE(ideatek_sound_stream_t);
    AT(ideatek_sound_stream_t, rate);
    AT(ideatek_sound_stream_t, ahead);
    AT(ideatek_sound_stream_t, channels);
}

static void show_sound_stream_state(void)
{
    SIZE(ideatek_sound_stream_state_t);
    AT(ideatek_sound_stream_state_t, dropped);
    AT(ideatek_sound_stream_state_t, queued);
    AT(ideatek_sound_stream_state_t, wanted);
}

static void show_clock_flags(void)
{
    SIZE(ideatek_clock_flags_t);
}

static void show_sound_clock(void)
{
    SIZE(ideatek_sound_clock_t);
    AT(ideatek_sound_clock_t, player);
    AT(ideatek_sound_clock_t, offset);
    AT(ideatek_sound_clock_t, bpm);
    AT(ideatek_sound_clock_t, beats_per_bar);
    AT(ideatek_sound_clock_t, flags);
}

static void show_quantum(void)
{
    SIZE(ideatek_quantum_t);
}

static void show_sound_sync(void)
{
    SIZE(ideatek_sound_sync_t);
    AT(ideatek_sound_sync_t, clock);
    AT(ideatek_sound_sync_t, fade_beats);
    AT(ideatek_sound_sync_t, quantum);
}

static void show_sound_beat(void)
{
    SIZE(ideatek_sound_beat_t);
    AT(ideatek_sound_beat_t, clock);
    AT(ideatek_sound_beat_t, beat);
    AT(ideatek_sound_beat_t, bar);
    AT(ideatek_sound_beat_t, order);
}

static void show_sound_meter(void)
{
    SIZE(ideatek_sound_meter_t);
    AT(ideatek_sound_meter_t, bus);
    AT(ideatek_sound_meter_t, attack);
    AT(ideatek_sound_meter_t, release);
}

static void show_sound_level(void)
{
    SIZE(ideatek_sound_level_t);
    AT(ideatek_sound_level_t, rms);
    AT(ideatek_sound_level_t, peak);
    AT(ideatek_sound_level_t, clipped);
}

static void show_sound_band(void)
{
    SIZE(ideatek_sound_band_t);
    AT(ideatek_sound_band_t, meter);
    AT(ideatek_sound_band_t, low);
    AT(ideatek_sound_band_t, high);
}

static void show_sound_band_level(void)
{
    SIZE(ideatek_sound_band_level_t);
    AT(ideatek_sound_band_level_t, magnitude);
}

static void show_descriptors(void)
{
    const char *descriptors[] = {
        ideatek_sound_format_t_desc, ideatek_sound_flags_t_desc,
        ideatek_sound_t_desc, ideatek_sound_state_t_desc,
        ideatek_sound_player_flags_t_desc, ideatek_sound_player_t_desc,
        ideatek_sound_playback_t_desc, ideatek_sound_ended_t_desc,
        ideatek_attenuation_t_desc, ideatek_spatial_flags_t_desc,
        ideatek_sound_spatial_t_desc, ideatek_sound_listener_t_desc,
        ideatek_bus_flags_t_desc, ideatek_sound_bus_t_desc,
        ideatek_effect_flags_t_desc, ideatek_sound_effect_t_desc,
        ideatek_filter_mode_t_desc, ideatek_sound_filter_t_desc,
        ideatek_sound_compressor_t_desc, ideatek_sound_limiter_t_desc,
        ideatek_sound_reverb_t_desc, ideatek_sound_delay_t_desc,
        ideatek_sound_zone_t_desc, ideatek_sound_material_t_desc,
        ideatek_voice_rule_t_desc, ideatek_sound_group_t_desc,
        ideatek_sound_voice_t_desc, ideatek_sound_stream_t_desc,
        ideatek_sound_stream_state_t_desc, ideatek_clock_flags_t_desc,
        ideatek_sound_clock_t_desc, ideatek_quantum_t_desc,
        ideatek_sound_sync_t_desc, ideatek_sound_beat_t_desc,
        ideatek_sound_meter_t_desc, ideatek_sound_level_t_desc,
        ideatek_sound_band_t_desc, ideatek_sound_band_level_t_desc };

    for (size_t i = 0; i < sizeof(descriptors) / sizeof(descriptors[0]); i++)
        printf("%s\n", descriptors[i]);
}

static void show_texts(void)
{
    const char *texts[] = {
        TEXT_OF(IDEATEK_SOUND_FORMAT_VALUES),
        TEXT_OF(IDEATEK_SOUND_FLAGS_VALUES), TEXT_OF(IDEATEK_SOUND_FIELDS),
        TEXT_OF(IDEATEK_SOUND_STATE_VALUES),
        TEXT_OF(IDEATEK_SOUND_PLAYER_FLAGS_VALUES),
        TEXT_OF(IDEATEK_SOUND_PLAYER_FIELDS),
        TEXT_OF(IDEATEK_SOUND_PLAYBACK_FIELDS),
        TEXT_OF(IDEATEK_SOUND_ENDED_FIELDS),
        TEXT_OF(IDEATEK_ATTENUATION_VALUES),
        TEXT_OF(IDEATEK_SPATIAL_FLAGS_VALUES),
        TEXT_OF(IDEATEK_SOUND_SPATIAL_FIELDS),
        TEXT_OF(IDEATEK_SOUND_LISTENER_FIELDS),
        TEXT_OF(IDEATEK_BUS_FLAGS_VALUES), TEXT_OF(IDEATEK_SOUND_BUS_FIELDS),
        TEXT_OF(IDEATEK_EFFECT_FLAGS_VALUES),
        TEXT_OF(IDEATEK_SOUND_EFFECT_FIELDS),
        TEXT_OF(IDEATEK_FILTER_MODE_VALUES),
        TEXT_OF(IDEATEK_SOUND_FILTER_FIELDS),
        TEXT_OF(IDEATEK_SOUND_COMPRESSOR_FIELDS),
        TEXT_OF(IDEATEK_SOUND_LIMITER_FIELDS),
        TEXT_OF(IDEATEK_SOUND_REVERB_FIELDS),
        TEXT_OF(IDEATEK_SOUND_DELAY_FIELDS), TEXT_OF(IDEATEK_SOUND_ZONE_FIELDS),
        TEXT_OF(IDEATEK_SOUND_MATERIAL_FIELDS),
        TEXT_OF(IDEATEK_VOICE_RULE_VALUES), TEXT_OF(IDEATEK_SOUND_GROUP_FIELDS),
        TEXT_OF(IDEATEK_SOUND_VOICE_FIELDS),
        TEXT_OF(IDEATEK_SOUND_STREAM_FIELDS),
        TEXT_OF(IDEATEK_SOUND_STREAM_STATE_FIELDS),
        TEXT_OF(IDEATEK_CLOCK_FLAGS_VALUES),
        TEXT_OF(IDEATEK_SOUND_CLOCK_FIELDS), TEXT_OF(IDEATEK_QUANTUM_VALUES),
        TEXT_OF(IDEATEK_SOUND_SYNC_FIELDS), TEXT_OF(IDEATEK_SOUND_BEAT_FIELDS),
        TEXT_OF(IDEATEK_SOUND_METER_FIELDS),
        TEXT_OF(IDEATEK_SOUND_LEVEL_FIELDS), TEXT_OF(IDEATEK_SOUND_BAND_FIELDS),
        TEXT_OF(IDEATEK_SOUND_BAND_LEVEL_FIELDS) };

    for (size_t i = 0; i < sizeof(texts) / sizeof(texts[0]); i++)
        printf("%s\n", texts[i]);
}

int main(int argc, char **argv)
{
    static const show_mode_t MODES[] = {
        { "sound_format", show_sound_format },
        { "sound_flags", show_sound_flags }, { "sound", show_sound },
        { "sound_state", show_sound_state },
        { "sound_player_flags", show_sound_player_flags },
        { "sound_player", show_sound_player },
        { "sound_playback", show_sound_playback },
        { "sound_ended", show_sound_ended },
        { "attenuation", show_attenuation },
        { "spatial_flags", show_spatial_flags },
        { "sound_spatial", show_sound_spatial },
        { "sound_listener", show_sound_listener },
        { "bus_flags", show_bus_flags }, { "sound_bus", show_sound_bus },
        { "effect_flags", show_effect_flags },
        { "sound_effect", show_sound_effect },
        { "filter_mode", show_filter_mode },
        { "sound_filter", show_sound_filter },
        { "sound_compressor", show_sound_compressor },
        { "sound_limiter", show_sound_limiter },
        { "sound_reverb", show_sound_reverb },
        { "sound_delay", show_sound_delay }, { "sound_zone", show_sound_zone },
        { "sound_material", show_sound_material },
        { "voice_rule", show_voice_rule }, { "sound_group", show_sound_group },
        { "sound_voice", show_sound_voice },
        { "sound_stream", show_sound_stream },
        { "sound_stream_state", show_sound_stream_state },
        { "clock_flags", show_clock_flags },
        { "sound_clock", show_sound_clock }, { "quantum", show_quantum },
        { "sound_sync", show_sound_sync }, { "sound_beat", show_sound_beat },
        { "sound_meter", show_sound_meter },
        { "sound_level", show_sound_level }, { "sound_band", show_sound_band },
        { "sound_band_level", show_sound_band_level },
        { "descriptors", show_descriptors }, { "texts", show_texts }
    };

    for (size_t i = 0; argc > 1 && i < sizeof(MODES) / sizeof(MODES[0]); i++)
        if (!strcmp(argv[1], MODES[i].name)) {
            MODES[i].show();
            return (0);
        }
    printf("usage: layout <mode>\n");
    return (2);
}
