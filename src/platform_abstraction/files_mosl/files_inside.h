/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef FILES_INSIDE_H
    #define FILES_INSIDE_H

    #include <os.h>

    #include "bridge_native.h"
    #include "files_paths.h"
    #include "ideatek_files.h"

    #define FILES_SETTINGS_FIELDS { \
        uint32_t read_bytes; \
        bridge_text_t user_folder; \
        bridge_text_t content_folder; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(files_settings_t, FILES_SETTINGS_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(files_settings_t, FILES_SETTINGS_FIELDS);

    #define FILES_TYPE(api, kind, name, T) BRIDGE_NATIVE_DECLARE(api, kind, \
        name, T, BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)

constexpr uint32_t STORAGE = 0;
constexpr uint32_t ACTION = 1;
constexpr uint32_t STATUS = 2;
constexpr uint32_t REQUEST = 3;
constexpr uint32_t RESULT = 4;
constexpr uint32_t CHANGE = 5;
constexpr uint32_t SETTINGS = 6;

constexpr uint32_t FILES_QUEUE = 256;
constexpr uint32_t FILES_EARLY = 256;
constexpr uint32_t FILES_WATCHES = 16;
constexpr uint32_t FILES_DEPTH = 64;
constexpr size_t FILES_STACK = 262144;
constexpr uint64_t FILES_WAIT = 100000000;

constexpr uint32_t CHANGE_HEAD = 24;

    #define CHANGE_SIZE(n) (CHANGE_HEAD + (((n) + 7u) & ~7u))
    #define CHANGE_BYTES(T) CHANGE_SIZE(sizeof(T))

constexpr uint32_t FILES_FREE = 0;
constexpr uint32_t FILES_WAITING = 1;
constexpr uint32_t FILES_RUNNING = 2;
constexpr uint32_t FILES_DONE = 3;
constexpr uint32_t FILES_CANCELED = 4;

typedef struct {
    uint64_t entity;
    ideatek_file_request_t request;
    const uint8_t *bytes;
    char path[FILES_PATH];
    char *data;
    uint32_t data_len;
    uint64_t size;
    uint32_t status;
    uint32_t state;
    uint32_t round;
} files_job_t;

typedef struct {
    uint64_t entity;
    uint32_t status;
} files_early_t;

typedef struct {
    uint64_t entity;
    os_watch_t *watch;
    char path[FILES_PATH];
    bool changed;
} files_watch_t;

typedef struct {
    uint32_t at;
    uint32_t size;
    uint32_t used;
    uint32_t side;
} files_half_t;

typedef uint32_t (*files_action_t)(files_job_t *job, const char *storage);

typedef struct {
    bool started;
    bool broken;
    bool stop;
    bool has_content;
    bool has_user;

    os_thread_t *thread;
    os_mutex_t *lock;
    os_event_t *wake;

    files_job_t jobs[FILES_QUEUE];
    uint32_t done[FILES_QUEUE];
    uint32_t done_count;
    files_early_t early[FILES_EARLY];
    uint32_t early_count;
    files_watch_t watches[FILES_WATCHES];
    uint32_t watch_count;

    files_half_t half;
    uint64_t gone[FILES_QUEUE + FILES_EARLY];
    uint32_t gone_count;

    char *content;
    char *user;
    uint32_t round;
    uint32_t request_list;
} files_module_t;

extern files_module_t files_module;

void declare_types(const bridge_api_t *api);
void add_runs(const bridge_api_t *api);

bool start_files(const bridge_api_t *api);

files_job_t *free_job(void);
void put_early(const bridge_api_t *api, uint64_t entity, uint32_t status);
void cancel_job(uint64_t entity);

void work_loop(void *arg);

uint32_t do_read(files_job_t *job, const char *storage);
uint32_t do_write(files_job_t *job, const char *storage);
uint32_t do_list(files_job_t *job, const char *storage);
uint32_t do_remove(files_job_t *job, const char *storage);

bool has_room(const bridge_api_t *api, uint32_t bytes);
void turn_results(const bridge_api_t *api);
bridge_text_t put_bytes(const bridge_api_t *api, const char *bytes,
    uint32_t len);
void publish(const bridge_api_t *api, uint64_t entity,
    ideatek_file_result_t result);

uint32_t start_watch(uint64_t entity, const char *path);
void stop_watch(uint64_t entity);
void poll_watches(const bridge_api_t *api);

#endif
