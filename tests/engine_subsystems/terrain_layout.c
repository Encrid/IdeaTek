/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <ideatek_terrain.h>

#define SIZE(type) printf("size %zu\nalignment %zu\n", sizeof(type), \
    alignof(type))
#define AT(type, field) printf("%s %zu\n", #field, offsetof(type, field))
#define TEXT_OF(...) BRIDGE_SCHEMA_TEXT(__VA_ARGS__)

typedef struct {
    const char *name;
    void (*show)(void);
} show_mode_t;

static void show_terrain_kind(void)
{
    SIZE(ideatek_terrain_kind_t);
}

static void show_terrain_channels(void)
{
    SIZE(ideatek_terrain_channels_t);
}

static void show_terrain_flags(void)
{
    SIZE(ideatek_terrain_flags_t);
}

static void show_terrain(void)
{
    SIZE(ideatek_terrain_t);
    AT(ideatek_terrain_t, heights);
    AT(ideatek_terrain_t, weights);
    AT(ideatek_terrain_t, holes);
    AT(ideatek_terrain_t, cell_size);
    AT(ideatek_terrain_t, height_scale);
    AT(ideatek_terrain_t, chunk_cells);
    AT(ideatek_terrain_t, component_quads);
    AT(ideatek_terrain_t, background_type);
    AT(ideatek_terrain_t, kind);
    AT(ideatek_terrain_t, channels);
    AT(ideatek_terrain_t, flags);
    AT(ideatek_terrain_t, background_solid);
}

static void show_block_shape(void)
{
    SIZE(ideatek_block_shape_t);
}

static void show_block_rotation(void)
{
    SIZE(ideatek_block_rotation_t);
}

static void show_terrain_material(void)
{
    SIZE(ideatek_terrain_material_t);
    AT(ideatek_terrain_material_t, terrain);
    AT(ideatek_terrain_material_t, material);
    AT(ideatek_terrain_material_t, physics_material);
    AT(ideatek_terrain_material_t, model);
    AT(ideatek_terrain_material_t, index);
    AT(ideatek_terrain_material_t, shape);
    AT(ideatek_terrain_material_t, rotation);
}

static void show_terrain_chunk(void)
{
    SIZE(ideatek_terrain_chunk_t);
    AT(ideatek_terrain_chunk_t, terrain);
    AT(ideatek_terrain_chunk_t, bytes);
    AT(ideatek_terrain_chunk_t, x);
    AT(ideatek_terrain_chunk_t, y);
    AT(ideatek_terrain_chunk_t, z);
    AT(ideatek_terrain_chunk_t, revision);
}

static void show_terrain_shape(void)
{
    SIZE(ideatek_terrain_shape_t);
}

static void show_terrain_op(void)
{
    SIZE(ideatek_terrain_op_t);
}

static void show_terrain_edit(void)
{
    SIZE(ideatek_terrain_edit_t);
    AT(ideatek_terrain_edit_t, terrain);
    AT(ideatek_terrain_edit_t, mesh);
    AT(ideatek_terrain_edit_t, center);
    AT(ideatek_terrain_edit_t, end);
    AT(ideatek_terrain_edit_t, size);
    AT(ideatek_terrain_edit_t, strength);
    AT(ideatek_terrain_edit_t, order);
    AT(ideatek_terrain_edit_t, material);
    AT(ideatek_terrain_edit_t, type);
    AT(ideatek_terrain_edit_t, shape);
    AT(ideatek_terrain_edit_t, op);
}

static void show_terrain_fill(void)
{
    SIZE(ideatek_terrain_fill_t);
    AT(ideatek_terrain_fill_t, terrain);
    AT(ideatek_terrain_fill_t, data);
    AT(ideatek_terrain_fill_t, x);
    AT(ideatek_terrain_fill_t, y);
    AT(ideatek_terrain_fill_t, z);
    AT(ideatek_terrain_fill_t, order);
}

static void show_viewer_flags(void)
{
    SIZE(ideatek_viewer_flags_t);
}

static void show_terrain_viewer(void)
{
    SIZE(ideatek_terrain_viewer_t);
    AT(ideatek_terrain_viewer_t, radius);
    AT(ideatek_terrain_viewer_t, vertical_ratio);
    AT(ideatek_terrain_viewer_t, peer);
    AT(ideatek_terrain_viewer_t, flags);
}

