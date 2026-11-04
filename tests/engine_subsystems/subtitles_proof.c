/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <ideatek_subtitles.h>

int main(void)
{
    ideatek_sound_caption_t value = { .priority = 3 };

    if (value.priority != 3)
        return (1);
    return (sizeof(ideatek_sound_caption_t_desc) > 1 ? 0 : 1);
}
