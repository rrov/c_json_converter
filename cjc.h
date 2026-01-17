#ifndef CJC_H
#define CJC_H

#include <stddef.h>
#include <limits.h>


/* Supporting 32-bit and 64-bit architecture */

typedef signed char cjc_int8;
typedef unsigned char cjc_uint8;

#if SHRT_MAX == 32767
    typedef signed short cjc_int16;
    typedef unsigned short cjc_uint16;
#else
    typedef signed int cjc_int16;
    typedef unsigned int cjc_uint16;
#endif

#if INT_MAX == 2147483647L
    typedef signed int cjc_int32;
    typedef unsigned int cjc_uint32;
#else
    typedef signed long cjc_int32;
    typedef unsigned long cjc_uint32;
#endif

#if LONG_MAX == 9223372036854775807L
    typedef long cjc_int64;
    typedef unsigned long cjc_uint64;
#else
    #if defined(_MSC_VER) || defined(__BORLANDC__)
        typedef __int64 cjc_int64;
        typedef unsigned __int64 cjc_uint64;
    #elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
        #include <stdint.h>
        typedef int64_t cjc_int64;
        typedef uint64_t cjc_uint64;
    #elif defined(__GNUC__) || defined(__clang__) || defined(__CC_ARM)
        __extension__ typedef long long cjc_int64;
        __extension__ typedef unsigned long long cjc_uint64;
    #else
        #define CJC_NO_INT64
    #endif
#endif

typedef cjc_uint8 cjc_bool;

#ifdef CJC_NO_INT64
typedef cjc_int32 cjc_counter;
#else
typedef cjc_int64 cjc_counter;
#endif

enum CJC_RESULT {
    CJC_RESULT_SUCCESS,
    CJC_END_OF_JSON,
    CJC_RESULT_MOVED_OUTSIDE_SCOPE /* Indicates cursor read all elements in array or object and moved outside */
};

enum CJC_VALUE_TYPE {
    CJC_VALUE_TYPE_INITIAL,
    CJC_VALUE_TYPE_INT8,
    CJC_VALUE_TYPE_UINT8,
    CJC_VALUE_TYPE_INT16,
    CJC_VALUE_TYPE_UINT16,
    CJC_VALUE_TYPE_INT32,
    CJC_VALUE_TYPE_UINT32,
    CJC_VALUE_TYPE_INT64,
    CJC_VALUE_TYPE_UINT64,
    CJC_VALUE_TYPE_STRING,
    CJC_VALUE_TYPE_BOOLEAN,
    CJC_VALUE_TYPE_NULL,
    CJC_VALUE_TYPE_OBJECT,
    CJC_VALUE_TYPE_ARRAY,
    CJC_VALUE_TYPE_EMPTY /* For example empty array or object */
};

struct CJC_Cursor {
    const char *json;
    size_t index;
};


/* === PARSING === */
/* Cursor movement (primitive) */
enum CJC_RESULT cjc_cursor_move_inside(struct CJC_Cursor *cursor);
enum CJC_RESULT cjc_cursor_move_outside(struct CJC_Cursor *cursor);
enum CJC_RESULT cjc_cursor_move_forward(struct CJC_Cursor *cursor);
enum CJC_RESULT cjc_cursor_move_backward(struct CJC_Cursor *cursor);

/* Cursor movement (advanced) */
enum CJC_RESULT cjc_cursor_find_key(struct CJC_Cursor *cursor, char *string);
enum CJC_RESULT cjc_cursor_find_key_deep(struct CJC_Cursor *cursor, char *string);

/* Value interpretations (INT) */
enum CJC_RESULT cjc_cursor_interpretate_value_as_int8(struct CJC_Cursor *cursor, cjc_int8 *value);
enum CJC_RESULT cjc_cursor_interpretate_value_as_uint8(struct CJC_Cursor *cursor, cjc_uint8 *value);
enum CJC_RESULT cjc_cursor_interpretate_value_as_int16(struct CJC_Cursor *cursor, cjc_int16 *value);
enum CJC_RESULT cjc_cursor_interpretate_value_as_uint16(struct CJC_Cursor *cursor, cjc_uint16 *value);
enum CJC_RESULT cjc_cursor_interpretate_value_as_int32(struct CJC_Cursor *cursor, cjc_int32 *value);
enum CJC_RESULT cjc_cursor_interpretate_value_as_uint32(struct CJC_Cursor *cursor, cjc_uint32 *value);

#ifndef CJC_NO_INT64
enum CJC_RESULT cjc_cursor_interpretate_value_as_int64(struct CJC_Cursor *cursor, cjc_int64 *value);
enum CJC_RESULT cjc_cursor_interpretate_value_as_uint64(struct CJC_Cursor *cursor, cjc_uint64 *value);
#endif

/* Value interpretations (STRING) */
enum CJC_RESULT cjc_cursor_value_symbol_strlen(struct CJC_Cursor *cursor, size_t *size); /* ignores the escapes */
enum CJC_RESULT cjc_cursor_value_char_strlen(struct CJC_Cursor *cursor, size_t *size);
enum CJC_RESULT cjc_cursor_value_symbol_strncpy(struct CJC_Cursor *cursor, char *dest, size_t n); /* ignores the escapes */
enum CJC_RESULT cjc_cursor_value_char_strncpy(struct CJC_Cursor *cursor, char *dest, size_t n);

/* Value interpretations (BOOLEAN) */
enum CJC_RESULT cjc_cursor_interpretate_value_as_boolean(struct CJC_Cursor *cursor, cjc_bool *value);

/* Value checks */
enum CJC_RESULT cjc_cursor_value_define_type(struct CJC_Cursor *cursor, enum CJC_VALUE_TYPE *type);
enum CJC_RESULT cjc_cursor_value_is_null(struct CJC_Cursor *cursor, cjc_bool *value);

/* Keys */
enum CJC_RESULT cjc_cursor_show_keys(struct CJC_Cursor *cursor, char **keys);

/* === WRITING === */




#endif
