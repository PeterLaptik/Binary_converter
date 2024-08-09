#ifndef BINCONVERTER_H_INCLUDED
#define BINCONVERTER_H_INCLUDED

/*************************************************************************************/
/*************************************************************************************/
/***  Converts numbers into binary view as a character sequence of '1' and '0',    ***/
/****          or character sequences of '0' and '1' into numbers                 ****/
/*****                                                                           *****/
/******   function names postfixes:                                           ********/
/*******  '_sp' means bytes are separated by space                           *********/
/******** '_formatted' means sign/exponent/fraction are separated by spaces **********/
/*********                                                                 ***********/
/***********************                                         *********************/
/*************************************************************************************/


#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

    /* Conversion numbers to binary representation as a char sequence of '0' and '1' */
    /* All returned char-pointers have to be freed manually */
    char* int_to_bin(int number);
    char* int_to_bin_sp(int number);
    char* uint_to_bin(unsigned int number);
    char* uint_to_bin_sp(unsigned int number);
    char* float_to_bin(float number);
    char* float_to_bin_sp(float number);
    char* float_to_bin_formatted(float number);
    char* double_to_bin(double number);
    char* double_to_bin_sp(double number);
    char* double_to_bin_formatted(double number);

    /* Getting sign/fraction/exponent for float and double numbers*/
    /* float */
    int is_negative_float(float num);
    float get_fraction_float(float num);
    int get_bin_exponent_float(float num);

    /* double */
    int is_negative_double(double num);
    int get_bin_exponent_double(double num);
    double get_fraction_double(double num);

    /* Reverse conversion: char sequence of '0' and '1' to numbers. Ignore space-separators */
    int bin_to_int(char* bin_val);
    unsigned int bin_to_uint(char* bin_val);
    double bin_to_double(char* bin_val);
    float bin_to_float(char* bin_val);

#ifdef __cplusplus
    }
#endif // __cplusplus


#endif // BINCONVERTER_H_INCLUDED
