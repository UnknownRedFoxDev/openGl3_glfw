#include <string.h>
#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"
#include <stdbool.h>

#define BUILD_FOLDER "build/"
#define SRC_FOLDER   "src/"

bool rebuild_modules(Cmd *cmd, int modulesCount, const char **modules) {
    Procs procs = {0};
    bool result = true;
    for (int i = 0; i < modulesCount; ++i) {
        const char *output_path = temp_sprintf("%s%s.o", BUILD_FOLDER, modules[i]);
        const char *input_path  = temp_sprintf("%s%s.cpp", SRC_FOLDER, modules[i]);
        if (needs_rebuild1(output_path, input_path)) {
            cmd_append(cmd, "g++",
                    "-std=c++17",
                    "-Wall", "-Wextra", "-O0", "-ggdb",
                    "-Isrc", "-Isrc/vendor", "-Isrc/tests",
                    "-c",
                    input_path,
                    "-o",
                    output_path);
            if (!cmd_run(cmd, .async = &procs)) return_defer(false);
        }
    }
    if (!procs_flush(&procs)) return_defer(false);

defer:
    da_free(procs);
    return result;
}

#define TEST_COUNT 11
#define MODULE_COUNT 11
#define VENDOR_COUNT 9

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);

    // Used to be the program name
    (void)shift(argv, argc);
    Cmd cmd = {0};
    bool run = false;

    const char *tests[TEST_COUNT] = {
        "tests/TestGameDeck",
        "tests/TestCardObj",
        "tests/Test",
        "tests/TestPlayerDeck",
        "tests/TestRefactoring",
        "tests/TestBatchRenderingTextures",
        "tests/TestSplitSpriteSheet",
        "tests/TestRefactoring2",
        "tests/TestTexture2D",
        "tests/TestClearColor",
        "tests/TestCardDecks"
    };
    const char *modules[MODULE_COUNT] = {
        "Pipeline",
        "GameClasses",
        "Shader",
        "ImageManipulation",
        "IndexBuffer",
        "Renderer",
        "Texture",
        "VertexBufferLayout",
        "VertexBuffer",
        "VertexArray",
        "main"
    };
    const char *vendors[VENDOR_COUNT] = {
        "vendor/ImGui/imgui_impl_opengl3",
        "vendor/ImGui/imgui_demo",
        "vendor/ImGui/imgui_impl_glfw",
        "vendor/ImGui/imgui_widgets",
        "vendor/ImGui/imgui",
        "vendor/ImGui/imgui_tables",
        "vendor/ImGui/imgui_draw",
        "vendor/glm/detail/glm",
        "vendor/stb_image/stb_image"
    };

    if (argc > 0) {
        char *flag = shift(argv, argc);
        if (strcmp(flag, "-run") == 0) run = true;
    }
    if (!mkdir_if_not_exists(BUILD_FOLDER)) return 1;
    if (!mkdir_if_not_exists(BUILD_FOLDER"tests/")) return 1;
    if (!mkdir_if_not_exists(BUILD_FOLDER"vendor/")) return 1;
    if (!mkdir_if_not_exists(BUILD_FOLDER"vendor/ImGui")) return 1;
    if (!mkdir_if_not_exists(BUILD_FOLDER"vendor/glm/")) return 1;
    if (!mkdir_if_not_exists(BUILD_FOLDER"vendor/glm/detail")) return 1;
    if (!mkdir_if_not_exists(BUILD_FOLDER"vendor/stb_image")) return 1;

    if (!rebuild_modules(&cmd, TEST_COUNT, tests)) return 1;
    if (!rebuild_modules(&cmd, MODULE_COUNT, modules)) return 1;
    if (!rebuild_modules(&cmd, VENDOR_COUNT, vendors)) return 1;

    if (needs_rebuild1(BUILD_FOLDER"main", SRC_FOLDER"main.cpp")) {
            cmd_append(&cmd, "g++",
                    "-std=c++17",
                    "-Wall", "-Wextra", "-O0", "-ggdb",
                    "-Isrc", "-Isrc/vendor", "-Isrc/tests");
            for (int i = 0; i < TEST_COUNT; ++i) {
                cmd_append(&cmd, temp_sprintf("%s%s.o", BUILD_FOLDER, modules[i]));
            }
            for (int i = 0; i < MODULE_COUNT; ++i) {
                cmd_append(&cmd, temp_sprintf("%s%s.o", BUILD_FOLDER, tests[i]));
            }
            for (int i = 0; i < VENDOR_COUNT; ++i) {
                cmd_append(&cmd, temp_sprintf("%s%s.o", BUILD_FOLDER, vendors[i]));
            }
            cmd_append(&cmd, "-o", BUILD_FOLDER"main", "-lGL", "-lglfw", "-lGLEW");
            if (!cmd_run(&cmd)) return 1;
    }

    if (run) {
        cmd_append(&cmd, BUILD_FOLDER"main");
        if (!cmd_run(&cmd)) return 1;
    }

    return 0;
}
