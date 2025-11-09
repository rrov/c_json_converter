#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "cjc.h"


const static char *json_example_1 = "{\"integer\":100}";
const static char *json_example_2 = "{\"message\":\"Hello World!\\n\"}";

int main()
{
    struct CJC_State state = {
        .json_string = json_example_1,
        .json_string_size = strlen(json_example_1),
    };

    int value;
    int result = cjc_parse_int(&state, "integer", &value);

    printf("Result: %i\n", result);

    return 0;
}
