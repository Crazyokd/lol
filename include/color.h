#ifndef LOL_COLOR_H_
#define LOL_COLOR_H_

#ifdef __cplusplus
extern "C" {
#endif

#define NOR              "\033[0m"       /* all off */
#define FGC_BLACK        "\033[30m"      /* Foreground Color: Black */
#define FGC_RED          "\033[31m"      /* Foreground Color: Red */
#define FGC_BOLD_RED     "\033[1;31m"    /* Foreground Color: Bold Red */
#define FGC_GREEN        "\033[32m"      /* Foreground Color: Green */
#define FGC_BOLD_GREEN   "\033[1;32m"    /* Foreground Color: Bold Green */
#define FGC_YELLOW       "\033[33m"      /* Foreground Color: Yellow */
#define FGC_BOLD_YELLOW  "\033[1;33m"    /* Foreground Color: Bold Yellow */
#define FGC_BOLD_BLUE    "\033[1;34m"    /* Foreground Color: Bold Blue */
#define FGC_BOLD_MAGENTA "\033[1;35m"    /* Foreground Color: Bold Magenta */
#define FGC_BOLD_CYAN    "\033[1;36m"    /* Foreground Color: Bold Cyan */
#define FGC_WHITE        "\033[37m"      /* Foreground Color: White  */
#define FGC_BOLD_WHITE   "\033[1;37m"    /* Foreground Color: Bold White  */
#define FGC_DEFAULT      "\033[39m"      /* Foreground Color: default */

#ifdef __cplusplus
}
#endif

#endif // LOL_COLOR_H_