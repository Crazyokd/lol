#include "lol.h"

#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc == 1) {
        lol_init2();
    } else if (argc == 2) {
        lol_init(NULL, atoi(argv[1]), NULL, LOL_NONE);
    } else {
        lol_init(argv[1], atoi(argv[2]), NULL, LOL_NONE);
    }

    lol_fatal("fatal");
    lol_error("error");
    lol_warn("warn");
    lol_info("info");
    lol_debug("debug");
    lol_trace("trace");
    return 0;
}
