#ifndef _ALINKER_HOOK_H_
#define _ALINKER_HOOK_H_

#include <stddef.h>

typedef struct {
    char *name;
    void *addr;
} symbol_hook_t;

extern symbol_hook_t *symbol_hooks;
extern size_t symbol_hooks_count;

int hook_lookup_symbol(char *name, size_t *index);

void hook_hookup_symbol(char *name, void *addr);

#endif
