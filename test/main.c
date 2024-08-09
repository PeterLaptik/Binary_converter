#include "../binconverter.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string.h>

#if defined(_MSC_VER) && !defined(__clang__) && !defined(__INTEL_COMPILER)
    #define _CRTDBG_MAP_ALLOC
    #include <stdlib.h>
    #include <crtdbg.h>
#endif // #if _MSC_VER && !__INTEL_COMPILER



/********************************************/
/********         Test         **************/
/********************************************/

#define TURN_ON_ASSERTION

int main(int argc, char **argv)
{
    int num_int;
    unsigned int num_uint;
    float num_float;
    double num_double;
    int sample_exponent;
    double sample_fraction;
    double delta;

    char *result;
    char *result_sp;

    printf("Start testing...\n");

    /* Integer value */
    /* positive number */
    /*
    500285
    00000000000001111010001000111101
    00000000 00000111 10100010 00111101
    */
    num_int = 500285;
    result = int_to_bin(num_int);
    result_sp = int_to_bin_sp(num_int);
    printf("Integer number (positive):%i\n", num_int);
    printf("Binary representation:'%s'\n", result);
    printf("Binary representation:'%s'\n\n", result_sp);
    #ifdef TURN_ON_ASSERTION
        assert(strcmp(result, "00000000000001111010001000111101")==0);
        assert(strcmp(result_sp,"00000000 00000111 10100010 00111101")==0);
    #endif // TURN_ON_ASSERTION
    free(result);
    free(result_sp);

    /* negative number */
    /*
    -500285
    11111111111110000101110111000011
    11111111 11111000 01011101 11000011
    */
    num_int = -500285;
    result = int_to_bin(num_int);
    result_sp = int_to_bin_sp(num_int);
    printf("Integer number(negative):%i\n", num_int);
    printf("Binary representation:'%s'\n", result);
    printf("Binary representation:'%s'\n\n", result_sp);
    #ifdef TURN_ON_ASSERTION
        assert(strcmp(result,"11111111111110000101110111000011")==0);
        assert(strcmp(result_sp,"11111111 11111000 01011101 11000011")==0);
    #endif // TURN_ON_ASSERTION
    free(result);
    free(result_sp);

    /* Unsigned integer value */
    /*
    255
    00000000000000000000000011111111
    00000000 00000000 00000000 11111111
    */
    num_uint = 255;
    result = uint_to_bin(num_uint);
    result_sp = uint_to_bin_sp(num_uint);
    printf("Unsigned integer number:%i\n", num_uint);
    printf("Binary representation:'%s'\n", result);
    printf("Binary representation:'%s'\n\n", result_sp);
    #ifdef TURN_ON_ASSERTION
        assert(strcmp(result,"00000000000000000000000011111111")==0);
        assert(strcmp(result_sp,"00000000 00000000 00000000 11111111")==0);
    #endif // TURN_ON_ASSERTION
    free(result);
    free(result_sp);

    /* Float number */
    /* positive number */
    /*
    105808.5254
    01000111110011101010100001000011
    01000111 11001110 10101000 01000011
    */
    num_float = 105808.5254f;
    result = float_to_bin(num_float);
    result_sp = float_to_bin_sp(num_float);
    printf("Float number(positive):%f\n", num_float);
    printf("Binary representation:'%s'\n", result);
    printf("Binary representation:'%s'\n\n", result_sp);
    #ifdef TURN_ON_ASSERTION
        assert(strcmp(result,"01000111110011101010100001000011")==0);
        assert(strcmp(result_sp,"01000111 11001110 10101000 01000011")==0);
    #endif // TURN_ON_ASSERTION
    free(result);
    free(result_sp);

    /* negative number */
    num_float = -105808.5254f;
    result = float_to_bin(num_float);
    result_sp = float_to_bin_sp(num_float);
    printf("Float number(negative):%f\n", num_float);
    printf("Binary representation:'%s'\n", result);
    printf("Binary representation:'%s'\n\n", result_sp);
    #ifdef TURN_ON_ASSERTION
        assert(strcmp(result,"11000111110011101010100001000011")==0);
        assert(strcmp(result_sp,"11000111 11001110 10101000 01000011")==0);
    #endif // TURN_ON_ASSERTION
    free(result);
    free(result_sp);

    /* Double number */
    /* positive number */
    /*
    105808.5254
    0100000011111001110101010000100001101000000010011101010010010101
    01000000 11111001 11010101 00001000 01101000 00001001 11010100 10010101
    */
    num_double = 105808.5254;
    result = double_to_bin(num_double);
    result_sp = double_to_bin_sp(num_double);
    printf("Double number(positive):%f\n", num_double);
    printf("Binary representation:'%s'\n", result);
    printf("Binary representation:'%s'\n\n", result_sp);
    #ifdef TURN_ON_ASSERTION
        assert(strcmp(result,"0100000011111001110101010000100001101000000010011101010010010101")==0);
        assert(strcmp(result_sp,"01000000 11111001 11010101 00001000 01101000 00001001 11010100 10010101")==0);
    #endif // TURN_ON_ASSERTION
    free(result);
    free(result_sp);

    /* negative number */
    /*
    -105808.5254
    1100000011111001110101010000100001101000000010011101010010010101
    11000000 11111001 11010101 00001000 01101000 00001001 11010100 10010101
    */
    num_double = -105808.5254;
    result = double_to_bin(num_double);
    result_sp = double_to_bin_sp(num_double);
    printf("Double number(negative):%f\n", num_double);
    printf("Binary representation:'%s'\n", result);
    printf("Binary representation:'%s'\n\n", result_sp);
    #ifdef TURN_ON_ASSERTION
        assert(strcmp(result,"1100000011111001110101010000100001101000000010011101010010010101")==0);
        assert(strcmp(result_sp,"11000000 11111001 11010101 00001000 01101000 00001001 11010100 10010101")==0);
    #endif // TURN_ON_ASSERTION
    free(result);
    free(result_sp);

    /* Signs */
    num_float = 15002220.5678f;
    num_double = -15002220.5678;
    if(is_negative_float(num_float))
        printf("Value of float %f is negative\n", num_float);
    else
        printf("Value of float %f is positive\n", num_float);

    if(is_negative_double(num_double))
        printf("Value of double %f is negative\n", num_double);
    else
        printf("Value of double %f is positive\n", num_double);

    #ifdef TURN_ON_ASSERTION
        assert(is_negative_float(num_float)==0);
        assert(is_negative_double(num_double)!=0);
    #endif // TURN_ON_ASSERTION


    num_float = 15002220.5678f;
    num_double = 15002220.5678;
    /* Fractions and exponents */
    result = float_to_bin_sp(num_float);
    result_sp = float_to_bin_formatted(num_float);
    printf("\n");
    printf("Float number: %f\n", num_float);
    printf("Binary representation fl: '%s'\n", result);
    printf("Binary representation: '%s'\n", result_sp);
    printf("Fraction of float %f is %.*f\n", num_float, 15, get_fraction_float(num_float));
    printf("Exponent of float %f is %i\n", num_float, get_bin_exponent_float(num_float));
    sample_fraction = frexp(num_float, &sample_exponent);
    printf("Sample values: fraction: %f, exponent: %d\n", sample_fraction, sample_exponent);
    free(result);
    free(result_sp);

    delta = (get_fraction_float(num_float)*pow(2,get_bin_exponent_float(num_float))-sample_fraction*pow(2, sample_exponent))/get_fraction_float(num_float)*pow(2,get_bin_exponent_float(num_float));
    printf("DELTA=%lf\n\n", delta);

    #ifdef TURN_ON_ASSERTION
        assert(get_fraction_float(num_float)*pow(2,get_bin_exponent_float(num_float))==sample_fraction*pow(2, sample_exponent));
    #endif // TURN_ON_ASSERTION

    printf("Double number: %f\n", num_double);
    result = double_to_bin_sp(num_double);
    result_sp = double_to_bin_formatted(num_double);
    printf("Binary representation: '%s'\n", result);
    printf("Binary representation: '%s'\n", result_sp);
    printf("Fraction of double %f is %.*lf\n", num_double, 15, get_fraction_double(num_double));
    printf("Exponent of double %f is %i\n", num_double, get_bin_exponent_double(num_double));
    sample_fraction = frexp(num_double, &sample_exponent);
    printf("Sample values: fraction: %f, exponent: %d\n", sample_fraction, sample_exponent);
    free(result);
    free(result_sp);

    delta = (get_fraction_double(num_double)*pow(2,get_bin_exponent_double(num_double))-sample_fraction*pow(2, sample_exponent))/get_fraction_double(num_double)*pow(2,get_bin_exponent_double(num_double));
    printf("DELTA=%lf\n\n", delta);
    #ifdef TURN_ON_ASSERTION
        assert(get_fraction_double(num_double)*pow(2,get_bin_exponent_double(num_double))==sample_fraction*pow(2, sample_exponent));
    #endif // TURN_ON_ASSERTION


    /* Check reverse translation */
    printf("\n%s", "Integer reverse translation:");
    printf("\nValue of 00000000 00000000 00001001 11000100: %d\n", bin_to_int(" 00000000 00000000 00001001 11000100"));  // 2500
    #ifdef TURN_ON_ASSERTION
        assert(bin_to_int(" 00000000 00000000 00001001 11000100")==2500);
    #endif // TURN_ON_ASSERTION

    printf("Value of 11110001 00001100 11111000 11110000: %d\n", bin_to_int("11110001 00001100 11111000 11110000"));  // -250808080
    #ifdef TURN_ON_ASSERTION
        assert(bin_to_int(" 11110001 00001100 11111000 11110000")==-250808080);
    #endif // TURN_ON_ASSERTION

    printf("\n%s", "Unsigned integer reverse translation:");
    printf("\nValue of 00101001 00100000 10001101 01010010: %d\n", bin_to_uint("00101001 00100000 10001101 01010010"));  // 689999186
    #ifdef TURN_ON_ASSERTION
        assert(bin_to_int(" 00101001 00100000 10001101 01010010")==689999186);
    #endif // TURN_ON_ASSERTION

    printf("\n%s", "Float reverse translation:\n");
    printf("Value of 01001001 00001101 10011010 00000010: %f\n", bin_to_float("01001001 00001101 10011010 00000010"));  // 580000.125
    #ifdef TURN_ON_ASSERTION
        assert(bin_to_float("01001001 00001101 10011010 00000010")==580000.125);
    #endif // TURN_ON_ASSERTION

    printf("Value of 11000000 10111000 00000000 00000000: %f\n", bin_to_float("11000000 10111000 00000000 00000000"));  // -9.87654
    #ifdef TURN_ON_ASSERTION
        assert(bin_to_float("11000000 10111000 00000000 00000000")==-5.75);
    #endif // TURN_ON_ASSERTION

    printf("\n%s", "Double reverse translation:");
    printf("\nValue of 01000001 10011110 00000000 10101010 00000000 00000000 00000000 00000000: %lf\n", bin_to_double("01000001 10011110 00000000 10101010 00000000 00000000 00000000 00000000"));  // 1.2584e8
    #ifdef TURN_ON_ASSERTION
        assert(bin_to_double(" 01000001 10011110 00000000 10101010 00000000 00000000 00000000 00000000")==1.2584e8);
    #endif // TURN_ON_ASSERTION

    printf("Value of 10111111 11101011 01011011 10110110 11000101 00100011 10001011 00100000: %lf\n", bin_to_double("10111111 11101011 01011011 10110110 11000101 00100011 10001011 00100000"));  // -0.85494555
    #ifdef TURN_ON_ASSERTION
        assert(bin_to_double(" 10111111 11101011 01011011 10110110 11000101 00100011 10001011 00100000")==-0.85494555);
    #endif // TURN_ON_ASSERTION


#if defined(_MSC_VER) && !defined(__INTEL_COMPILER) && !defined(__INTEL_COMPILER)
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    _CrtDumpMemoryLeaks();
#endif


    return 0;
}
