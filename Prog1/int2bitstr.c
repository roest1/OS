#include <stdio.h>
#include <assert.h>
/*
 * This program tests int2bitstr by printing out any 32-bit integer as a
 * 32-length string of zeros and ones.  The number is also printed in hex
 * with formated binary for correctness checking.
 */
unsigned f2u(float f)
{
    union
    {
        unsigned u;
        float f;
    } v;
    v.u = 0;
    v.f = f;
    return v.u;
}

int main(void)
{
    char str[33], hex[5];
    int count, i;
    int int2bitstr(int I, char *str);
    int get_exp_value(float f);

    hex[4] = '\0';
    do
    {
        int k;
        printf("Enter integer to convert to bits: ");
        assert(scanf("%d", &i) == 1);
        count = int2bitstr(i, str);
        printf("%d : %s\n", i, str);
        printf("#1s in the bit pattern: %d\n", count);

        /*
         *   Now print out binary numbers 4 digits at a time for hex confirmation
         */
        printf("0x%x : ", i);
        for (k = 0; k < 32; k += 4)
        {
            char *sp = str + k;
            hex[0] = sp[0];
            hex[1] = sp[1];
            hex[2] = sp[2];
            hex[3] = sp[3];
            printf("%s ", hex);
        }
        printf("\n");
        float f = i;
        int2bitstr(f2u(f), str);
        printf("%f : %s\n", f, str);
        printf("exp : %d\n", get_exp_value(f));
    } while (i);
    return (0); /* signal normal completion */
}

/*
 * 1.) Stores binary representation of I in str[32]
 * 2.) Takes input I, an integer, and outputs the number of "set" ones in I's binary representation
 */
/*
 * FORBIDDEN OPERATIONS : Switch statements, function calls, macro invocations,
 * addition, subtraction, division, modulus, and multiplication
 */

int int2bitstr(int I, char *str)
{
    int count = 0;  // count of 1's in binary(I)
    int stridx = 0; // index to insert in str[]
    for (int i = 31; i >= 0; i--)
    {
        // first char in str = MSB in I
        if (I & (1 << i))
        { // if 2^i position is set,
            str[stridx] = '1';
            count++;
        }
        else
        {
            str[stridx] = '0';
        }
        stridx++;
    }
    str[32] = '\0'; // strings are null-terminated
    return count;
}
/*
 * For input f, return exponent value E(f)
 * The bias of float is (single precision) = 127
 * For special values, such as infinity, we can return -128
 */
/*
 * FORBIDDEN OPERATIONS : Loops, switch statements, macro incovations
 * function calls (except unsigned f2u(float f)), division, modulus, and
 * multiplication, relative comparison operators (<, >, <=, >=)
 */
int get_exp_value(float f)
{
    unsigned f2u(float f);    // instantiate float to unsigned integer funciton
    unsigned int ui = f2u(f); // call the function by passing the argument
    // exp bits are the 8 bits located to the right of the sign bit
    ui = ui << 1;  // discard the sign bit
    ui = ui >> 24; // discard the fractional part
    if (ui == 0)
    {
        return 1 - 127;
    } // denormalized
    else if (ui == 1)
    {
        return -128;
    }                // special value
    return ui - 127; // normalized
}