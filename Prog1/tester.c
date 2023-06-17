#include <stdio.h>
#include <assert.h>
/*
 * This program tests int2bitstr by printing out any 32-bit integer as a 
 * 32-length string of zeros and ones.  The number is also printed in hex
 * with formated binary for correctness checking.
 */
unsigned f2u(float f)
{
  union {
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

   hex[4]='\0';
   do
   {
      int k;
      printf("Enter integer to convert to bits: ");
      assert(scanf("%d", &i) == 1);
      count = int2bitstr(i, str);
      printf("%d : %s\n", i, str);
      printf("#1s in the bit pattern: %d\n",count);

/*
 *   Now print out binary numbers 4 digits at a time for hex confirmation 
 */
      printf("0x%x : ", i);
      for (k=0; k<32; k+=4){
        char *sp = str+k;
        hex[0] = sp[0];
        hex[1] = sp[1];
        hex[2] = sp[2];
        hex[3] = sp[3];
        printf("%s ",hex);
      }
      printf("\n");
      float f = i;
      int2bitstr(f2u(f),str);
      printf("%f : %s\n",f,str);
      printf("exp : %d\n", get_exp_value(f));
   }
   while(i);
   return(0);  /* signal normal completion */
}