/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <string.h>

#include <glslang/Include/glslang_c_interface.h>
#include <glslang/Public/resource_limits_c.h>

static const char RIGHT[] =
    "#version 450\n"
    "layout(location = 0) in vec3 color;\n"
    "layout(location = 0) out vec4 result;\n"
    "void main() { result = vec4(color * PALETTE_GAIN, 1.0); }\n";

static const char FAULT[] =
    "#version 450\n"
    "layout(location = 0) out vec4 result;\n"
    "void main() { result = vec4(missing, 1.0); }\n";

static const char PREAMBLE[] = "#define PALETTE_GAIN 2.0\n";

static glslang_input_t make_input(const char *code)
{
    return ((glslang_input_t){
        .language = GLSLANG_SOURCE_GLSL, .stage = GLSLANG_STAGE_FRAGMENT,
        .client = GLSLANG_CLIENT_VULKAN,
        .client_version = GLSLANG_TARGET_VULKAN_1_3,
        .target_language = GLSLANG_TARGET_SPV,
        .target_language_version = GLSLANG_TARGET_SPV_1_6,
        .code = code, .default_version = 450,
        .default_profile = GLSLANG_NO_PROFILE,
        .messages = GLSLANG_MSG_SPV_RULES_BIT | GLSLANG_MSG_VULKAN_RULES_BIT,
        .resource = glslang_default_resource(),
    });
}

static bool parse(glslang_shader_t *shader, const glslang_input_t *input)
{
    if (glslang_shader_preprocess(shader, input)
        && glslang_shader_parse(shader, input))
        return (true);
    printf("%s\n", glslang_shader_get_info_log(shader));
    return (false);
}

static int generate(glslang_shader_t *shader)
{
    glslang_program_t *program = glslang_program_create();
    const unsigned int *words;
    int status = 1;

    glslang_program_add_shader(program, shader);
    if (glslang_program_link(program, GLSLANG_MSG_SPV_RULES_BIT
        | GLSLANG_MSG_VULKAN_RULES_BIT)) {
        glslang_program_SPIRV_generate(program, GLSLANG_STAGE_FRAGMENT);
        words = glslang_program_SPIRV_get_ptr(program);
        printf("words %zu magic %08x version %08x\n",
            glslang_program_SPIRV_get_size(program), words[0], words[1]);
        status = 0;
    } else
        printf("%s\n", glslang_program_get_info_log(program));
    glslang_program_delete(program);
    return (status);
}

static int compile(const char *code, bool preamble)
{
    glslang_input_t input = make_input(code);
    glslang_shader_t *shader = glslang_shader_create(&input);
    int status = 1;

    if (preamble)
        glslang_shader_set_preamble(shader, PREAMBLE);
    if (parse(shader, &input))
        status = generate(shader);
    glslang_shader_delete(shader);
    return (status);
}

int main(int argc, char **argv)
{
    const char *mode = argc > 1 ? argv[1] : "";
    int status = 2;

    glslang_initialize_process();
    if (strcmp(mode, "right") == 0)
        status = compile(RIGHT, true);
    if (strcmp(mode, "fault") == 0)
        status = compile(FAULT, true);
    if (strcmp(mode, "bare") == 0)
        status = compile(RIGHT, false);
    if (status == 2)
        printf("the mode is right, fault or bare\n");
    glslang_finalize_process();
    return (status);
}
