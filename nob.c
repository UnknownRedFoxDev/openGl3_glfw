#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

#define BUILD_FOLDER "build/"
#define SRC_FOLDER   "src/"

Cmd cmd = {0};

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);

    if (!nob_mkdir_if_not_exists(BUILD_FOLDER)) return 1;

    nob_cmd_append(&cmd, BUILD_FOLDER"my_program");
    if (!nob_cmd_run(&cmd)) return 1;

    return 0;
}
