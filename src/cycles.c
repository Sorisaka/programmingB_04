/* To compile this file, first try:
   gcc -O -fno-inline -I../include cycles.c ../src/clock64.o
*/
#include <stdio.h>
#include "clock.h"
#define N 100000000
#define data_t int
#define OP +
#define IDENT 1
#define INIT 1
#define INC 1

int s = 0;

data_t run(int n)
{
  int i;
  data_t y = IDENT;
  data_t a = INIT;
  for (i = 0; i < n; i++) {
    y = y OP a ;
    a = a + INC;
  }
  s++;  // to make this function "impure" and block function call motion
  return y;
}

int main(int argc, char *argv[])
{
  int trials = 10;
  double t;
  int k;
  volatile int result; // to make sure assignment is performed
  result = run(N);     // warm up CPU and cache
  for (k = 0; k < trials; k++) {
    start_counter();
    result = run(N);
    t = get_counter();
    printf("%.4f\n", t/N);
  }
  return 0;
}
