
#include <stdio.h>
#include "cjc.h"

static const char *json_example_1 = "{\"message\":\"Hello World!\\n\"}";
static const char *json_example_2 = "[\"Hello World!\"]";

void json_example_1_test_1(void)
{
    enum CJC_Result result;
    struct CJC_Cursor cursor;

    cursor.json = json_example_1;
    cursor.index = 0;
    cursor.is_array = 0;

    result = cjc_cursor_move_inside(&cursor);
    
    printf("result: %d, cursor index: %lu\n", result, cursor.index);

    if (cursor.index != 1)
    {
        printf("ERROR: cursor index should be 1\n");
    }
}

void json_example_2_test_1(void)
{
    enum CJC_Result result;
    struct CJC_Cursor cursor;

    cursor.json = json_example_2;
    cursor.index = 0;
    cursor.is_array = 0;

    result = cjc_cursor_move_inside(&cursor);
    
    printf("result: %d, cursor index: %lu\n", result, cursor.index);

    if (cursor.index != 1)
    {
        printf("ERROR: cursor index should be 1\n");
    }
}

int main(void)
{
    json_example_1_test_1();
    json_example_2_test_1();
    return 0;
}
