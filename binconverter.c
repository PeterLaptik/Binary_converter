#include "binconverter.h"
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_CHAR_BUFFER 255
#define EXPONENT_BIAS_FLOAT -127
#define EXPONENT_BIAS_DOUBLE -1023

/* Symbols */
#define SYMBOL_ZERO '0'
#define SYMBOL_ONE '1'
#define SYMBOL_SEPARATOR ' '

/* Buffer sizes for character sequences */
#define BITS_IN_BYTE 8
#define SIZE_OF_INT sizeof(int)*BITS_IN_BYTE + 1
#define SIZE_OF_INT_WITH_SP sizeof(int)*BITS_IN_BYTE + sizeof(int)
#define SIZE_OF_FLOAT sizeof(float)*BITS_IN_BYTE + 1
#define SIZE_OF_FLOAT_WITH_SP sizeof(float)*BITS_IN_BYTE + sizeof(float)
#define SIZE_OF_FLOAT_FR sizeof(float)*BITS_IN_BYTE + 3
#define SIZE_OF_DOUBLE sizeof(double)*BITS_IN_BYTE + 1
#define SIZE_OF_DOUBLE_WITH_SP sizeof(double)*BITS_IN_BYTE + sizeof(double)
#define SIZE_OF_DOUBLE_FR sizeof(double)*BITS_IN_BYTE + 3

/* Masks */
#define MASK_FLOAT_FRACTION 0x007FFFFF
#define MASK_FLOAT_EXPONENT 0x3F800000
#define MASK_DOUBLE_FRACTION 0x000FFFFFFFFFFFFF
#define MASK_DOUBLE_EXPONENT 0x3FF0000000000000


void reverse(char* input, size_t size); /* makes the character string reversed */
char* trim_string(char* str);   /* remove non-digit characters from string */


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


/* Checks type sizes */
int check_compatibility()
{
    if(sizeof(float)!=sizeof(uint32_t))
        return 0;

    if(sizeof(double)!=sizeof(uint64_t))
        return 0;

    return 1;
}


char* int_to_bin(int number)
{
    static char result[SIZE_OF_INT];
    int scanner;    /* bit scanner */
    int cursor;     /* string cursor */

    /* clear buffer */
    for(int i=0; i<SIZE_OF_INT; ++i)
        result[i] = 0;

    /* scan value */
    cursor = 0;
    scanner = 1;
    while(scanner)
    {
        if(scanner & number)
            result[cursor] = SYMBOL_ONE;
        else
            result[cursor] = SYMBOL_ZERO;
        scanner <<= 1;
        cursor++;
    }
    reverse(result, SIZE_OF_INT);
    return result;
}


char* int_to_bin_sp(int number)
{
    static char result[SIZE_OF_INT_WITH_SP];
    int scanner;    /* bit scanner */
    int cursor;     /* string cursor */

    /* clear buffer */
    for(int i=0; i<SIZE_OF_INT_WITH_SP; ++i)
        result[i] = 0;

    /* scan value */
    cursor = 0;
    scanner = 1;
    while(scanner)
    {
        if(scanner & number)
            result[cursor] = SYMBOL_ONE;
        else
            result[cursor] = SYMBOL_ZERO;
        scanner <<= 1;
        cursor++;
        /* add separator */
        if(!((cursor+1)%(BITS_IN_BYTE + 1)))
        {
            result[cursor] = SYMBOL_SEPARATOR;
            cursor++;
        }
    }
    reverse(result, SIZE_OF_INT_WITH_SP);
    return result;
}


char* uint_to_bin(unsigned int number)
{
    static char result[SIZE_OF_INT];
    int scanner;    /* bit scanner */
    int cursor;     /* string cursor */

    /* clear buffer */
    for(int i=0; i<SIZE_OF_INT; ++i)
        result[i] = 0;

    /* scan value */
    cursor = 0;
    scanner = 1;
    while(scanner)
    {
        if(scanner & number)
            result[cursor] = SYMBOL_ONE;
        else
            result[cursor] = SYMBOL_ZERO;
        scanner <<= 1;
        cursor++;
    }
    reverse(result, SIZE_OF_INT);
    return result;
}


char* uint_to_bin_sp(unsigned int number)
{
    static char result[SIZE_OF_INT_WITH_SP];
    int scanner;    /* bit scanner */
    int cursor;     /* string cursor */

    /* clear buffer */
    for(int i=0; i<SIZE_OF_INT_WITH_SP; ++i)
        result[i] = 0;

    /* scan value */
    cursor = 0;
    scanner = 1;
    while(scanner)
    {
        if(scanner & number)
            result[cursor] = SYMBOL_ONE;
        else
            result[cursor] = SYMBOL_ZERO;
        scanner <<= 1;
        cursor++;
        /* add separator */
        if(!((cursor+1)%(BITS_IN_BYTE + 1)))
        {
            result[cursor] = SYMBOL_SEPARATOR;
            cursor++;
        }
    }
    reverse(result, SIZE_OF_INT_WITH_SP);
    return result;
}


