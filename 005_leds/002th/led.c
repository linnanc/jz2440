
#define GPFCON (*((volatile unsigned int *)0x56000050))
#define GPFDAT (*((volatile unsigned int *)0x56000054))

void delay(volatile int d)
{
    while (d--);
}

int main(void)
{
    int value = 0;
    int tmp;
    /* 设置 GPF4/5/6 配置成输出引脚 */
    GPFCON &= ~((3 << 8) | (3 << 10) | (3 << 12));
    GPFCON |= ((1 << 8) | (1 << 10) |  (1 << 12));
    
    /* 循环点亮 */
    while (1) {
        tmp = ~value;
        tmp &= 7;
        GPFDAT &= ~(7 << 4);
        GPFDAT |= (tmp << 4);
        delay(100000);
        value++;
        if (value == 8) {
            value = 0;
        }
    }

}
