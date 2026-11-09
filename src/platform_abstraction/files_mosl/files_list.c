/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdlib.h>
#include <string.h>

#include "files_inside.h"

static size_t put_number(char *out, uint64_t value)
{
    char digits[20];
    size_t count = 0;

    do {
        digits[count++] = (char)('0' + value % 10);
        value /= 10;
    } while (value);
    for (size_t i = 0; i < count; i++)
        out[i] = digits[count - 1 - i];
    return (count);
}

static uint32_t add_entry(files_job_t *job, os_folder_t *f, const char *name,
    bool is_folder)
{
    char line[FILES_PATH + 32];
    size_t len = strlen(name);
    uint64_t size = 0;
    bool bad = (strchr(name, '\n') != nullptr) | (len >= FILES_PATH);

    if (!is_folder && os_folder_entry_size(f, name, &size) != 0)
        return (IDEATEK_FILE_STATUS_FAILED);
    memcpy(line, name, len);
    line[len] = is_folder ? '/' : ' ';
    len += 1 + !is_folder * put_number(line + len + 1, size);
    line[len++] = '\n';
    len *= !bad;
    if (job->data_len + len > files_module.half.size)
        return (IDEATEK_FILE_STATUS_TOO_BIG);
    memcpy(job->data + job->data_len, line, len);
    job->data_len += (uint32_t)len;
    return (IDEATEK_FILE_STATUS_OK);
}

uint32_t do_list(files_job_t *job, const char *storage)
{
    char full[FILES_PATH];
    os_folder_t *f;
    const char *name;
    bool is_folder = false;
    uint32_t status = IDEATEK_FILE_STATUS_OK;

    if (!path_join(full, sizeof(full), storage, job->path))
        return (IDEATEK_FILE_STATUS_DENIED);
    f = os_folder_open(full);
    if (!f)
        return (IDEATEK_FILE_STATUS_NOT_FOUND);
    job->data = malloc(files_module.half.size + 1);
    name = job->data ? os_folder_read_entry(f, &is_folder) : nullptr;
    status = job->data ? status : IDEATEK_FILE_STATUS_FAILED;
    while (name && status == IDEATEK_FILE_STATUS_OK) {
        status = add_entry(job, f, name, is_folder);
        name = os_folder_read_entry(f, &is_folder);
    }
    os_folder_close(f);
    return (status);
}
