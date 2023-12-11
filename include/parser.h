#ifndef LOL_PARSER_H_
#define LOL_PARSER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "core.h"

int read_yaml_config(const char *filename);
void parse_yaml_config();

#ifdef __cplusplus
}
#endif

#endif  // LOL_PARSER_H_