/**
 * @file start.S
 * @author linnanc
 * @brief main 入口
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

    /* 调用 main */
    bl main

halt:
    b halt
