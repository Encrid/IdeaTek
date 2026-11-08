/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "frame_inside.h"

BRIDGE_NATIVE(step)
{
    ideatek_tick_t *tick = &frame_module.values.tick;
    uint32_t runs = frame_module.run + 1;
    bool next = runs < tick->steps;

    tick->tick += next;
    tick->elapsed += next * tick->step;
    frame_module.run = runs;
    BRIDGE_NATIVE_ATTACH(api, 0, TICK, *tick);
    return (next * BRIDGE_AGAIN);
}
