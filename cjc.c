#include "cjc.h"
#include <stddef.h>

enum CURSOR_MOVE_DIRECTION {
    CURSOR_MOVE_DIRECTION_BACKWARD,
    CURSOR_MOVE_DIRECTION_FORWARD
};

enum CURSOR_MOVE_RESULT {
    CURSOR_MOVE_RESULT_FOUND_SYNTAX_CHAR,
    CURSOR_MOVE_RESULT_START_OF_JSON,
    CURSOR_MOVE_RESULT_END_OF_JSON
};

enum CURSOR_MOVE_CHARACTER {
    CURSOR_MOVE_CHARACTER_SQUARE_OPENED_BRACKETS    = '[',
    CURSOR_MOVE_CHARACTER_SQUARE_CLOSED_BRACKETS    = ']',
    CURSOR_MOVE_CHARACTER_CURLY_OPENED_BRACKETS     = '{',
    CURSOR_MOVE_CHARACTER_CURLY_CLOSED_BRACKETS     = '}',
    CURSOR_MOVE_CHARACTER_COLON                     = ':',
    CURSOR_MOVE_CHARACTER_COMMA                     = ','
};

static cjc_bool cursor_on_escaped_character(struct CJC_Cursor *cursor, char character)
{
    return cursor->index != 0 && cursor->json[cursor->index] == character && cursor->json[cursor->index - 1] == '\\';
}

static char cursor_on_json_syntax_char(struct CJC_Cursor *cursor)
{
    /* ['{', '}', '[', ']', ':', ','] */
    switch (cursor->json[cursor->index])
    {
        case CURSOR_MOVE_CHARACTER_CURLY_OPENED_BRACKETS:
        case CURSOR_MOVE_CHARACTER_CURLY_CLOSED_BRACKETS:
        case CURSOR_MOVE_CHARACTER_SQUARE_OPENED_BRACKETS:
        case CURSOR_MOVE_CHARACTER_SQUARE_CLOSED_BRACKETS:
        case CURSOR_MOVE_CHARACTER_COLON:
        case CURSOR_MOVE_CHARACTER_COMMA:   return 1;
        default:                            return 0;
    }
}

static enum CURSOR_MOVE_RESULT cursor_move(struct CJC_Cursor *cursor, enum CURSOR_MOVE_DIRECTION direction)
{
    char inside_string = 0;
    do
    {
        if (cursor->json[cursor->index] == '"' && !cursor_on_escaped_character(cursor, '"'))
            inside_string ? --inside_string : ++inside_string;

        switch (direction) {
            case CURSOR_MOVE_DIRECTION_FORWARD:
            {
                if (cursor->json[cursor->index] != '\0')
                { 
                    ++cursor->index;
                    break;
                } 
                else return CURSOR_MOVE_RESULT_END_OF_JSON;
            }
            case CURSOR_MOVE_DIRECTION_BACKWARD:
            {
                if (cursor->index != 0)
                { 
                    --cursor->index;
                    break;
                } 
                else return CURSOR_MOVE_RESULT_START_OF_JSON;
            }
        }
    } while (inside_string || !cursor_on_json_syntax_char(cursor));

    return CURSOR_MOVE_RESULT_FOUND_SYNTAX_CHAR;
}

/* May be useless */
static enum CURSOR_MOVE_RESULT cursor_move_to(
    struct CJC_Cursor *cursor, 
    enum CURSOR_MOVE_DIRECTION direction, 
    enum CURSOR_MOVE_CHARACTER character
)
{
    do
    {
        switch (cursor_move(cursor, direction))
        {
            case CURSOR_MOVE_RESULT_FOUND_SYNTAX_CHAR: if ((char)character == cursor->json[cursor->index]) return CURSOR_MOVE_RESULT_FOUND_SYNTAX_CHAR;
            case CURSOR_MOVE_RESULT_END_OF_JSON:    return CURSOR_MOVE_RESULT_END_OF_JSON;
            case CURSOR_MOVE_RESULT_START_OF_JSON:  return CURSOR_MOVE_RESULT_START_OF_JSON;
        }
    } while (1);
}

enum CJC_RESULT cjc_cursor_move_inside(struct CJC_Cursor *cursor)
{
    long square_brackets_count  = 0;
    long curly_brackets_count   = 0;

    do
    {
        switch (cursor_move(cursor, CURSOR_MOVE_DIRECTION_FORWARD))
        {
            case CURSOR_MOVE_RESULT_FOUND_SYNTAX_CHAR:  break;
            case CURSOR_MOVE_RESULT_END_OF_JSON:        return CJC_END_OF_JSON;
            case CURSOR_MOVE_RESULT_START_OF_JSON:      return CJC_START_OF_JSON;
        }

        switch (cursor->json[cursor->index])
        {
            case CURSOR_MOVE_CHARACTER_SQUARE_OPENED_BRACKETS:  ++square_brackets_count;    break;
            case CURSOR_MOVE_CHARACTER_CURLY_OPENED_BRACKETS:   ++curly_brackets_count;     break;
            case CURSOR_MOVE_CHARACTER_SQUARE_CLOSED_BRACKETS:  --square_brackets_count;    break;
            case CURSOR_MOVE_CHARACTER_CURLY_CLOSED_BRACKETS:   --curly_brackets_count;     break;
        }

    } while (curly_brackets_count == 1 || square_brackets_count == 1);

