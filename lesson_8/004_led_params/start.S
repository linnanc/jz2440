/**
 * @file start.S
 * @author linnanc
 * @brief led_on 入口
 * @version 0.1
 * @date 2022-07-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */
 .text
 .global _start

 _start:
    /* 设置内存 sp 栈 */
    ldr sp, = 4096 /* nand 启动 */
    @ ldr sp, = 0x40000000 + 4096 /* nor 启动 */

    /* 用 r0 传递参数4 */
    mov r0, #4
    bl led_on

    /* r0 传递参数100000 */
    ldr r0, = 100000
    bl delay

    mov r0, #5
    bl led_on


halt:
    b halt
