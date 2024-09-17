# 解读内核的oops



某次内核崩溃打印出来的oops信息

```shell
The system is going down NOW!
[  247.140000] CPU 3 Unable to handle kernel paging request at virtual address c13ff838, epc == 8026e338, ra == 80271408
Sent SIGTERM to[  247.150000] Oops[#1]:
 all processes
[  247.160000] CPU: 3 PID: 672 Comm: dhcpd Tainted: G  R        O   3.18.24 #1
[  247.160000] task: 8df18d28 ti: 8a72a000 task.ti: 8a72a000
[  247.170000] $ 0   : 00000000 80a5000c 8d613eb0 c13ff838
[  247.180000] $ 4   : 8fbc5a31 8242e43c fffffffc ffff00fe
[  247.180000] $ 8   : 8a72bfe0 0000ff00 00000001 000004e5
[  247.190000] $12   : 00000000 000000a6 00000007 00000001
[  247.190000] $16   : 8a72beb0 8242e43c 0000c350 8a72bf10
[  247.200000] $20   : 005b0000 0055b894 00000000 0055b898
[  247.200000] $24   : 8fbc5a30 02faf080
[  247.210000] $28   : 8a72a000 8a72bdf8 00000003 80271408
[  247.220000] Hi    : 00000039
[  247.220000] Lo    : 8ab12100
[  247.220000] epc   : 8026e338 rb_erase+0x240/0x40c
[  247.230000] ra    : 80271408 timerqueue_del+0x38/0xc0
[  247.230000] Status: 1000ff02 KERNEL EXL
[  247.240000] Cause : 40800008
[  247.240000] BadVA : c13ff838
[  247.240000] PrId  : 0001a120 (MIPS interAptiv (multi))
[  247.250000] Modules linked in: epd(O) bc_notify(O) [last unloaded: linkMon]
[  247.260000] Process dhcpd (pid: 672, threadinfo=8a72a000, task=8df18d28, tls=7780b8c0)
[  247.270000] Stack : 8a72bf10 005b0000 0055b894 8069ad38 8a72beb0 8242e430 0000c350 8006c3dc
          8242e430 8006c2c0 8c364c80 00000000 0000c350 00000000 8a72beb0 8a72beb0
          00000000 8006c47c 8a72beb0 00000001 0000c350 8a72bf10 005b0000 00000001
          00000000 8a72beb0 00000001 8069a5c4 8a72be5c 00000000 00000000 8069adf4
          0000c350 00000001 00000001 00000001 7feaf8f4 8006cb08 855c3b00 8fd29970
          ...
[  247.300000] Call Trace:
[  247.310000] [<8026e338>] rb_erase+0x240/0x40c
[  247.310000] [<80271408>] timerqueue_del+0x38/0xc0
[  247.320000] [<8006c3dc>] __remove_hrtimer.isra.35+0x4c/0x7c
[  247.320000] [<8006c47c>] hrtimer_try_to_cancel+0x70/0xa4
[  247.330000] [<8069a5c4>] do_nanosleep+0x74/0x118
[  247.330000] [<8006cb08>] hrtimer_nanosleep+0xb0/0x13c
[  247.340000] [<8006cc30>] SyS_nanosleep+0x9c/0xbc
[  247.340000] [<8000dfe8>] handle_sys+0x128/0x14c
[  247.350000]
[  247.350000]
```



```shell
[  247.140000] CPU 3 Unable to handle kernel paging request at virtual address c13ff838, epc == 8026e338, ra == 80271408
```

这里能够简要的告诉是什么问题触发了oops，显然是由于分页错误导致的。如果是由代码直接调用BUG()/BUG_ON()一类的，还能给出源代码中触发的行号。



```shell
[  247.150000] Oops[#1]:
```

