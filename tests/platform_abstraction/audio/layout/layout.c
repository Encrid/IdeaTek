/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <ideatek_audio.h>

#define SIZE(type) printf("size %zu\nalignment %zu\n", sizeof(type), \
    alignof(type))
#define AT(type, field) printf("%s %zu\n", #field, offsetof(type, field))
#define TEXT_OF(...) BRIDGE_SCHEMA_TEXT(__VA_ARGS__)

typedef struct {
    const char *name;
    void (*show)(void);
} show_mode_t;

static void show_direction(void)
{
    SIZE(ideatek_audio_direction_t);
}

static void show_target(void)
{
    SIZE(ideatek_audio_target_t);
}

static void show_status(void)
{
    SIZE(ideatek_audio_status_t);
}

static void show_device(void)
{
    SIZE(ideatek_audio_device_t);
    AT(ideatek_audio_device_t, name);
    AT(ideatek_audio_device_t, direction);
    AT(ideatek_audio_device_t, is_default);
}

static void show_request(void)
{
    SIZE(ideatek_audio_request_t);
    AT(ideatek_audio_request_t, device);
    AT(ideatek_audio_request_t, rate);
    AT(ideatek_audio_request_t, ahead);
    AT(ideatek_audio_request_t, direction);
    AT(ideatek_audio_request_t, target);
    AT(ideatek_audio_request_t, channels);
}

static void show_frames(void)
{
    SIZE(ideatek_audio_frames_t);
    AT(ideatek_audio_frames_t, at);
    AT(ideatek_audio_frames_t, frames);
    AT(ideatek_audio_frames_t, data);
}

static void show_state(void)
{
    SIZE(ideatek_audio_state_t);
    AT(ideatek_audio_state_t, device);
    AT(ideatek_audio_state_t, dropped);
    AT(ideatek_audio_state_t, rate);
    AT(ideatek_audio_state_t, queued);
    AT(ideatek_audio_state_t, wanted);
    AT(ideatek_audio_state_t, channels);
    AT(ideatek_audio_state_t, status);
}

static void show_descriptors(void)
{
    static const char *const DESCRIPTORS[] = {
        ideatek_audio_direction_t_desc, ideatek_audio_target_t_desc,
        ideatek_audio_status_t_desc, ideatek_audio_device_t_desc,
        ideatek_audio_request_t_desc, ideatek_audio_frames_t_desc,
        ideatek_audio_state_t_desc
    };

    for (size_t i = 0; i < sizeof(DESCRIPTORS) / sizeof(DESCRIPTORS[0]); i++)
        printf("%s\n", DESCRIPTORS[i]);
}

static void show_texts(void)
{
    static const char *const TEXTS[] = {
        TEXT_OF(IDEATEK_AUDIO_DIRECTION_VALUES),
        TEXT_OF(IDEATEK_AUDIO_TARGET_VALUES),
        TEXT_OF(IDEATEK_AUDIO_STATUS_VALUES),
        TEXT_OF(IDEATEK_AUDIO_DEVICE_FIELDS),
        TEXT_OF(IDEATEK_AUDIO_REQUEST_FIELDS),
        TEXT_OF(IDEATEK_AUDIO_FRAMES_FIELDS),
        TEXT_OF(IDEATEK_AUDIO_STATE_FIELDS)
    };

    for (size_t i = 0; i < sizeof(TEXTS) / sizeof(TEXTS[0]); i++)
        printf("%s\n", TEXTS[i]);
}

int main(int argc, char **argv)
{
    static const show_mode_t MODES[] = {
        { "direction", show_direction }, { "target", show_target },
        { "status", show_status }, { "device", show_device },
        { "request", show_request }, { "frames", show_frames },
        { "state", show_state }, { "descriptors", show_descriptors },
        { "texts", show_texts }
    };

    for (size_t i = 0; argc > 1 && i < sizeof(MODES) / sizeof(MODES[0]); i++)
        if (!strcmp(argv[1], MODES[i].name)) {
            MODES[i].show();
            return (0);
        }
    printf("usage: layout <mode>\n");
    return (2);
}
