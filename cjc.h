#ifndef CJC_H
#define CJC_H

#include <stddef.h>
#include <stdint.h>

struct CJC_State
{
    const char *json_string;
    const size_t json_string_size;
};

int cjc_parse_int(struct CJC_State *state, char *key, int *value_destination);

#endif
