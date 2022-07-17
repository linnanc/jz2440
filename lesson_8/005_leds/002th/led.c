/**
 * @file led.c
 * @author 循环点亮 LED，使用宏定义寄存器
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
    // volatile unsigned int *pGPFCON = (volatile unsigned int *)0x56000050;
    // volatile unsigned int *pGPFDAT = (volatile unsigned int *)0x56000054;

    int val = 0;
    int tmp;
    /* 设置 GPFCON 让 GPF4/GPF5/GPF6 为输出引脚 */
    
    /**
     * 3 是 0b11
     * 下面这句代码的意思是把 [9:8] [11:10] [13:12] 位清0
     */
    
    // *pGPFCON &= ~((3 << 8) | (3 << 10) | (3 << 12));
    GPFCON &= ~((3 << 8) | (3 << 10) | (3 << 12));

    /**
     *  下面这句代码的意思是把 [9:8] [11:10] [13:12] 位赋值为 01
     */
    // *pGPFCON |= ((1 << 8) | (1 << 10) | (1 << 12));
    GPFCON |= ((1 << 8) | (1 << 10) | (1 << 12));

    /* 循环点亮 */
    while (1) {
        tmp = ~val;
        tmp &= 7;
        // *pGPFCON &= ~(7 << 4);
        // *pGPFDAT |= (tmp << 4);
        GPFCON &= ~(7 << 4);
        GPFDAT |= (tmp << 4);
        delay(100000);
        val++;
        if (val == 8) {
            val = 0;
        }
    }

    return 0;
}