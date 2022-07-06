/**
 * @brief 点亮LED1
 * 
 */
int main(void)
{
    unsigned int *gpf_con = (unsigned int *)0x56000050;
    unsigned int *gpf_dat = (unsigned int *)0x56000054;

    /* 配置GPF4为输出引脚 */
    *gpf_con = 0x100;

    /* 配置GPF4输出低电平 */
    *gpf_dat = 0;

    return 0;
}
