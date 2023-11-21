#ifndef LOL_CORE_H_
#define LOL_CORE_H_

#include "core-config.h"
#include "color.h"
#include <stdarg.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif


/* add compile check for printf */
#if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ > 4)
#if !defined (__clang__) && ((__GNUC__ > (4)) || \
     ((__GNUC__ == (4)) && (__GNUC_MINOR__ >= (4))))
#define LOL_GNUC_PRINTF(f, v) __attribute__ ((format (gnu_printf, f, v)))
#else
#define LOL_GNUC_PRINTF(f, v) __attribute__ ((format (__printf__, f, v)))
#endif
#define LOL_GNUC_NORETURN __attribute__((__noreturn__))
#else
#define LOL_GNUC_PRINTF(f, v)
#define LOL_GNUC_NORETURN
#endif


typedef struct list_s {
    struct list_s *prev, *next;
} list_t;

void *list_next(void *lnode);
void *list_first(const list_t *list);


#define LOL_FUNC __func__

#ifdef __cplusplus
}
#endif

#endif /* LOL_CORE_H_ */
