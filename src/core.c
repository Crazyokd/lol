#include "core.h"

inline void *list_next(void *lnode)
{
    list_t *node = lnode;
    return node->next;
}

inline void *list_first(const list_t *list)
{
    return list->next;
}