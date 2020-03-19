#include "../binconverter.h"
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string.h>

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

    printf("Start testing...\n");

    if(!check_compatibility())
        perror("Error: types sizes are not compatible...");

    assert(check_compatibility());
    printf("Compatibility...OK\n\n");

    /* Integer value */
    /* positive number */
    num_int = 500285;
    printf("Integer number (positive):%i\n", num_int);
    printf("Binary representation:%s\n", int_to_bin(num_int));
    printf("Binary representation:%s\n\n", int_to_bin_sp(num_int));
    #ifdef TURN_ON_ASSERTION
        assert(strcmp(int_to_bin(num_int),"00000000000001111010001000111101")==0);
    #endif // TURN_ON_ASSERTION

    /* negative number */
    num_int = -500285;
    printf("Integer number(negative):%i\n", num_int);
    printf("Binary representation:%s\n", int_to_bin(num_int));
    printf("Binary representation:%s\n\n", int_to_bin_sp(num_int));
    #ifdef TURN_ON_ASSERTION
        assert(strcmp(int_to_bin(num_int),"11111111111110000101110111000011")==0);
    #endif // TURN_ON_ASSERTION

    /* Unsigned integer value */
    num_uint = 255;
    printf("Unsigned integer number:%i\n", num_uint);
    printf("Binary representation:%s\n", uint_to_bin(num_uint));
    printf("Binary representation:%s\n\n", uint_to_bin_sp(num_uint));
    #ifdef TURN_ON_ASSERTION
        assert(strcmp(uint_to_bin(num_uint),"00000000000000000000000011111111")==0);
    #endif // TURN_ON_ASSERTION

    /* Float number */
    /* positive number */
    num_float = 105808.5254;
    printf("Float number(positive):%f\n", num_float);
    printf("Binary representation:%s\n", float_to_bin(num_float));
    printf("Binary representation:%s\n\n", float_to_bin_sp(num_float));
