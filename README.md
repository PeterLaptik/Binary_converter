# Binary_converter
Simple decimal to binary (binary to decimal)  number converter.

### Example:
```C
#include <stdio.h>
#include "binconverter.h"

int main(int argc, char **argv)
{
    printf("\n%s\n", int_to_bin_sp(123456789));       // 00000111 01011011 11001101 00010101
    printf("%s\n", float_to_bin_formatted(-1.258));   // 1 01111111 01000010000011000100101
    printf("%f\n", bin_to_float("01000010 01001010 00111000 01010010"));  // 50.555
    return 0;
}
```
