#include "binconverter.h"
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <limits.h>
#include <assert.h>
#include <stdlib.h>

#define MAX_CHAR_BUFFER 255
#define EXPONENT_BIAS_FLOAT -127
#define EXPONENT_BIAS_DOUBLE -1023

/* Symbols */
#define SYMBOL_ZERO '0'
#define SYMBOL_ONE '1'
#define SYMBOL_SEPARATOR ' '

/* Buffer sizes for character sequences */
#define BUF_SIZE_OF_INT sizeof(int)*CHAR_BIT + 1
#define BUF_SIZE_OF_INT_WITH_SP sizeof(int)*CHAR_BIT + sizeof(int)
#define BUF_SIZE_OF_FLOAT sizeof(float)*CHAR_BIT + 1
#define BUF_SIZE_OF_FLOAT_WITH_SP sizeof(float)*CHAR_BIT + sizeof(float)
#define BUF_SIZE_OF_FLOAT_FR sizeof(float)*CHAR_BIT + 3
#define BUF_SIZE_OF_DOUBLE sizeof(double)*CHAR_BIT + 1
#define BUF_SIZE_OF_DOUBLE_WITH_SP sizeof(double)*CHAR_BIT + sizeof(double)
#define BUF_SIZE_OF_DOUBLE_FR sizeof(double)*CHAR_BIT + 3

/* Masks */
#define MASK_FLOAT_FRACTION 0x007FFFFF
#define MASK_FLOAT_EXPONENT 0x3F800000
#define MASK_DOUBLE_FRACTION 0x000FFFFFFFFFFFFF
#define MASK_DOUBLE_EXPONENT 0x3FF0000000000000


/* Size checks: getting compile error on wrong sizes */
/* 32-bit float and 64-bit double are expected */
typedef char float_type_size_check[sizeof(float) == sizeof(uint32_t) ? 1 : -1];
typedef char double_type_size_check[sizeof(double) == sizeof(uint64_t) ? 1 : -1];

void reverse_string(char* input, size_t size);
char* trim_string(char* str);   /* removes all non-digit characters from string */


/* Numbers mappings */
struct Display_double
{
    unsigned int fraction_2: 32;
    unsigned int fraction_1: 20;
    unsigned int exponent: 11;
    unsigned int sign: 1;
};

struct Display_float
{
    unsigned int fraction: 23;
    unsigned int exponent: 8;
    unsigned int sign: 1;
};


char* int_to_bin(int number)
{
    char *result;
    int scanner;    /* bit scanner */
    int cursor;     /* string cursor */

    /* scan value */
    cursor = 0;
    scanner = 1;
    result = calloc(BUF_SIZE_OF_INT, sizeof(char));
    while(scanner)
    {
        result[cursor] = scanner & number ? SYMBOL_ONE : SYMBOL_ZERO;
        scanner <<= 1;
        cursor++;
    }
    reverse_string(result, BUF_SIZE_OF_INT - 1);
    return result;
}


char* int_to_bin_sp(int number)
{
    char *result;
    int scanner;    /* bit scanner */
    int cursor;     /* string cursor */

    /* scan value */
    cursor = 0;
    scanner = 1;
    result = calloc(BUF_SIZE_OF_INT_WITH_SP, sizeof(char));
    while(scanner)
    {
        result[cursor] = scanner & number ? SYMBOL_ONE : SYMBOL_ZERO;
        scanner <<= 1;
        cursor++;
        /* add separator */
        if(!((cursor + 1)%(CHAR_BIT + 1)) && cursor != BUF_SIZE_OF_INT_WITH_SP - 1)
        {
            result[cursor] = SYMBOL_SEPARATOR;
            cursor++;
        }
    }
    reverse_string(result, BUF_SIZE_OF_INT_WITH_SP - 1);
    return result;
}


char* uint_to_bin(unsigned int number)
{
    char *result;
    int scanner;    /* bit scanner */
    int cursor;     /* string cursor */

    /* scan value */
    cursor = 0;
    scanner = 1;
    result = calloc(BUF_SIZE_OF_INT, sizeof(char));
    while(scanner)
    {
        result[cursor] = scanner & number ? SYMBOL_ONE : SYMBOL_ZERO;
        scanner <<= 1;
        cursor++;
    }
    reverse_string(result, BUF_SIZE_OF_INT - 1);
    return result;
}


