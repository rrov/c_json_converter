#ifndef CJC_H
#define CJC_H

enum CJC_Result {
    CJC_RESULT_SUCCESS,
    CJC_RESULT_END_OF_SCOPE
};

struct CJC_Cursor {
    const unsigned char *json;
    unsigned long index;
    unsigned char is_array;
};

/* Cursor movement */
enum CJC_Result cjc_cursor_move_inside(struct CJC_Cursor *cursor);
enum CJC_Result cjc_cursor_move_outside(struct CJC_Cursor *cursor);
enum CJC_Result cjc_cursor_move_forward(struct CJC_Cursor *cursor);
enum CJC_Result cjc_cursor_move_backward(struct CJC_Cursor *cursor);



#endif
