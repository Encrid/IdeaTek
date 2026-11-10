/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef KTX2_INSIDE_H
    #define KTX2_INSIDE_H

    #include "bridge_native.h"
    #include "bridge_native_value.h"
    #include "ideatek_resources.h"
    #include "ktx2_pure.h"
    #include "resources_arena.h"

    #define KTX2_SETTINGS_FIELDS { \
        uint32_t texture_bytes; \
        uint32_t max_textures; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ktx2_settings_t, KTX2_SETTINGS_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ktx2_settings_t, KTX2_SETTINGS_FIELDS);

    #define KTX2_TYPE(api, kind, name, T) BRIDGE_NATIVE_DECLARE(api, kind, \
        name, T, BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)

constexpr uint32_t ENTITY = 0;
constexpr uint32_t RESOURCE = 6;
constexpr uint32_t LOADER = 7;
constexpr uint32_t BYTES = 8;
constexpr uint32_t OUTCOME = 9;
constexpr uint32_t TEXTURE = 10;
constexpr uint32_t SAMPLER = 11;
constexpr uint32_t SETTINGS = 12;

constexpr uint32_t LIST_BYTES = 0;
constexpr uint32_t LIST_SWAP = 1;
constexpr uint32_t LIST_GONE = 2;

typedef struct {
    uint64_t entity;
    uint32_t now;
    uint32_t old;
    uint32_t revision;
    uint32_t pad;
} ktx2_texture_t;

typedef struct {
    uint32_t lists[3];
    uint64_t loader;
    bool started;
    bool broken;
    ktx2_settings_t s;
    ktx2_texture_t *textures;
    res_blocks_t blocks;
    uint32_t arena_at;
    uint32_t serial;
} ktx2_module_t;

extern ktx2_module_t ktx2_module;

void declare_types(const bridge_api_t *api);
void spawn_loader(const bridge_api_t *api);
void add_observers(const bridge_api_t *api);
bool start_module(const bridge_api_t *api);

ktx2_texture_t *find_texture(uint64_t entity);
ktx2_texture_t *add_texture(uint64_t entity);
void free_block(uint32_t *key);
void forget_texture(ktx2_texture_t *t);

void load_one(const bridge_api_t *api, uint64_t id,
    const ideatek_resource_bytes_t *b);
bool give_texture(const bridge_api_t *api, uint64_t id,
    const ktx2_file_t *f, ktx2_bytes_t file);
void give_sampler(const bridge_api_t *api, uint64_t id,
    const ideatek_resource_bytes_t *b);
void give_outcome(const bridge_api_t *api, uint64_t id, bool ok);

void say_path(const bridge_api_t *api, uint64_t id, const char *rest);

#endif
