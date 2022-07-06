#include <stdio.h>

int main(void)
{
    int a = 0x12345678;
    unsigned char c = (unsigned char)a;
    if (c == 0x78) {
        printf("little endian\n");
    } else if (c == 0x12) {
        printf("big endian\n");
    } else {
        printf("error unknown byte order\n");
    }

    return 0;
}