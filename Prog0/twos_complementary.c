#include <stdio.h>

int main() {
        int i = 100;
        int i_sign = i >> 31;
        i_sign = i_sign & 0x1;
        printf("Sign bit of %d: %d\n", i, i_sign);

        int j = -1000;
        int j_sign = j >> 31;
        j_sign = j_sign & 0x1;
        printf("Sign bit of %d: %d\n", j, j_sign);

        return 0;
}