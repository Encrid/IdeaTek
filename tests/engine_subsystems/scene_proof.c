/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <ideatek_scene.h>

int main(void)
{
    ideatek_model_node_t node = { .parent = IDEATEK_MODEL_NONE };

    if (node.parent != 0xFFFFFFFF)
        return (1);
    return (sizeof(ideatek_model_node_t_desc) > 1 ? 0 : 1);
}
