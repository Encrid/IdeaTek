/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <string.h>

#include "resources_paths.h"

static void show_split(const res_text_t *text)
{
    res_path_t p;

    path_split(text, &p);
    printf("split [%.*s] [%.*s] [%.*s]\n", (int)p.source.len, p.source.at,
        (int)p.path.len, p.path.at, (int)p.label.len, p.label.at);
}

static void show_variable(const res_text_t *text, const res_text_t *value)
{
    res_text_t name;
    char out[64];
    res_buffer_t buffer = { out, sizeof(out), 0 };
    bool has = path_variable(text, &name);

    printf("variable %d [%.*s]\n", has, (int)name.len, name.at);
    printf("resolve %d [%s]\n", path_resolve(text, value, &buffer), out);
}

static void show_rest(const res_text_t *a, const res_text_t *b)
{
    res_text_t extension;
    char out[32];
    res_buffer_t buffer = { out, sizeof(out), 0 };

    printf("extension %d [%.*s]\n", path_extension(a, &extension),
        (int)extension.len, extension.at);
    printf("join %d [%s]\n", path_join(b, a, &buffer), out);
    printf("under %d\n", path_is_under(a, b));
    printf("name %d\n", source_name_is_allowed(a));
}

int main(int argc, char **argv)
{
    res_text_t a = { argc > 1 ? argv[1] : "", 0 };
    res_text_t b = { argc > 2 ? argv[2] : "", 0 };

    a.len = (uint32_t)strlen(a.at);
    b.len = (uint32_t)strlen(b.at);
    show_split(&a);
    show_variable(&a, &b);
    show_rest(&a, &b);
    return (0);
}
