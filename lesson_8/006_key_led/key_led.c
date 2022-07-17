/**
 * @file key_led.c
 * @author 使用按键控制 LED
 * @brief 
 * @version 0.1
 * @date 2022-07-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "s3c2440_soc.h"

void delay(volatile int d)
{
    while (d--);
}

int main(void)
{
    int val1;
    int val2;

    /* 设置 GPF4/5/6 配置成输出引脚 */
    /**
     * 3 是 0b11
     * 下面这句代码的意思是把 [9:8] [11:10] [13:12] 位清0
     */
    GPFCON &= ~((3 << 8) | (3 << 10) | (3 << 12));
    /**
     *  下面这句代码的意思是把 [9:8] [11:10] [13:12] 位赋值为 01
     */
    GPFCON |= ((1 << 8) | (1 << 10) |  (1 << 12));

    /* 设置 GPF0/2 为输入引脚*/
    GPFCON &= ~((3 << 0) | (3 << 4));
    GPGCON &= ~(3 << 6);
    
    /* 循环点亮 */
    while (1) {
        val1 = GPFDAT;
        val2 = GPGDAT;
        
        if (val1 & (1<<0)) {
            /* 松开 */
            GPFDAT |= (1 << 6);
        } else {
            /* 按下 */
            GPFDAT &= ~(1 << 6);
        }
        
        if (GPFDAT & (1<<2)) {
            GPFDAT |= (1 << 5);
        } else {
            GPFDAT &= ~(1 << 5);
        }

        if (GPGDAT & (1<<3)) {
            GPFDAT |= (1 << 4);
        } else {
            GPFDAT &= ~(1 << 4);
        }
    }

}
