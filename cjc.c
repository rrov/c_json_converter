#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "cjc.h"


int cjc_parse_int(struct CJC_State *state, char *key, int *value_destination)
{
    size_t key_size = strlen(key);
    size_t read_key_size = 0;

    bool read_key = false;
    bool skip_key = false;
    bool key_matched = false;

    for (size_t i = 0; i < state->json_string_size; i++)
    {
        printf("%c", state->json_string[i]);

        if (state->json_string[i] == '"')
        {
            if (read_key)
            {
                printf("\nKey matched!\n");
                key_matched = true;
            }

            read_key = !read_key;
            skip_key = false;
            continue;
        }

        if (skip_key)
        {
            continue;
        }

        if (read_key && read_key_size < key_size)
        {
            if (state->json_string[i] != key[read_key_size])
            {
                skip_key = true;
                read_key_size = 0;
                continue;
            }
            read_key_size++;
        }
    }
    printf("\n");

    return 0;
}
