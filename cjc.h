#ifndef CJC_H
#define CJC_H

#include <stddef.h>
#include <stdint.h>

struct CJC_State
{
    const char *json_string;
    const size_t json_string_size;
};

struct CJC_JsonKeySubstring
{
    struct CJC_State *state;
    
    const char *key;
    const size_t key_size;
    
    size_t index;
    size_t shift;
};

int cjc_parse_int(struct CJC_State *state, char *key, int *value_destination);

#endif
