#ifndef LOL_COMPILER_OPT_H_
#define LOL_COMPILER_OPT_H_

/* likely/unlikely */
#if __GNUC__ >= 3

#define likely(expr) __builtin_expect(!!(expr), 1)
#define unlikely(expr) __builtin_expect(!!(expr), 0)

#else /* __GNUC__ */

/* #warning "No compiler optimizations supported try gcc 4.x" */
#define likely(expr) (expr)
#define unlikely(expr) (expr)

#endif /* __GNUC__ */

#endif