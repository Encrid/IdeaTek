/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef SOCKETS_INSIDE_H
    #define SOCKETS_INSIDE_H

    #include <stddef.h>

    #include <os.h>

    #include "bridge_native.h"
    #include "gns_flat.h"
    #include "ideatek_sockets.h"
    #include "sockets_values.h"

    #define SOCKETS_SETTINGS_FIELDS { \
        uint32_t message_bytes; \
        uint32_t timeout_initial; \
        uint32_t timeout_connected; \
        uint32_t send_buffer; \
        uint32_t recv_buffer; \
        uint32_t recv_buffer_messages; \
        uint32_t recv_max_message; \
        uint32_t nagle_time; \
        uint32_t mtu; \
        bridge_text_t allow_without_auth; \
        bridge_text_t allow_localhost_without_auth; \
        bridge_text_t unencrypted; \
        bridge_text_t stun_servers; \
        bridge_text_t turn_servers; \
        bridge_text_t turn_users; \
        bridge_text_t turn_passwords; \
        bridge_text_t ice_transports; \
        uint32_t ice_penalty; \
        bridge_text_t ice_implementation; \
        uint32_t fake_loss_send; \
        uint32_t fake_loss_recv; \
        uint32_t fake_lag_send; \
        uint32_t fake_lag_recv; \
        uint32_t fake_reorder_send; \
        uint32_t fake_reorder_recv; \
        uint32_t fake_reorder_time; \
        uint32_t fake_duplicate_send; \
        uint32_t fake_duplicate_recv; \
        uint32_t fake_duplicate_time_max; \
        uint32_t fake_jitter_send_avg; \
        uint32_t fake_jitter_recv_avg; \
        uint32_t fake_jitter_send_max; \
        uint32_t fake_jitter_recv_max; \
        uint32_t fake_jitter_send_pct; \
        uint32_t fake_jitter_recv_pct; \
        uint32_t fake_rate_send; \
        uint32_t fake_rate_recv; \
        uint32_t fake_burst_send; \
        uint32_t fake_burst_recv; \
        uint32_t max_connections; \
        uint32_t max_listen_sockets; \
        uint32_t max_addresses; \
        uint32_t session_channels; \
        bridge_text_t debug_output; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(sockets_settings_t, SOCKETS_SETTINGS_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(sockets_settings_t, SOCKETS_SETTINGS_FIELDS);

    #define SOCKETS_TYPE(api, kind, name, T) BRIDGE_NATIVE_DECLARE(api, kind, \
        name, T, BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
    #define SOCKETS_OBSERVER(api, name, slot, events, changes, count, \
        before, before_count) \
        BRIDGE_NATIVE_OBSERVER(api, name, events, (&(bridge_term_t){ slot, \
            BRIDGE_READ, BRIDGE_AND, 0 }), 1, changes, count, before, \
            before_count)

constexpr uint32_t ENTITY_T = 0;
constexpr uint32_t ADDRESS_T = 1;
constexpr uint32_t IDENTITY_T = 2;
constexpr uint32_t SEND_T = 3;
constexpr uint32_t FLAG_T = 4;
constexpr uint32_t LISTEN_KIND_T = 5;
constexpr uint32_t LISTEN_STATUS_T = 6;
constexpr uint32_t ACTION_T = 7;
constexpr uint32_t STATUS_T = 8;
constexpr uint32_t AUTH_T = 9;
constexpr uint32_t SESSION_ACTION_T = 10;
constexpr uint32_t ADDRESS_STATUS_T = 11;
constexpr uint32_t IDENTITY_REQUEST = 12;
constexpr uint32_t IDENTITY_STATE = 13;
constexpr uint32_t LISTEN_REQUEST = 14;
constexpr uint32_t LISTEN_STATE = 15;
constexpr uint32_t CONNECTION_REQUEST = 16;
constexpr uint32_t INBOUND = 17;
constexpr uint32_t CONNECTION_STATE = 18;
constexpr uint32_t LANES_REQUEST = 19;
constexpr uint32_t LANE_STATES = 20;
constexpr uint32_t SESSION = 21;
constexpr uint32_t SESSION_REQUEST = 22;
constexpr uint32_t MESSAGES_OUT = 23;
constexpr uint32_t MESSAGES_IN = 24;
constexpr uint32_t SESSION_MESSAGES_OUT = 25;
constexpr uint32_t SESSION_MESSAGES_IN = 26;
constexpr uint32_t SIGNALS_OUT = 27;
constexpr uint32_t SIGNALS_IN = 28;
constexpr uint32_t ADDRESS_REQUEST = 29;
constexpr uint32_t ADDRESS_RESULT = 30;
constexpr uint32_t SETTINGS = 31;

