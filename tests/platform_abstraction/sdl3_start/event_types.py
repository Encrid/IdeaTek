# Copyright (C) 2026 Enkrid
#
# Under the GNU Affero General Public License, version 3 only: see license.
import re
import subprocess
import sys

TAKEN = {
    'the window module': [
        ('SDL_EVENT_QUIT', 'SDL_EVENT_QUIT'),
        ('SDL_EVENT_DISPLAY_FIRST', 'SDL_EVENT_DISPLAY_LAST'),
        ('SDL_EVENT_WINDOW_FIRST', 'SDL_EVENT_WINDOW_LAST'),
        ('SDL_EVENT_CLIPBOARD_UPDATE', 'SDL_EVENT_CLIPBOARD_UPDATE'),
        ('SDL_EVENT_DROP_FILE', 'SDL_EVENT_DROP_POSITION'),
    ],
    'the input module': [
        ('SDL_EVENT_KEY_DOWN', 'SDL_EVENT_SCREEN_KEYBOARD_HIDDEN'),
        ('SDL_EVENT_MOUSE_MOTION', 'SDL_EVENT_MOUSE_REMOVED'),
        ('SDL_EVENT_JOYSTICK_AXIS_MOTION', 'SDL_EVENT_JOYSTICK_UPDATE_COMPLETE'),
        ('SDL_EVENT_GAMEPAD_AXIS_MOTION', 'SDL_EVENT_GAMEPAD_STEAM_HANDLE_UPDATED'),
        ('SDL_EVENT_FINGER_DOWN', 'SDL_EVENT_FINGER_CANCELED'),
    ],
}
KEPT = {'SDL_EVENT_FIRST', 'SDL_EVENT_POLL_SENTINEL', 'SDL_EVENT_ENUM_PADDING'}


def read_types(header, extra):
    text = open(header, encoding='utf-8').read()
    body = re.search(r'typedef enum SDL_EventType\s*\{(.*?)\}\s*SDL_EventType;', text, re.S).group(1)
    body = re.sub(r'/\*.*?\*/', '', body, flags=re.S)
    types = {}
    value = -1
    for item in body.split(','):
        match = re.match(r'\s*(SDL_EVENT_\w+)\s*(?:=\s*(\w+))?\s*$', item)
        if not match:
            continue
        name, given = match.groups()
        if given is None:
            value += 1
        elif given in types:
            types[name] = types[given]
            continue
        else:
            value = int(given, 0)
        types[name] = value
    for item in extra:
        name, given = item.split('=')
        types[name] = int(given, 0)
    return types


def read_off(probe):
    out = subprocess.run([probe, 'types'], capture_output=True, text=True, check=True).stdout
    ranges = []
    for line in out.splitlines():
        match = re.match(r'off 0x([0-9a-f]+)-0x([0-9a-f]+)$', line)
        if match:
            ranges.append((int(match.group(1), 16), int(match.group(2), 16)))
    return ranges


def main():
    header, probe = sys.argv[1], sys.argv[2]
    types = read_types(header, sys.argv[3:])
    off = read_off(probe)
    taken = [(types[a], types[b]) for ranges in TAKEN.values() for a, b in ranges]
    faults = 0
    for name, value in sorted(types.items(), key=lambda item: item[1]):
        is_off = any(a <= value <= b for a, b in off)
        is_taken = any(a <= value <= b for a, b in taken)
        if name in KEPT or name.endswith('_FIRST') or (name.endswith('_LAST') and name != 'SDL_EVENT_LAST'):
            continue
        if is_off and is_taken:
            print('taken and off: %s' % name)
            faults += 1
        elif not is_off and not is_taken:
            print('not in the list: %s' % name)
            faults += 1
    if faults:
        return 1
    print('each type of SDL_EventType is off or taken: %d types' % len(types))
    return 0


sys.exit(main())
