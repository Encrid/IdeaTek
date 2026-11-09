/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef SOCKETS_VALUES_H
    #define SOCKETS_VALUES_H

    #include <stdint.h>

    #include "ideatek_sockets.h"

constexpr uint32_t RECORD_OUT_HEAD = 15;
constexpr uint32_t RECORD_IN_HEAD = 23;
constexpr uint32_t SESSION_RECORD_HEAD = 17;
constexpr uint32_t SIGNAL_OUT_HEAD = 140;
constexpr uint32_t SIGNAL_IN_HEAD = 4;

constexpr uint32_t ALLOW_COUNT = 3;
constexpr uint32_t UNENCRYPTED_COUNT = 4;
constexpr uint32_t ICE_COUNT = 3;
constexpr uint32_t LOG_COUNT = 9;

typedef struct {
    uint32_t at;
    uint32_t size;
    uint32_t head;
    uint32_t tail;
} sockets_ring_t;

typedef struct {
    uint64_t connection;
    int64_t number;
    int32_t channel;
    uint32_t length;
    uint16_t lane;
    uint8_t flags;
    const uint8_t *bytes;
} sockets_record_t;

extern const char *const ALLOW_WORDS[ALLOW_COUNT];
extern const char *const UNENCRYPTED_WORDS[UNENCRYPTED_COUNT];
extern const char *const ICE_WORDS[ICE_COUNT];
extern const char *const LOG_WORDS[LOG_COUNT];

uint32_t word_number(const char *text, const char *const *words,
    uint32_t count);
int32_t ice_mask(const char *text);
ideatek_connection_status_t status_of(int32_t state);
ideatek_connection_flag_t flags_of(int32_t flags);
int send_of(ideatek_send_t flags);
ideatek_send_t send_back(int flags);

uint32_t read_message(const uint8_t *at, uint32_t left, sockets_record_t *r);
uint32_t read_session_message(const uint8_t *at, uint32_t left,
    sockets_record_t *r);
uint32_t read_signal(const uint8_t *at, uint32_t left, sockets_record_t *r);
uint32_t write_message(uint8_t *at, uint32_t room, const sockets_record_t *r);
uint32_t write_session_message(uint8_t *at, uint32_t room,
    const sockets_record_t *r);

bool ring_put(uint8_t *base, sockets_ring_t *ring, const void *bytes,
    uint32_t len);
uint32_t ring_take(const uint8_t *base, sockets_ring_t *ring,
    const uint8_t **bytes);

#endif