constexpr uint32_t LIST_IDENTITY = 0;
constexpr uint32_t LIST_LISTEN = 1;
constexpr uint32_t LIST_CONNECTION = 2;
constexpr uint32_t LIST_LANES = 3;
constexpr uint32_t LIST_SESSION = 4;
constexpr uint32_t LIST_ADDRESS = 5;
constexpr uint32_t LIST_MESSAGES_OUT = 6;
constexpr uint32_t LIST_SESSION_MESSAGES_OUT = 7;
constexpr uint32_t LIST_SIGNALS_IN = 8;
constexpr uint32_t LIST_COUNT = 9;

constexpr uint32_t SOCKETS_NAME = 256;
constexpr uint32_t SOCKETS_ADDRESSES = 16;
constexpr uint32_t SOCKETS_SIGNAL_RING = 65536;
constexpr uint32_t SOCKETS_LINE_RING = 16384;
constexpr uint32_t SOCKETS_LINE = 256;
constexpr uint32_t SOCKETS_TEXT = 1024;
constexpr uint32_t SOCKETS_BATCH = 64;
constexpr uint32_t SOCKETS_TEXTS = 9;
constexpr uint32_t SOCKETS_CERTIFICATE = 512;
constexpr uint32_t SOCKETS_SIGNAL = 4096;
constexpr size_t SOCKETS_STACK = 262144;
constexpr uint64_t SOCKETS_WAIT = 100000000;

