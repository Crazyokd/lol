#include "core.h"

inline void *list_next(void *lnode)
{
    list_t *node = lnode;
    return node->next;
}

inline void *list_first(const list_t *list)
{
    return list->next;
}

lol_log_level_e lol_log_level_string_to_enum(const char *level) {
    lol_log_level_e level_e;
    if (!strcmp(level, "FATAL")) {
        level_e = LOL_LOG_FATAL;
    } else if (!strcmp(level, "ERROR")) {
        level_e = LOL_LOG_ERROR;
    } else if (!strcmp(level, "WARN")) {
        level_e = LOL_LOG_WARN;
    } else if (!strcmp(level, "INFO")) {
        level_e = LOL_LOG_INFO;
    } else if (!strcmp(level, "DEBUG")) {
        level_e = LOL_LOG_DEBUG;
    } else if (!strcmp(level, "TRACE")) {
        level_e = LOL_LOG_TRACE;
    } else {
        level_e = LOL_LOG_INFO;
    }
    return level_e;
}