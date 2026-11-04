/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <ideatek_web.h>

#define SIZE(type) printf("size %zu\nalignment %zu\n", sizeof(type), \
    alignof(type))
#define AT(type, field) printf("%s %zu\n", #field, offsetof(type, field))
#define TEXT_OF(...) BRIDGE_SCHEMA_TEXT(__VA_ARGS__)

typedef struct {
    const char *name;
    void (*show)(void);
} show_mode_t;

static void show_web_method(void)
{
    SIZE(ideatek_web_method_t);
}

static void show_web_request(void)
{
    SIZE(ideatek_web_request_t);
    AT(ideatek_web_request_t, url);
    AT(ideatek_web_request_t, headers);
    AT(ideatek_web_request_t, body);
    AT(ideatek_web_request_t, timeout);
    AT(ideatek_web_request_t, max_redirects);
    AT(ideatek_web_request_t, method);
}

static void show_web_state(void)
{
    SIZE(ideatek_web_state_t);
}

static void show_web_error(void)
{
    SIZE(ideatek_web_error_t);
}

static void show_web_response(void)
{
    SIZE(ideatek_web_response_t);
    AT(ideatek_web_response_t, headers);
    AT(ideatek_web_response_t, body);
    AT(ideatek_web_response_t, status);
    AT(ideatek_web_response_t, state);
    AT(ideatek_web_response_t, error);
}

static void show_descriptors(void)
{
    const char *descriptors[] = {
        ideatek_web_method_t_desc, ideatek_web_request_t_desc,
        ideatek_web_state_t_desc, ideatek_web_error_t_desc,
        ideatek_web_response_t_desc };

    for (size_t i = 0; i < sizeof(descriptors) / sizeof(descriptors[0]); i++)
        printf("%s\n", descriptors[i]);
}

static void show_texts(void)
{
    const char *texts[] = {
        TEXT_OF(IDEATEK_WEB_METHOD_VALUES), TEXT_OF(IDEATEK_WEB_REQUEST_FIELDS),
        TEXT_OF(IDEATEK_WEB_STATE_VALUES), TEXT_OF(IDEATEK_WEB_ERROR_VALUES),
        TEXT_OF(IDEATEK_WEB_RESPONSE_FIELDS) };

    for (size_t i = 0; i < sizeof(texts) / sizeof(texts[0]); i++)
        printf("%s\n", texts[i]);
}

int main(int argc, char **argv)
{
    static const show_mode_t MODES[] = {
        { "web_method", show_web_method }, { "web_request", show_web_request },
        { "web_state", show_web_state }, { "web_error", show_web_error },
        { "web_response", show_web_response },
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
