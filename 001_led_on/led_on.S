/*
 * 点亮LED：GPF4
 */

.text
.global _start
 /* 
  * 配置GPF4为输出引脚
  * 把0x100写到地址0x56000050
  */

_start:
    ldr r1, = 0x56000050
    ldr r0, = 0x400
    str r0, [r1]
    @ mov r0, #0x100

 /* 
  * 设置GPF4输出高电平
  * 把0写到地址0x56000054
  */
    ldr r1, = 0x56000054
    ldr r0, = 0
    str r0, [r1]

/*
 * 死循环
 */
halt:
    b halt
