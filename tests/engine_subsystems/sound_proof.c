/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <ideatek_sound.h>

int main(void)
{
    ideatek_sound_player_t value = { .volume = 1.0f, .pitch = 1.0f };

    if (value.pitch != 1.0f)
        return (1);
    return (sizeof(ideatek_sound_player_t_desc) > 1 ? 0 : 1);
}
