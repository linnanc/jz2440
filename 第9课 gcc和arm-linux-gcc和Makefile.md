代码是如何变成程序的？

以下面的代码为例：

```c
#include <stdio.h>

#define MAX 20
#define MIN 10

#define _DEBUG
#define SETBIT(x) (1 << x)

int main(int argc, char *argv[])
{
    printf("Hello World\n");
    printf("MAX = %d, MIN = %d, MAX + MIN = %d\n", MAX, MIN, MAX + MIN);

#ifdef _DEBUG
    printf("SETBIT(5) = %d, SETBIT(6) = %d\n", SETBIT(5), SETBIT(6));
    printf("SETBIT(SETBIT(2)) = %d\n", SETBIT(SETBIT(2)));
#endif
    return 0;    
}
```

在使用`gcc hello.c`命令编译后就变成了可执行程序。实际上`gcc`会对源代码进行预处理，编译，汇编，链接四个步骤。先来熟悉一下`gcc`，它的常用选项如下：

`gcc`常用选项：

`-v`: 查看`gcc`编译器的版本，显示`gcc`执行时的详细过程。

`-o <file>`: 指定输出文件为`file`

`-E`: 预处理，如：

```shell
gcc -E -o hello.i hello.c
```

经过预处理后，会把头文件中的符号包含进来，并进行宏替换。

`-S`: 只编译，不会汇编、链接：

```shell
gcc -S -o hello.s hello.i
```

`-c`: 编译和汇编，不会链接

```shell
gcc -c -o hello.o hello.s
```

小结：输入文件的后缀名和选项共同决定`gcc`到底决定哪些操作。

在编译过程中，除非使用了`-E`，`-S`，`-c`选项，否则最后的步骤都是链接。

链接：将汇编生成的`OBJ`文件，系统库`OBJ`文件，库文件链接起来，最终生成可以在特定平台运行的可执行程序。

`crtl.o` `crti.o` `crtbegin.o` `crtend.o` `crtn.o` 是 `gcc` 加入的系统标准启动文件。对于一般的应用程序，这些启动是必须的。

`-lc`: 链接`libc`库文件。

## `Makefile`的引入及规则

```shell
gcc -o test a.c b.c -v
```

使用上面的命令编译会重新编译`a.c`和`b.c`两个文件，当我们只修改其中的一个时，完全可以只编译修改的那个文件。可以对它们分别编译：

```shell
gcc -c -o a.o a.c
gcc -c -o b.o b.c
gcc -o test a.o b.o
```

当我们修改了`a.c`时，只需要重新编译`a.c`，执行下面的命令：

```SHELL
gcc -c -o a.o a.c
gcc -o test a.o b.o
```

如果修改的文件过多时怎么判断哪些文件是被修改了呢？可以通过比较`.o`和`.c`文件的修改时间，如果`.c`比`.o`更新，那么说明代码被修改了，需要重新编译。

`Makefile`的规则：

```
目标文件: 依赖文件
	命令
```

当依赖文件比目标文件新时，执行命令。

```makefile
test: a.o b.o
        gcc -o test a.o b.o

a.o: a.c
        gcc -c -o a.o a.c

b.o: b.c
        gcc -c -o b.o b.c
```

第一次执行`make`会执行三条命令：

```SHELL
gcc -c -o a.o a.c
gcc -c -o b.o b.c
gcc -o test a.o b.o
```

修改`a.c`后执行`make`:

```shell
gcc -c -o a.o a.c
gcc -o test a.o b.o
```

## `Makefile`的语法

### 通配符

`%`：通配符，如`%.o`表示所有`%.o`文件，`%.c`表示所有`.c`文件。

`$@`：表示目标

`$<`：表示第1个依赖文件

`$^`：表示所有的依赖文件

例子：

```MAKEFILE
test: a.o b.o c.o
        gcc -o test $^

%.o: %.c
        gcc -c -o $@ $<

clean:
        rm *.o test               
```

### 假想目标

`Makefile`在执行`make`命令时如果无目标，则默认执行第一个目标。上面的`Makefile`在执行`make`时，会执行目标`test`，执行`make clean`时会执行目标`clean`。

如果在`Makefile`的同级目录下创建一个`clean`文件：

```shell
touch clean
```

再执行`make clean`，会提示`clean`已经是最新了。为了解决这个问题，引入假想目标。

```MAKEFILE
.PHONY: clean
```

这时，当`clean`文件存在时也会去执行目标。

### 即时变量、延时变量、`export`

```
A := XXX # A 的值即刻确定，在定义时即确定
B = XXX # B 的值使用时确定

:= # 即时变量
= # 延时变量
?= # 延时变量，只有第一次定义才起效
+= # 附加，它是即时变量还是延时变量取决于前面的定义，如果前面没有定义呢？那它就是延时变量
```

## `Makefile`内置函数

`$(foreach var, list, text)`：对于`list`里的每一个变量执行`text`操作。

`$(filter pattern..., text)`：从`text`中取出符合格式`pattern`的值。

`$(filter-out pattern..., text)`：从`text`中取出不符合`pattern`的值。

`$(wildcard pattern)`：`pattern`定义了文件名的格式，`wildcard`取出其中存在的文件。

`$(patsubst pattern, replacement, text)`：将`text`中符合`pattern`的子串替换成`replacement`。

## `Makefile`实例

我们在`c.c`里面引入了一个头文件`c.h`，它们的内容如下：

```C
#define C 2

```

```C
#include <stdio.h>
#include "c.h"

int func1()
{
    printf("hello c.c %d\n", C);
}

```

之前的`Makefile`修改宏定义`C`后，并不会重新编译。接下来我们改进它，让它能支持头文件的依赖。

使用

```shell
gcc -M c.c
```

可以打印出`c.c`的所有依赖。使用下面的命令，把依赖写入到文件`c.d`

```shell
gcc -M -MF c.d c.c
```

编译`c.o`把依赖写入文件`c.d`

```shell
gcc -c -o c.o c.c -MD -MF c.d
```

`CFLAGS`编译选项
