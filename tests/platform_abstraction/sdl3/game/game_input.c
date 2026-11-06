/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "game.h"

#define STRUCT(api, name, T) BRIDGE_NATIVE_COMPONENT(api, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define TYPED(api, kind, name, T) BRIDGE_NATIVE_DECLARE(api, kind, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)

typedef void (*game_print_t)(const bridge_api_t *, uint64_t, const void *);

void game_declare_input(const bridge_api_t *api)
{
    game_input_t *s = &game.input;

    TYPED(api, BRIDGE_ENUM, "ideatek:ideatek_input_event_kind_t", ideatek_input_event_kind_t);
    TYPED(api, BRIDGE_BITMASK, "ideatek:ideatek_modifier_t", ideatek_modifier_t);
    TYPED(api, BRIDGE_ENUM, "ideatek:ideatek_mouse_mode_t", ideatek_mouse_mode_t);
    TYPED(api, BRIDGE_ENUM, "ideatek:ideatek_cursor_t", ideatek_cursor_t);
    TYPED(api, BRIDGE_BITMASK, "ideatek:ideatek_gamepad_button_t", ideatek_gamepad_button_t);
    TYPED(api, BRIDGE_ENUM, "ideatek:ideatek_finger_event_kind_t", ideatek_finger_event_kind_t);
    s->input_event = STRUCT(api, "ideatek:InputEvent", ideatek_input_event_t);
    s->keyboard = STRUCT(api, "ideatek:Keyboard", ideatek_keyboard_t);
    s->key_event = STRUCT(api, "ideatek:KeyEvent", ideatek_key_event_t);
    s->text_request = STRUCT(api, "ideatek:TextRequest", ideatek_text_request_t);
    s->text_event = STRUCT(api, "ideatek:TextEvent", ideatek_text_event_t);
    s->mouse = STRUCT(api, "ideatek:Mouse", ideatek_mouse_t);
    s->mouse_motion = STRUCT(api, "ideatek:MouseMotion", ideatek_mouse_motion_t);
    s->mouse_button = STRUCT(api, "ideatek:MouseButton", ideatek_mouse_button_t);
    s->mouse_wheel = STRUCT(api, "ideatek:MouseWheel", ideatek_mouse_wheel_t);
    s->mouse_request = STRUCT(api, "ideatek:MouseRequest", ideatek_mouse_request_t);
    s->gamepad = STRUCT(api, "ideatek:Gamepad", ideatek_gamepad_t);
    s->gamepad_state = STRUCT(api, "ideatek:GamepadState", ideatek_gamepad_state_t);
    s->gamepad_event = STRUCT(api, "ideatek:GamepadEvent", ideatek_gamepad_event_t);
    s->gamepad_request = STRUCT(api, "ideatek:GamepadRequest", ideatek_gamepad_request_t);
    s->finger_event = STRUCT(api, "ideatek:FingerEvent", ideatek_finger_event_t);
}

static void print_focus(const bridge_api_t *api, uint64_t id, const void *v)
{
    const ideatek_input_event_t *e = v;

    (void)id;
    bridge_native_say(api, BRIDGE_LOG_INFO, "r%u focus %s order %u",
        game.round + 1, e->kind ? "lost" : "gained", e->order);
}

static void print_key(const bridge_api_t *api, uint64_t id, const void *v)
{
    const ideatek_key_event_t *e = v;

    (void)id;
    bridge_native_say(api, BRIDGE_LOG_INFO, "r%u key code 0x%08x key 0x%08x "
        "char %u mods 0x%04x down %d repeat %d order %u", game.round + 1,
        e->code, e->key, e->character, e->modifiers, e->down, e->repeat,
        e->order);
}

static void print_text(const bridge_api_t *api, uint64_t id, const void *v)
{
    const ideatek_text_event_t *e = v;

    (void)id;
    bridge_native_say(api, BRIDGE_LOG_INFO, "r%u text %u order %u",
        game.round + 1, e->code_point, e->order);
}

static void print_motion(const bridge_api_t *api, uint64_t id, const void *v)
{
    const ideatek_mouse_motion_t *e = v;

    (void)id;
    bridge_native_say(api, BRIDGE_LOG_INFO, "r%u motion at %.1f %.1f rel %.1f "
        "%.1f buttons %u order %u", game.round + 1, e->x, e->y, e->rel_x,
        e->rel_y, e->buttons, e->order);
}

static void print_button(const bridge_api_t *api, uint64_t id, const void *v)
{
    const ideatek_mouse_button_t *e = v;

    (void)id;
    bridge_native_say(api, BRIDGE_LOG_INFO, "r%u button %u down %d clicks %u "
        "at %.1f %.1f order %u", game.round + 1, e->button, e->down, e->clicks,
        e->x, e->y, e->order);
}

static void print_wheel(const bridge_api_t *api, uint64_t id, const void *v)
{
    const ideatek_mouse_wheel_t *e = v;

    (void)id;
    bridge_native_say(api, BRIDGE_LOG_INFO, "r%u wheel %.2f %.2f order %u",
        game.round + 1, e->ticks_x, e->ticks_y, e->order);
}

