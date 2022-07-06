
void delay(volatile int d)
{
    while (d--);
}

int main(void)
{
    unsigned int *gpf_con = (unsigned int *)0x56000050;
    unsigned int *gpf_dat = (unsigned int *)0x56000054;
    int value = 0;
    int tmp;
    /* 设置 GPF4/5/6 配置成输出引脚 */
    *gpf_con &= ~((3 << 8) | (3 << 10) | (3 << 12));
    *gpf_con |= ((1 << 8) | (1 << 10) |  (1 << 12));

    /* 循环点亮 */
    while (1) {
        tmp = ~value;
        tmp &= 7;
        *gpf_dat &= ~(7 << 4);
        *gpf_dat |= (tmp << 4);
        delay(100000);
        value++;
        if (value == 8) {
            value = 0;
        }
    }

}
