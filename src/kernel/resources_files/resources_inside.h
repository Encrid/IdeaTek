/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef RESOURCES_INSIDE_H
    #define RESOURCES_INSIDE_H

    #include "bridge_native.h"
    #include "ideatek_files.h"
    #include "ideatek_frame.h"
    #include "ideatek_resources.h"
    #include "resources_arena.h"
    #include "resources_fold.h"
    #include "resources_hash.h"
    #include "resources_meta.h"
    #include "resources_paths.h"
    #include "resources_pick.h"

    #define RES_SETTINGS_FIELDS { \
        bool hot_reload; \
        uint32_t reload_delay_ms; \
        bridge_text_t meta_check; \
        bridge_text_t meta_paths; \
        bridge_text_t mode; \
        bridge_text_t processed_folder; \
        bridge_text_t sources; \
        uint32_t load_bytes; \
        uint32_t head_bytes; \
        uint32_t max_resources; \
        uint32_t max_uses; \
        uint32_t max_loaders; \
        uint32_t max_sources; \
        uint32_t max_variables; \
        uint32_t max_reads; \
        uint32_t max_labels; \
        uint32_t path_bytes; \
        uint32_t settings_bytes; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(res_settings_t, RES_SETTINGS_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(res_settings_t, RES_SETTINGS_FIELDS);

    #define RESOURCES_TYPE(api, kind, name, T) BRIDGE_NATIVE_DECLARE(api, \
        kind, name, T, BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
    #define RESOURCES_TAG(api, name) BRIDGE_NATIVE_DECLARE_TAG(api, name, \
        BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
    #define RESOURCES_SINGLE(api, name, T) BRIDGE_NATIVE_DECLARE(api, \
        BRIDGE_STRUCT, name, T, BRIDGE_STRIP, BRIDGE_SINGLETON, \
        BRIDGE_ON_DESTROY_REMOVE)

constexpr uint32_t ENTITY = 0;
constexpr uint32_t STORAGE = 1;
constexpr uint32_t ACTION = 2;
constexpr uint32_t STATUS = 3;
constexpr uint32_t FILE_REQUEST = 4;
constexpr uint32_t FILE_RESULT = 5;
constexpr uint32_t FILE_CHANGE = 6;
constexpr uint32_t FRAME = 7;
constexpr uint32_t LOAD_STATE = 8;
constexpr uint32_t ERROR = 9;
constexpr uint32_t LOAD = 10;
constexpr uint32_t ADD = 11;
constexpr uint32_t HANDLE = 12;
constexpr uint32_t RESOURCE = 13;
constexpr uint32_t LOADER = 14;
constexpr uint32_t BYTES = 15;
constexpr uint32_t READ = 16;
constexpr uint32_t OUTCOME = 17;
constexpr uint32_t LABEL = 18;
constexpr uint32_t RAW = 19;
constexpr uint32_t SOURCE = 20;
constexpr uint32_t SOURCE_READ = 21;
constexpr uint32_t SOURCE_RESULT = 22;
constexpr uint32_t RELOAD = 23;
constexpr uint32_t MODIFIED = 24;
constexpr uint32_t READY = 25;
constexpr uint32_t FAILED = 26;
constexpr uint32_t VARIABLE = 27;
constexpr uint32_t SETTINGS = 28;

constexpr uint32_t RES_DEFAULT = UINT32_MAX - 1;
constexpr uint32_t RES_USER = UINT32_MAX - 2;
constexpr uint32_t RES_RAW = UINT32_MAX - 3;
constexpr uint32_t RES_PASSES = 8;
constexpr uint32_t RES_PATH = 4096;
constexpr uint32_t RES_LISTS = 12;

constexpr uint32_t STEP_NEW = 0;
constexpr uint32_t STEP_META = 1;
constexpr uint32_t STEP_PICK = 2;
constexpr uint32_t STEP_READ = 3;
constexpr uint32_t STEP_READING = 4;
constexpr uint32_t STEP_LOADER = 5;
constexpr uint32_t STEP_DONE = 6;

