/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "SDL_internal.h"
#include "audio/SDL_sysaudio.h"

struct SDL_PrivateAudioData {
    Uint8 *mixbuf;
    Uint32 io_delay;
    Uint64 frame;
};

typedef struct {
    char key[16];
    bool denied;
    SDL_AudioSpec spec;
} test_handle_t;

typedef struct {
    char *key;
    char *name;
    bool recording;
    bool is_default;
    bool denied;
    SDL_AudioSpec spec;
} test_device_t;

static const char HINT[] = "IDEATEK_TEST_AUDIO";
static const char OUTPUT[] = "IDEATEK_TEST_AUDIO_OUTPUT";
static const char TIMESCALE[] = "IDEATEK_TEST_AUDIO_TIMESCALE";
static const char FALLBACK[] = "a,Speakers,p,1,48000,2;b,Microphone,r,1,48000,1";

static test_handle_t *handles[64];
static int handle_count;
static test_handle_t *defaults[2];
static bool detecting;

static bool read_device(char *text, test_device_t *d)
{
    char *left = NULL;
    char *fields[7] = { NULL };

    for (int i = 0; i < 7; i++) {
        fields[i] = SDL_strtok_r(i ? NULL : text, ",", &left);
        if (!fields[i] && i < 6) {
            return false;
        }
    }
    d->key = fields[0];
    d->name = fields[1];
    d->recording = fields[2][0] == 'r';
    d->is_default = fields[3][0] == '1';
    d->spec.format = SDL_AUDIO_F32;
    d->spec.freq = SDL_atoi(fields[4]);
    d->spec.channels = SDL_atoi(fields[5]);
    d->denied = fields[6] && SDL_strcmp(fields[6], "denied") == 0;
    return true;
}

static test_handle_t *find_handle(const char *key)
{
    for (int i = 0; i < handle_count; i++) {
        if (SDL_strcmp(handles[i]->key, key) == 0) {
            return handles[i];
        }
    }
    return NULL;
}

static bool is_named(const char *text, const char *key)
{
    size_t len = SDL_strlen(key);

    for (const char *at = text; at; at = SDL_strchr(at, ';')) {
        at += (*at == ';');
        if (SDL_strncmp(at, key, len) == 0 && at[len] == ',') {
            return true;
        }
    }
    return false;
}

static void remove_missing(const char *text)
{
    for (int i = handle_count; i--;) {
        if (!is_named(text, handles[i]->key)) {
            SDL_AudioDeviceDisconnected(
                SDL_FindPhysicalAudioDeviceByHandle(handles[i]));
            handles[i] = handles[--handle_count];
        }
    }
}

static void add_device(const test_device_t *d)
{
    test_handle_t *handle = SDL_calloc(1, sizeof(*handle));

    if (!handle || handle_count == 64) {
        SDL_free(handle);
        return;
    }
    SDL_strlcpy(handle->key, d->key, sizeof(handle->key));
    handle->denied = d->denied;
    handle->spec = d->spec;
    if (!SDL_AddAudioDevice(d->recording, d->name, &d->spec, handle)) {
        SDL_free(handle);
        return;
    }
    handles[handle_count++] = handle;
}

static void SDLCALL apply_devices(void *unused, const char *name,
    const char *old, const char *value)
{
    char *copy = SDL_strdup(value && *value ? value : FALLBACK);
    char *left = NULL;
    test_device_t d;

    remove_missing(copy);
    for (char *row = SDL_strtok_r(copy, ";", &left); row;
            row = SDL_strtok_r(NULL, ";", &left)) {
        if (read_device(row, &d) && !find_handle(d.key)) {
            add_device(&d);
        }
    }
    SDL_free(copy);
    copy = SDL_strdup(value && *value ? value : FALLBACK);
    left = NULL;
    for (char *row = SDL_strtok_r(copy, ";", &left); row;
            row = SDL_strtok_r(NULL, ";", &left)) {
        if (read_device(row, &d) && d.is_default && find_handle(d.key)) {
            defaults[d.recording] = find_handle(d.key);
            if (!detecting) {
                SDL_DefaultAudioDeviceChanged(
                    SDL_FindPhysicalAudioDeviceByHandle(defaults[d.recording]));
            }
        }
    }
    SDL_free(copy);
}

