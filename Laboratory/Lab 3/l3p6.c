#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
    primul byte inversat cu ultimul
*/


unsigned int reverse(unsigned int b) {
  
   unsigned int rev =0;

   while(b>0){

       rev<<=1;

       if((b&1) ==1)
            rev^=1;

        b>>=1;
   }

   return rev;
}

int main()

{   
    unsigned int b=12;
    printf("%d", reverse(b));
    return 0;
}