static void print_pad_event(const bridge_api_t *api, uint64_t id,
    const void *v)
{
    const ideatek_gamepad_event_t *e = v;

    (void)id;
    bridge_native_say(api, BRIDGE_LOG_INFO, "r%u pad button 0x%05x down %d "
        "same %d order %u", game.round + 1, e->button, e->down,
        e->gamepad.id == game.pad_entity, e->order);
}

static void print_finger(const bridge_api_t *api, uint64_t id, const void *v)
{
    const ideatek_finger_event_t *e = v;

    (void)id;
    bridge_native_say(api, BRIDGE_LOG_INFO, "r%u finger %llu kind %u at %.1f "
        "%.1f order %u", game.round + 1, (unsigned long long)e->id, e->kind,
        e->x, e->y, e->order);
}

static void print_keyboard(const bridge_api_t *api, uint64_t id,
    const void *v)
{
    const ideatek_keyboard_t *k = v;

    game.input_entity = id;
    bridge_native_say(api, BRIDGE_LOG_INFO, "r%u keyboard %016llx %016llx "
        "mods 0x%04x", game.round + 1, (unsigned long long)k->keys[0],
        (unsigned long long)k->keys[1], k->modifiers);
}

static void print_mouse(const bridge_api_t *api, uint64_t id, const void *v)
{
    const ideatek_mouse_t *m = v;

    game.input_entity = id;
    bridge_native_say(api, BRIDGE_LOG_INFO, "r%u mouse at %.1f %.1f buttons %u",
        game.round + 1, m->x, m->y, m->buttons);
}

static void print_pad_state(const bridge_api_t *api, uint64_t id,
    const void *v)
{
    const ideatek_gamepad_state_t *s = v;

    (void)id;
    bridge_native_say(api, BRIDGE_LOG_INFO, "r%u pad state %.2f %.2f %.2f "
        "%.2f %.2f %.2f buttons 0x%05x", game.round + 1, s->left_x, s->left_y,
        s->right_x, s->right_y, s->left_trigger, s->right_trigger, s->buttons);
}

static void print_pad(const bridge_api_t *api, uint64_t id, const void *v)
{
    game.pad_entity = id;
    bridge_native_say(api, BRIDGE_LOG_INFO, "r%u pad [%s]", game.round + 1,
        ((const ideatek_gamepad_t *)v)->name);
}

static void see(const bridge_api_t *api, uint32_t index, uint32_t slot,
    game_print_t print)
{
    const bridge_list_t *l = bridge_native_get_list(api,
        game.input.lists[index]);
    const bridge_trigger_t *t = (const bridge_trigger_t *)(api->private_base
        + l->at);
    const void *cell;

    for (uint32_t i = 0; i < l->count; i++) {
        cell = game_cell(api, t[i].id, slot);
        if (cell)
            print(api, t[i].id, cell);
        else
            bridge_native_say(api, BRIDGE_LOG_INFO, "r%u gone %llu",
                game.round + 1, (unsigned long long)t[i].id);
    }
}

#define SEEN(name, index, field, print) BRIDGE_NATIVE(name) \
    { see(api, index, game.input.field, print); return (0); }

SEEN(seen_focus, 0, input_event, print_focus)
SEEN(seen_key, 1, key_event, print_key)
SEEN(seen_text, 2, text_event, print_text)
SEEN(seen_motion, 3, mouse_motion, print_motion)
SEEN(seen_button, 4, mouse_button, print_button)
SEEN(seen_wheel, 5, mouse_wheel, print_wheel)
SEEN(seen_pad_event, 6, gamepad_event, print_pad_event)
SEEN(seen_finger, 7, finger_event, print_finger)
SEEN(seen_keyboard, 8, keyboard, print_keyboard)
SEEN(seen_mouse, 9, mouse, print_mouse)
SEEN(seen_pad_state, 10, gamepad_state, print_pad_state)
SEEN(seen_pad, 11, gamepad, print_pad)

#define WATCH(api, name, index, field, events) \
    game.input.lists[index] = BRIDGE_NATIVE_OBSERVER(api, name, events, \
        (&(bridge_term_t){ game.input.field, BRIDGE_READ, BRIDGE_AND, 0 }), \
        1, nullptr, 0, nullptr, 0)

void game_watch_input(const bridge_api_t *api)
{
    uint32_t both = BRIDGE_ON_ENTER | BRIDGE_ON_ATTACH;

    WATCH(api, "seen_focus", 0, input_event, BRIDGE_ON_ENTER);
    WATCH(api, "seen_key", 1, key_event, BRIDGE_ON_ENTER);
    WATCH(api, "seen_text", 2, text_event, BRIDGE_ON_ENTER);
    WATCH(api, "seen_motion", 3, mouse_motion, BRIDGE_ON_ENTER);
    WATCH(api, "seen_button", 4, mouse_button, BRIDGE_ON_ENTER);
    WATCH(api, "seen_wheel", 5, mouse_wheel, BRIDGE_ON_ENTER);
    WATCH(api, "seen_pad_event", 6, gamepad_event, BRIDGE_ON_ENTER);
    WATCH(api, "seen_finger", 7, finger_event, BRIDGE_ON_ENTER);
    WATCH(api, "seen_keyboard", 8, keyboard, both);
    WATCH(api, "seen_mouse", 9, mouse, both);
    WATCH(api, "seen_pad_state", 10, gamepad_state, both);
    WATCH(api, "seen_pad", 11, gamepad, both);
}
