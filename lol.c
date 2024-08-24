#include "lol.h"

#include <stddef.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

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

#define LOL_MAX_LEN 8192

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

lol_t *lol_list = NULL;

static const char *g_lol_domain = NULL;

static void file_writer(lol_t *log, lol_level_e level, const char *string)
{
    fprintf(log->out, "%s", string);
    fflush(log->out);
}

/* the size parameter include trailing '\0' */
int lol_vsnprintf(char *str, size_t size, const char *format, va_list ap)
{
    int r = -1;
    r = vsnprintf(str, size, format, ap);
    str[size-1] = '\0';

    return r;
}

char *lol_vslprintf(char *str, char *last, const char *format, va_list ap)
{
    int r = -1;

    if (!str)
        return NULL;

    if (str < last)
        r = lol_vsnprintf(str, last - str, format, ap);

    return (str + r);
}

char *lol_slprintf(char *str, char *last, const char *format, ...)
{
    char *res;
    va_list ap;

    va_start(ap, format);
    {
    int r = -1;
    if (!str)
        return NULL;
    if (str < last)
        r = lol_vsnprintf(str, last - str, format, ap);
    res = (str + r);
    }
    va_end(ap);

    return res;
}

static char *log_content(char *buf, char *last,
        const char *format, va_list ap)
{
    va_list bp;

    va_copy(bp, ap);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-nonliteral"
    buf = lol_vslprintf(buf, last, format, bp);
#pragma GCC diagnostic pop
    va_end(bp);

    return buf;
}

static char *log_timestamp(char *buf, char *last,
        int use_color)
{
    struct timeval tv;
    struct tm tm;
    char nowstr[32];

    gettimeofday(&tv, NULL);
    memset(&tm, 0, sizeof(tm));
    (void)localtime_r(&(tv.tv_sec), &tm);
    strftime(nowstr, sizeof nowstr, "%m/%d %H:%M:%S", &tm);

    buf = lol_slprintf(buf, last, "%s%s.%03d%s: ",
            use_color ? FGC_GREEN : "",
            nowstr, (int)(tv.tv_usec/1000),
            use_color ? NOR : "");

    return buf;
}

static char *log_domain(char *buf, char *last,
        const char *name, int use_color)
{
    buf = lol_slprintf(buf, last, "[%s%s%s] ",
            use_color ? FGC_YELLOW : "",
            name,
            use_color ? NOR : "");

    return buf;
}

const char *level_strings[] = {
    NULL,
    "FATAL", "ERROR", "WARNING", "INFO", "DEBUG", "TRACE",
};

static char *log_level(char *buf, char *last,
        lol_level_e level, int use_color)
{
    const char *colors[] = {
        NOR,
        FGC_BOLD_RED, FGC_RED, FGC_BOLD_YELLOW, 
        FGC_BOLD_GREEN, FGC_BOLD_CYAN, FGC_WHITE,
    };

    buf = lol_slprintf(buf, last, "%s%s%s: ",
            use_color ? colors[level] : "",
            level_strings[level],
            use_color ? NOR : "");

    return buf;
}

static char *log_linefeed(char *buf, char *last)
{
    if (buf > last - 2)
        buf = last - 2;

    return lol_slprintf(buf, last, "\n");
}

void lol_vprintf(lol_level_e level, const char *domain_name,
    int err, const char *file, int line, const char *func,
    int content_only, const char *format, va_list ap)
{
    lol_t *log = NULL;
    char logstr[LOL_MAX_LEN];
    char *p, *last;

    if (!domain_name) {
        domain_name = g_lol_domain;
    }

    /* 遍历所有domain */
    for (log = lol_list; log; log = log->next) {
        /* 通过domain_name匹配目标lol_t */
        if (log->domain && domain_name && strcmp(log->domain, domain_name))
            continue;
        if ((!log->domain || !domain_name) && log->domain != domain_name)
            continue;

        if (log->level < level)
            return;

        p = logstr;
        last = logstr + LOL_MAX_LEN;

        if (!content_only) {
            if (log->print.timestamp)
                p = log_timestamp(p, last, log->print.color);
            if (log->print.domain)
                p = log_domain(p, last, domain_name, log->print.color);
            if (log->print.level)
                p = log_level(p, last, level, log->print.color);
        }

        p = log_content(p, last, format, ap);

        if (err) {
            char errbuf[LOL_MAX_LEN];
            p = lol_slprintf(p, last, " (%d:%s)",
                    (int)err, strerror_r(err, errbuf, LOL_MAX_LEN));
        }

        if (!content_only) {
            if (log->print.fileline)
                p = lol_slprintf(p, last, " (%s:%d)", file, line);
            if (log->print.function)
                p = lol_slprintf(p, last, " %s()", func);
            if (log->print.linefeed) 
                p = log_linefeed(p, last);
        }

        log->writer(log, level, logstr);
    }

}


void lol_printf(lol_level_e level, const char *domain_id,
    int err, const char *file, int line, const char *func,
    int content_only, const char *format, ...)
{
    va_list args;

    va_start(args, format);
    lol_vprintf(level, domain_id,
            err, file, line, func, content_only, format, args);
    va_end(args);
}


int lol_init(const char *domain, lol_level_e std_level, const char *file,
             lol_level_e file_level)
{
    /* the lol only can be initialized once */
    if (lol_list) return -1;

    /* init log_list */
    lol_list = calloc(1, sizeof(lol_t));
    if (!lol_list) {
        return -1;
    }

    /* set log domain and level */
    if (domain && strlen(domain)) {
        lol_list->domain = malloc(strlen(domain)+1);
        if (!lol_list->domain) {
            free(lol_list);
            return -1;
        }
        strcpy(lol_list->domain, domain);
        lol_list->print.domain = 1;
    }
    g_lol_domain = domain;
    lol_list->level = std_level;

    /* add default writer */
    lol_list->out = stderr;
    lol_list->writer = file_writer;

    /* add default properties */
#if !defined(_WIN32)
    lol_list->print.color = 1;
#endif
    lol_list->print.timestamp = 1;
    lol_list->print.level = 1;
    lol_list->print.fileline = 1;
    lol_list->print.function = 1;
    lol_list->print.linefeed = 1;

    return 0;
}

int lol_init2()
{
    return lol_init(NULL, LOL_INFO, NULL, LOL_NONE);
}

void lol_fini()
{
    lol_t *log = NULL;
    lol_t *prev = NULL;
    for (log = lol_list; log; log = log->next, free(prev)) {
        if (log->print.domain) {
            free(log->domain);
        }
        prev = log;
    }
}
