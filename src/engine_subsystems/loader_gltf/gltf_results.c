/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "gltf_inside.h"

static const char *const NAMES[] = {
    "success", "the data is too short", "the format is unknown",
    "the JSON is not valid", "the glTF is not valid", "the options are wrong",
    "a file is not found", "a read failed", "no room for the parse",
    "the glTF is of version 1",
};

const char *name_of_result(cgltf_result r)
{
    return (NAMES[(size_t)r]);
}
