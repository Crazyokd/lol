#ifndef LOL_H
#define LOL_H

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NOR "\033[0m"                 /* all off */
#define FGC_BLACK "\033[30m"          /* Foreground Color: Black */
#define FGC_RED "\033[31m"            /* Foreground Color: Red */
#define FGC_BOLD_RED "\033[1;31m"     /* Foreground Color: Bold Red */
#define FGC_GREEN "\033[32m"          /* Foreground Color: Green */
#define FGC_BOLD_GREEN "\033[1;32m"   /* Foreground Color: Bold Green */
#define FGC_YELLOW "\033[33m"         /* Foreground Color: Yellow */
#define FGC_BOLD_YELLOW "\033[1;33m"  /* Foreground Color: Bold Yellow */
#define FGC_BOLD_BLUE "\033[1;34m"    /* Foreground Color: Bold Blue */
#define FGC_BOLD_MAGENTA "\033[1;35m" /* Foreground Color: Bold Magenta */
#define FGC_BOLD_CYAN "\033[1;36m"    /* Foreground Color: Bold Cyan */
#define FGC_WHITE "\033[37m"          /* Foreground Color: White  */
#define FGC_BOLD_WHITE "\033[1;37m"   /* Foreground Color: Bold White  */
#define FGC_DEFAULT "\033[39m"        /* Foreground Color: default */

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

typedef struct lol_s {
  struct {
    uint8_t color : 1;
    uint8_t timestamp : 1;
    uint8_t domain : 1;
    uint8_t level : 1;
    uint8_t fileline : 1;
    uint8_t function : 1;
    uint8_t linefeed : 1;
  } print;

  char *domain;
  lol_level_e level;
  FILE *out;
  void (*writer)(struct lol_s *log, lol_level_e level, const char *string);

  struct lol_s *next;
} lol_t;

#define LOL_FUNC __func__
#define LOL_MAX_LEN 8192

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
