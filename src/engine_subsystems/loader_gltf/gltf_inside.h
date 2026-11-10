/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef GLTF_INSIDE_H
    #define GLTF_INSIDE_H

    #include "bridge_native.h"
    #include "bridge_native_value.h"
    #include "cgltf.h"
    #include "ideatek_animation.h"
    #include "ideatek_resources.h"
    #include "ideatek_scene.h"
    #include "resources_arena.h"

    #define GLTF_SETTINGS_FIELDS { \
        uint32_t parse_bytes; \
        uint32_t output_bytes; \
        uint32_t max_files; \
        uint32_t max_parts; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(gltf_settings_t, GLTF_SETTINGS_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(gltf_settings_t, GLTF_SETTINGS_FIELDS);

    #define GLTF_TYPE(api, kind, name, T) BRIDGE_NATIVE_DECLARE(api, kind, \
        name, T, BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
    #define GLTF_TAG(api, name) BRIDGE_NATIVE_DECLARE_TAG(api, name, \
        BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)

constexpr uint32_t ENTITY = 0;
constexpr uint32_t RESOURCE = 9;
constexpr uint32_t LOADER = 10;
constexpr uint32_t BYTES = 11;
constexpr uint32_t OUTCOME = 12;
constexpr uint32_t LABEL = 13;
constexpr uint32_t LOAD = 14;
constexpr uint32_t HANDLE = 15;
constexpr uint32_t RAW = 16;
constexpr uint32_t FAILED = 17;
constexpr uint32_t RELOAD = 18;
constexpr uint32_t MODEL = 19;
constexpr uint32_t MESH = 20;
constexpr uint32_t MATERIAL = 21;
constexpr uint32_t CLEARCOAT = 22;
constexpr uint32_t SPECULAR = 23;
constexpr uint32_t TRANSMISSION = 24;
constexpr uint32_t ANISOTROPY = 25;
constexpr uint32_t DIFFUSE = 26;
constexpr uint32_t UNLIT = 27;
constexpr uint32_t BINDS = 28;
constexpr uint32_t CLIP = 29;
constexpr uint32_t SETTINGS = 30;

constexpr uint32_t LIST_BYTES = 0;
constexpr uint32_t LIST_HANDLES = 1;
constexpr uint32_t LIST_RAW = 2;
constexpr uint32_t LIST_FAILED = 3;
constexpr uint32_t LIST_SWAP = 4;
constexpr uint32_t LIST_GONE = 5;

constexpr uint32_t KIND_MESH = 0;
constexpr uint32_t KIND_MATERIAL = 1;
constexpr uint32_t KIND_SKIN = 2;
constexpr uint32_t KIND_CLIP = 3;
constexpr uint32_t KIND_SCENE = 4;
constexpr uint32_t KINDS = 5;

constexpr uint32_t DEP_BUFFER = 0;
constexpr uint32_t DEP_IMAGE = 1;

constexpr uint32_t GLTF_NONE = 0xFFFFFFFF;
constexpr uint32_t MAX_STREAMS = 256;
constexpr uint32_t NAME_ROOM = 256;
constexpr uint32_t PATH_ROOM = 8192;

typedef struct {
    uint64_t entity;
    uint32_t now;
    uint32_t old;
    uint32_t texts;
    uint32_t revision;
    bool waiting;
} gltf_file_t;

typedef struct {
    uint64_t file;
    uint64_t use;
    uint64_t resource;
    uint64_t hash;
    uint32_t kind;
    uint32_t index;
    bool kept;
} gltf_dep_t;

typedef struct {
    uint64_t resource;
    uint32_t key;
    uint32_t at;
    uint32_t size;
} gltf_cache_t;

typedef struct {
    uint64_t file;
    uint64_t entity;
    uint64_t hash;
    uint32_t revision;
    uint32_t kind;
} gltf_part_t;

typedef struct {
    gltf_dep_t *dep;
    const char *path;
    uint32_t path_len;
    const char *settings;
    uint32_t settings_len;
} gltf_new_t;

typedef struct {
    const bridge_api_t *api;
    gltf_file_t *f;
    const char *folder;
    uint32_t folder_len;
    gltf_new_t *news;
    uint32_t count;
    char *room;
    const char *settings;
    uint32_t settings_len;
} gltf_deps_t;

typedef struct {
    uint8_t *base;
    uint32_t at;
} gltf_out_t;

