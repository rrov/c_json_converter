#include <stdio.h>
#include "cjc.h"


static const char *json_example_1 = "{\"message\":\"Hello World!\\n\"}";
static const char *json_example_2 = "[\"Hello World!\"]";
static const char *json_example_3 = "{\"nested\":{\"message\":\"Hello World!\\n\"}}";
static const char *json_example_4 = "{\"nes [ { ted\":{\"message\":\"Hello World!\\n\"}}";
static const char *json_example_5 = "{\"name\":\"Some Name\",\"age\":100,\"items\":[\"Item 1\",{\"value\":\"Item 2\"},[\"Item 3\"]],\"children\":[{\"name\":\"Some Name 2\",\"age\":50,\"items\":[\"Item 1\",{\"value\":\"Item 2\"},[\"Item 3\"]]},{\"name\":\"Some Name 3\",\"age\":60,\"items\":[\"Item 1\",{\"value\":\"Item 2\"},[\"Item 3\"]]}]}";


void result_error_print(enum CJC_RESULT *result, cjc_bool *fail)
{
    printf("ERROR:  %d\n", *result);
    *fail = 1;
}


void cursor_index_error_print(struct CJC_Cursor *cursor, cjc_bool *fail, size_t index)
{
    printf("ERROR: cursor index should be %lu, got: %lu\n", index, cursor->index);
    *fail = 1;
}

void test_status_print(char *test_name, cjc_bool fail)
{
    if (fail)   printf("FAIL: %s\n", test_name);
    else        printf("SUCCESS: %s\n", test_name);
}

void cjc_cursor_move_inside_test_1(void)
{
    enum CJC_RESULT     result;
    struct CJC_Cursor   cursor;
    cjc_bool            fail;

    cursor.json         = json_example_1;
    cursor.index        = 0;
    fail                = 0;

    if ((result = cjc_cursor_move_inside(&cursor))) result_error_print(&result, &fail);
    if (cursor.index != 1) cursor_index_error_print(&cursor, &fail, 1);

    test_status_print("cjc_cursor_move_inside_test_1", fail);
}

void cjc_cursor_move_inside_test_2(void)
{
    enum CJC_RESULT     result;
    struct CJC_Cursor   cursor;
    cjc_bool            fail;

    cursor.json         = json_example_1;
    cursor.index        = 0;
    fail                = 0;

    if ((result = cjc_cursor_move_inside(&cursor))) result_error_print(&result, &fail);
    if ((result = cjc_cursor_move_inside(&cursor)) != CJC_END_OF_JSON) result_error_print(&result, &fail);
    
    test_status_print("cjc_cursor_move_inside_test_2", fail);
}

void cjc_cursor_move_inside_test_3(void)
{
    enum CJC_RESULT     result;
    struct CJC_Cursor   cursor;
    cjc_bool            fail;

    cursor.json         = json_example_2;
    cursor.index        = 0;
    fail                = 0;

    if ((result = cjc_cursor_move_inside(&cursor))) result_error_print(&result, &fail);
    if (cursor.index != 1) cursor_index_error_print(&cursor, &fail, 1);

    test_status_print("cjc_cursor_move_inside_test_3", fail);
}

void cjc_cursor_move_inside_test_4(void)
{
    enum CJC_RESULT     result;
    struct CJC_Cursor   cursor;
    cjc_bool            fail;

    cursor.json         = json_example_2;
    cursor.index        = 0;
    fail                = 0;

    if ((result = cjc_cursor_move_inside(&cursor))) result_error_print(&result, &fail);
    if ((result = cjc_cursor_move_inside(&cursor)) != CJC_END_OF_JSON) result_error_print(&result, &fail);
    
    test_status_print("cjc_cursor_move_inside_test_4", fail);
}