char* uint_to_bin_sp(unsigned int number)
{
    char *result;
    int scanner;    /* bit scanner */
    int cursor;     /* string cursor */

    /* scan value */
    cursor = 0;
    scanner = 1;
    result = calloc(BUF_SIZE_OF_INT_WITH_SP, sizeof(char));
    while(scanner)
    {
        result[cursor] = scanner & number ? SYMBOL_ONE : SYMBOL_ZERO;
        scanner <<= 1;
        cursor++;
        /* add separator */
        if(!((cursor + 1)%(CHAR_BIT + 1)) && cursor != BUF_SIZE_OF_INT_WITH_SP - 1)
        {
            result[cursor] = SYMBOL_SEPARATOR;
            cursor++;
        }
    }
    reverse_string(result, BUF_SIZE_OF_INT_WITH_SP - 1);
    return result;
}


char* float_to_bin(float number)
{
    char *result;
    uint32_t *number_view = (uint32_t*)&number;
    int scanner;    /* bit scanner */
    int cursor;     /* string cursor */

    assert(sizeof(float)==sizeof(uint32_t));

    /* scan value */
    cursor = 0;
    scanner = 1;
    result = calloc(BUF_SIZE_OF_FLOAT, sizeof(char));
    while(scanner)
    {
        result[cursor] = scanner & *number_view ? SYMBOL_ONE : SYMBOL_ZERO;
        scanner <<= 1;
        cursor++;
    }
    reverse_string(result, BUF_SIZE_OF_FLOAT - 1);
    return result;
}


char* float_to_bin_sp(float number)
{
    char *result;
    uint32_t *number_view = (uint32_t*)&number;
    int scanner;    /* bit scanner */
    int cursor;     /* string cursor */

    assert(sizeof(float)==sizeof(uint32_t));

    /* scan value */
    cursor = 0;
    scanner = 1;
    result = calloc(BUF_SIZE_OF_FLOAT_WITH_SP, sizeof(char));
    while(scanner)
    {
        result[cursor] = scanner & *number_view ? SYMBOL_ONE : SYMBOL_ZERO;
        scanner <<= 1;
        cursor++;
        /* add separator */
        if(!((cursor + 1)%(CHAR_BIT + 1)) && cursor != BUF_SIZE_OF_FLOAT_WITH_SP - 1)
        {
            result[cursor] = SYMBOL_SEPARATOR;
            cursor++;
        }
    }
    reverse_string(result, BUF_SIZE_OF_FLOAT_WITH_SP - 1);
    return result;
}


char* float_to_bin_formatted(float number)
{
    const int SIGN = 3;
    const int EXPONENT = 12;

    char *result;
    uint32_t *number_view = (uint32_t*)&number;
    int scanner;    /* bit scanner */
    int cursor;     /* string cursor */

    assert(sizeof(float)==sizeof(uint32_t));

    /* scan value */
    cursor = 0;
    scanner = 1;
    result = calloc(BUF_SIZE_OF_FLOAT_FR, sizeof(char));
    while(scanner)
    {
        result[cursor] = scanner & *number_view ? SYMBOL_ONE : SYMBOL_ZERO;
        scanner <<= 1;
        cursor++;
        /* add separator */
        if((cursor == BUF_SIZE_OF_FLOAT_FR - SIGN)||(cursor == BUF_SIZE_OF_FLOAT_FR - EXPONENT))
        {
            result[cursor] = SYMBOL_SEPARATOR;
            cursor++;
        }

    }
    reverse_string(result, BUF_SIZE_OF_FLOAT_FR - 1);
    return result;
}


char* double_to_bin(double number)
{
    char *result;
    uint64_t *number_view = (uint64_t*)&number;
    uint64_t scanner;   /* bit scanner */
    int cursor;         /* string cursor */

    assert(sizeof(double)==sizeof(uint64_t));

    /* scan value */
    cursor = 0;
    scanner = 1;
    result = calloc(BUF_SIZE_OF_DOUBLE, sizeof(char));
    while(scanner)
    {
        result[cursor] = scanner & *number_view ? SYMBOL_ONE : SYMBOL_ZERO;
        scanner <<= 1;
        cursor++;
    }
    reverse_string(result, BUF_SIZE_OF_DOUBLE - 1);
    return result;
}


