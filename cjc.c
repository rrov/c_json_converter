#include "cjc.h"
static unsigned char character_is_syntax_character(char *character)
{
    switch (*character)
    {
        case '{':
        case '}':
        case '[':
        case ']':
        case ':':
        case ',':
        case '"': return 1;
        default: return 0;
    }
}

static unsigned char cursor_on_escaped_character(struct CJC_Cursor *cursor)
{
    return cursor->index != 0 && cursor->json[cursor->index - 1] == '\\';
}

enum CJC_Result cjc_cursor_move_inside(struct CJC_Cursor *cursor)
{
    unsigned char inside_quote = 0;
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
        if (cursor->json[cursor->index] == '"') inside_quote = inside_quote > 0 ? 0 : 1;
        
        ++cursor->index;
    }

    if (cursor->json[cursor->index] == '\0') return CJC_END_OF_JSON;

    ++cursor->index;
    return CJC_RESULT_SUCCESS;
}

enum CJC_Result cjc_cursor_move_outside(struct CJC_Cursor *cursor)
{
    unsigned long square_brackets_count = 0;
    unsigned long curly_brackers_count  = 0;
    unsigned char inside_quote = 0;

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
        ) ||
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
        if (cursor->json[cursor->index] == '"') inside_quote = inside_quote > 0 ? 0 : 1;
        
        --cursor->index;
    }

    return CJC_RESULT_SUCCESS;
}
