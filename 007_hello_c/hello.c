#include <stdio.h>

#define MAX 20
#define MIN 10

#define _DEBUG
#define SETBIT(x) (1 << x)

int main(int argc, char *argv[])
{
    printf("Hello World\n");
    printf("MAX = %d, MIN = %d, MAX + MIN = %d\n", MAX, MIN, MAX + MIN);

#ifdef _DEBUG
    printf("SETBIT(5) = %d, SETBIT(6) = %d\n", SETBIT(5), SETBIT(6));
    printf("SETBIT(SETBIT(2)) = %d\n", SETBIT(SETBIT(2)));
#endif
    return 0;    
}