/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "input_inside.h"

static const uint32_t OTHER_PAGES[] = {
    0x000C0032, 0x00010083, 0x000C009C, 0x000C009D, 0x000C00B0, 0x000C00B1,
    0x000C00B2, 0x000C00B3, 0x000C00B4, 0x000C00B5, 0x000C00B6, 0x000C00B7,
    0x000C00B8, 0x000C00CD, 0x000C0087, 0x000C0201, 0x000C0202, 0x000C0203,
    0x000C0204, 0x000C0207, 0x000C0208, 0x000C0209, 0x000C0221, 0x000C0223,
    0x000C0224, 0x000C0225, 0x000C0226, 0x000C0227, 0x000C022A,
};

static const char PUNCTUATION[] = "\r\x1b\b\t -=[]\\";
static const char MORE_PUNCTUATION[] = ";'`,./";

uint32_t usage_of_scancode(SDL_Scancode scancode)
{
    if (scancode >= 4 && scancode <= 231)
        return (0x00070000u | (uint32_t)scancode);
    if (scancode >= SDL_SCANCODE_SLEEP
        && scancode <= SDL_SCANCODE_AC_BOOKMARKS)
        return (OTHER_PAGES[scancode - SDL_SCANCODE_SLEEP]);
    return (0);
}

static uint32_t usage_of_character(uint32_t c)
{
    const char *at = SDL_strchr(PUNCTUATION, (int)c);
    const char *more = SDL_strchr(MORE_PUNCTUATION, (int)c);

    if (c >= 'a' && c <= 'z')
        return (0x00070004u + (c - 'a'));
    if (c >= '1' && c <= '9')
        return (0x0007001Eu + (c - '1'));
    if (c == '0' || c == 0x7F)
        return (c == '0' ? 0x00070027u : 0x0007004Cu);
    if (c && c < 0x80 && at)
        return (0x00070028u + (uint32_t)(at - PUNCTUATION));
    if (c && c < 0x80 && more)
        return (0x00070033u + (uint32_t)(more - MORE_PUNCTUATION));
    return (0);
}

uint32_t usage_of_keycode(SDL_Keycode keycode)
{
    if (keycode & SDLK_SCANCODE_MASK)
        return (usage_of_scancode((SDL_Scancode)(keycode
            & ~SDLK_SCANCODE_MASK)));
    if (keycode & SDLK_EXTENDED_MASK)
        return (0);
    return (usage_of_character(keycode));
}

uint32_t character_of(SDL_Scancode scancode, SDL_Keymod mod)
{
    SDL_Keycode key = SDL_GetKeyFromScancode(scancode, mod, false);

    if (key & (SDLK_SCANCODE_MASK | SDLK_EXTENDED_MASK))
        return (0);
    if (key < 0x20 || key == 0x7F)
        return (0);
    return (key);
}
