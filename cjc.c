#include "cjc.h"
#include <stddef.h>
#include <stdio.h>


static cjc_bool cursor_on_escaped_character(struct CJC_Cursor *cursor, char character)
{
    return cursor->index != 0 && cursor->json[cursor->index] == character && cursor->json[cursor->index - 1] == '\\';
}

enum CJC_RESULT cjc_cursor_move_inside(struct CJC_Cursor *cursor)
{
    char inside_string          = 0;
    long square_brackets_count  = 0;
    long curly_brackets_count   = 0;

    while (1)
    {
        switch (cursor->json[cursor->index])
        {
            case '"': inside_string && !cursor_on_escaped_character(cursor, '"') ? --inside_string : ++inside_string; break;
            case '[': if (!inside_string) ++square_brackets_count; break;
            case '{': if (!inside_string) ++curly_brackets_count; break;
            case ']': if (!inside_string) --square_brackets_count; break;
            case '}': if (!inside_string) --curly_brackets_count; break;
        }

        if (curly_brackets_count > 0 || square_brackets_count > 0) break;
        if (cursor->json[cursor->index] == '\0') return CJC_END_OF_JSON;

        ++cursor->index;
    }

    ++cursor->index;
    if (cursor->json[cursor->index] == '\0') return CJC_END_OF_JSON;
    return CJC_RESULT_SUCCESS;
}

enum CJC_RESULT cjc_cursor_move_outside(struct CJC_Cursor *cursor)
{
    long square_brackets_count  = 0;
    long curly_brackets_count   = 0;
    char inside_quote           = 0;

    if (cursor->index > 0)
    {
        --cursor->index;
    }

    while
    (
        (
            inside_quote                != 0    ||
            square_brackets_count       != 0    ||
            curly_brackets_count        != 0
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
            if (cursor->json[cursor->index] == '}') --curly_brackets_count;
            if (cursor->json[cursor->index] == ']') --square_brackets_count;
            if (cursor->json[cursor->index] == '{') ++curly_brackets_count;
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
    char comma_count            = 0;
    char inside_string          = 0;
    long square_brackets_count  = 0;
    long curly_brackets_count   = 0;

    while (1)
    {
        switch (cursor->json[cursor->index])
        {
            case '"': inside_string && !cursor_on_escaped_character(cursor, '"') ? --inside_string : ++inside_string; break;
            case '[': if (!inside_string) ++square_brackets_count; break;
            case '{': if (!inside_string) ++curly_brackets_count; break;
            case ']': if (!inside_string) --square_brackets_count; break;
            case '}': if (!inside_string) --curly_brackets_count; break;
            case ',': if (!inside_string && curly_brackets_count == 0 && square_brackets_count == 0) ++comma_count; break;
        }

        if (comma_count == 1 || (curly_brackets_count < 0 || square_brackets_count < 0)) break;
        if (cursor->json[cursor->index] == '\0') return CJC_END_OF_JSON;

        ++cursor->index;
    }

    if (curly_brackets_count < 0 || square_brackets_count < 0) return CJC_END_OF_SCOPE;
    if (cursor->json[cursor->index] == ',') ++cursor->index;
    return CJC_RESULT_SUCCESS;
}

enum CJC_RESULT cjc_cursor_move_backward(struct CJC_Cursor *cursor)
{
    char comma_count            = 0;
    char inside_string          = 0;
    long square_brackets_count  = 0;
    long curly_brackets_count   = 0;

    if (cursor->json[cursor->index]) ++inside_string; /* The cursor can refer to '"' char */

    while (1)
    {
        switch (cursor->json[cursor->index])
        {
            case '"': inside_string && !cursor_on_escaped_character(cursor, '"') ? --inside_string : ++inside_string; break;
            case '[': if (!inside_string) ++square_brackets_count; break;
            case '{': if (!inside_string) ++curly_brackets_count; break;
            case ']': if (!inside_string) --square_brackets_count; break;
            case '}': if (!inside_string) --curly_brackets_count; break;
            case ',': if (!inside_string && curly_brackets_count == 0 && square_brackets_count == 0) ++comma_count; break;
        }

        if (comma_count == 2 || (curly_brackets_count > 0 || square_brackets_count > 0)) break;
        if (cursor->index == 0) return CJC_START_OF_JSON;

        --cursor->index;
    }

    ++cursor->index;
    if (curly_brackets_count > 0 || square_brackets_count > 0) return CJC_START_OF_SCOPE;
    return CJC_RESULT_SUCCESS;
}