typedef struct {
    uint32_t at;
    uint32_t len;
} gltf_span_t;

typedef struct {
    uint8_t semantic;
    uint8_t set;
    uint16_t target;
    uint32_t comps;
    uint32_t format;
    uint32_t offset;
} gltf_stream_t;

typedef struct {
    gltf_stream_t streams[MAX_STREAMS];
    uint32_t count;
    uint32_t vertices;
    uint32_t indices;
    uint32_t targets;
    uint32_t submeshes;
    uint32_t index_offset;
    uint32_t size;
    uint8_t topology;
    bool wide;
    bool full;
} gltf_plan_t;

typedef struct {
    uint8_t *dst;
    uint32_t at;
    uint32_t base;
    bool wide;
} gltf_emit_t;

typedef struct {
    ideatek_model_node_t *nodes;
    ideatek_light_t *lights;
    ideatek_entity_t *materials;
    float *weights;
    ideatek_projection_t *cameras;
    char *names;
    uint32_t counts[5];
} gltf_tables_t;

typedef struct {
    const bridge_api_t *api;
    cgltf_data *data;
    gltf_file_t *file;
    gltf_out_t out;
    gltf_span_t *spans[KINDS];
    gltf_span_t *labels[KINDS];
    gltf_part_t **parts[KINDS];
    gltf_plan_t *plans;
    float *durations;
    uint32_t *ranks;
    uint32_t *remap;
    uint32_t remap_count;
    uint8_t *temp;
    uint32_t *order;
    uint32_t *where;
    uint32_t *channels;
    ideatek_model_t *models;
    ideatek_model_t file_model;
    gltf_span_t file_span;
    uint32_t joint_count;
    uint32_t start;
    gltf_part_t *fallback;
    gltf_span_t fallback_label;
} gltf_build_t;

typedef struct {
    uint32_t lists[6];
    uint64_t loader;
    bridge_text_t types[2];
    bool started;
    bool broken;
    gltf_settings_t s;
    gltf_file_t *files;
    gltf_dep_t *deps;
    gltf_cache_t *caches;
    gltf_part_t *parts;
    res_blocks_t blocks;
    uint32_t arena_at;
    uint8_t *parse;
    uint32_t parse_used;
    uint32_t serial;
} gltf_module_t;

extern gltf_module_t gltf_module;

const bridge_trigger_t *get_triggers(const bridge_api_t *api,
    uint32_t list, uint32_t *count);

void declare_types(const bridge_api_t *api);
void declare_outputs(const bridge_api_t *api);
void spawn_loader(const bridge_api_t *api);
void add_observers(const bridge_api_t *api);
bool start_module(const bridge_api_t *api);

gltf_file_t *find_file(uint64_t entity);
gltf_file_t *add_file(uint64_t entity);
void forget_file(gltf_file_t *f);
void free_key(uint32_t *key);
bool take_block(uint32_t size, uint32_t *key, uint32_t *start);

void *parse_take(size_t size);
cgltf_result parse_file(const uint8_t *bytes, uint32_t len, cgltf_data **data);
cgltf_options parse_options(void);
const char *name_of_result(cgltf_result r);

uint64_t hash_text(const char *text, size_t len, uint64_t seed);
uint32_t join_path(const char *folder, uint32_t folder_len, const char *uri,
    char *out);

gltf_dep_t *find_dep(uint64_t use);
gltf_dep_t *find_dep_of(uint64_t file, uint32_t kind, size_t index);
bool plan_deps(const bridge_api_t *api, gltf_file_t *f, cgltf_data *data);
bool plan_one(gltf_deps_t *p, uint32_t kind, const char *uri, uint32_t index);
bool spawn_deps(gltf_deps_t *p);
void drop_deps(const bridge_api_t *api, gltf_file_t *f);
uint32_t sampler_settings(const cgltf_data *data, const cgltf_image *image,
    char *out);
bool is_external(const cgltf_image *image);

gltf_cache_t *find_cache(uint64_t resource);
void keep_bytes(const bridge_api_t *api, uint64_t resource);
void free_caches(void);
bool is_used(uint64_t resource);
void wake_users(const bridge_api_t *api, uint64_t resource, const char *rest);
bool is_ready(const gltf_file_t *f);
void try_build(const bridge_api_t *api, gltf_file_t *f);

