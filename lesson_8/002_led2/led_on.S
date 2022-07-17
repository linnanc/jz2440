/**
 * @file led_on.S
 * @author linnanc
 * @brief 点亮 LED2：GPF5
 * @version 0.1
 * @date 2022-07-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */
.text
.global _start

/**
 * 配置 GPF5 为输出引脚
 * 把 0x400 写到地址 0x56000050
 */
_start:
    ldr r1, = 0x56000050
    @ mov r0, #0x400
    ldr r0, = 0x400
    str r0, [r1]

/**
 * 设置 GPF5 输出高电平
 * 把 0 写到地址 0x56000054
 */
    ldr r1, = 0x56000054
    @ mov r0, #0
    ldr r0, = 0
    str r0, [r1]

/**
 * 死循环
 */
halt:
    b halt