//    printf("compare: %d\n", strcmp(float_to_bin(num_float),"01000111110011101010100001000011"));
//    printf("compare: %d\n", strlen("01000111110011101010100001000011"));
//    printf("compare: %d\n", strlen(float_to_bin(num_float)));
//    printf("symbol:%d\n", strcmp(float_to_bin(num_float),"01000111110011101010100001000011"));
    #ifdef TURN_ON_ASSERTION
        assert(strcmp(float_to_bin(num_float),"01000111110011101010100001000011")==0);
    #endif // TURN_ON_ASSERTION

    /* negative number */
    num_float = -105808.5254;
    printf("Float number(negative):%f\n", num_float);
    printf("Binary representation:%s\n", float_to_bin(num_float));
    printf("Binary representation:%s\n\n", float_to_bin_sp(num_float));
    #ifdef TURN_ON_ASSERTION
        assert(strcmp(float_to_bin(num_float),"11000111110011101010100001000011")==0);
    #endif // TURN_ON_ASSERTION

    /* Double number */
    /* positive number */
    num_double = 105808.5254;
    printf("Double number(positive):%f\n", num_double);
    printf("Binary representation:%s\n", double_to_bin(num_double));
    printf("Binary representation:%s\n\n", double_to_bin_sp(num_double));
    #ifdef TURN_ON_ASSERTION
        assert(strcmp(double_to_bin(num_double),"0100000011111001110101010000100001101000000010011101010010010101")==0);
    #endif // TURN_ON_ASSERTION

    /* negative number */
    num_double = -105808.5254;
    printf("Double number(negative):%f\n", num_double);
    printf("Binary representation:%s\n", double_to_bin(num_double));
    printf("Binary representation:%s\n\n", double_to_bin_sp(num_double));
    #ifdef TURN_ON_ASSERTION
        assert(strcmp(double_to_bin(num_double),"1100000011111001110101010000100001101000000010011101010010010101")==0);
    #endif // TURN_ON_ASSERTION

    /* Signs */
    num_float = 15002220.5678;
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

    num_float = 15002220.5678;
    num_double = 15002220.5678;
    /* Fractions and exponents */
    printf("\n");
    printf("Float number: %f\n", num_float);
    printf("Binary representation fl:%s\n", float_to_bin_sp(num_float));
    printf("Binary representation:%s\n", float_to_bin_formatted(num_float));
    printf("Fraction of float %f is %.*f\n", num_float, 15, get_fraction_float(num_float));
    printf("Exponent of float %f is %i\n", num_float, get_bin_exponent_float(num_float));
    sample_fraction = frexp(num_float, &sample_exponent);
    printf("Sample values: fraction: %f, exponent: %d\n", sample_fraction, sample_exponent);

    delta = (get_fraction_float(num_float)*pow(2,get_bin_exponent_float(num_float))-sample_fraction*pow(2, sample_exponent))/get_fraction_float(num_float)*pow(2,get_bin_exponent_float(num_float));
    printf("DELTA=%lf\n\n", delta);

    #ifdef TURN_ON_ASSERTION
        assert(get_fraction_float(num_float)*pow(2,get_bin_exponent_float(num_float))==sample_fraction*pow(2, sample_exponent));
    #endif // TURN_ON_ASSERTION

    printf("Double number: %f\n", num_double);
    printf("Binary representation:%s\n", double_to_bin_sp(num_double));
    printf("Binary representation:%s\n", double_to_bin_formatted(num_double));
    printf("Fraction of double %f is %.*lf\n", num_double, 15, get_fraction_double(num_double));
    printf("Exponent of double %f is %i\n", num_double, get_bin_exponent_double(num_double));
    sample_fraction = frexp(num_double, &sample_exponent);
    printf("Sample values: fraction: %f, exponent: %d\n", sample_fraction, sample_exponent);

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
    num_float = bin_to_float("01001001 00001101 10011010 00000010");
    printf("Value of 01001001 00001101 10011010 00000010: %f\n", num_float);  // 580000.125
    delta = (fabs(num_float) - fabs(580000.125))/580000.125;
    printf("DELTA: %f\n\n", delta);
    #ifdef TURN_ON_ASSERTION
        assert(delta==0);
    #endif // TURN_ON_ASSERTION

    num_float = bin_to_float("11000000 10111000 00000000 00000000");
    printf("Value of 11000000 10111000 00000000 00000000: %f\n", num_float);  // -5.75
    delta = (fabs(num_float) - fabs(-5.75))/-5.75;
    printf("DELTA: %f\n\n", delta);
    #ifdef TURN_ON_ASSERTION
        assert(delta==0);
    #endif // TURN_ON_ASSERTION

    printf("\n%s", "Double reverse translation:");
    num_double = bin_to_double("01000001 10011110 00000000 10101010 00000000 00000000 00000000 00000000");
    printf("\nValue of 01000001 10011110 00000000 10101010 00000000 00000000 00000000 00000000: %f\n", num_double);  // 1.2584e8
    delta = (fabs(num_double) - fabs(1.2584e8))/1.2584e8;
    printf("DELTA: %f\n\n", delta);
    #ifdef TURN_ON_ASSERTION
        assert(delta==0);
    #endif // TURN_ON_ASSERTION

    num_double = bin_to_double("10111111 11101011 01011011 10110110 11000101 00100011 10001011 00100000");
    printf("Value of 10111111 11101011 01011011 10110110 11000101 00100011 10001011 00100000: %f\n", num_double);  // -0.85494555
    delta = (fabs(num_double) - fabs(0.85494555))/-0.85494555;
    printf("DELTA: %f", delta);
    #ifdef TURN_ON_ASSERTION
        assert(delta==0);
    #endif // TURN_ON_ASSERTION

    printf("%s", "\n\nTested...OK");
    return 0;
}
