#include "lol.h"

#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

#define THREAD_CNT 100
#define THREAD_LOG_CNT 100

void *log_thread(void *arg)
{
    pthread_t tid = pthread_self();
    char tid_str[32];
    snprintf(tid_str, sizeof(tid_str), "%lu", (unsigned long)tid);

    lol_init(tid_str, tid % 7, NULL, LOL_NONE);

    for (int i = 0; i < THREAD_LOG_CNT; i++) {
        lol_fatal2(tid_str, "");
        lol_error2(tid_str, "");
        lol_warn2(tid_str, "");
        lol_info2(tid_str, "");
        lol_debug2(tid_str, "");
        lol_trace2(tid_str, "");
    }

    return NULL;
}

int main() {
    pthread_t threads[THREAD_CNT];
    for (int i = 0; i < THREAD_CNT; i++) {
        pthread_create(threads+i, NULL, log_thread, NULL);
    }

    for (int i = 0; i < THREAD_CNT; i++) {
        pthread_join(threads[i], NULL);
    }

    lol_fini();
    return 0;
}
