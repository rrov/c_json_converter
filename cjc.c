#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

const static char *json_example_1 = "{\"message\":\"Hello World!\\n\"}";

struct State
{
    const char* json;
    size_t json_size;
    size_t char_index;
};

int main()
{
    struct State state = {
        .json = json_example_1,
        .json_size = strlen(json_example_1),
        .char_index = 0,
    };

    printf("json: %s\nposition: %lu\n", state.json, state.char_index);

    while (state.char_index <= state.json_size)
    {
        char letter = state.json[state.char_index];

        if (letter == '\0')
        {
            printf("The end of string on index %lu\n", state.char_index);
            break;
        }

        if (letter == '"')
        {
            printf("I found the letter '%c'\n", letter);
        }

        state.char_index++;
    }


    return 0;
}