char* float_to_bin(float number)
{
    static char result[SIZE_OF_FLOAT];
    uint32_t *number_view = (uint32_t*)&number;
    int scanner;    /* bit scanner */
    int cursor;     /* string cursor */

    /* clear buffer */
    for(int i=0; i<SIZE_OF_FLOAT; ++i)
        result[i] = 0;

    if(sizeof(float)!=sizeof(uint32_t))
        return result;

    /* scan value */
    cursor = 0;
    scanner = 1;
    while(scanner)
    {
        if(scanner & *number_view)
            result[cursor] = SYMBOL_ONE;
        else
            result[cursor] = SYMBOL_ZERO;
        scanner <<= 1;
        cursor++;
    }
    reverse(result, SIZE_OF_FLOAT);
    return result;
}


char* float_to_bin_sp(float number)
{
    static char result[SIZE_OF_FLOAT_WITH_SP];
    uint32_t *number_view = (uint32_t*)&number;
    int scanner;    /* bit scanner */
    int cursor;     /* string cursor */

    /* clear buffer */
    for(int i=0; i<SIZE_OF_FLOAT_WITH_SP; ++i)
        result[i] = 0;

    if(sizeof(float)!=sizeof(uint32_t))
        return result;

    /* scan value */
    cursor = 0;
    scanner = 1;
    while(scanner)
    {
        if(scanner & *number_view)
            result[cursor] = SYMBOL_ONE;
        else
            result[cursor] = SYMBOL_ZERO;
        scanner <<= 1;
        cursor++;
        /* add separator */
        if(!((cursor+1)%(BITS_IN_BYTE + 1)))
        {
            result[cursor] = SYMBOL_SEPARATOR;
            cursor++;
        }
    }
    reverse(result, SIZE_OF_FLOAT_WITH_SP);
    return result;
}


char* float_to_bin_formatted(float number)
{
    const int SIGN = 3;
    const int EXPONENT = 12;

    static char result[SIZE_OF_FLOAT_FR];
    uint32_t *number_view = (uint32_t*)&number;
    int scanner;    /* bit scanner */
    int cursor;     /* string cursor */

    /* clear buffer */
    for(int i=0; i<SIZE_OF_FLOAT_FR; ++i)
        result[i] = 0;

    if(sizeof(float)!=sizeof(uint32_t))
        return result;

    /* scan value */
    cursor = 0;
    scanner = 1;
    while(scanner)
    {
        if(scanner & *number_view)
            result[cursor] = SYMBOL_ONE;
        else
            result[cursor] = SYMBOL_ZERO;
        scanner <<= 1;
        cursor++;
        /* add separator */
        if((cursor==SIZE_OF_FLOAT_FR - SIGN)||(cursor==SIZE_OF_FLOAT_FR - EXPONENT))
        {
            result[cursor] = SYMBOL_SEPARATOR;
            cursor++;
        }

    }
    reverse(result, SIZE_OF_FLOAT_FR);
    return result;
}


char* double_to_bin(double number)
{
    static char result[SIZE_OF_DOUBLE];
    uint64_t *number_view = (uint64_t*)&number;
    uint64_t scanner;   /* bit scanner */
    int cursor;         /* string cursor */

    /* clear buffer */
    for(int i=0; i<SIZE_OF_DOUBLE; ++i)
        result[i] = 0;

    if(sizeof(double)!=sizeof(uint64_t))
        return result;

    /* scan value */
    cursor = 0;
    scanner = 1;
    while(scanner)
    {
        if(scanner & *number_view)
            result[cursor] = SYMBOL_ONE;
        else
            result[cursor] = SYMBOL_ZERO;
        scanner <<= 1;
        cursor++;
    }
    reverse(result, SIZE_OF_DOUBLE);
    return result;
}


char* double_to_bin_sp(double number)
{
    static char result[SIZE_OF_DOUBLE_WITH_SP];
    uint64_t *number_view = (uint64_t*)&number;
    uint64_t scanner;   /* bit scanner */
    int cursor;         /* string cursor */

    /* clear buffer */
    for(int i=0; i<SIZE_OF_DOUBLE_WITH_SP; ++i)
        result[i] = 0;

    if(sizeof(double)!=sizeof(uint64_t))
        return result;

    /* scan value */
    cursor = 0;
    scanner = 1;

    while(scanner)
    {
        if(scanner & *number_view)
            result[cursor] = SYMBOL_ONE;
        else
            result[cursor] = SYMBOL_ZERO;

        scanner <<= 1;
        cursor++;
        /* add separator */
        if(!((cursor+1)%(BITS_IN_BYTE + 1)))
        {
            result[cursor] = SYMBOL_SEPARATOR;
            cursor++;
        }
    }
    reverse(result, SIZE_OF_DOUBLE_WITH_SP);
    return result;
}


