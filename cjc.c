#include "cjc.h"
#include <stddef.h>


static cjc_bool cursor_on_escaped_character(struct CJC_Cursor *cursor, char character)
{
    return cursor->index != 0 && cursor->json[cursor->index] == character && cursor->json[cursor->index - 1] == '\\';
}

enum CJC_Result cjc_cursor_move_inside(struct CJC_Cursor *cursor)
{
    cjc_bool inside_quote = 0;
    while
    (
        inside_quote                    != 0    ||
        (
            cursor->json[cursor->index] != '{'  &&
            cursor->json[cursor->index] != '['  &&
            cursor->json[cursor->index] != '\0'
        )
    )
    {
        /* Each stop of cursor should be outside any quote */
        if (cursor->json[cursor->index] == '"' && !cursor_on_escaped_character(cursor, '"'))
            inside_quote = inside_quote > 0 ? 0 : 1;
        
        ++cursor->index;
    }

    if (cursor->json[cursor->index] == '\0') return CJC_END_OF_JSON;

    ++cursor->index;
    return CJC_RESULT_SUCCESS;
}

enum CJC_Result cjc_cursor_move_outside(struct CJC_Cursor *cursor)
{
    size_t square_brackets_count    = 0;
    size_t curly_brackers_count     = 0;
    cjc_bool inside_quote           = 0;

    if (cursor->index > 0)
    {
        --cursor->index;
    }

    while
    (
        (
            inside_quote                != 0    ||
            square_brackets_count       != 0    ||
            curly_brackers_count        != 0
        ) 
        ||
        (
            cursor->json[cursor->index] != '{'  &&
            cursor->json[cursor->index] != '['  &&
            cursor->index               != 0
        )
    )
    {
        if (inside_quote == 0)
        {
            if (cursor->json[cursor->index] == '}') ++curly_brackers_count;
            if (cursor->json[cursor->index] == ']') ++square_brackets_count;
            if (cursor->json[cursor->index] == '{') --curly_brackers_count;
            if (cursor->json[cursor->index] == '[') --square_brackets_count;
        }

        /* Each stop of cursor should be outside any quote */
        if (cursor->json[cursor->index] == '"' && !cursor_on_escaped_character(cursor, '"'))
            inside_quote = inside_quote > 0 ? 0 : 1;
        
        --cursor->index;
    }

    return CJC_RESULT_SUCCESS;
}

enum CJC_Result cjc_cursor_move_forward(struct CJC_Cursor *cursor)
{
    size_t      square_brackets_count   = 0;
    size_t      curly_brackers_count    = 0;
    cjc_bool    inside_quote            = 0;
    cjc_bool    after_comma             = 0;

    while 
    (
        (
            inside_quote            == 0 && 
            after_comma             == 0 && 
            cursor->json[cursor->index] != '\0'
        )
    )
    {

        if (inside_quote == 0)
        {
            if (cursor->json[cursor->index] == '}') ++curly_brackers_count;
            if (cursor->json[cursor->index] == ']') ++square_brackets_count;
            if (cursor->json[cursor->index] == '{') --curly_brackers_count;
            if (cursor->json[cursor->index] == '[') --square_brackets_count;
        }

        /* Each stop of cursor should be outside any quote */
        if (cursor->json[cursor->index] == '"' && !cursor_on_escaped_character(cursor, '"'))
            inside_quote = inside_quote > 0 ? 0 : 1;

        

        if      (inside_quote == 1)                     after_comma = 0;
        else if (cursor->json[cursor->index] == ',')    after_comma = 1;

        ++cursor->index;
    }

    if (cursor->json[cursor->index] == '\0') return CJC_END_OF_JSON;

    return CJC_RESULT_SUCCESS;
}
