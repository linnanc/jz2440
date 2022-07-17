
.text
.global _start

_start:
    /* 关闭看门狗 */
    ldr r0, = 0x53000000;
    ldr r1, = 0
    str r1, [r0]

    /* 设置内存：sp 栈 */
    /* 
     * 分辨是 nor/nand 启动
     * 写0到0地址，再读出来
     * 如果得到0，表示0地址上的内容被修改了，它对应 ram，这就是 nand 启动
     * 否则就是nor启动
     */
    mov r1, #0 /* 取0地址 */
    ldr r0, [r1] /* 读出原来r1的值备份 */
    str r1, [r1] /* 写 0->[0] */
    ldr r2, [r1] /* r2 = [0] */
    cmp r1, r2 /* 判断 r1 == r2 ? */
    ldr sp, = 0x40000000 + 4096 /* 先假设是 nor 启动 */
    moveq sp, #4096 /* 如果 r1 == r2 表明 nand 启动 */
    streq r0, [r1] /* 恢复原来的值 */

    bl main

halt:
    b halt
