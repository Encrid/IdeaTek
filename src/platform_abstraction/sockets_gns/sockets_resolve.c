/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#if defined(_WIN32)
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <netdb.h>
    #include <netinet/in.h>
    #include <sys/socket.h>
#endif

#include "sockets_inside.h"

static void put_address(sockets_name_t *n, const struct addrinfo *a)
{
    ideatek_address_t spare = {};
    const struct sockaddr_in *v4 = (const void *)a->ai_addr;
    const struct sockaddr_in6 *v6 = (const void *)a->ai_addr;
    bool is_v4 = a->ai_family == AF_INET;
    bool is_v6 = a->ai_family == AF_INET6;
    bool keep = (n->count < SOCKETS_ADDRESSES) & (is_v4 | is_v6);
    uint32_t at = n->count - (n->count == SOCKETS_ADDRESSES);

    spare.bytes[10] = 0xff * is_v4;
    spare.bytes[11] = 0xff * is_v4;
    memcpy(spare.bytes + 12, &v4->sin_addr, 4 * is_v4);
    memcpy(spare.bytes, &v6->sin6_addr, 16 * is_v6);
    memcpy(&n->addresses[at], &spare, sizeof(spare) * keep);
    n->count += keep;
}

static void resolve(sockets_name_t *n)
{
    struct addrinfo hints = { .ai_family = AF_UNSPEC,
        .ai_socktype = SOCK_DGRAM };
    struct addrinfo *found = nullptr;
    int result = getaddrinfo(n->name, nullptr, &hints, &found);
    bool none = (result == EAI_NONAME) | (result == EAI_AGAIN);
    bool failed = (result != 0) & !none;
    bool ok;

    n->count = 0;
    for (const struct addrinfo *a = found; !result && a; a = a->ai_next)
        put_address(n, a);
    if (found)
        freeaddrinfo(found);
    ok = (n->count != 0) & !failed;
    n->status = ok * IDEATEK_ADDRESS_STATUS_OK
        + failed * IDEATEK_ADDRESS_STATUS_FAILED
        + !(ok | failed) * IDEATEK_ADDRESS_STATUS_NOT_FOUND;
}

static sockets_name_t *pick_name(void)
{
    sockets_name_t *n = sockets_module.names;
    sockets_name_t *picked = nullptr;

    for (uint32_t i = sockets_module.name_max; i > 0; i--)
        picked = n[i - 1].state == SOCKETS_WAITING ? &n[i - 1] : picked;
    if (picked)
        picked->state = SOCKETS_RUNNING;
    return (picked);
}

void names_loop([[maybe_unused]] void *arg)
{
    sockets_name_t *n;
    bool stop = false;

    while (!stop) {
        os_mutex_lock(sockets_module.lock);
        stop = sockets_module.stop;
        n = stop ? nullptr : pick_name();
        os_mutex_unlock(sockets_module.lock);
        if (!n && !stop)
            os_event_wait(sockets_module.wake, SOCKETS_WAIT);
        if (!n)
            continue;
        resolve(n);
        os_mutex_lock(sockets_module.lock);
        n->state = SOCKETS_DONE;
        os_mutex_unlock(sockets_module.lock);
    }
}
