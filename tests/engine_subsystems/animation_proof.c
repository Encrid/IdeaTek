/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <ideatek_animation.h>

int main(void)
{
    ideatek_clip_play_t value = { .speed = 1.0f, .weight = 1.0f };

    if (value.speed != 1.0f)
        return (1);
    return (sizeof(ideatek_clip_play_t_desc) > 1 ? 0 : 1);
}
