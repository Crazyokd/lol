#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "lol.h"

#define DOMAIN_CNT   100
#define MAX_BUF_SIZE 16

static int REPEAT_PRINT_CNT = 500;
static int DO_NOT_USE_SYS = 1;
void test_lol_x2()
{
    char buffer[MAX_BUF_SIZE];
    for (int i = 0; i < DOMAIN_CNT; i++) {
        snprintf(buffer, MAX_BUF_SIZE, "%d", i);
        for (int j = 0; j < REPEAT_PRINT_CNT; j++) {
            lol_fatal2(buffer, "");
            lol_error2(buffer, "");
            lol_warn2(buffer, "");
            lol_info2(buffer, "");
            lol_debug2(buffer, "");
            lol_trace2(buffer, "");
        }
    }
}

void test_lol_x2_2(char bufs[DOMAIN_CNT][MAX_BUF_SIZE])
{
    for (int i = 0; i < DOMAIN_CNT; i++) {
        for (int j = 0; j < REPEAT_PRINT_CNT; j++) {
            lol_fatal2(bufs[i], "");
            lol_error2(bufs[i], "");
            lol_warn2(bufs[i], "");
            lol_info2(bufs[i], "");
            lol_debug2(bufs[i], "");
            lol_trace2(bufs[i], "");
        }
    }
}

#define test_repeat_print(d)                   \
  for (int j = 0; j < REPEAT_PRINT_CNT; ++j) { \
    lol_fatal3(d, "");                         \
    lol_error3(d, "");                         \
    lol_warn3(d, "");                          \
    lol_info3(d, "");                          \
    lol_debug3(d, "");                         \
    lol_trace3(d, "");                         \
  }

void test_lol_x3()
{
    test_repeat_print(0);
    test_repeat_print(1);
    test_repeat_print(2);
    test_repeat_print(3);
    test_repeat_print(4);
    test_repeat_print(5);
    test_repeat_print(6);
    test_repeat_print(7);
    test_repeat_print(8);
    test_repeat_print(9);
    test_repeat_print(10);
    test_repeat_print(11);
    test_repeat_print(12);
    test_repeat_print(13);
    test_repeat_print(14);
    test_repeat_print(15);
    test_repeat_print(16);
    test_repeat_print(17);
    test_repeat_print(18);
    test_repeat_print(19);
    test_repeat_print(20);
    test_repeat_print(21);
    test_repeat_print(22);
    test_repeat_print(23);
    test_repeat_print(24);
    test_repeat_print(25);
    test_repeat_print(26);
    test_repeat_print(27);
    test_repeat_print(28);
    test_repeat_print(29);
    test_repeat_print(30);
    test_repeat_print(31);
    test_repeat_print(32);
    test_repeat_print(33);
    test_repeat_print(34);
    test_repeat_print(35);
    test_repeat_print(36);
    test_repeat_print(37);
    test_repeat_print(38);
    test_repeat_print(39);
    test_repeat_print(40);
    test_repeat_print(41);
    test_repeat_print(42);
    test_repeat_print(43);
    test_repeat_print(44);
    test_repeat_print(45);
    test_repeat_print(46);
    test_repeat_print(47);
    test_repeat_print(48);
    test_repeat_print(49);
    test_repeat_print(50);
    test_repeat_print(51);
    test_repeat_print(52);
    test_repeat_print(53);
    test_repeat_print(54);
    test_repeat_print(55);
    test_repeat_print(56);
    test_repeat_print(57);
    test_repeat_print(58);
    test_repeat_print(59);
    test_repeat_print(60);
    test_repeat_print(61);
    test_repeat_print(62);
    test_repeat_print(63);
    test_repeat_print(64);
    test_repeat_print(65);
    test_repeat_print(66);
    test_repeat_print(67);
    test_repeat_print(68);
    test_repeat_print(69);
    test_repeat_print(70);
    test_repeat_print(71);
    test_repeat_print(72);
    test_repeat_print(73);
    test_repeat_print(74);
    test_repeat_print(75);
    test_repeat_print(76);
    test_repeat_print(77);
    test_repeat_print(78);
    test_repeat_print(79);
    test_repeat_print(80);
    test_repeat_print(81);
    test_repeat_print(82);
    test_repeat_print(83);
    test_repeat_print(84);
    test_repeat_print(85);
    test_repeat_print(86);
    test_repeat_print(87);
    test_repeat_print(88);
    test_repeat_print(89);
    test_repeat_print(90);
    test_repeat_print(91);
    test_repeat_print(92);
    test_repeat_print(93);
    test_repeat_print(94);
    test_repeat_print(95);
    test_repeat_print(96);
    test_repeat_print(97);
    test_repeat_print(98);
    test_repeat_print(99);
}

void test_init(int domain, char bufs[DOMAIN_CNT][MAX_BUF_SIZE])
{
    snprintf(bufs[domain], MAX_BUF_SIZE, "%d", domain);

    if (DO_NOT_USE_SYS) {
        lol_add_domain(bufs[domain], LOL_NONE, NULL, LOL_NONE);
    } else {
        lol_add_domain(bufs[domain], domain % 6 + 1, NULL, LOL_NONE);
    }
}

int main(int argc, char **argv)
{
    struct timeval start, end;
    double time_used1, time_used2, time_used3;

    // init
    char bufs[DOMAIN_CNT][MAX_BUF_SIZE];
    lol_init("main", LOL_INFO, NULL, LOL_NONE);
    if (argc < 4) {
        // 输出帮助信息
        printf("Usage: %s [TEST_LOL_X2] [TEST_LOL_X2_2] [TEST_LOL_X3] "
               "[REPEAT_PRINT_CNT] "
               "[DO_NOT_USE_SYS]\n",
               argv[0]);
        return 1;
    }
    int test1 = argc > 1 && atoi(argv[1]);
    int test2 = argc > 2 && atoi(argv[2]);
    int test3 = argc > 3 && atoi(argv[3]);
    if (argc > 4) REPEAT_PRINT_CNT = atoi(argv[4]);
    if (argc > 5) DO_NOT_USE_SYS = atoi(argv[5]);
    for (int i = 0; i < DOMAIN_CNT; i++) {
        test_init(i, bufs);
    }

    gettimeofday(&start, NULL);
    if (test1) test_lol_x2();
    gettimeofday(&end, NULL);
    time_used1 =
        (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;

    gettimeofday(&start, NULL);
    if (test2) test_lol_x2_2(bufs);
    gettimeofday(&end, NULL);
    time_used2 =
        (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;

    gettimeofday(&start, NULL);
    if (test3) test_lol_x3();
    gettimeofday(&end, NULL);
    time_used3 =
        (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;

    printf("test_lol_v2: Time used: %f seconds\n", time_used1);
    printf("test_lol_v2_2: Time used: %f seconds\n", time_used2);
    printf("test_lol_v3: Time used: %f seconds\n", time_used3);

    lol_fini();
    return 0;
}