void refuse_file(const bridge_api_t *api, gltf_file_t *f, const char *rest);
void say_path(const bridge_api_t *api, uint64_t id, const char *rest);
const char *path_of(const bridge_api_t *api, uint64_t id, uint32_t *len);
void say_index(const bridge_api_t *api, uint64_t id, const char *what,
    size_t index);
void give_outcome(const bridge_api_t *api, uint64_t id, bool ok);
void refuse_id(const bridge_api_t *api, uint64_t id, const char *rest);
void wait_for(const bridge_api_t *api, gltf_file_t *f);
void give_materials(gltf_build_t *b);

bool wire_buffers(gltf_build_t *b);
void say_images(gltf_build_t *b);
const cgltf_image *image_of(const cgltf_texture *t);
bool build_file(const bridge_api_t *api, gltf_file_t *f, cgltf_data *data);
bool plan_parts(gltf_build_t *b);
bool take_arrays(gltf_build_t *b);
gltf_part_t *get_part(gltf_build_t *b, uint32_t kind, const char *label,
    uint32_t len);
size_t count_of(const cgltf_data *data, uint32_t kind);
uint32_t label_of(const cgltf_data *data, uint32_t kind, size_t index,
    char *out);

uint32_t out_put(gltf_out_t *o, const void *src, uint32_t len);
void *out_take(gltf_out_t *o, uint32_t len);
void out_align(gltf_out_t *o, uint32_t align);
void write_all(gltf_build_t *b);
void attach_all(gltf_build_t *b);
bridge_text_t text_of(const gltf_build_t *b, gltf_span_t span);

extern const cgltf_material GLTF_DEFAULT;

void rank_joints(gltf_build_t *b);
uint32_t node_index(const cgltf_data *data, const cgltf_node *node);
uint32_t name_of_node(const cgltf_data *data, const cgltf_node *node,
    char *out);
uint64_t target_of(const cgltf_data *data, const cgltf_node *node);

uint8_t topology_of(cgltf_primitive_type type);
bool needs_default(const cgltf_data *data);
bool is_usable(const gltf_plan_t *plan, const cgltf_primitive *p);
bool stream_of(const cgltf_attribute *a, gltf_stream_t *s);
uint32_t stride_of(const gltf_stream_t *s);
void plan_mesh(gltf_build_t *b, size_t mesh);
uint32_t indices_of(const cgltf_primitive *p);
const cgltf_accessor *position_of(const cgltf_primitive *p);
bool is_packed(const cgltf_primitive *p);
void write_mesh(gltf_build_t *b, size_t mesh);
void write_vertices(gltf_build_t *b, size_t mesh, uint8_t *base);
void fill_stream(gltf_build_t *b, const cgltf_primitive *p,
    const gltf_stream_t *s, uint8_t *dst);
void write_joints(gltf_build_t *b, const cgltf_accessor *a, uint16_t *dst);
void remap_of(gltf_build_t *b, size_t mesh);
const cgltf_accessor *accessor_of(const cgltf_primitive *p,
    const gltf_stream_t *s);
void write_indices(gltf_build_t *b, size_t mesh, uint8_t *dst);

void write_skin(gltf_build_t *b, size_t skin);
void write_clip(gltf_build_t *b, size_t clip);
void write_model(gltf_build_t *b, cgltf_node *const *roots, size_t count,
    ideatek_model_t *model);
void count_tables(gltf_build_t *b, uint32_t n, gltf_tables_t *t);
void fill_node(gltf_build_t *b, gltf_tables_t *t, uint32_t i);
void place_node(gltf_build_t *b, const cgltf_node *n,
    ideatek_model_node_t *out);
void split_matrix(const float m[16], ideatek_model_node_t *out);
float weight_of(const cgltf_node *n, uint32_t k);
void fill_light(const cgltf_light *l, ideatek_light_t *out);
void fill_camera(const cgltf_camera *c, ideatek_projection_t *out);
void fill_materials(gltf_build_t *b, gltf_tables_t *t, const cgltf_node *n,
    ideatek_model_node_t *out);

ideatek_texture_slot_t slot_of(gltf_build_t *b, const cgltf_texture_view *v);
void give_material(gltf_build_t *b, uint64_t id, const cgltf_material *m);
void give_extensions(gltf_build_t *b, uint64_t id, const cgltf_material *m);
void give_volume(gltf_build_t *b, uint64_t id, const cgltf_material *m);

#endif
