#ifndef LOL_LOG_H_
#define LOL_LOG_H_

#include <stdint.h>
#include "core.h"
#include "lol-strings.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef enum {
    LOL_LOG_NONE = 0,
    LOL_LOG_FATAL,
    LOL_LOG_ERROR,
    LOL_LOG_WARN,
    LOL_LOG_INFO,
    LOL_LOG_DEBUG,
    LOL_LOG_TRACE,
    LOL_LOG_DEFAULT = LOL_LOG_INFO,
    LOL_LOG_FULL = LOL_LOG_TRACE,
} lol_log_level_e;


void lol_log_vprintf(lol_log_level_e level, int domain_id,
    int err, const char *file, int line, const char *func,
    int content_only, const char *format, va_list ap);

void lol_log_printf(lol_log_level_e level, int domain_id,
    int err, const char *file, int line, const char *func,
    int content_only, const char *format, ...)
    LOL_GNUC_PRINTF(8, 9);

#ifndef LOL_DOMAIN
#define LOL_DOMAIN 1
#endif

#define lol_log_message(level, err, ...) \
    lol_log_printf(level, LOL_DOMAIN, \
    err, __FILE__, __LINE__, LOL_FUNC,  \
    0, __VA_ARGS__)

#define lol_log_print(level, ...) \
    lol_log_printf(level, LOL_DOMAIN, \
    0, NULL, 0, NULL,  \
    1, __VA_ARGS__) 

#define lol_fatal(...) lol_log_message(LOL_LOG_FATAL, 0, __VA_ARGS__)
#define lol_error(...) lol_log_message(LOL_LOG_ERROR, 0, __VA_ARGS__)
#define lol_warn(...) lol_log_message(LOL_LOG_WARN, 0, __VA_ARGS__)
#define lol_info(...) lol_log_message(LOL_LOG_INFO, 0, __VA_ARGS__)
#define lol_debug(...) lol_log_message(LOL_LOG_DEBUG, 0, __VA_ARGS__)
#define lol_trace(...) lol_log_message(LOL_LOG_TRACE, 0, __VA_ARGS__)


typedef enum {
    LOL_LOG_STDERR_TYPE,
    LOL_LOG_FILE_TYPE,
} lol_log_type_e;

typedef struct lol_log_s {
    list_t node;
    lol_log_type_e type;

    union {
        struct {
            FILE *out;
            const char *name;
        } file;
    };

    struct {
        uint8_t color:1;
        uint8_t timestamp:1;
        uint8_t domain:1;
        uint8_t level:1;
        uint8_t fileline:1;
        uint8_t function:1;
        uint8_t linefeed:1;
    } print;

    void (*writer)(struct lol_log_s *log, lol_log_level_e level, const char *string);

} lol_log_t;

typedef struct lol_log_domain_s {
    int id;
    lol_log_level_e level;
    const char *name;
} lol_log_domain_t;

#define HUGE_LEN 8192  /* Can Stack */

#ifdef __cplusplus
}
#endif

#endif // LOL_LOG_H_