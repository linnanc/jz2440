# 第8课 第1个ARM裸板程序及引申

## 1. 编写第1个裸板程序点亮`LED`

点亮`LED`，分为三步：

1. 看原理图确定控制`LED`的引脚
2. 看主芯片手册确定如何设置/控制引脚
3. 编写程序

### 1.1 看原理图

在原理图中，`LED`灯的符号如下图：

![image-20220628063227419](https://cdn.jsdelivr.net/gh/linnanc/image@main/img/image-20220628063227419.png)



查看`JZ2440`原理图，找到`LED`（`page 3`）部分，如下图：

![image-20220628063432888](https://cdn.jsdelivr.net/gh/linnanc/image@main/img/image-20220628063432888.png)

当`nLED1`输出高电平时，没有压差，`LED`会熄灭，当`nLED1`输出低电平时有压差，`LED`会点亮。上图中，`nLED1`称为`net`网络，同名的`net`表示连接在一起，`n`表示低电平有效，在上图中表示低电平点亮`LED`。根据`net`，可以找到，`nLED1`与`S3C2440`的`GPF4`连接。（`page 14`）

![image-20220628064109370](https://cdn.jsdelivr.net/gh/linnanc/image@main/img/image-20220628064109370.png)

找到了控制`nLED1`的引脚，接下来就需要查看`datasheet`如何来配置它了。

### 1.2 看芯片手册

一个管脚可以配置成输出引脚，也可以配置成输入引脚。我们需要将`GPF4`配置成输出引脚，让它输出低电平。在芯片手册中，找到`GPF`控制寄存器部分，如下图：

![image-20220628064939702](https://cdn.jsdelivr.net/gh/linnanc/image@main/img/image-20220628064939702.png)

`GPF`有`GPFCON`和`GPFDAT`、`GPFUP`、`Reserved`四个寄存器。`GPFCON`是`GPF`的配置寄存器，如下图，配置`GPFCON`可以将`GPF4`设置为`Output`输出引脚。

![image-20220628065147057](https://cdn.jsdelivr.net/gh/linnanc/image@main/img/image-20220628065147057.png)

配置`GPFDAT`的值为0，则可以让`GPF4`输出低电平，如下图：

![image-20220628065329805](https://cdn.jsdelivr.net/gh/linnanc/image@main/img/image-20220628065329805.png)

### 插话：`S3C2440`框架与启动过程：

`S3C2440`是一个`SOC`（`system on chip`）芯片，它里面包含有`CPU`，`GPIO`（`General Purpose Input Output`）控制器，`4K`的`SRAM`，`Nand`控制器等。如下图所示：

![image-20220628070508489](https://cdn.jsdelivr.net/gh/linnanc/image@main/img/image-20220628070508489.png)

`S3C2440`可以设置为`NOR`启动也可以设置为`NAND`启动。

* `NOR`启动流程：
  * `NOR Flash`基地址为0。片内`RAM`地址为`0x4000000`
  * `CPU`读出`NOR`上第1条指令（前4字节）执行
  * `CPU`继续读其它指令执行
* `NAND`启动流程：
  * 片内`4K` `RAM`基地址为0。`NOR`不可访问。
  * 2440的硬件会把`NAND`前`4K`的内容复制到片内`RAM`内存，然后`CPU`从0地址取出第1条指令执行。

### 1.3 编写代码

通过看原理图我们知道了要点亮`LED1`需要让`GPF4`输出低电平，而让`GPF4`输出低电平，需要先将`GPF4`配置为输出引脚，再设置`GPFDATA`的值为0。

![image-20220703111023243](https://cdn.jsdelivr.net/gh/linnanc/image@main/img/image-20220703111023243.png)

第`[9:8]`位为01配置成输出引脚。`GPFCON`的地址是0x56000050，把0x100写到地址0x56000050就将`GPF4`配置成了输出引脚。把0写到`GPFDATA`0x56000054上，就是让`GPF4`输出低电平。在写代码时，要用到几条汇编指令，如下：

```assembly
/* LDR 读内存 */
LDR R0, [R1] /* 假设R1的值是x，读取地址x上的数据（4字节）保存到R0中 */
/* STR 写内存 */
STR RO, [R1] /* 假设R1的值是x，把R0的值写到地址x上（4字节）*/
/* B 跳转 */
/* MOV */ 
MOV R0, R1 /* 把R1的值赋给R0 R0 = R1 */
MOV R0, #0x100 /* R0 = 0x100 */
LDR R0, = 0x12345678 /* 伪指令，会被拆分为几条真正的ARM指令 R0 = 0x12345678 */
```

编写代码如下：

```assembly
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
    ldr r1, = 56000050
    ldr r0, = 0x100
    @ mov r0, #0x100

 /* 
  * 设置GPF4输出高电平
  * 把0写到地址0x56000054
  */
    ldr r1, = 0x56000054
    ldr r0, = 0

/*
 * 死循环
 */
halt:
    b halt

```

编译：

```SHELL
arm-linux-gcc -c -o led_on.o led_on.S
```

链接：

```shell
 arm-linux-ld -Ttext 0 led_on.o -o led_on.elf
```

生成`bin`文件

```SHELL
arm-linux-objcopy -O binary -S led_on.elf led_on.bin
```

编写`makefile`就不用每次都输入上面命令编译了。

```MAKEFILE
all:
	arm-linux-gcc -c -o led_on.o led_on.S
	arm-linux-ld -Ttext 0 led_on.o -o led_on.elf
	arm-linux-objcopy -O binary -S led_on.elf led_on.bin

clean:
	rm *.bin *.o *.elf
```

### 1.4 烧录

打卡`cmdline`，输入`oflash led_on.bin`，烧录步骤如下：

```shell
Z:\projects\jz2440\001_led_on>oflash led_on.bin

+---------------------------------------------------------+
|   Flash Programmer v1.5.2 for OpenJTAG of www.100ask.net  |
|   OpenJTAG is a USB to JTAG & RS232 tool based FT2232   |
|   This programmer supports both of S3C24X0 & S3C6410    |
|   Author: Email/MSN(thisway.diy@163.com), QQ(17653039)  |
+---------------------------------------------------------+
Usage:
1. oflash, run with cfg.txt or prompt
2. oflash [file], write [file] to flash with prompt
3. oflash [-f config_file]
4. oflash [jtag_type] [cpu_type] [flash_type] [read_or_write] [offset] [file]
Select the JTAG type:
0. OpenJTAG
1. Dongle JTAG(parallel port)
2. Wiggler JTAG(parallel port)
Enter the number: 0
Select the CPU:
0. S3C2410
1. S3C2440
2. S3C6410
Enter the number: 1

device: 4 "2232C"
deviceID: 0x14575118
SerialNumber: FT5E544bA
Description: USB<=>JTAG&RS232 AS3C2440 detected, cpuID = 0x0032409d

[Main Menu]
 0:Nand Flash prog     1:Nor Flash prog   2:Memory Rd/Wr     3:Exit
Select the function to test:0

[NAND Flash JTAG Programmer]
Scan nand flash:
Device 0: NAND 256MiB 3,3V 8-bit, sector size 128 KiB
Total size: 256 MiB
 0:Nand Flash Program      1:Nand Flash Print BlkPage   2:Exit
Select the function to test :0

[NAND Flash Writing Program]

Source size: 0x24

Available target block number: 0~2047
Input target block number:0
target start block number     =0
target size        (0x20000*1) =0x20000
STATUS:
Ep

Z:\projects\jz2440\001_led_on>
```

## 2. 汇编与机器码

将`led_on.elf`反汇编：

```SHELL
arm-linux-objdump -D led_on.elf > led_on.dis
```

查看如下：

```assembly
led_on.elf:     file format elf32-littlearm

Disassembly of section .text:

00000000 <_start>:
   0:	e59f1014 	ldr	r1, [pc, #20]	; 1c <.text+0x1c>
   4:	e3a00c01 	mov	r0, #256	; 0x100
   8:	e5810000 	str	r0, [r1]
   c:	e59f100c 	ldr	r1, [pc, #12]	; 20 <.text+0x20>
  10:	e3a00000 	mov	r0, #0	; 0x0
  14:	e5810000 	str	r0, [r1]

00000018 <halt>:
  18:	eafffffe 	b	18 <halt>
  1c:	56000050 	undefined
  20:	56000054 	undefined
```

汇编码的注解：

`ARM`指令集：

| `User`模式 | `SVC`模式 | `IRQ`模式 | `FIQ`模式 | `APCS` |
| ---------- | --------- | --------- | --------- | ------ |
| `R0`       | `R0`      | `R0`      | `R0`      | `a1`   |
| `R1`       | `R1`      | `R1`      | `R1`      | `a2`   |
| `R2`       | `R2`      | `R2`      | `R2`      | `a3`   |
| `R3`       | `R3`      | `R3`      | `R3`      | `a4`   |
| `R4`       | `R4`      | `R4`      | `R4`      | `v1`   |
| `R5`       | `R5`      | `R5`      | `R5`      | `v2`   |
| `R6`       | `R6`      | `R6`      | `R6`      | `v3`   |
| `R7`       | `R7`      | `R7`      | `R7`      | `v4`   |
| `R8`       | `R8`      | `R8`      | `R8_fiq`  | `v5`   |
| `R9`       | `R9`      | `R9`      | `R9_fiq`  | `v6`   |
| `R10`      | `R10`     | `R10`     | `R10_fiq` | `s1`   |
| `R11`      | `R11`     | `R11`     | `R11_fiq` | `fp`   |
| `R12`      | `R12`     | `R12`     | `R12_fiq` | `ip`   |
| `R13`      | `R13_svc` | `R13_irq` | `R13_fiq` | `sp`   |
| `R14`      | `R14_svc` | `R14_irq` | `R14_fiq` | `lr`   |
| `R15/PC`   |           |           |           | `pc`   |

`pc`是程序计数器，`lr`是返回地址，`sp`是栈指针。`pc` = 当前指令 + 8，`ARM`系统中`CPU`是以流水线方式执行，当前执行地址`A`的指令，已经在对地址`A`+4的指令进行译码，已经在读取地址`A`+8的指令。

作业：修改`led_on.S`点亮`LED2`

要点亮`LED2`，需要让`GPF5`输出低电平，将`GPFCON`的第`[11:10]`位设置为1，如下：

```assembly
_start:
    ldr r1, = 56000050
    @ldr r0, = 0x100
    ldr r0, = 0x500
```

查看反汇编，我们直接修改这段`e3a00c01`这段机器码，将0x100修改成0x500。要先弄明白这段机器码中哪些位代表0x100，查看`ARM`架构手册`ARM Architecture Reference Manual.pdf`，`MOV`指令如下：

![image-20220703153153584](https://cdn.jsdelivr.net/gh/linnanc/image@main/img/image-20220703153153584.png)

`[11:0]`位上的值表示立即数，其中`[11:8] bit`的值为`Rotate_imm`，`[7:0] bit`的值为`Immed_8`。

立即数 = `immed_8` 循环右移 `(2 * rotate_imm)`

## 3. 字节序和位操作

* 大端字节序：高位存放在低地址
* 小端字节序：低位存放在低地址，一般的`ARM`芯片都是使用的小端字节序。

作业：编写代码判断当前`CPU`的字节序。

```c
#include <stdio.h>

int main(void)
{
    int a = 0x12345678;
    unsigned char c = (unsigned char)a;
    if (c == 0x78) {
        printf("little endian\n");
    } else if (c == 0x12) {
        printf("big endian\n");
    } else {
        printf("error: unknown byte order\n");
    }

    return 0;
}
```

* 位操作
  * 移位：
    * 左移 >>
    * 右移 <<
  * 取反 ~
  * 位与 &
  * 位或 |

## 4. 编写`C`代码点亮`LED`

编写`C`代码很简单，如下：

```c
void main(void)
{
    unsigned int *gpf_con = 0x56000050;
    unsigned int *gpf_dat = 0x56000054;

    /* 配置GPF4为输出引脚 */
    *gpf_con = 0x100;

    /* 配置GPF4输出低电平 */
    *gpf_dat = 0;
}
```

有两个问题：1. 我们写出的`main`函数由谁调用？2. `main`函数中变量保存在内存中，这个内存地址是多少？

还需要写汇编代码，给`main`函数设置内存，调用`main`函数。

```assembly

.text
.global _start

_start:
    /* 设置内存：sp 栈 */
    ldr sp, = 4096 /* nand 启动 */
    @ ldr sp, = 0x40000000 + 4096 /* nor 启动 */

    /* 调用 main */
    bl main

halt:
    b halt

```

## 5. 常用汇编指令

```assembly
ADD
SUB R0, R1, #4 @ R0 = R1 - 4
SUB R0, R1, R2 @ R0 = R1 - R2
BL @ branch and link 跳转，并把返回地址保存在lr寄存器
LDM @ 读内存，写入多个寄存器
STM @ 把多个寄存器的值写入内存
LDMIA @ IA 表示 increment after 过后增加
STMDB @ DB 表示 decrement before 预先减少
@ 例如： 
@ 假设 sp = 4096，先减后存，高编号寄存器存放在高地址。先减：sp' = sp - 4 = 4092，后存，4092 ~ 4095 存放 pc的值
@ sp! 表示 sp = 最终被修改的 sp 的值
stmdb sp!, {fp, ip, lr, pc} 
```

## 6. 解析`C`程序的内部指令

下面是对`led.c`反编译的结果：

```assembly
led.elf:     file format elf32-littlearm

Disassembly of section .text:

00000000 <_start>:
   0:   e3a0da01        mov     sp, #4096       ; 0x1000
   4:   eb000000        bl      c <main> @ 跳到0xc执行，lr = 8

00000008 <halt>:
   8:   eafffffe        b       8 <halt>

0000000c <main>:
   c:   e1a0c00d        mov     ip, sp @ ip = sp = 4096
  10:   e92dd800        stmdb   sp!, {fp, ip, lr, pc} @ pc等于当前指令地址+8，所以pc = 0x18, lr = 8, ip = 4096, fp 未知 sp 最终 = 4096 - 4 * 4 = 4080
  14:   e24cb004        sub     fp, ip, #4      ; 0x4 @ fp = ip - 4 = 4092
  18:   e24dd008        sub     sp, sp, #8      ; 0x8 @ sp = sp - 8 = 4072
  1c:   e3a03456        mov     r3, #1442840576 ; 0x56000000 @ r3 = 0x56000000
  20:   e2833050        add     r3, r3, #80     ; 0x50 @ r3 = 56000050
  24:   e50b3010        str     r3, [fp, #-16] @ r3 存入[fp - 16] = 4076
  28:   e3a03456        mov     r3, #1442840576 ; 0x56000000 
  2c:   e2833054        add     r3, r3, #84     ; 0x54
  30:   e50b3014        str     r3, [fp, #-20] @ r3 0x56000064 存入 [fp - 20] = 4072
  34:   e51b2010        ldr     r2, [fp, #-16] @ r2 = fp - 16，从4076取值
  38:   e3a03c01        mov     r3, #256        ; 0x100 @ r3 = 0x100
  3c:   e5823000        str     r3, [r2] @ r3 的值存入 r2
  40:   e51b2014        ldr     r2, [fp, #-20] @ r2 = [fp -20] 
  44:   e3a03000        mov     r3, #0  ; 0x0 @ r3 = 0
  48:   e5823000        str     r3, [r2] @ r3 的值存入 r2
  4c:   e3a03000        mov     r3, #0  ; 0x0
  50:   e1a00003        mov     r0, r3 @ return 0
  54:   e24bd00c        sub     sp, fp, #12     ; 0xc  @ 恢复栈 sp = fp - 12 
  58:   e89da800        ldmia   sp, {fp, sp, pc} @ 从栈中恢复寄存器
Disassembly of section .comment:

00000000 <.comment>:
   0:   43434700        cmpmi   r3, #0  ; 0x0
   4:   4728203a        undefined
   8:   2029554e        eorcs   r5, r9, lr, asr #10
   c:   2e342e33        mrccs   14, 1, r2, cr4, cr3, {1}
  10:   Address 0x10 is out of bounds.

```

先来思考几个问题：

1. 为何要设置栈？

   因为`C`函数需要用

2. 怎么使用栈？

   1. 保存局部变量
   2. 保存`lr`等寄存器

3. 调用者如何传参数给被调用者？

4. 被调用者如何传返回值给调用者？

5. 怎么从栈中恢复寄存器？

`ARM`寄存器的使用规则：

`r0`到`r3`用来传递参数和返回值，在子程序调用之间，可以将 `r0` - `r3` 用于任何用途。`r4`到`r11`被用来存放函数的局部变量，可能被使用到，所以需要在函数入口保存它们，在函数出口恢复它们。

## 7. 使用按键控制`LED`

之前写的程序，一段时间后就会重新运行，这是由于看门狗的缘故。

> 看门狗，又叫watchdog timer，是一个定时器电路，一般有一个输入，叫喂狗(kicking the dog/service the dog），一个输出到MCU的RST端，MCU正常工作的时候，每隔一段时间输出一个信号到喂狗端，给 WDT清零，如果超过规定的时间不喂狗（一般在程序跑飞时），WDT定时超过，就会给出一个复位信号到MCU，使MCU复位。防止MCU死机. 看门狗的作用就是防止程序发生死循环，或者说程序跑飞。

接下来，我们将看门狗关闭。

```assembly

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

```

再在`C`代码里边编写使用按键控制`LED`的逻辑：

需求分析：开发板上有`S2`，`S3`，`S4`三个按键，按下`S2`，`LED D12`亮起，松开熄灭，按下`S3`，`LED D11`亮起，松开熄灭，按下`S4`，`LED D10`亮起。

先来看原理图：

![image-20220706074048428](https://cdn.jsdelivr.net/gh/linnanc/image@main/img/image-20220706074048428.png)

按键松开时，`EINT`处的电压高电平，按键按下时，`EINT`变成低电平。将`EINT`配置成输入引脚，然后读取它的电平，当它的电平为低时，点亮`LED`，为高时熄灭`LED`。查阅芯片手册，找到控制它们的寄存器。

示例代码：

```c

#define GPFCON (*((volatile unsigned int *)0x56000050))
#define GPFDAT (*((volatile unsigned int *)0x56000054))
#define GPGCON (*((volatile unsigned int *)0x56000060))
#define GPGDAT (*((volatile unsigned int *)0x56000064))

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

```

