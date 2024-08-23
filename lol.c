#include "lol.h"

#include <stddef.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

lol_t *lol_log_list = NULL;

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

    /* 遍历所有domain */
    for (log = lol_log_list; log; log = log->next) {
        /* 通过domain_name匹配目标lol_t */
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
    /* init log_list */
    lol_log_list = calloc(1, sizeof(lol_t));
    if (!lol_log_list) {
        return -1;
    }

    /* set log domain and level */
    if (domain && strlen(domain)) {
        lol_log_list->domain = malloc(strlen(domain)+1);
        if (!lol_log_list->domain) {
            free(lol_log_list);
            return -1;
        }
        strcpy(lol_log_list->domain, domain);
        lol_log_list->print.domain = 1;
    }
    lol_log_list->level = std_level;

    /* add default writer */
    lol_log_list->out = stderr;
    lol_log_list->writer = file_writer;

    /* add default properties */
#if !defined(_WIN32)
    lol_log_list->print.color = 1;
#endif
    lol_log_list->print.timestamp = 1;
    lol_log_list->print.level = 1;
    lol_log_list->print.fileline = 1;
    lol_log_list->print.function = 1;
    lol_log_list->print.linefeed = 1;

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
    for (log = lol_log_list; log; log = log->next, free(prev)) {
        if (log->print.domain) {
            free(log->domain);
        }
        prev = log;
    }
}
