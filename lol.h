#ifndef LOL_H
#define LOL_H

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  LOL_NONE = 0,
  LOL_FATAL,
  LOL_ERROR,
  LOL_WARN,
  LOL_INFO,
  LOL_DEBUG,
  LOL_TRACE,
  LOL_DEFAULT = LOL_INFO,
  LOL_FULL = LOL_TRACE,
} lol_level_e;

#define LOL_FUNC __func__

/* API */
/**
 * initialize lol with custom settings
 */
int lol_init(const char *domain, lol_level_e std_level, const char *file,
             lol_level_e file_level);
/**
 * initialize lol with default settings
 */
int lol_init2();
void lol_fini();

void lol_printf(lol_level_e level, const char *domain_id,
    int err, const char *file, int line, const char *func,
    int content_only, const char *format, ...);

#define lol_message(level, errno, ...) \
  lol_printf(level, NULL, errno, __FILE__, __LINE__, LOL_FUNC, 0, __VA_ARGS__)

#define lol_fatal(...) lol_message(LOL_FATAL, 0, __VA_ARGS__)
#define lol_error(...) lol_message(LOL_ERROR, 0, __VA_ARGS__)
#define lol_warn(...) lol_message(LOL_WARN, 0, __VA_ARGS__)
#define lol_info(...) lol_message(LOL_INFO, 0, __VA_ARGS__)
#define lol_debug(...) lol_message(LOL_DEBUG, 0, __VA_ARGS__)
#define lol_trace(...) lol_message(LOL_TRACE, 0, __VA_ARGS__)
/* TODO: add lol_xxx_with_domain function family */

#ifdef __cplusplus
}
#endif

#endif
