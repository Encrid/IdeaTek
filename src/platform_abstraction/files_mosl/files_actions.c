/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdlib.h>
#include <string.h>

#include "files_inside.h"

static uint32_t keep_slice(files_job_t *job, const char *bytes, size_t len)
{
    uint64_t offset = job->request.offset;
    uint64_t length = job->request.length;
    bool over = offset > len;
    uint64_t start = over * len + !over * offset;
    uint64_t left = len - start;
    bool cut = (length != 0) & (length < left);
    uint64_t take = cut * length + !cut * left;

    job->size = len;
    if (take > files_module.half.size)
        return (IDEATEK_FILE_STATUS_TOO_BIG);
    job->data = malloc(take + 1);
    if (!job->data)
        return (IDEATEK_FILE_STATUS_FAILED);
    memcpy(job->data, bytes + start, take);
    job->data_len = (uint32_t)take;
    return (IDEATEK_FILE_STATUS_OK);
}

uint32_t do_read(files_job_t *job, const char *storage)
{
    char folder[FILES_PATH];
    char name[FILES_PATH];
    char full[FILES_PATH];
    os_folder_t *f;
    const char *bytes;
    size_t len = 0;
    uint32_t status = IDEATEK_FILE_STATUS_NOT_FOUND;

    path_split(job->path, folder, name);
    if (!path_join(full, sizeof(full), storage, folder))
        return (IDEATEK_FILE_STATUS_DENIED);
    f = os_folder_open(full);
    bytes = f ? os_folder_read_file(f, name, &len) : nullptr;
    if (bytes)
        status = keep_slice(job, bytes, len);
    os_folder_close(f);
    return (status);
}

uint32_t do_write(files_job_t *job, const char *storage)
{
    char full[FILES_PATH];
    char parent[FILES_PATH];
    char name[FILES_PATH];
    char part[FILES_PATH + 8];
    size_t len;
    bool ok;

    if (!path_join(full, sizeof(full), storage, job->path))
        return (IDEATEK_FILE_STATUS_DENIED);
    path_split(full, parent, name);
    len = strlen(full);
    memcpy(part, full, len);
    memcpy(part + len, ".part", 6);
    ok = os_folder_make(parent) == 0
        && os_file_write(part, job->bytes, job->request.data.len, false) == 0
        && os_path_move(part, full) == 0;
    return (ok ? IDEATEK_FILE_STATUS_OK : IDEATEK_FILE_STATUS_FAILED);
}

static uint32_t folder_status(const char *full)
{
    os_folder_t *f = os_folder_open(full);
    bool is_folder;
    bool has;

    if (!f)
        return (IDEATEK_FILE_STATUS_FAILED);
    has = os_folder_read_entry(f, &is_folder) != nullptr;
    os_folder_close(f);
    return (has ? IDEATEK_FILE_STATUS_DENIED : IDEATEK_FILE_STATUS_OK);
}

uint32_t do_remove(files_job_t *job, const char *storage)
{
    char full[FILES_PATH];
    int entry;
    uint32_t status;

    if (!path_join(full, sizeof(full), storage, job->path))
        return (IDEATEK_FILE_STATUS_DENIED);
    entry = os_path_entry(full);
    if (entry <= 0)
        return (entry ? IDEATEK_FILE_STATUS_FAILED
            : IDEATEK_FILE_STATUS_NOT_FOUND);
    status = entry == 3 ? folder_status(full) : IDEATEK_FILE_STATUS_OK;
    if (status != IDEATEK_FILE_STATUS_OK)
        return (status);
    return (os_path_remove(full) == 0 ? IDEATEK_FILE_STATUS_OK
        : IDEATEK_FILE_STATUS_FAILED);
}
