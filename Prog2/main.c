#include <stdio.h>
#include <assert.h>

int prog2(int i, int j, int *k, int a[5], int *l);

int main() {
        int k = 6;
        int l = 0, res;
        int a[5] = {7, 0, 8, 0, 3};

        res = prog2(6,9,&k,a,&l);
        if(res != 9-6+2) {
                printf("return value should be=%d; got=%d\n", 9-6+2, res);
                assert(0);
        }
        if(k != 36) {
                printf("k*6: expected=36, got=%d\n", k);
                assert(0);
        }
        if(l != 18) {
                printf("array sum: expected=18, got=%d\n", l);
                assert(0);
        }

        printf("j-i+2= %d\n", res);
        printf("k*6= %d\n", k);
        printf("array sum= %d\n\n", l);

        a[1] = l;
        res = prog2(11,17,&k,a,&l);
        if(res != 17-11+2) {
                printf("return value should be=%d; got=%d\n", 17-11+2, res);
                assert(0);
        }
        if(k != 36*6) {
                printf("k*6*6: expected=216, got=%d\n", k);
                assert(0);
        }
        if(l != 18*2) {
                printf("array sum 2: expected=36, got=%d\n", l);
                assert(0);
        }

        printf("j-i+2= %d\n", res);
        printf("k*6*6= %d\n", k);
        printf("array sum 2= %d\n\n", l);
        return 0;
}