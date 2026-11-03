/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <ideatek_files.h>

int main(void)
{
    ideatek_file_request_t request = { .action = IDEATEK_FILE_ACTION_LIST };

    if (request.storage != IDEATEK_STORAGE_CONTENT)
        return (1);
    return (sizeof(ideatek_file_result_t_desc) > 1 ? 0 : 1);
}
