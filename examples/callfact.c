#include <stdio.h>
extern long _t4Fact4FACT(long);
int main(){
    long int n;
    scanf("%ld",&n);
    printf("Factorial of %ld is %ld",n, _t4Fact4FACT(n));
    return 0;
}