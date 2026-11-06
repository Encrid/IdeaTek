/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "input_inside.h"

static ideatek_finger_event_kind_t kind_of(uint32_t type)
{
    static const ideatek_finger_event_kind_t KINDS[] = {
        IDEATEK_FINGER_EVENT_KIND_BEGAN, IDEATEK_FINGER_EVENT_KIND_ENDED,
        IDEATEK_FINGER_EVENT_KIND_MOVED, IDEATEK_FINGER_EVENT_KIND_CANCELED,
    };

    return (KINDS[type - SDL_EVENT_FINGER_DOWN]);
}

static void keep_finger(const ideatek_finger_event_t *event)
{
    input_finger_t *list = input_module.fingers;
    uint32_t i = 0;

    while (i < input_module.finger_count && list[i].id != event->id)
        i++;
    if (event->kind == IDEATEK_FINGER_EVENT_KIND_ENDED
        || event->kind == IDEATEK_FINGER_EVENT_KIND_CANCELED) {
        if (i < input_module.finger_count)
            list[i] = list[--input_module.finger_count];
        return;
    }
    if (i == input_module.finger_count && i < INPUT_FINGERS)
        input_module.finger_count++;
    if (i < INPUT_FINGERS)
        list[i] = (input_finger_t){ event->id, event->x, event->y };
}

static void run_finger(const bridge_api_t *api, ideatek_finger_event_t *event)
{
    if (!joins_run(api, FINGER_EVENT, event->id)) {
        start_run(api, FINGER_EVENT, event->id, sizeof(*event));
        event->order = next_order();
        input_module.run.value.finger = *event;
    }
    input_module.run.value.finger.x = event->x;
    input_module.run.value.finger.y = event->y;
}

void take_finger(const bridge_api_t *api, const SDL_Event *e)
{
    const SDL_TouchFingerEvent *f = &e->tfinger;
    SDL_Window *w = SDL_GetWindowFromID(f->windowID);
    int size[2] = { 1, 1 };
    ideatek_finger_event_t event = { .id = f->fingerID,
        .kind = kind_of(f->type) };

    if (w)
        SDL_GetWindowSizeInPixels(w, &size[0], &size[1]);
    event.x = f->x * (float)size[0];
    event.y = f->y * (float)size[1];
    keep_finger(&event);
    if (event.kind == IDEATEK_FINGER_EVENT_KIND_MOVED) {
        run_finger(api, &event);
        return;
    }
    event.order = next_order();
    spawn_event(api, FINGER_EVENT, &event, sizeof(event));
}

void cancel_fingers(const bridge_api_t *api)
{
    ideatek_finger_event_t event = {
        .kind = IDEATEK_FINGER_EVENT_KIND_CANCELED };

    for (uint32_t i = 0; i < input_module.finger_count; i++) {
        event.id = input_module.fingers[i].id;
        event.x = input_module.fingers[i].x;
        event.y = input_module.fingers[i].y;
        event.order = next_order();
        spawn_event(api, FINGER_EVENT, &event, sizeof(event));
    }
    input_module.finger_count = 0;
}