static void TEST_DetectDevices(SDL_AudioDevice **default_playback,
    SDL_AudioDevice **default_recording)
{
    detecting = true;
    SDL_AddHintCallback(HINT, apply_devices, NULL);
    detecting = false;
    *default_playback = SDL_FindPhysicalAudioDeviceByHandle(defaults[0]);
    *default_recording = SDL_FindPhysicalAudioDeviceByHandle(defaults[1]);
}

static bool TEST_OpenDevice(SDL_AudioDevice *device)
{
    const test_handle_t *handle = device->handle;
    const char *hint = SDL_GetHint(TIMESCALE);

    if (handle->denied) {
        return SDL_SetError("the test refuses the device");
    }
    device->spec = handle->spec;
    SDL_UpdatedAudioDeviceFormat(device);
    device->hidden = SDL_calloc(1, sizeof(*device->hidden));
    if (!device->hidden) {
        return false;
    }
    if (!device->recording) {
        device->hidden->mixbuf = SDL_malloc(device->buffer_size);
        if (!device->hidden->mixbuf) {
            return false;
        }
    }
    device->hidden->io_delay = (device->sample_frames * 1000)
        / device->spec.freq;
    if (hint && SDL_atof(hint) >= 0.0) {
        device->hidden->io_delay = (Uint32)SDL_round(
            device->hidden->io_delay * SDL_atof(hint));
    }
    return true;
}

static void wait_device(SDL_AudioDevice *device)
{
    Uint32 waited = 0;

    while (waited < device->hidden->io_delay
            && !SDL_GetAtomicInt(&device->shutdown)) {
        SDL_Delay(10);
        waited += 10;
    }
}

static bool TEST_WaitDevice(SDL_AudioDevice *device)
{
    wait_device(device);
    return true;
}

static void TEST_ThreadInit(SDL_AudioDevice *device)
{
    wait_device(device);
}

static bool TEST_PlayDevice(SDL_AudioDevice *device, const Uint8 *buffer,
    int buflen)
{
    const char *path = SDL_GetHint(OUTPUT);
    SDL_IOStream *io = path ? SDL_IOFromFile(path, "ab") : NULL;

    if (io) {
        SDL_WriteIO(io, buffer, (size_t)buflen);
        SDL_CloseIO(io);
    }
    return true;
}

static Uint8 *TEST_GetDeviceBuf(SDL_AudioDevice *device, int *buffer_size)
{
    return device->hidden->mixbuf;
}

static int TEST_RecordDevice(SDL_AudioDevice *device, void *buffer, int buflen)
{
    float *samples = buffer;
    int channels = device->spec.channels;
    int frames = buflen / (channels * (int)sizeof(float));

    for (int f = 0; f < frames; f++) {
        for (int c = 0; c < channels; c++) {
            samples[f * channels + c] = (float)(device->hidden->frame % 100)
                / 100.0f;
        }
        device->hidden->frame++;
    }
    return frames * channels * (int)sizeof(float);
}

static void TEST_CloseDevice(SDL_AudioDevice *device)
{
    if (device->hidden) {
        SDL_free(device->hidden->mixbuf);
        SDL_free(device->hidden);
        device->hidden = NULL;
    }
}

static void TEST_FreeDeviceHandle(SDL_AudioDevice *device)
{
    SDL_free(device->handle);
}

static void TEST_Deinitialize(void)
{
    SDL_RemoveHintCallback(HINT, apply_devices, NULL);
    handle_count = 0;
    defaults[0] = NULL;
    defaults[1] = NULL;
}

static bool TEST_Init(SDL_AudioDriverImpl *impl)
{
    impl->DetectDevices = TEST_DetectDevices;
    impl->OpenDevice = TEST_OpenDevice;
    impl->ThreadInit = TEST_ThreadInit;
    impl->WaitDevice = TEST_WaitDevice;
    impl->PlayDevice = TEST_PlayDevice;
    impl->GetDeviceBuf = TEST_GetDeviceBuf;
    impl->WaitRecordingDevice = TEST_WaitDevice;
    impl->RecordDevice = TEST_RecordDevice;
    impl->CloseDevice = TEST_CloseDevice;
    impl->FreeDeviceHandle = TEST_FreeDeviceHandle;
    impl->Deinitialize = TEST_Deinitialize;
    impl->HasRecordingSupport = true;
    handle_count = 0;
    return true;
}

AudioBootStrap IDEATEK_TEST_AUDIO_bootstrap = {
    "ideatek_test", "The audio driver of the tests of IdeaTek", TEST_Init,
    true, false
};
