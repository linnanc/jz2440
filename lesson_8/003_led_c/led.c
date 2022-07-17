/**
 * @file led.c
 * @author linnanc
 * @brief C 程序点亮 LED1
 * @version 0.1
 * @date 2022-07-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */

int main()
{
    unsigned int *pGPFCON = (unsigned int *)0x56000050;
    unsigned int *pGPFDAT = (unsigned int *)0x56000054;

    /* 配置 GPF4 为输出引脚 */
    *pGPFCON = 0x100;

    /* 设置 GPF4 输出0 */
    *pGPFDAT = 0;

    return 0;
}