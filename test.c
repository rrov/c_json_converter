
#include <stdio.h>
#include "cjc.h"

static const char *json_example_1 = "{\"message\":\"Hello World!\\n\"}";
static const char *json_example_2 = "[\"Hello World!\"]";

void cjc_cursor_move_inside_test_1(void)
{
    enum CJC_Result result;
    struct CJC_Cursor cursor;

    cursor.json = json_example_1;
    cursor.index = 0;

    result = cjc_cursor_move_inside(&cursor);
    
    printf("result: %2d, cursor index: %4lu\n", result, cursor.index);

    if (cursor.index != 1)
    {
        printf("ERROR: cursor index should be 1\n");
    }
}

void cjc_cursor_move_inside_test_2(void)
{
    enum CJC_Result result;
    struct CJC_Cursor cursor;

    cursor.json = json_example_1;
    cursor.index = 0;

    result = cjc_cursor_move_inside(&cursor);
    result = cjc_cursor_move_inside(&cursor);
    
    printf("result: %2d, cursor index: %4lu\n", result, cursor.index);

    if (result != CJC_END_OF_JSON)
    {
        printf("ERROR: result should be CJC_END_OF_JSON\n");
    }
}

void cjc_cursor_move_inside_test_3(void)
{
    enum CJC_Result result;
    struct CJC_Cursor cursor;

    cursor.json = json_example_2;
    cursor.index = 0;

    result = cjc_cursor_move_inside(&cursor);
    
    printf("result: %2d, cursor index: %4lu\n", result, cursor.index);

    if (cursor.index != 1)
    {
        printf("ERROR: cursor index should be 1\n");
    }
}

void cjc_cursor_move_inside_test_4(void)
{
    enum CJC_Result result;
    struct CJC_Cursor cursor;

    cursor.json = json_example_2;
    cursor.index = 0;

    result = cjc_cursor_move_inside(&cursor);
    result = cjc_cursor_move_inside(&cursor);

    printf("result: %2d, cursor index: %4lu\n", result, cursor.index);

    if (result != CJC_END_OF_JSON)
    {
        printf("ERROR: result should be CJC_END_OF_JSON\n");
    }
}

int main(void)
{
    cjc_cursor_move_inside_test_1();
    cjc_cursor_move_inside_test_2();
    cjc_cursor_move_inside_test_3();
    cjc_cursor_move_inside_test_4();
    return 0;
}