void cjc_cursor_move_outside_test_1(void)
{
    enum CJC_RESULT     result;
    struct CJC_Cursor   cursor;
    cjc_bool            fail;

    cursor.json         = json_example_3;
    cursor.index        = 0;
    fail                = 0;

    if ((result = cjc_cursor_move_inside(&cursor))) result_error_print(&result, &fail);
    if ((result = cjc_cursor_move_inside(&cursor))) result_error_print(&result, &fail);

    if (cursor.index != 11) cursor_index_error_print(&cursor, &fail, 11);

    if ((result = cjc_cursor_move_outside(&cursor))) result_error_print(&result, &fail);
    
    if (cursor.index != 10) cursor_index_error_print(&cursor, &fail, 10);

    if ((result = cjc_cursor_move_outside(&cursor))) result_error_print(&result, &fail);

    if (cursor.index != 0) cursor_index_error_print(&cursor, &fail, 0);

    test_status_print("cjc_cursor_move_outside_test_1", fail);
}

void cjc_cursor_move_outside_test_2(void)
{
    enum CJC_RESULT     result;
    struct CJC_Cursor   cursor;
    cjc_bool            fail;

    cursor.json         = json_example_4;
    cursor.index        = 0;
    fail                = 0;

    if ((result = cjc_cursor_move_inside(&cursor))) result_error_print(&result, &fail);
    if ((result = cjc_cursor_move_inside(&cursor))) result_error_print(&result, &fail);

    if (cursor.index != 16) cursor_index_error_print(&cursor, &fail, 16);

    if ((result = cjc_cursor_move_outside(&cursor))) result_error_print(&result, &fail);
    
    if (cursor.index != 15) cursor_index_error_print(&cursor, &fail, 15);

    if ((result = cjc_cursor_move_outside(&cursor))) result_error_print(&result, &fail);

    if (cursor.index != 0) cursor_index_error_print(&cursor, &fail, 0);

    test_status_print("cjc_cursor_move_outside_test_2", fail);
}

void cjc_cursor_move_forward_test_1(void)
{
    enum CJC_RESULT     result;
    struct CJC_Cursor   cursor;
    cjc_bool            fail;

    cursor.json         = json_example_5;
    cursor.index        = 0;
    fail                = 0;

    if ((result = cjc_cursor_move_inside(&cursor))) result_error_print(&result, &fail);
    if ((result = cjc_cursor_move_forward(&cursor))) result_error_print(&result, &fail);

    if (cursor.index != 20) cursor_index_error_print(&cursor, &fail, 20);

    if ((result = cjc_cursor_move_forward(&cursor))) result_error_print(&result, &fail);
    if (cursor.index != 30) cursor_index_error_print(&cursor, &fail, 30);

    if ((result = cjc_cursor_move_forward(&cursor))) result_error_print(&result, &fail);
    if (cursor.index != 79) cursor_index_error_print(&cursor, &fail, 79);

    if ((result = cjc_cursor_move_forward(&cursor)) != CJC_END_OF_SCOPE) result_error_print(&result, &fail);
    
    test_status_print("cjc_cursor_move_forward_test_1", fail);
}

void cjc_cursor_move_backward_test_1(void)
{
    enum CJC_RESULT     result;
    struct CJC_Cursor   cursor;
    cjc_bool            fail;

    cursor.json         = json_example_5;
    cursor.index        = 79;
    fail                = 0;

    if ((result = cjc_cursor_move_backward(&cursor))) result_error_print(&result, &fail);
    if (cursor.index != 30) cursor_index_error_print(&cursor, &fail, 30);

    test_status_print("cjc_cursor_move_backward_test_1", fail);
}

int main(void)
{
    cjc_cursor_move_inside_test_1();
    cjc_cursor_move_inside_test_2();
    cjc_cursor_move_inside_test_3();
    cjc_cursor_move_inside_test_4();
    
    cjc_cursor_move_outside_test_1();
    cjc_cursor_move_outside_test_2();

    cjc_cursor_move_forward_test_1();

    cjc_cursor_move_backward_test_1();
    
    return 0;
}