    return CJC_RESULT_SUCCESS;
}

enum CJC_RESULT cjc_cursor_move_outside(struct CJC_Cursor *cursor)
{
    long square_brackets_count  = 0;
    long curly_brackets_count   = 0;

    do
    {
        switch (cursor_move(cursor, CURSOR_MOVE_DIRECTION_FORWARD))
        {
            case CURSOR_MOVE_RESULT_FOUND_SYNTAX_CHAR:  break;
            case CURSOR_MOVE_RESULT_END_OF_JSON:        return CJC_END_OF_JSON;
            case CURSOR_MOVE_RESULT_START_OF_JSON:      return CJC_START_OF_JSON;
        }

        switch (cursor->json[cursor->index])
        {
            case CURSOR_MOVE_CHARACTER_SQUARE_OPENED_BRACKETS:  ++square_brackets_count;    break;
            case CURSOR_MOVE_CHARACTER_CURLY_OPENED_BRACKETS:   ++curly_brackets_count;     break;
            case CURSOR_MOVE_CHARACTER_SQUARE_CLOSED_BRACKETS:  --square_brackets_count;    break;
            case CURSOR_MOVE_CHARACTER_CURLY_CLOSED_BRACKETS:   --curly_brackets_count;     break;
        }

    } while (curly_brackets_count == -1 || square_brackets_count == -1);

    return CJC_RESULT_SUCCESS;
}

enum CJC_RESULT cjc_cursor_move_forward(struct CJC_Cursor *cursor)
{
    long square_brackets_count  = 0;
    long curly_brackets_count   = 0;

    switch (cursor->json[cursor->index])
    {
        case CURSOR_MOVE_CHARACTER_SQUARE_CLOSED_BRACKETS:
        case CURSOR_MOVE_CHARACTER_CURLY_CLOSED_BRACKETS: return CJC_END_OF_SCOPE;
    }

    do
    {
        switch (cursor_move(cursor, CURSOR_MOVE_DIRECTION_FORWARD))
        {
            case CURSOR_MOVE_RESULT_FOUND_SYNTAX_CHAR:  break;
            case CURSOR_MOVE_RESULT_END_OF_JSON:        return CJC_END_OF_JSON;
            case CURSOR_MOVE_RESULT_START_OF_JSON:      return CJC_START_OF_JSON;
        }

        switch (cursor->json[cursor->index])
        {
            case CURSOR_MOVE_CHARACTER_SQUARE_OPENED_BRACKETS:  ++square_brackets_count;                    break;
            case CURSOR_MOVE_CHARACTER_CURLY_OPENED_BRACKETS:   ++curly_brackets_count;                     break;
            case CURSOR_MOVE_CHARACTER_SQUARE_CLOSED_BRACKETS:  --square_brackets_count;                    break;
            case CURSOR_MOVE_CHARACTER_CURLY_CLOSED_BRACKETS:   --curly_brackets_count;                     break;
            case CURSOR_MOVE_CHARACTER_COMMA: if (curly_brackets_count == 0 && square_brackets_count == 0)  break;
        }

    } while (curly_brackets_count == -1 || square_brackets_count == -1);

    return CJC_RESULT_SUCCESS;
}

enum CJC_RESULT cjc_cursor_move_backward(struct CJC_Cursor *cursor)
{
    long square_brackets_count  = 0;
    long curly_brackets_count   = 0;

    switch (cursor->json[cursor->index])
    {
        case CURSOR_MOVE_CHARACTER_SQUARE_OPENED_BRACKETS:
        case CURSOR_MOVE_CHARACTER_CURLY_OPENED_BRACKETS: return CJC_START_OF_SCOPE;
    }

    do
    {
        switch (cursor_move(cursor, CURSOR_MOVE_DIRECTION_BACKWARD))
        {
            case CURSOR_MOVE_RESULT_FOUND_SYNTAX_CHAR:  break;
            case CURSOR_MOVE_RESULT_END_OF_JSON:        return CJC_END_OF_JSON;
            case CURSOR_MOVE_RESULT_START_OF_JSON:      return CJC_START_OF_JSON;
        }

        switch (cursor->json[cursor->index])
        {
            case CURSOR_MOVE_CHARACTER_SQUARE_OPENED_BRACKETS:  ++square_brackets_count;                    break;
            case CURSOR_MOVE_CHARACTER_CURLY_OPENED_BRACKETS:   ++curly_brackets_count;                     break;
            case CURSOR_MOVE_CHARACTER_SQUARE_CLOSED_BRACKETS:  --square_brackets_count;                    break;
            case CURSOR_MOVE_CHARACTER_CURLY_CLOSED_BRACKETS:   --curly_brackets_count;                     break;
            case CURSOR_MOVE_CHARACTER_COMMA: if (curly_brackets_count == 0 && square_brackets_count == 0)  break;
        }

    } while (curly_brackets_count == 1 || square_brackets_count == 1);

    return CJC_RESULT_SUCCESS;
}
