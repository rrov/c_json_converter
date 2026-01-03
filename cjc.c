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
    while (cursor->json[cursor->index] != '{') ++cursor->index;
    return CJC_RESULT_SUCCESS;
}