constexpr uint32_t CHANGE_HEAD = 24;
constexpr uint32_t ON_REQUEST = BRIDGE_ON_ENTER | BRIDGE_ON_ATTACH;

    #define CHANGE_SIZE(n) (CHANGE_HEAD + (((n) + 7u) & ~7u))
    #define CHANGE_BYTES(T) CHANGE_SIZE(sizeof(T))
    #define SOCKETS_NUMBER(field, low, high, config, as_float, range) \
        { offsetof(sockets_settings_t, field), low, high, config, as_float, \
            #field, range }

constexpr uint32_t SOCKETS_FREE = 0;
constexpr uint32_t SOCKETS_WAITING = 1;
constexpr uint32_t SOCKETS_RUNNING = 2;
constexpr uint32_t SOCKETS_DONE = 3;

constexpr uint32_t TEXT_ALLOW = 0;
constexpr uint32_t TEXT_ALLOW_LOCALHOST = 1;
constexpr uint32_t TEXT_UNENCRYPTED = 2;
constexpr uint32_t TEXT_STUN = 3;
constexpr uint32_t TEXT_TURN = 4;
constexpr uint32_t TEXT_TURN_USERS = 5;
constexpr uint32_t TEXT_TURN_PASSWORDS = 6;
constexpr uint32_t TEXT_ICE = 7;
constexpr uint32_t TEXT_ICE_IMPLEMENTATION = 8;

typedef struct {
    uint64_t entity;
    uint32_t handle;
    gns_identity_t peer;
    ideatek_connection_state_t last;
    uint32_t lanes;
    uint32_t failed_sends;
    bool session;
    bool closed;
    bool symmetric;
    bool forgotten;
} sockets_connection_t;

typedef struct {
    uint64_t entity;
    uint32_t handle;
    uint32_t kind;
} sockets_listen_t;

typedef struct {
    uint64_t entity;
    char name[SOCKETS_NAME];
    ideatek_address_t addresses[SOCKETS_ADDRESSES];
    uint32_t count;
    uint32_t status;
    uint32_t state;
} sockets_name_t;

typedef struct {
    uint32_t at;
    uint32_t size;
    uint32_t used;
    uint32_t side;
} sockets_half_t;

typedef struct {
    uint32_t offset;
    uint32_t low;
    uint32_t high;
    int config;
    bool as_float;
    const char *name;
    const char *range;
} sockets_number_t;

typedef struct {
    const bridge_api_t *api;
    bool started;
    bool broken;
    bool stop;

    gns_sockets_t *sockets;
    gns_utils_t *utils;
    gns_messages_t *messages;
    uint64_t self;
    gns_identity_t identity;
    uint32_t certificate_at;
    uint32_t auth_status;

    sockets_connection_t *connections;
    uint32_t connection_max;
    sockets_listen_t *listens;
    uint32_t listen_max;
    sockets_name_t *names;
    uint32_t name_max;
    uint32_t channels;
    char *texts[SOCKETS_TEXTS];
    char *debug_level;

    os_thread_t *thread;
    os_mutex_t *lock;
    os_event_t *wake;

    sockets_half_t half;
    sockets_ring_t signals;
    sockets_ring_t lines;
    uint64_t *gone;
    uint32_t gone_count;
    uint64_t *later;
    uint32_t later_count;
    uint32_t lists[LIST_COUNT];
} sockets_module_t;

extern sockets_module_t sockets_module;
extern const sockets_number_t SOCKETS_NUMBERS[];
extern const uint32_t SOCKETS_NUMBER_COUNT;

void declare_types(const bridge_api_t *api);
void add_runs(const bridge_api_t *api);
bool start_sockets(const bridge_api_t *api);
bool check_numbers(const bridge_api_t *api, const sockets_settings_t *s);
bool copy_texts(const bridge_api_t *api, const sockets_settings_t *s);
bool check_words(const bridge_api_t *api);
void apply_settings(const bridge_api_t *api);
bool start_gns(const gns_identity_t *identity, char *reason);
void attach_identity(const bridge_api_t *api);
void set_certificate(const bridge_api_t *api, bridge_text_t text);

uint32_t find_entity(uint64_t entity);
uint32_t find_peer(const gns_identity_t *peer);
sockets_connection_t *take_slot(uint64_t entity);
void free_slot(sockets_connection_t *c);
void drop_closed(void);

void turn_half(const bridge_api_t *api);
void drop_later(uint64_t entity);
bridge_text_t put_bytes(const bridge_api_t *api, const void *bytes,
    uint32_t len);
uint32_t room_left(void);

void fill_info(ideatek_connection_state_t *s, const gns_info_t *info);
void fill_status(ideatek_connection_state_t *s, const gns_status_t *st);
void fill_state(const bridge_api_t *api, sockets_connection_t *c,
    ideatek_connection_state_t *state);
void attach_state(const bridge_api_t *api, sockets_connection_t *c);
void attach_states(const bridge_api_t *api);
void say_request(const bridge_api_t *api, uint64_t entity, const char *field,
    const char *outside);
bool judge(const bridge_api_t *api, uint64_t entity,
    const ideatek_connection_request_t *r);

void accept_connection(const bridge_api_t *api, uint64_t entity);
void close_connection(const bridge_api_t *api, uint64_t entity,
    const ideatek_connection_request_t *r);
void stop_all(const bridge_api_t *api);
void on_changed(gns_changed_t *changed);
void say_bound(const bridge_api_t *api, const char *what, uint32_t max,
    uint64_t entity);
void attach_problem(const bridge_api_t *api, uint64_t entity,
    const char *text);

void attach_lanes(const bridge_api_t *api);
void receive_connections(const bridge_api_t *api);
void receive_sessions(const bridge_api_t *api);
const ideatek_batch_t *batch_of(const bridge_api_t *api, uint64_t entity,
    uint32_t slot);
void attach_batch(const bridge_api_t *api, uint32_t slot, uint32_t start);
void say_malformed(const bridge_api_t *api, uint64_t entity, uint32_t at);
void send_batch(const bridge_api_t *api, uint64_t entity, bool session);
void send_session_one(const bridge_api_t *api, const sockets_record_t *r);
void say_refused(const bridge_api_t *api, uint64_t entity,
    const char *reason, sockets_connection_t *c);
const char *make_pair(const bridge_api_t *api, sockets_connection_t *c);
bridge_text_t write_lanes(const bridge_api_t *api,
    sockets_connection_t *c);
gns_signaling_t *make_signaling(void);
bool put_signal(uint64_t entity, const gns_identity_t *peer,
    const void *bytes, uint32_t len);
sockets_connection_t *take_session(const bridge_api_t *api, uint64_t entity,
    const gns_identity_t *peer);
void attach_signals(const bridge_api_t *api);
void on_session_request(gns_session_request_t *request);
void on_session_failed(gns_session_failed_t *failed);
gns_signaling_t *create_signaling(gns_sockets_t *sockets,
    const gns_identity_t *peer, int local_port, int remote_port);
void names_loop(void *arg);
void publish_names(const bridge_api_t *api);
void on_output(int level, const char *text);
void say_lines(const bridge_api_t *api);

#endif
