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

enum CJC_Result cjc_cursor_move_inside(struct CJC_Cursor *cursor)
{
    while (
        cursor->json[cursor->index] != '{' &&
        cursor->json[cursor->index] != '\0' &&
        cursor->json[cursor->index] != '['
    ) ++cursor->index;
    
    if (cursor->json[cursor->index] == '\0') return CJC_END_OF_JSON;
    if (cursor->json[cursor->index] == '{') cursor->is_array = 0;
    if (cursor->json[cursor->index] == '[') cursor->is_array = 1;

    ++cursor->index;
    return CJC_RESULT_SUCCESS;
}
