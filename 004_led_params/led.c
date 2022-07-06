

/**
 * @brief 
 * 
 * @param d 延时时间，volatile 避免编译器优化
 */
void delay(volatile int d)
{
    while (d--);
}

int led_on(int which)
{

    unsigned int *gpf_con = (unsigned int *)0x56000050;
    unsigned int *gpf_dat = (unsigned int *)0x56000054;
    if (which == 4) {
        *gpf_con = 0x100;
    } else if (which == 5) {
        *gpf_con = 0x400;
    }

    *gpf_dat = 0;
    return 0;
}