static void show_terrain_split(void)
{
    SIZE(ideatek_terrain_split_t);
    AT(ideatek_terrain_split_t, source);
    AT(ideatek_terrain_split_t, piece);
    AT(ideatek_terrain_split_t, order);
}

static void show_terrain_scatter(void)
{
    SIZE(ideatek_terrain_scatter_t);
    AT(ideatek_terrain_scatter_t, terrain);
    AT(ideatek_terrain_scatter_t, mesh);
    AT(ideatek_terrain_scatter_t, density);
    AT(ideatek_terrain_scatter_t, min_scale);
    AT(ideatek_terrain_scatter_t, max_scale);
    AT(ideatek_terrain_scatter_t, seed);
    AT(ideatek_terrain_scatter_t, material);
    AT(ideatek_terrain_scatter_t, align);
}

static void show_descriptors(void)
{
    const char *descriptors[] = {
        ideatek_terrain_kind_t_desc, ideatek_terrain_channels_t_desc,
        ideatek_terrain_flags_t_desc, ideatek_terrain_t_desc,
        ideatek_block_shape_t_desc, ideatek_block_rotation_t_desc,
        ideatek_terrain_material_t_desc, ideatek_terrain_chunk_t_desc,
        ideatek_terrain_shape_t_desc, ideatek_terrain_op_t_desc,
        ideatek_terrain_edit_t_desc, ideatek_terrain_fill_t_desc,
        ideatek_viewer_flags_t_desc, ideatek_terrain_viewer_t_desc,
        ideatek_terrain_split_t_desc, ideatek_terrain_scatter_t_desc };

    for (size_t i = 0; i < sizeof(descriptors) / sizeof(descriptors[0]); i++)
        printf("%s\n", descriptors[i]);
}

static void show_texts(void)
{
    const char *texts[] = {
        TEXT_OF(IDEATEK_TERRAIN_KIND_VALUES),
        TEXT_OF(IDEATEK_TERRAIN_CHANNELS_VALUES),
        TEXT_OF(IDEATEK_TERRAIN_FLAGS_VALUES), TEXT_OF(IDEATEK_TERRAIN_FIELDS),
        TEXT_OF(IDEATEK_BLOCK_SHAPE_VALUES),
        TEXT_OF(IDEATEK_BLOCK_ROTATION_VALUES),
        TEXT_OF(IDEATEK_TERRAIN_MATERIAL_FIELDS),
        TEXT_OF(IDEATEK_TERRAIN_CHUNK_FIELDS),
        TEXT_OF(IDEATEK_TERRAIN_SHAPE_VALUES),
        TEXT_OF(IDEATEK_TERRAIN_OP_VALUES),
        TEXT_OF(IDEATEK_TERRAIN_EDIT_FIELDS),
        TEXT_OF(IDEATEK_TERRAIN_FILL_FIELDS),
        TEXT_OF(IDEATEK_VIEWER_FLAGS_VALUES),
        TEXT_OF(IDEATEK_TERRAIN_VIEWER_FIELDS),
        TEXT_OF(IDEATEK_TERRAIN_SPLIT_FIELDS),
        TEXT_OF(IDEATEK_TERRAIN_SCATTER_FIELDS) };

    for (size_t i = 0; i < sizeof(texts) / sizeof(texts[0]); i++)
        printf("%s\n", texts[i]);
}

int main(int argc, char **argv)
{
    static const show_mode_t MODES[] = {
        { "terrain_kind", show_terrain_kind },
        { "terrain_channels", show_terrain_channels },
        { "terrain_flags", show_terrain_flags }, { "terrain", show_terrain },
        { "block_shape", show_block_shape },
        { "block_rotation", show_block_rotation },
        { "terrain_material", show_terrain_material },
        { "terrain_chunk", show_terrain_chunk },
        { "terrain_shape", show_terrain_shape },
        { "terrain_op", show_terrain_op },
        { "terrain_edit", show_terrain_edit },
        { "terrain_fill", show_terrain_fill },
        { "viewer_flags", show_viewer_flags },
        { "terrain_viewer", show_terrain_viewer },
        { "terrain_split", show_terrain_split },
        { "terrain_scatter", show_terrain_scatter },
        { "descriptors", show_descriptors }, { "texts", show_texts }
    };

    for (size_t i = 0; argc > 1 && i < sizeof(MODES) / sizeof(MODES[0]); i++)
        if (!strcmp(argv[1], MODES[i].name)) {
            MODES[i].show();
            return (0);
        }
    printf("usage: layout <mode>\n");
    return (2);
}
