#include <stdio.h>
#include "cjc.h"


static const char *json = "{\"name\":\"Some Name\",\"age\":100,\"items\":[\"Item 1\",{\"value\":\"Item 2\"},[\"Item 3\"]],\"children\":[{\"name\":\"Some Name 2\",\"age\":50,\"items\":[\"Item 1\",{\"value\":\"Item 2\"},[\"Item 3\"]]},{\"name\":\"Some Name 3\",\"age\":60,\"items\":[\"Item 1\",{\"value\":\"Item 2\"},[\"Item 3\"]]}]}";


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

/*
    enum CJC_RESULT     result;
    struct CJC_Cursor   cursor;
    cjc_bool            fail;

    cursor.json         = json_example_1;
    cursor.index        = 0;
    fail                = 0;

    if ((result = cjc_cursor_move_inside(&cursor))) result_error_print(&result, &fail);
    if (cursor.index != 1) cursor_index_error_print(&cursor, &fail, 1);

    test_status_print("cjc_cursor_move_inside_test_1", fail);
*/


int main(void)
{
       
    return 0;
}
