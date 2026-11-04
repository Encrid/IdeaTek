/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <ideatek_web.h>

int main(void)
{
    ideatek_web_request_t value = { .max_redirects = 20 };

    if (value.max_redirects != 20)
        return (1);
    return (sizeof(ideatek_web_request_t_desc) > 1 ? 0 : 1);
}
