#ifndef LOL_H
#define LOL_H

#include <stdarg.h>

#if defined _WIN32 || defined __CYGWIN__
  #ifdef BUILDING_DLL
    #ifdef __GNUC__
      #define DLL_PUBLIC __attribute__((dllexport))
    #else
      #define DLL_PUBLIC __declspec(dllexport)
    #endif
  #else
    #ifdef __GNUC__
      #define DLL_PUBLIC __attribute__((dllimport))
    #else
      #define DLL_PUBLIC __declspec(dllimport)
    #endif
  #endif
  #define DLL_LOCAL
#else
  #if __GNUC__ >= 4
    #define DLL_PUBLIC __attribute__((visibility("default")))
    #define DLL_LOCAL  __attribute__((visibility("hidden")))
  #else
    #define DLL_PUBLIC
    #define DLL_LOCAL
  #endif
#endif

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

#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901L
  #if defined __GNUC__ && __GNUC__ >= 2
    #define LOL_FUNC __FUNCTION__
  #else
    #define LOL_FUNC "<unknown>"
  #endif
#else
  #define LOL_FUNC __func__
#endif

#if defined(__GNUC__)
  #define lol_likely(x)   __builtin_expect(!!(x), 1)
  #define lol_unlikely(x) __builtin_expect(!!(x), 0)
#else
  #define lol_likely(v)   v
  #define lol_unlikely(v) v
#endif
/* API */
/**
 * initialize lol with custom settings
 *
 * it support multiple calls in multi-thread environment.
 */
DLL_PUBLIC int lol_init(const char *domain, lol_level_e std_level,
                        const char *file, lol_level_e file_level);
/**
 * initialize lol with default settings
 */
#define lol_init2() lol_init(NULL, LOL_INFO, NULL, LOL_NONE);

/**
 * release all resources allocated by lol
 *
 * it support multiple calls in single thread environment.
 * but thread-unsafe
 */
DLL_PUBLIC void lol_fini();

DLL_PUBLIC int lol_add_domain(const char *domain, lol_level_e std_level,
                              const char *file, lol_level_e file_level);

DLL_PUBLIC lol_level_e lol_string_to_level(const char *level);

/**
 * get specific lol_t by domain(for performance)
 */
void *lol_get(const char *domain);

DLL_PUBLIC void lol_printf(lol_level_e level, void *log, const char *domain_id,
                           int err, const char *file, int line,
                           const char *func, int content_only,
                           const char *format, ...);

#define lol_message(level, log, domain, errno, ...)                      \
  lol_printf(level, log, domain, errno, __FILE__, __LINE__, LOL_FUNC, 0, \
             __VA_ARGS__)

#define lol_fatal(...)     lol_message(LOL_FATAL, 0, 0, 0, __VA_ARGS__)
#define lol_error(...)     lol_message(LOL_ERROR, 0, 0, 0, __VA_ARGS__)
#define lol_warn(...)      lol_message(LOL_WARN, 0, 0, 0, __VA_ARGS__)
#define lol_info(...)      lol_message(LOL_INFO, 0, 0, 0, __VA_ARGS__)
#define lol_debug(...)     lol_message(LOL_DEBUG, 0, 0, 0, __VA_ARGS__)
#define lol_trace(...)     lol_message(LOL_TRACE, 0, 0, 0, __VA_ARGS__)

/* lol_xxx_with_domain function family */
#define lol_fatal2(d, ...) lol_message(LOL_FATAL, 0, d, 0, __VA_ARGS__)
#define lol_error2(d, ...) lol_message(LOL_ERROR, 0, d, 0, __VA_ARGS__)
#define lol_warn2(d, ...)  lol_message(LOL_WARN, 0, d, 0, __VA_ARGS__)
#define lol_info2(d, ...)  lol_message(LOL_INFO, 0, d, 0, __VA_ARGS__)
#define lol_debug2(d, ...) lol_message(LOL_DEBUG, 0, d, 0, __VA_ARGS__)
#define lol_trace2(d, ...) lol_message(LOL_TRACE, 0, d, 0, __VA_ARGS__)

