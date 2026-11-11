/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "cgltf.h"
#include "ktx2_pure.h"

typedef struct {
    uint32_t models;
    uint32_t meshes;
    uint32_t textures;
    uint64_t vertices;
    uint64_t indices;
    uint64_t texels;
} bench_level_t;

static uint8_t *read_all(const char *path, uint64_t *len)
{
    FILE *f = fopen(path, "rb");
    uint8_t *bytes = nullptr;
    long size;

    if (!f)
        return (nullptr);
    fseek(f, 0, SEEK_END);
    size = ftell(f);
    fseek(f, 0, SEEK_SET);
    bytes = size > 0 ? malloc((size_t)size) : nullptr;
    if (bytes && fread(bytes, 1, (size_t)size, f) != (size_t)size) {
        free(bytes);
        bytes = nullptr;
    }
    fclose(f);
    *len = bytes ? (uint64_t)size : 0;
    return (bytes);
}

static void unpack_primitive(const cgltf_primitive *p, bench_level_t *b)
{
    const cgltf_accessor *a;
    cgltf_size n;
    float *floats;
    uint32_t *indices;

    for (cgltf_size k = 0; k < p->attributes_count; k++) {
        a = p->attributes[k].data;
        n = cgltf_accessor_unpack_floats(a, nullptr, 0);
        floats = malloc(n * sizeof(float));
        cgltf_accessor_unpack_floats(a, floats, n);
        b->vertices += p->attributes[k].type == cgltf_attribute_type_position
            ? a->count : 0;
        free(floats);
    }
    if (!p->indices)
        return;
    indices = malloc(p->indices->count * sizeof(uint32_t));
    cgltf_accessor_unpack_indices(p->indices, indices, sizeof(uint32_t),
        p->indices->count);
    b->indices += p->indices->count;
    free(indices);
}

static void read_image(const char *dir, const cgltf_image *image,
    bench_level_t *b)
{
    char path[512];
    ktx2_bytes_t file;
    ktx2_file_t f = {};
    uint8_t *to;

    snprintf(path, sizeof(path), "%s/%s", dir, image->uri);
    file.at = read_all(path, &file.len);
    if (file.at && !ktx2_read(file, &f)) {
        to = malloc(f.texel_bytes);
        ktx2_copy(file, &f, to);
        b->textures++;
        b->texels += f.texel_bytes;
        free(to);
    }
    free((void *)file.at);
}

static void read_level(const char *dir, uint32_t i, bench_level_t *b)
{
    char path[512];
    cgltf_options options = {};
    cgltf_data *data = nullptr;

    snprintf(path, sizeof(path), "%s/level_%03u.gltf", dir, i);
    if (cgltf_parse_file(&options, path, &data) != cgltf_result_success)
        return;
    if (cgltf_load_buffers(&options, data, path) == cgltf_result_success) {
        b->models++;
        for (cgltf_size m = 0; m < data->meshes_count; m++) {
            b->meshes++;
            for (cgltf_size p = 0; p < data->meshes[m].primitives_count; p++)
                unpack_primitive(&data->meshes[m].primitives[p], b);
        }
        for (cgltf_size k = 0; k < data->images_count; k++)
            read_image(dir, &data->images[k], b);
    }
    cgltf_free(data);
}

static const uint8_t IDENTIFIER[12] = { 0xAB, 0x4B, 0x54, 0x58, 0x20, 0x32,
    0x30, 0xBB, 0x0D, 0x0A, 0x1A, 0x0A };

static void put_u32(FILE *f, uint32_t v)
{
    fwrite(&v, 4, 1, f);
}

static void put_u64(FILE *f, uint64_t v)
{
    fwrite(&v, 8, 1, f);
}

static void put_dfd(FILE *f)
{
    const uint8_t basic[16] = { 0, 0, 0, 0, 2, 0, 88, 0, 1, 1, 1, 0, 0, 0, 0,
        0 };
    const uint8_t planes[8] = { 4, 0, 0, 0, 0, 0, 0, 0 };
    const uint8_t channels[4] = { 0, 1, 2, 15 };
    uint8_t sample[8];

    put_u32(f, 92);
    fwrite(basic, 1, sizeof(basic), f);
    fwrite(planes, 1, sizeof(planes), f);
    for (uint32_t c = 0; c < 4; c++) {
        sample[0] = (uint8_t)(c * 8);
        sample[1] = 0;
        sample[2] = 7;
        sample[3] = channels[c];
        memset(sample + 4, 0, 4);
        fwrite(sample, 1, sizeof(sample), f);
        put_u32(f, 0);
        put_u32(f, 255);
    }
}

static void write_ktx2(const char *path, uint32_t side, uint32_t seed)
{
    FILE *f = fopen(path, "wb");
    uint64_t texels = (uint64_t)side * side * 4;
    const uint32_t head[9] = { 37, 1, side, side, 0, 0, 1, 1, 0 };
    uint8_t texel;

    if (!f)
        return;
    fwrite(IDENTIFIER, 1, sizeof(IDENTIFIER), f);
    fwrite(head, 4, 9, f);
    put_u32(f, 104);
    put_u32(f, 92);
    put_u32(f, 0);
    put_u32(f, 0);
    put_u64(f, 0);
    put_u64(f, 0);
    put_u64(f, 196);
    put_u64(f, texels);
    put_u64(f, texels);
    put_dfd(f);
    for (uint64_t k = 0; k < texels; k++) {
        texel = (uint8_t)(((k / 4) % side ^ (k / 4) / side) + seed
            + k % 4 * 64);
        fputc(texel, f);
    }
    fclose(f);
}

