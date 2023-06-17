#include <stdio.h>
#include <assert.h>

typedef struct {
        char block[4];
        char valid;
        char tag;
} line;

unsigned char check_cache(line cache[4], unsigned char addr);

int main() {
        line cache[4];

        cache[0].valid = 0x1;
        cache[0].tag = 0x0;
        cache[0].block[0] = 0xA;
        cache[0].block[1] = 0xB;
        cache[0].block[2] = 0xC;
        cache[0].block[3] = 0xD;

        cache[1].valid = 0x0;
        cache[1].tag = 0x7;

        cache[2].valid = 0x0;
        cache[2].tag = 0xA;

        cache[3].valid = 0x1;
        cache[3].tag = 0x3;
        cache[3].block[0] = 0x2A;
        cache[3].block[1] = 0x2B;
        cache[3].block[2] = 0x2C;
        cache[3].block[3] = 0x2D;

        unsigned char res;
        int input;
        unsigned char uc;
        do {
                printf("Enter a memory address (0-255) for cache access: ");
                assert(scanf("%d", &input) == 1);
                uc = (unsigned char)input;
                res = check_cache(cache, uc);
                if(res == 0xFF)
                        printf("cache MISS for address 0x%x\n", uc);
                else
                        printf("cache HIT for 0x%x: 0x%x\n", uc, res);
        } while(uc);
        return(0);
}