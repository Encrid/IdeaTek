/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef TEST_SDL3_GAME_SOCKETS_H
    #define TEST_SDL3_GAME_SOCKETS_H

    #include "game.h"

    #if defined(GAME_SOCKETS)

    #include "ideatek_sockets.h"

    typedef struct {
        uint32_t identity_request;
        uint32_t identity_state;
        uint32_t listen_request;
        uint32_t listen_state;
        uint32_t connection_request;
        uint32_t inbound;
        uint32_t connection_state;
        uint32_t lanes_request;
        uint32_t lane_states;
        uint32_t session;
        uint32_t session_request;
        uint32_t messages_out;
        uint32_t messages_in;
        uint32_t session_messages_out;
        uint32_t session_messages_in;
        uint32_t signals_out;
        uint32_t signals_in;
        uint32_t address_request;
        uint32_t address_result;
    } game_sockets_slots_t;

    #define GAME_SOCKS 256

    typedef struct {
        uint64_t entities[GAME_SOCKS];
        uint32_t count;
        uint64_t self;
        char identity[128];
        char folder[512];
        uint32_t sent_signals;
    } game_sockets_t;

    extern game_sockets_t game_sockets;

    uint32_t game_sock_index(uint64_t entity);
    uint32_t game_sock_add(uint64_t entity);
    bridge_text_t game_sock_bytes(const bridge_api_t *api, const void *bytes,
        uint32_t len);
    void game_sock_text(char *out, size_t size, const bridge_api_t *api,
        bridge_text_t text);
    void game_sock_address(char *out, size_t size,
        const ideatek_address_t *a);
    void game_sockets_watch_shows(const bridge_api_t *api);
    void game_sockets_signals_step(const bridge_api_t *api);
    void game_sockets_write_signals(const bridge_api_t *api,
        const ideatek_batch_t *batch);
    bool game_sockets_show_step(const bridge_api_t *api, const char *verb);

#else

    typedef struct {
        uint32_t unused;
    } game_sockets_slots_t;

    #endif

    extern game_sockets_slots_t game_sockets_slots;

    bool game_sockets_step(const bridge_api_t *api, const char *verb,
        char *args);
    void game_declare_sockets(const bridge_api_t *api);
    void game_watch_sockets(const bridge_api_t *api);
#endif
