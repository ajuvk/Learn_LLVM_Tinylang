#include <stdio.h>

extern long _t3Gcd3GCD(long, long);

int main(int argc, char *argv[]) {
  long x;
  long y;
  scanf("%ld",&x);
  scanf("%ld",&y);

  printf("%ld",_t3Gcd3GCD(x,y));
  return 0;
}