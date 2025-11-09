#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "cjc.h"


static int find_json_key_substring(struct CJC_JsonKeySubstring *substring);
static int parse_int_value(struct CJC_JsonKeySubstring *key_substring, int *value_destination);


int cjc_parse_int(struct CJC_State *state, char *key, int *value_destination)
{
    struct CJC_JsonKeySubstring json_key_substring = {
        .state = state,
        .key = key,
        .key_size = strlen(key),
        .index = 0,
        .shift = 0,
    };

    if (find_json_key_substring(&json_key_substring) != 0)
    {
        printf("Can not find key %s\n", key);
        return 1;
    }

    printf("Key index: %lu; Key shift: %lu\n", json_key_substring.index, json_key_substring.shift);

    return 0;
}


static int find_json_key_substring(struct CJC_JsonKeySubstring *substring)
{
    size_t read_key_size = 0;

    // TODO: rewrite to bits in byte
    bool read_key = false;
    bool skip_key = false;
    bool key_matched = false;

    for (size_t i = 0; i < substring->state->json_string_size; i++)
    {
        if (substring->state->json_string[i] == '"')
        {
            if (read_key)
            {
                substring->shift = (i - 1) - substring->index;
                key_matched = true;
                break;
            } 
            else
            {
                substring->index = i;
            }

            read_key = !read_key;
            skip_key = false;
            continue;
        }

        if (skip_key)
        {
            continue;
        }

        if (read_key && read_key_size < substring->key_size)
        {
            if (substring->state->json_string[i] != substring->key[read_key_size])
            {
                skip_key = true;
                read_key_size = 0;
                continue;
            }
            read_key_size++;
        }
    }

    return 0;
}

static int parse_int_value(struct CJC_JsonKeySubstring *key_substring, int *value_destination)
{
    return 0;
}