char* double_to_bin_formatted(double number)
{
    const int SIGN = 3;
    const int EXPONENT = 15;
    static char result[SIZE_OF_DOUBLE_FR];
    uint64_t *number_view = (uint64_t*)&number;
    uint64_t scanner;   /* bit scanner */
    int cursor;         /* string cursor */

    /* clear buffer */
    for(int i=0; i<SIZE_OF_DOUBLE_FR; ++i)
        result[i] = 0;

    if(sizeof(double)!=sizeof(uint64_t))
        return result;

    /* scan value */
    cursor = 0;
    scanner = 1;

    while(scanner)
    {
        if(scanner & *number_view)
            result[cursor] = SYMBOL_ONE;
        else
            result[cursor] = SYMBOL_ZERO;

        scanner <<= 1;
        cursor++;
        /* add separator */
        if((cursor==SIZE_OF_DOUBLE_FR - SIGN)||(cursor==SIZE_OF_DOUBLE_FR - EXPONENT))
        {
            result[cursor] = SYMBOL_SEPARATOR;
            cursor++;
        }
    }
    reverse(result, SIZE_OF_DOUBLE_FR);
    return result;
}


int get_bin_exponent_float(float num)
{
    struct Display_float *num_map;

    if(sizeof(float)!=sizeof(uint32_t))
        return 0;

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

    if(sizeof(float)!=sizeof(uint32_t))
        return 0;

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

    if(sizeof(float)!=sizeof(uint32_t))
        return 0;

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

    if(sizeof(double)!=sizeof(uint64_t))
        return 0;

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

    if(sizeof(double)!=sizeof(uint64_t))
        return 0;

    num_map = (struct Display_double*)(&num);
    return (int)(num_map->exponent + EXPONENT_BIAS_DOUBLE);
}


int is_negative_double(double num)
{
    struct Display_double *num_map;

    if(sizeof(double)!=sizeof(uint64_t))
        return 0;

    num_map = (struct Display_double*)(&num);
    return (int)(num_map->sign);
}


int bin_to_int(char* bin_val)
{
    int result;
    int current_bit;
    int cursor;
    size_t length;

    char* bin_number = trim_string(bin_val);
    reverse(bin_number, strlen(bin_number));
    length = strlen(bin_number);

    if(length!=sizeof(int)*BITS_IN_BYTE)
        return 0;

    result = 0;
    for(cursor=0; cursor<length; cursor++)
    {
        current_bit = ((int)1)<<cursor;
        if(bin_number[cursor]==SYMBOL_ONE){
            result |= current_bit;
        }
    }

    return result;
}


unsigned int bin_to_uint(char* bin_val)
{
    unsigned int result;
    unsigned int current_bit;
    unsigned int cursor;
    size_t length;

    char* bin_number = trim_string(bin_val);
    reverse(bin_number, strlen(bin_number));
    length = strlen(bin_number);

    if(length!=sizeof(unsigned int)*BITS_IN_BYTE)
        return 0;

    result = 0;
    for(cursor=0; cursor<length; cursor++)
    {
        current_bit = ((unsigned int)1)<<cursor;
        if(bin_number[cursor]==SYMBOL_ONE){
            result |= current_bit;
        }
    }

    return result;
}


float bin_to_float(char* bin_val)
{
    void *result_ptr;
    uint32_t result;
    uint32_t current_bit;
    uint32_t cursor;
    size_t length;

    char* bin_number = trim_string(bin_val);
    reverse(bin_number, strlen(bin_number));
    length = strlen(bin_number);

    if((length!=sizeof(uint32_t)*BITS_IN_BYTE)||(sizeof(uint32_t)!=sizeof(float)))
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

    return *((float*)(result_ptr));
}


double bin_to_double(char* bin_val)
{
    void *result_ptr;
    uint64_t result;
    uint64_t current_bit;
    uint64_t cursor;
    size_t length;

    char* bin_number = trim_string(bin_val);
    reverse(bin_number, strlen(bin_number));
    length = strlen(bin_number);

    if((length!=sizeof(uint64_t)*BITS_IN_BYTE)||(sizeof(uint64_t)!=sizeof(double)))
        return 0;

    result = 0;
    result_ptr = &result;
    for(cursor=0; cursor<length; cursor++)
    {
        current_bit = ((uint64_t)1)<<cursor;

        if(bin_number[cursor]==SYMBOL_ONE)
            result = result | current_bit;
    }

    return *((double*)(result_ptr));
}


/* Remove spaces and other non-digit characters from string */
char* trim_string(char* str)
{
    static char buffer[MAX_CHAR_BUFFER];
    size_t length;
    size_t cursor_string;
    size_t cursor_buffer;

    /* Clear buffer */
    for(cursor_buffer=0; cursor_buffer<MAX_CHAR_BUFFER; cursor_buffer++)
        buffer[cursor_buffer] = 0;

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
void reverse(char* input, size_t size)
{
    int counter;

    if(size%2==0)
        size--;
    else
        size -= 2;

    counter = 0;
    while(counter<size)
    {
        /* XOR swap */
        input[counter] ^= input[size];
        input[size] ^= input[counter];
        input[counter++] ^= input[size--];
    }
}
