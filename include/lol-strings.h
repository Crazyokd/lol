#ifndef LOL_STRINGS_H_
#define LOL_STRINGS_H_

#include "core.h"
#include "stddef.h"
#include "string.h"

#ifdef __cplusplus
extern "C" {
#endif

/* (variable) formatted output to sized buffer */
int lol_vsnprintf(char *str, size_t size, const char *format, va_list ap);
/* (variable) safe limited formatted output */
char *lol_vslprintf(char *str, char *last, const char *format, va_list ap);
char *lol_slprintf(char *str, char *last, const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif