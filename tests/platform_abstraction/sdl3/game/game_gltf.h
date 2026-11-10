/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef TEST_SDL3_GAME_GLTF_H
    #define TEST_SDL3_GAME_GLTF_H

    #include "game.h"

    #if defined(GAME_GLTF)

    #include "ideatek_animation.h"
    #include "ideatek_scene.h"

    typedef struct {
        uint32_t color;
        uint32_t rgb;
        uint32_t topology;
        uint32_t index_format;
        uint32_t slot;
        uint32_t alpha;
        uint32_t model;
        uint32_t mesh;
        uint32_t material;
        uint32_t clearcoat;
        uint32_t specular;
        uint32_t transmission;
        uint32_t anisotropy;
        uint32_t diffuse;
        uint32_t unlit;
        uint32_t binds;
        uint32_t clip;
    } game_gltf_slots_t;

#else

    typedef struct {
        uint32_t unused;
    } game_gltf_slots_t;

    #endif

    extern game_gltf_slots_t game_gltf_slots;

    bool game_gltf_step(const bridge_api_t *api, const char *verb, char *args);
    void game_declare_gltf(const bridge_api_t *api);
    void game_watch_gltf(const bridge_api_t *api);
#endif
