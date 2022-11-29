#include <alinker/hook.h>
#include <stdlib.h>
#include <string.h>

symbol_hook_t *symbol_hooks = NULL;
size_t symbol_hooks_count = 0;

int hook_lookup_symbol(char *name, size_t *index) {
    for (size_t i = 0; i < symbol_hooks_count; ++i) {
        if (strcmp(name, symbol_hooks[i].name) == 0) {
            if (index != NULL) {
                *index = i;
            }
            return 1;
        }
    }
    if (index != NULL) {
        *index = -1;
    }
    return 0;
}

void hook_hookup_symbol(char *name, void *addr) {
    if (symbol_hooks == NULL || symbol_hooks_count == 0) {
        symbol_hooks = (symbol_hook_t *)malloc(sizeof(symbol_hook_t));
        symbol_hooks_count = 1;
        symbol_hook_t hook;
        hook.name = name;
        hook.addr = addr;
        symbol_hooks[0] = hook;
    } else {
        size_t index;
        int found = hook_lookup_symbol(name, &index);
        if (found) {
            symbol_hooks[index].name = name;
            symbol_hooks[index].addr = addr;
        } else {
            symbol_hook_t hook;
            hook.name = name;
            hook.addr = addr;
            ++symbol_hooks_count;
            symbol_hooks = realloc(symbol_hooks, symbol_hooks_count * sizeof(symbol_hook_t));
            symbol_hooks[symbol_hooks_count - 1] = hook;
        }
    }
}
