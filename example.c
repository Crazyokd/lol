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

    /* test multiple domain */
    lol_fatal2("no-domain", "fatal");
    lol_error2("no-domain", "error");
    lol_warn2("no-domain", "warn");
    lol_info2("no-domain", "info");
    lol_debug2("no-domain", "debug");
    lol_trace2("no-domain", "trace");

    lol_add_domain("test", LOL_INFO, NULL, LOL_NONE);
    lol_fatal2("test", "fatal");
    lol_error2("test", "error");
    lol_warn2("test", "warn");
    lol_info2("test", "info");
    lol_debug2("test", "debug");
    lol_trace2("test", "trace");

    int ret = lol_add_domain("test", LOL_ERROR, NULL, LOL_NONE);
    if (ret != 0) {
        lol_warn("failed to add domain");
    }
    lol_fatal2("test", "fatal");
    lol_error2("test", "error");
    lol_warn2("test", "warn");
    lol_info2("test", "info");
    lol_debug2("test", "debug");
    lol_trace2("test", "trace");
    lol_fini();
    return 0;
}
