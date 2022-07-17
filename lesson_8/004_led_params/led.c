/**
 * @file led.c
 * @author linnanc
 * @brief 
 * @version 0.1
 * @date 2022-07-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */

void delay(volatile int d)
{
    while (d--);
}

int led_on(int which)
{
    unsigned int *pGPFCON = (unsigned int *)0x56000050;
    unsigned int *pGPFDAT = (unsigned int *)0x56000054;

    if (which == 4) {
        /* 配置 GPF4 为输出引脚 */
        *pGPFCON = 0x100;
    } else if (which == 5) {
        /* 配置 GPF5 为输出引脚 */
        *pGPFCON = 0x400;
    }

    /* 设置 GPF4/5 输出0 */
    *pGPFDAT = 0;
    
    return 0;
}