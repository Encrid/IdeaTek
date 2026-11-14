/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "render_inside.h"

void answer_capture(const bridge_api_t *api, render_capture_t *c,
    ideatek_capture_status_t status)
{
    bool ok = status == IDEATEK_CAPTURE_STATUS_OK;
    uint32_t bytes = ok * c->size.width * c->size.height * 4;
    bridge_text_t *half = &render_module.halves[render_module.side];
    ideatek_capture_t value = { .pixels = { half->at, bytes },
        .round = c->round, .width = ok * c->size.width,
        .height = ok * c->size.height, .status = status };

    if (ok) {
        memcpy(api->shared_base + half->at,
            render_module.device.mapped[c->slot], bytes);
        render_module.side ^= 1;
    }
    value.pixels.at *= ok;
    BRIDGE_NATIVE_ATTACH(api, c->entity, render_module.slots[CAPTURE], value);
    c->state = CAPTURE_FREE;
}

void finish_captures(const bridge_api_t *api, const ideatek_swapchain_t *swap)
{
    render_capture_t *c = render_module.captures;

    for (uint32_t k = 0; k < RENDER_CAPTURES; k++)
        if (c[k].state == CAPTURE_COPYING && c[k].slot == swap->slot)
            answer_capture(api, &c[k], IDEATEK_CAPTURE_STATUS_OK);
}

void fail_captures(const bridge_api_t *api)
{
    for (uint32_t k = 0; k < RENDER_CAPTURES; k++)
        if (render_module.captures[k].state == CAPTURE_COPYING)
            answer_capture(api, &render_module.captures[k],
                IDEATEK_CAPTURE_STATUS_FAILED);
}

void skip_captures(const bridge_api_t *api)
{
    render_capture_t *c = render_module.captures;

    for (uint32_t k = 0; k < RENDER_CAPTURES; k++) {
        c[k].round = render_module.round;
        if (c[k].state == CAPTURE_WAITING)
            answer_capture(api, &c[k], IDEATEK_CAPTURE_STATUS_NO_TARGET);
    }
}