constexpr uint32_t READ_META = 0;
constexpr uint32_t READ_FILE = 1;
constexpr uint32_t READ_RANGE = 2;

constexpr uint32_t TAG_READY = 1;
constexpr uint32_t TAG_FAILED = 2;
constexpr uint32_t TAG_MODIFIED = 4;
constexpr uint32_t TAG_RAW = 8;

constexpr uint32_t LIST_LOADS = 0;
constexpr uint32_t LIST_ADDS = 1;
constexpr uint32_t LIST_LOADERS = 2;
constexpr uint32_t LIST_SOURCES = 3;
constexpr uint32_t LIST_VARIABLES = 4;
constexpr uint32_t LIST_OUTCOMES = 5;
constexpr uint32_t LIST_LABELS = 6;
constexpr uint32_t LIST_RANGES = 7;
constexpr uint32_t LIST_RELOADS = 8;
constexpr uint32_t LIST_RESULTS = 9;
constexpr uint32_t LIST_CHANGES = 10;
constexpr uint32_t LIST_ANSWERS = 11;

typedef struct {
    uint64_t entity;
    uint32_t source;
    uint32_t loader;
    uint32_t parent;
    uint32_t count;
    uint32_t step;
    uint32_t read;
    uint32_t block_at;
    uint32_t block_len;
    uint32_t path_len;
    uint32_t type_len;
    uint32_t settings_len;
    uint32_t tags;
    uint64_t size;
    uint64_t range_offset;
    uint64_t range_length;
    double reload_at;
    ideatek_resource_t view;
    res_action_t action;
    ideatek_resource_error_t outcome;
    uint8_t priority;
    uint8_t range_priority;
    bool has_outcome;
    bool has_range;
    bool reloading;
    bool reload_asked;
    bool reload_set;
    bool fallback_used;
    bool seen;
    bool fresh;
    bool ready;
    bool changed;
    uint32_t pending;
} res_resource_t;

typedef struct {
    uint64_t entity;
    uint64_t owner;
    uint32_t resource;
    uint8_t priority;
    bool reload_owner;
    bool add;
    bool handled;
} res_use_t;

typedef struct {
    uint64_t entity;
    bool ranged;
} res_loader_t;

typedef struct {
    uint64_t entity;
    uint64_t watch;
    char name[RES_NAME];
    uint32_t name_len;
    uint32_t folder_at;
    uint32_t folder_len;
    bool used;
} res_source_t;

typedef struct {
    uint64_t entity;
} res_variable_t;

typedef struct {
    uint64_t entity;
    uint32_t resource;
    uint32_t kind;
    uint64_t offset;
    uint64_t length;
    ideatek_file_status_t status;
    ideatek_resource_error_t error;
    bool done;
} res_read_t;

typedef struct {
    bool started;
    bool broken;
    bool no_clock;
    bool dirty;
    bool variables_changed;
    bool processed;
    bool found_done;
    uint32_t meta_check;
    res_settings_t s;
    res_resource_t *resources;
    res_use_t *uses;
    res_loader_t *loaders;
    res_source_t *sources;
    res_variable_t *variables;
    res_read_t *reads;
    res_loader_view_t *views;
    uint8_t *texts;
    uint32_t texts_at;
    uint32_t slot_bytes;
    uint32_t arena_at;
    res_map_t by_key;
    res_map_t by_entity;
    res_map_t use_by_entity;
    res_blocks_t blocks;
    res_fold_t *deps;
    res_fold_t *recs;
    uint64_t default_watch;
    uint32_t paths_at;
    double now;
    uint32_t round;
    uint32_t found;
    uint32_t lists[RES_LISTS];
} res_module_t;

typedef struct {
    res_text_t full;
    res_text_t keyed;
    res_text_t type;
    res_text_t settings;
    res_path_t p;
} res_load_t;

typedef struct {
    res_text_t data;
    uint64_t size;
    ideatek_resource_error_t error;
    bool module;
} res_arrival_t;

extern res_module_t res_module;