static void write_bin(const char *path, uint32_t grid, uint32_t seed)
{
    FILE *f = fopen(path, "wb");
    uint32_t count = grid * grid;
    float v[3];
    uint16_t index[6];

    if (!f)
        return;
    for (uint32_t k = 0; k < count; k++) {
        v[0] = (float)(k % grid + seed);
        v[1] = 0.0f;
        v[2] = (float)(k / grid);
        fwrite(v, sizeof(float), 3, f);
    }
    v[0] = 0.0f;
    v[1] = 1.0f;
    v[2] = 0.0f;
    for (uint32_t k = 0; k < count; k++)
        fwrite(v, sizeof(float), 3, f);
    for (uint32_t k = 0; k < count; k++) {
        v[0] = (float)(k % grid) / (float)(grid - 1);
        v[1] = (float)(k / grid) / (float)(grid - 1);
        fwrite(v, sizeof(float), 2, f);
    }
    for (uint32_t k = 0; k < (grid - 1) * (grid - 1); k++) {
        index[0] = (uint16_t)(k / (grid - 1) * grid + k % (grid - 1));
        index[1] = (uint16_t)(index[0] + grid);
        index[2] = (uint16_t)(index[0] + 1);
        index[3] = index[2];
        index[4] = index[1];
        index[5] = (uint16_t)(index[1] + 1);
        fwrite(index, sizeof(uint16_t), 6, f);
    }
    fclose(f);
}

static void write_gltf(const char *path, uint32_t i, uint32_t grid)
{
    FILE *f = fopen(path, "w");
    uint32_t count = grid * grid;
    uint32_t indices = (grid - 1) * (grid - 1) * 6;
    uint32_t index_bytes = (indices * 2 + 3) / 4 * 4;

    if (!f)
        return;
    fprintf(f, "{\"asset\":{\"version\":\"2.0\"},\"buffers\":[{\"uri\":"
        "\"mesh_%03u.bin\",\"byteLength\":%u}],\"bufferViews\":["
        "{\"buffer\":0,\"byteOffset\":0,\"byteLength\":%u,\"target\":34962},"
        "{\"buffer\":0,\"byteOffset\":%u,\"byteLength\":%u,\"target\":34962},"
        "{\"buffer\":0,\"byteOffset\":%u,\"byteLength\":%u,\"target\":34962},"
        "{\"buffer\":0,\"byteOffset\":%u,\"byteLength\":%u,\"target\":34963}"
        "],", i, count * 32 + index_bytes, count * 12, count * 12,
        count * 12, count * 24, count * 8, count * 32, indices * 2);
    fprintf(f, "\"accessors\":[{\"bufferView\":0,\"componentType\":5126,"
        "\"count\":%u,\"type\":\"VEC3\",\"min\":[%u,0,0],\"max\":[%u,0,%u]},"
        "{\"bufferView\":1,\"componentType\":5126,\"count\":%u,\"type\":"
        "\"VEC3\"},{\"bufferView\":2,\"componentType\":5126,\"count\":%u,"
        "\"type\":\"VEC2\"},{\"bufferView\":3,\"componentType\":5123,"
        "\"count\":%u,\"type\":\"SCALAR\"}],", count, i, i + grid - 1,
        grid - 1, count, count, indices);
    fprintf(f, "\"images\":[{\"uri\":\"tex_%03u.ktx2\"}],\"samplers\":[{"
        "\"magFilter\":9729,\"minFilter\":9729}],\"textures\":[{\"source\":0,"
        "\"sampler\":0}],\"materials\":[{\"pbrMetallicRoughness\":{"
        "\"baseColorTexture\":{\"index\":0}}}],\"meshes\":[{\"primitives\":[{"
        "\"attributes\":{\"POSITION\":0,\"NORMAL\":1,\"TEXCOORD_0\":2},"
        "\"indices\":3,\"material\":0}]}],\"nodes\":[{\"mesh\":0}],"
        "\"scenes\":[{\"nodes\":[0]}],\"scene\":0}\n", i);
    fclose(f);
}

static void write_level(const char *dir, uint32_t files, uint32_t side,
    uint32_t grid)
{
    char path[512];

    for (uint32_t i = 0; i < files; i++) {
        snprintf(path, sizeof(path), "%s/level_%03u.gltf", dir, i);
        write_gltf(path, i, grid);
        snprintf(path, sizeof(path), "%s/mesh_%03u.bin", dir, i);
        write_bin(path, grid, i);
        snprintf(path, sizeof(path), "%s/tex_%03u.ktx2", dir, i);
        write_ktx2(path, side, i);
    }
    printf("written %u\n", files);
}

int main(int argc, char **argv)
{
    const char *mode = argc > 1 ? argv[1] : "";
    const char *dir = argc > 2 ? argv[2] : ".";
    uint32_t files = argc > 3 ? (uint32_t)atoi(argv[3]) : 1;
    bench_level_t b = {};
    struct timespec t0;
    struct timespec t1;

    if (!strcmp(mode, "write")) {
        write_level(dir, files, argc > 4 ? (uint32_t)atoi(argv[4]) : 256,
            argc > 5 ? (uint32_t)atoi(argv[5]) : 32);
        return (0);
    }
    timespec_get(&t0, TIME_UTC);
    for (uint32_t i = 0; i < files; i++)
        read_level(dir, i, &b);
    timespec_get(&t1, TIME_UTC);
    printf("level %u models %u meshes %u textures %u vertices %llu indices "
        "%llu texels %llu\nload %.3f ms\n", files, b.models, b.meshes,
        b.textures, (unsigned long long)b.vertices,
        (unsigned long long)b.indices, (unsigned long long)b.texels,
        (double)(t1.tv_sec - t0.tv_sec) * 1e3
        + (double)(t1.tv_nsec - t0.tv_nsec) / 1e6);
    return (b.models == files ? 0 : 1);
}