char* double_to_bin_sp(double number)
{
    char *result;
    uint64_t *number_view = (uint64_t*)&number;
    uint64_t scanner;   /* bit scanner */
    int cursor;         /* string cursor */

    assert(sizeof(double)==sizeof(uint64_t));

    /* scan value */
    cursor = 0;
    scanner = 1;
    result = calloc(BUF_SIZE_OF_DOUBLE_WITH_SP, sizeof(char));
    while(scanner)
    {
        result[cursor] = scanner & *number_view ? SYMBOL_ONE : SYMBOL_ZERO;
        scanner <<= 1;
        cursor++;
        /* add separator */
        if(!((cursor + 1)%(CHAR_BIT + 1)) && cursor != BUF_SIZE_OF_DOUBLE_WITH_SP - 1)
        {
            result[cursor] = SYMBOL_SEPARATOR;
            cursor++;
        }
    }
    reverse_string(result, BUF_SIZE_OF_DOUBLE_WITH_SP - 1);
    return result;
}


char* double_to_bin_formatted(double number)
{
    const int SIGN = 3;
    const int EXPONENT = 15;

    char *result;
    uint64_t *number_view = (uint64_t*)&number;
    uint64_t scanner;   /* bit scanner */
    int cursor;         /* string cursor */

    assert(sizeof(double)==sizeof(uint64_t));

    /* scan value */
    cursor = 0;
    scanner = 1;
    result = calloc(BUF_SIZE_OF_DOUBLE_FR, sizeof(char));
    while(scanner)
    {
        result[cursor] = scanner & *number_view ? SYMBOL_ONE : SYMBOL_ZERO;
        scanner <<= 1;
        cursor++;
        /* add separator */
        if((cursor == BUF_SIZE_OF_DOUBLE_FR - SIGN)||(cursor == BUF_SIZE_OF_DOUBLE_FR - EXPONENT))
        {
            result[cursor] = SYMBOL_SEPARATOR;
            cursor++;
        }
    }
    reverse_string(result, BUF_SIZE_OF_DOUBLE_FR - 1);
    return result;
}


int get_bin_exponent_float(float num)
{
    struct Display_float *num_map;

    assert(sizeof(float)==sizeof(uint32_t));

    num_map = (struct Display_float*)(&num);
    return (int)(num_map->exponent + EXPONENT_BIAS_FLOAT);
}


float get_fraction_float(float num)
{
    void *result;
    uint32_t result_num;
    uint32_t *num_pointer;
    uint32_t *mask_pointer;

    uint32_t double_fraction_mask = MASK_FLOAT_FRACTION;
    uint32_t exp_mask = MASK_FLOAT_EXPONENT;

    assert(sizeof(float)==sizeof(uint32_t));

    /* Remove exponent bits */
    num_pointer = (uint32_t*)&num;
    mask_pointer = (uint32_t*)&double_fraction_mask;
    result_num = (*num_pointer)&(*mask_pointer);
    result = (void*) &result_num;

    /* Append zero-exponent */
    num_pointer = (uint32_t*)result;
    mask_pointer = (uint32_t*)&exp_mask;
    result_num = (*num_pointer)|(*mask_pointer);
    result = (void*) &result_num;

    return *((float*)result);
}


int is_negative_float(float num)
{
    struct Display_float *num_map;

    assert(sizeof(float)==sizeof(uint32_t));

    num_map = (struct Display_float*)(&num);
    return (int)(num_map->sign);
}


double get_fraction_double(double num)
{
    void *result;
    uint64_t result_num;
    uint64_t *num_pointer;
    uint64_t *mask_pointer;

    uint64_t double_fraction_mask = MASK_DOUBLE_FRACTION;
    uint64_t exp_mask = MASK_DOUBLE_EXPONENT;

    assert(sizeof(double)==sizeof(uint64_t));

    /* Remove exponent bits */
    num_pointer = (uint64_t*)&num;
    mask_pointer = (uint64_t*)&double_fraction_mask;
    result_num = (*num_pointer)&(*mask_pointer);
    result = (void*)(&result_num);

    /* Append zero-exponent */
    num_pointer = (uint64_t*)result;
    mask_pointer = (uint64_t*)&exp_mask;
    result_num = (*num_pointer)|(*mask_pointer);
    result = ((void*)&result_num);

    return *((double*)result);
}


int get_bin_exponent_double(double num)
{
    struct Display_double *num_map;

    assert(sizeof(double)==sizeof(uint64_t));

    num_map = (struct Display_double*)(&num);
    return (int)(num_map->exponent + EXPONENT_BIAS_DOUBLE);
}


