#include <errno.h>
#include <getopt.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "lol.h"

#define THREAD_CNT     1000
#define THREAD_LOG_CNT 1000

static void *log_thread(void *arg)
{
    pthread_t tid = pthread_self();
    char tid_str[32];
    snprintf(tid_str, sizeof(tid_str), "%lu", (unsigned long)tid);

    lol_init(tid_str, tid % 6 + 1, NULL, LOL_NONE);

    for (int i = 0; i < *(int *)arg; i++) {
        lol_fatal2(tid_str, "");
        lol_error2(tid_str, "");
        lol_warn2(tid_str, "");
        lol_info2(tid_str, "");
        lol_debug2(tid_str, "");
        lol_trace2(tid_str, "");
    }

    return arg;
}

static void test_errno()
{
    lol_init("errno", LOL_INFO, NULL, LOL_NONE);
    FILE *f = fopen("nonexist", "r");
    // perror("");
    lol_error_e(errno, "");
    lol_error_de("errno", errno, "");
    if (f) {
        fclose(f);
    }
}

static void print_usage(FILE *out, const char *program)
{
    fprintf(out, "Usage: %s [-t <thread-count>] [-l <log-count>] [-h]\n",
            program);
}

int main(int argc, char **argv)
{
    int opt;
    int thr_cnt = 0;
    int log_cnt = 0;
    while ((opt = getopt(argc, argv, "t:l:h")) != -1) {
        switch (opt) {
        case 't':
            thr_cnt = atoi(optarg);
            break;
        case 'l':
            log_cnt = atoi(optarg);
            break;
        case 'h':
            print_usage(stdout, argv[0]);
            exit(EXIT_SUCCESS);
        default: /* '?' */
            print_usage(stderr, argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    if (thr_cnt > THREAD_CNT || log_cnt > THREAD_LOG_CNT) {
        fprintf(stderr,
                "The exec count must be less than %d and The log count must be "
                "less than %d\n",
                THREAD_CNT, THREAD_LOG_CNT);
        exit(EXIT_FAILURE);
    }
    if (thr_cnt <= 0) {
        thr_cnt = THREAD_CNT;
    }
    if (log_cnt <= 0) {
        log_cnt = THREAD_LOG_CNT;
    }

    pthread_t threads[THREAD_CNT];
    for (int i = 0; i < thr_cnt; i++) {
        pthread_create(threads + i, NULL, log_thread, &log_cnt);
    }

    for (int i = 0; i < thr_cnt; i++) {
        pthread_join(threads[i], NULL);
    }

    test_errno();

    lol_fini();
    return 0;
}
