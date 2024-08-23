#include "lol.h"

int main() {
    lol_init2();
    lol_fatal("fatal");
    lol_error("error");
    lol_warn("warn");
    lol_info("info");
    lol_debug("debug");
    lol_trace("trace");
    return 0;
}