int is_negative_double(double num)
{
    struct Display_double *num_map;

    assert(sizeof(double)==sizeof(uint64_t));

    num_map = (struct Display_double*)(&num);
    return (int)(num_map->sign);
}


int bin_to_int(char* bin_val)
{
    int result;
    int current_bit;
    int cursor;
    size_t length;
    char* bin_number;

    bin_number = trim_string(bin_val);
    reverse_string(bin_number, strlen(bin_number));
    length = strlen(bin_number);

    if(length!=sizeof(int)*CHAR_BIT)
        return 0;

    result = 0;
    for(cursor=0; cursor<length; cursor++)
    {
        current_bit = ((int)1)<<cursor;
        if(bin_number[cursor]==SYMBOL_ONE){
            result |= current_bit;
        }
    }

    free(bin_number);
    return result;
}


unsigned int bin_to_uint(char* bin_val)
{
    unsigned int result;
    unsigned int current_bit;
    unsigned int cursor;
    size_t length;
    char* bin_number;

    bin_number = trim_string(bin_val);
    reverse_string(bin_number, strlen(bin_number));
    length = strlen(bin_number);

    if(length!=sizeof(unsigned int)*CHAR_BIT)
        return 0;

    result = 0;
    for(cursor=0; cursor<length; cursor++)
    {
        current_bit = ((unsigned int)1)<<cursor;
        if(bin_number[cursor]==SYMBOL_ONE){
            result |= current_bit;
        }
    }

    free(bin_number);
    return result;
}


float bin_to_float(char* bin_val)
{
    void *result_ptr;
    uint32_t result;
    uint32_t current_bit;
    uint32_t cursor;
    size_t length;
    char* bin_number;

    bin_number = trim_string(bin_val);
    reverse_string(bin_number, strlen(bin_number));
    length = strlen(bin_number);

    assert(sizeof(float)==sizeof(uint32_t));

    if(length!=sizeof(uint32_t)*CHAR_BIT)
        return 0;

    result = 0;
    result_ptr = &result;
    for(cursor=0; cursor<length; cursor++)
    {
        current_bit = ((uint32_t)1)<<cursor;
        if(bin_number[cursor]==SYMBOL_ONE){
            result |= current_bit;
        }
    }

    free(bin_number);
    return *((float*)(result_ptr));
}


double bin_to_double(char* bin_val)
{
    void *result_ptr;
    uint64_t result;
    uint64_t current_bit;
    uint64_t cursor;
    size_t length;
    char* bin_number;

    bin_number = trim_string(bin_val);
    reverse_string(bin_number, strlen(bin_number));
    length = strlen(bin_number);

    assert(sizeof(double)==sizeof(uint64_t));

    if(length!=sizeof(uint64_t)*CHAR_BIT)
        return 0;

    result = 0;
    result_ptr = &result;
    for(cursor=0; cursor<length; cursor++)
    {
        current_bit = ((uint64_t)1)<<cursor;
        if(bin_number[cursor]==SYMBOL_ONE)
            result |= current_bit;
    }

    free(bin_number);
    return *((double*)(result_ptr));
}


/* Remove spaces and other non-digit characters from string */
char* trim_string(char* str)
{
    char *buffer;
    size_t length;
    size_t cursor_string;
    size_t cursor_buffer;

    buffer = calloc(MAX_CHAR_BUFFER, sizeof(char));

    length = strlen(str);
    cursor_buffer = cursor_string = 0;

    while((cursor_string<length)&&(cursor_buffer<MAX_CHAR_BUFFER))
    {
        if(!isdigit(str[cursor_string]))
        {
            cursor_string++;
            continue;
        }
        buffer[cursor_buffer] = str[cursor_string];
        cursor_buffer++;
        cursor_string++;
    }
    return buffer;
}


/* Makes the character string reversed */
void reverse_string(char* input, size_t size)
{
    size_t left_cursor;
    size_t right_cursor;

    left_cursor = 0;
    right_cursor = size - 1;
    while(left_cursor<right_cursor)
    {
        /* XOR swap */
        input[left_cursor] ^= input[right_cursor];
        input[right_cursor] ^= input[left_cursor];
        input[left_cursor] ^= input[right_cursor];
        ++left_cursor;
        --right_cursor;
    }
}