/* lol_xxx_with_domain for performance */
#define lol_fatal3(d, ...)                                    \
  do {                                                        \
    static void *lol_name_##d = NULL;                         \
    if (lol_unlikely(lol_name_##d == NULL)) {                 \
      lol_name_##d = lol_get(#d);                             \
    }                                                         \
    lol_message(LOL_FATAL, lol_name_##d, #d, 0, __VA_ARGS__); \
  } while (0);
#define lol_error3(d, ...)                                    \
  do {                                                        \
    static void *lol_name_##d = NULL;                         \
    if (lol_unlikely(lol_name_##d == NULL)) {                 \
      lol_name_##d = lol_get(#d);                             \
    }                                                         \
    lol_message(LOL_ERROR, lol_name_##d, #d, 0, __VA_ARGS__); \
  } while (0);
#define lol_warn3(d, ...)                                    \
  do {                                                       \
    static void *lol_name_##d = NULL;                        \
    if (lol_unlikely(lol_name_##d == NULL)) {                \
      lol_name_##d = lol_get(#d);                            \
    }                                                        \
    lol_message(LOL_WARN, lol_name_##d, #d, 0, __VA_ARGS__); \
  } while (0);
#define lol_info3(d, ...)                                    \
  do {                                                       \
    static void *lol_name_##d = NULL;                        \
    if (lol_unlikely(lol_name_##d == NULL)) {                \
      lol_name_##d = lol_get(#d);                            \
    }                                                        \
    lol_message(LOL_INFO, lol_name_##d, #d, 0, __VA_ARGS__); \
  } while (0);
#define lol_debug3(d, ...)                                    \
  do {                                                        \
    static void *lol_name_##d = NULL;                         \
    if (lol_unlikely(lol_name_##d == NULL)) {                 \
      lol_name_##d = lol_get(#d);                             \
    }                                                         \
    lol_message(LOL_DEBUG, lol_name_##d, #d, 0, __VA_ARGS__); \
  } while (0);
#define lol_trace3(d, ...)                                    \
  do {                                                        \
    static void *lol_name_##d = NULL;                         \
    if (lol_unlikely(lol_name_##d == NULL)) {                 \
      lol_name_##d = lol_get(#d);                             \
    }                                                         \
    lol_message(LOL_TRACE, lol_name_##d, #d, 0, __VA_ARGS__); \
  } while (0);

/* lol_xxx_with_errno function family */
#define lol_fatal_e(e, ...)     lol_message(LOL_FATAL, 0, 0, e, __VA_ARGS__)
#define lol_error_e(e, ...)     lol_message(LOL_ERROR, 0, 0, e, __VA_ARGS__)
#define lol_warn_e(e, ...)      lol_message(LOL_WARN, 0, 0, e, __VA_ARGS__)
#define lol_info_e(e, ...)      lol_message(LOL_INFO, 0, 0, e, __VA_ARGS__)
#define lol_debug_e(e, ...)     lol_message(LOL_DEBUG, 0, 0, e, __VA_ARGS__)
#define lol_trace_e(e, ...)     lol_message(LOL_TRACE, 0, 0, e, __VA_ARGS__)

/* lol_xxx_with_domain_and_errno function family */
#define lol_fatal_de(d, e, ...) lol_message(LOL_FATAL, 0, d, e, __VA_ARGS__)
#define lol_error_de(d, e, ...) lol_message(LOL_ERROR, 0, d, e, __VA_ARGS__)
#define lol_warn_de(d, e, ...)  lol_message(LOL_WARN, 0, d, e, __VA_ARGS__)
#define lol_info_de(d, e, ...)  lol_message(LOL_INFO, 0, d, e, __VA_ARGS__)
#define lol_debug_de(d, e, ...) lol_message(LOL_DEBUG, 0, d, e, __VA_ARGS__)
#define lol_trace_de(d, e, ...) lol_message(LOL_TRACE, 0, d, e, __VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif
