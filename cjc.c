#include "cjc.h"
#include <stddef.h>


static cjc_bool cursor_on_escaped_character(struct CJC_Cursor *cursor, char character)
{
    return cursor->index != 0 && cursor->json[cursor->index] == character && cursor->json[cursor->index - 1] == '\\';
}

enum CJC_RESULT cjc_cursor_move_inside(struct CJC_Cursor *cursor)
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

enum CJC_RESULT cjc_cursor_move_outside(struct CJC_Cursor *cursor)
{
    cjc_counter square_brackets_count   = 0;
    cjc_counter curly_brackers_count    = 0;
    cjc_bool inside_quote               = 0;

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
            if (cursor->json[cursor->index] == '}') --curly_brackers_count;
            if (cursor->json[cursor->index] == ']') --square_brackets_count;
            if (cursor->json[cursor->index] == '{') ++curly_brackers_count;
            if (cursor->json[cursor->index] == '[') ++square_brackets_count;
        }

        /* Each stop of cursor should be outside any quote */
        if (cursor->json[cursor->index] == '"' && !cursor_on_escaped_character(cursor, '"'))
            inside_quote = inside_quote > 0 ? 0 : 1;
        
        --cursor->index;
    }

    return CJC_RESULT_SUCCESS;
}

enum CJC_RESULT cjc_cursor_move_forward(struct CJC_Cursor *cursor)
{
    cjc_counter square_brackets_count   = 0;
    cjc_counter curly_brackers_count    = 0;
    cjc_bool    inside_quote            = 0;
    cjc_bool    after_comma             = 0;

    while 
    (
        inside_quote            == 0 && 
        after_comma             == 0 && 
        cursor->json[cursor->index] != '\0'
    )
    {
        if (inside_quote == 0)
        {
            if (cursor->json[cursor->index] == '}') --curly_brackers_count;
            if (cursor->json[cursor->index] == ']') --square_brackets_count;
            if (cursor->json[cursor->index] == '{') ++curly_brackers_count;
            if (cursor->json[cursor->index] == '[') ++square_brackets_count;
        }

        /* Each stop of cursor should be outside any quote */
        if (cursor->json[cursor->index] == '"' && !cursor_on_escaped_character(cursor, '"'))
            inside_quote = inside_quote > 0 ? 0 : 1;

        if      (inside_quote == 1)                     after_comma = 0;
        else if (cursor->json[cursor->index] == ',')    after_comma = 1;

        ++cursor->index;
    }

    if (cursor->json[cursor->index] == '\0')                    return CJC_END_OF_JSON;
    if (square_brackets_count < 0 || curly_brackers_count < 0)  return CJC_RESULT_MOVED_OUTSIDE_SCOPE;

    return CJC_RESULT_SUCCESS;
}

enum CJC_RESULT cjc_cursor_move_backward(struct CJC_Cursor *cursor)
{
    cjc_counter square_brackets_count   = 0;
    cjc_counter curly_brackers_count    = 0;
    cjc_bool    inside_quote            = 0;
    cjc_bool    before_comma            = 0;

    while 
    (
        inside_quote            == 0 && 
        before_comma            == 0 && 
        cursor->index           != 0
    )
    {
        if (inside_quote == 0)
        {
            if (cursor->json[cursor->index] == '}') --curly_brackers_count;
            if (cursor->json[cursor->index] == ']') --square_brackets_count;
            if (cursor->json[cursor->index] == '{') ++curly_brackers_count;
            if (cursor->json[cursor->index] == '[') ++square_brackets_count;
        }

        /* Each stop of cursor should be outside any quote */
        if (cursor->json[cursor->index] == '"' && !cursor_on_escaped_character(cursor, '"'))
            inside_quote = inside_quote > 0 ? 0 : 1;

        if      (inside_quote == 1)                     before_comma = 0;
        else if (cursor->json[cursor->index] == ',')    before_comma = 1;

        --cursor->index;
    }

    if (before_comma) ++cursor->index;
    if (square_brackets_count > 0 || curly_brackers_count > 0)  return CJC_RESULT_MOVED_OUTSIDE_SCOPE;

    return CJC_RESULT_SUCCESS;
}