static inline res_text_t res_text(const bridge_api_t *api, bridge_text_t text)
{
    return ((res_text_t){ (const char *)api->shared_base + text.at,
        text.len });
}

static inline void touch(uint32_t i)
{
    res_module.resources[i].changed = true;
    res_module.dirty = true;
}

void declare_types(const bridge_api_t *api);
void add_runs(const bridge_api_t *api);
bool read_settings(const bridge_api_t *api);
bool start_resources(const bridge_api_t *api);
void read_sources(const bridge_api_t *api);
bridge_text_t folder_of(uint32_t source);
void start_watch(const bridge_api_t *api, uint32_t source);
res_source_t *free_source(void);

char *slot_of(uint32_t i);
res_text_t text_of(uint32_t i, uint32_t part);
uint32_t find_resource(const res_text_t *path, const res_text_t *type);
uint32_t take_resource(const res_text_t *path, const res_text_t *type,
    const res_text_t *settings);
uint32_t resource_of(uint64_t entity);
void bind_resource(uint32_t i, uint64_t entity);
void drop_resource(uint32_t i);

uint32_t use_of(uint64_t entity);
void place_uses(const bridge_api_t *api);
void place_add(const bridge_api_t *api, uint32_t k);
void record_uses(const bridge_api_t *api, uint32_t list, bool add);
void attach_handle(const bridge_api_t *api, uint32_t k, uint64_t resource);
void check_owners(const bridge_api_t *api);
void free_dead(const bridge_api_t *api);
void cancel_read(const bridge_api_t *api, uint32_t i);

uint32_t find_source(const res_text_t *name);
uint32_t find_variable(const bridge_api_t *api, const res_text_t *name);
void add_loader(const bridge_api_t *api, uint64_t id);
void reload_owners(const bridge_api_t *api, uint32_t i);
uint32_t slot_with(const uint64_t *entities, uint32_t count, uint32_t stride,
    uint64_t entity);

bool is_module_source(uint32_t source);
bool write_path(const bridge_api_t *api, uint32_t i, uint32_t kind,
    res_buffer_t *out);
void ask_file(const bridge_api_t *api, const res_read_t *read,
    bridge_text_t path);
void ask_source(const bridge_api_t *api, const res_read_t *read,
    bridge_text_t path);
bool start_read(const bridge_api_t *api, uint32_t i, uint32_t kind);
void start_reads(const bridge_api_t *api);
uint32_t read_of(uint64_t entity);
bool copy_bytes(const bridge_api_t *api, uint32_t i, const res_text_t *data);
void read_arrival(const bridge_api_t *api, uint32_t j, res_arrival_t *out);
void take_results(const bridge_api_t *api);

void fail_resource(const bridge_api_t *api, uint32_t i,
    ideatek_resource_error_t error);
void apply_meta(const bridge_api_t *api, uint32_t i, const res_text_t *bytes,
    ideatek_resource_error_t error);
void pick_for(const bridge_api_t *api, uint32_t i);
uint32_t build_views(const bridge_api_t *api);
void step_resource(const bridge_api_t *api, uint32_t i);
void give_bytes(const bridge_api_t *api, uint32_t i);

void finish_loads(const bridge_api_t *api);
void fold_states(const bridge_api_t *api);
void write_view(const bridge_api_t *api, uint32_t i);
void turn_tags(const bridge_api_t *api);

uint32_t label_record(uint32_t parent, const res_text_t *label);
uint32_t new_label(const bridge_api_t *api, uint32_t parent,
    const res_text_t *label, uint64_t entity);
void place_labels(const bridge_api_t *api);
bool label_handle(const bridge_api_t *api, uint32_t use);
void mark_labels(const bridge_api_t *api, uint32_t parent);

void start_ranges(const bridge_api_t *api);
void give_range(const bridge_api_t *api, uint32_t i,
    const res_arrival_t *in);

void tick_time(const bridge_api_t *api);
void begin_reload(const bridge_api_t *api, uint32_t i);
void start_reloads(const bridge_api_t *api);

#endif