[#1]发生oops的数量，此处是1次



```shell
[  247.160000] CPU: 3 PID: 672 Comm: dhcpd Tainted: G  R        O   3.18.24 #1
```

CPU后的数字是错误所在逻辑CPU的编号，PID表示正在运行的进程ID，内核污染原因(G和R)，内核版本(3.18.24)。

内核污染原因包括私有驱动加载(P)，模块强制加载(F)，模块强制卸载(R)，机器检查异常发生(M)，检测到错误页(B)等。

如果涉及到了某项原因，就会显示为`Tainted: G PF R`这样。如果不存在问题，就会显示为`Not Tainted`。

其中Tainted的表示可以从内核中 kernel/panic.c 中找到：

| Tainted | 描述                                                         |
| ------- | ------------------------------------------------------------ |
| ‘G’     | if all modules loaded have a GPL or compatible license       |
| ‘P’     | if any proprietary module has been loaded. Modules without a MODULE_LICENSE or with a MODULE_LICENSE that is not recognised by insmod as GPL compatible are assumed to be proprietary. |
| ‘F’     | if any module was force loaded by “insmod -f”.               |
| ‘S’     | if the Oops occurred on an SMP kernel running on hardware that hasn’t been certified as safe to run multiprocessor. Currently this occurs only on various Athlons that are not SMP capable. |
| ‘R’     | if a module was force unloaded by “rmmod -f”.                |
| ‘M’     | if any processor has reported a Machine Check Exception.     |
| ‘B’     | if a page-release function has found a bad page reference or some unexpected page flags. |
| ‘U’     | if a user or user application specifically requested that the Tainted flag be set. |
| ‘D’     | if the kernel has died recently, i.e. there was an OOPS or BUG. |
| ‘W’     | if a warning has previously been issued by the kernel.       |
| ‘C’     | if a staging module / driver has been loaded.                |
| ‘I’     | if the kernel is working around a sever bug in the platform’s firmware (BIOS or similar). |



```shell
[  247.160000] task: 8df18d28 ti: 8a72a000 task.ti: 8a72a000
```

`task`表示当前进程的地址， `task.ti`表示当前进程栈的地址。



```shell
[  247.170000] $ 0   : 00000000 80a5000c 8d613eb0 c13ff838
[  247.180000] $ 4   : 8fbc5a31 8242e43c fffffffc ffff00fe
[  247.180000] $ 8   : 8a72bfe0 0000ff00 00000001 000004e5
[  247.190000] $12   : 00000000 000000a6 00000007 00000001
[  247.190000] $16   : 8a72beb0 8242e43c 0000c350 8a72bf10
[  247.200000] $20   : 005b0000 0055b894 00000000 0055b898
[  247.200000] $24   : 8fbc5a30 02faf080
[  247.210000] $28   : 8a72a000 8a72bdf8 00000003 80271408
```

异常发生时寄存器0~32的值。



```
[  247.220000] Hi    : 00000039
[  247.220000] Lo    : 8ab12100
[  247.220000] epc   : 8026e338 rb_erase+0x240/0x40c
[  247.230000] ra    : 80271408 timerqueue_del+0x38/0xc0
[  247.230000] Status: 1000ff02 KERNEL EXL
[  247.240000] Cause : 40800008
[  247.240000] BadVA : c13ff838
[  247.240000] PrId  : 0001a120 (MIPS interAptiv (multi))
```

异常发生时寄存器`Hi，Lo，epc，ra，SR，Cause，Badvaddr，PrId` 的值。

`rb_erase+0x240/0x40c`表示错误发生的地址是`rb_erase`函数的第0x240个字节，0x40c表示`rb_erase`函数的大小。



```shell
[  247.250000] Modules linked in: epd(O) bc_notify(O) [last unloaded: linkMon]
```

`Modules linked in`为加载了的模块列表，`linkMon`为上次卸载的模块。



```
Process dhcpd (pid: 672, threadinfo=8a72a000, task=8df18d28, tls=7780b8c0)
```

异常发生时对应的用户态进程信息。



```shell
[  247.270000] Stack : 8a72bf10 005b0000 0055b894 8069ad38 8a72beb0 8242e430 0000c350 8006c3dc
          8242e430 8006c2c0 8c364c80 00000000 0000c350 00000000 8a72beb0 8a72beb0
          00000000 8006c47c 8a72beb0 00000001 0000c350 8a72bf10 005b0000 00000001
          00000000 8a72beb0 00000001 8069a5c4 8a72be5c 00000000 00000000 8069adf4
          0000c350 00000001 00000001 00000001 7feaf8f4 8006cb08 855c3b00 8fd29970
          ...
```

栈信息。



```shell
[  247.300000] Call Trace:
[  247.310000] [<8026e338>] rb_erase+0x240/0x40c
[  247.310000] [<80271408>] timerqueue_del+0x38/0xc0
[  247.320000] [<8006c3dc>] __remove_hrtimer.isra.35+0x4c/0x7c
[  247.320000] [<8006c47c>] hrtimer_try_to_cancel+0x70/0xa4
[  247.330000] [<8069a5c4>] do_nanosleep+0x74/0x118
[  247.330000] [<8006cb08>] hrtimer_nanosleep+0xb0/0x13c
[  247.340000] [<8006cc30>] SyS_nanosleep+0x9c/0xbc
[  247.340000] [<8000dfe8>] handle_sys+0x128/0x14c
[  247.350000]
[  247.350000]
```

函数调用栈信息。



## 参考文章

1. [手把手教你解读内核的oops - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/575679671)
2. [Linux内核crash/Oops异常定位分析方法 - 苏小北1024 - 博客园 (cnblogs.com)](https://www.cnblogs.com/muahao/p/7452737.html)
3. [[monitor\] 4. Linux内核Oops机制 - pwl999 - 博客园 (cnblogs.com)](https://www.cnblogs.com/pwl999/p/15535061.html)

