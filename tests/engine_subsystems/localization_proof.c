/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <ideatek_localization.h>

int main(void)
{
    ideatek_text_key_t value = { .key = 7, .source_hash = 0 };

    if (value.key != 7)
        return (1);
    return (sizeof(ideatek_text_key_t_desc) > 1 ? 0 : 1);
}
