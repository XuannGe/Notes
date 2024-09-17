# 输入reboot命令后设备挂死



## 问题现象

理想情况下，在命令行输入reboot命令后，设备会立即重启。但本台设备在命令行输入reboot命令后会挂死，无法立即重启。



**挂死图**



## 问题定位

### 问题复现

问题复现需要保证硬件设备相同和软件环境相同，并注意问题是概率性出现还是必定出现。

如本问题便是必定出现类型。



第一次问题复现现场

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
Code: 8c430008  10600005  00000000 <8c640000> 30840001  10800051  00000000  8c440004  10800005
[  247.360000] ---[ end trace 6b0af07d2a704414 ]---
[  247.370000] Fatal exception: panic in 5 seconds
[  263.600000] BUG: spinl 761 Comm: DVRIPThreadPool Tainted: G  R   D    O   3.18.24 #1
[  263.630000] Stack : 00000000 00000004 00000006 000000f0 00000000 00000000 00000000 00000000
          80a55442 00000049 000002f9 00000002 13dc6000 8a063450 807c3604 808470a7
          000002f9 00000002 80a5413c 8a063450 0127cc90 01280000 89115b84 80690a74
          00000006 800229c8 80a55442 00000000 807c7754 891159d4 13dc6000 89115900
          00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
          ...
```



第二次问题复现现场

```shell
The system is going down NOW!
Sent SIGTERM to[  281.180000] CPU 2 Unable to handle kernel paging request at virtual address c13ff848, epc == 8027132c, ra == 80271320
 all processes
[  281.190000] Oops[#1]:
[  281.200000] CPU: 2 PID: 1 Comm: init Tainted: G  R        O   3.18.24 #1
[  281.200000] task: 854335c8 ti: 85434000 task.ti: 85434000
[  281.210000] $ 0   : 00000000 7f8fa664 8d04feb0 00000001
[  281.220000] $ 4   : 85435eb0 8242e43c c9e43350 00000000
[  281.220000] $ 8   : 0000c350 00000041 00000041 00000000
[  281.230000] $12   : 7f8fa610 7797c3e0 00000000 004a0000
[  281.230000] $16   : 85435eb0 8242e43c 8242e430 00000000
[  281.240000] $20   : b33d1900 00000000 00000000 00000000
[  281.240000] $24   : c13ff838 b33ddc50
[  281.250000] $28   : 85434000 85435dd8 00000000 80271320
[  281.260000] Hi    : 00000000
[  281.260000] Lo    : 00000000
[  281.260000] epc   : 8027132c timerqueue_add+0x9c/0x128
[  281.270000] ra    : 80271320 timerqueue_add+0x90/0x128
[  281.270000] Status: 1000ff02 KERNEL EXL
[  281.280000] Cause : 40800008
[  281.280000] BadVA : c13ff848
[  281.280000] PrId  : 0001a120 (MIPS interAptiv (multi))
[  281.290000] Modules linked in: epd(O) bc_notify(O) [last unloaded: linkMon]
[  281.300000] Process init (pid: 1, threadinfo=85434000, task=854335c8, tls=779bd460)
[  281.310000] Stack : 81048fa0 85435eb0 00000000 8242e430 85435eb0 8242e430 8242e430 8006c5f0
          8fc021d4 00000000 807c9678 80095e64 00000000 0000c350 00000041 b33ddc50
          85435e50 00000001 85435eb0 00000001 0000c350 85435f10 7f8fab74 0047e434
          004a0000 8006c71c 80092d54 00000000 00000000 00000000 0000c350 00000001
          00000001 85435e58 85435e60 8069a5a8 8fc020f0 8008acac 00000000 8069adf4
          ...
[  281.340000] Call Trace:
[  281.350000] [<8027132c>] timerqueue_add+0x9c/0x128
[  281.350000] [<8006c5f0>] __hrtimer_start_range_ns+0x110/0x218
[  281.360000] [<8006c71c>] hrtimer_start_range_ns+0x24/0x30
[  281.360000] [<8069a5a8>] do_nanosleep+0x58/0x118
[  281.370000] [<8006cb08>] hrtimer_nanosleep+0xb0/0x13c
[  281.370000] [<8006cc30>] SyS_nanosleep+0x9c/0xbc
[  281.380000] [<8000dfe8>] handle_sys+0x128/0x14c
[  281.380000]
[  281.390000]
```



### 通过Oops 信息定位问题的产生原因

设备在命令行输入reboot命令后会挂死，内核会打印出一系列Oops 信息，需要特别关注信息内容中的以下几点：

1. 文本描述信息。简要的说明是什么问题触发了Oops 。
2. EPC寄存器和RA寄存器的值。当错误发生时，EPC寄存器保存异常返回地址，RA寄存器保存被调用函数的返回地址。
3. 栈回溯信息，保留了函数的调用信息。



于是去查看这两次问题现场，看看内核打印出来的关键信息：

```shell
/*文本描述信息*/
[  247.140000] CPU 3 Unable to handle kernel paging request at virtual address c13ff838, epc == 8026e338, ra == 80271408
......
/*EPC寄存器和RA寄存器的值*/
[  247.220000] epc   : 8026e338 rb_erase+0x240/0x40c
[  247.230000] ra    : 80271408 timerqueue_del+0x38/0xc0
......
/*栈回溯信息*/
[  247.300000] Call Trace:
[  247.310000] [<8026e338>] rb_erase+0x240/0x40c
[  247.310000] [<80271408>] timerqueue_del+0x38/0xc0
[  247.320000] [<8006c3dc>] __remove_hrtimer.isra.35+0x4c/0x7c
[  247.320000] [<8006c47c>] hrtimer_try_to_cancel+0x70/0xa4
[  247.330000] [<8069a5c4>] do_nanosleep+0x74/0x118
[  247.330000] [<8006cb08>] hrtimer_nanosleep+0xb0/0x13c
[  247.340000] [<8006cc30>] SyS_nanosleep+0x9c/0xbc
[  247.340000] [<8000dfe8>] handle_sys+0x128/0x14c
```

```shell
/*文本描述信息*/
[  281.180000] CPU 2 Unable to handle kernel paging request at virtual address c13ff848, epc == 8027132c, ra == 80271320
......
/*EPC寄存器和RA寄存器的值*/
[  281.260000] epc   : 8027132c timerqueue_add+0x9c/0x128
[  281.270000] ra    : 80271320 timerqueue_add+0x90/0x128
......
/*栈回溯信息*/
[  281.340000] Call Trace:
[  281.350000] [<8027132c>] timerqueue_add+0x9c/0x128
[  281.350000] [<8006c5f0>] __hrtimer_start_range_ns+0x110/0x218
[  281.360000] [<8006c71c>] hrtimer_start_range_ns+0x24/0x30
[  281.360000] [<8069a5a8>] do_nanosleep+0x58/0x118
[  281.370000] [<8006cb08>] hrtimer_nanosleep+0xb0/0x13c
[  281.370000] [<8006cc30>] SyS_nanosleep+0x9c/0xbc
[  281.380000] [<8000dfe8>] handle_sys+0x128/0x14c
```

跟据以上信息，可以推测内核发生了这些错误：

1. 分页错误
2. 与定时器使用相关的错误。



### 通过weCI版本更新记录和SVN日志定位问题的引入时间

在weCI上找一个通过测试验收的，离问题出现最近的，更早的软件版本，把它下载到同样的设备上，在命令行中输入reboot，发现设备可以正常重启，没有挂死。



**weci图**

**正常图**



于是推测问题是在该正常版本之后引入的。

由于推测是定时器的使用出了问题导致内核崩溃，所以查看svn日志中与kernel文件夹相关的部分，果然有一条与定时器相关的更新记录。



**svn日志图**

**svn记录**



原来kernel文件夹中的什么文件和packshop文件夹中的什么文件都被修改了，查看它们的修改点。



**更新前后程序对比**



咦，感觉没有问题啊，脚本发出了驱动卸载命令，驱动在卸载时也释放定时器资源了呀。



### 通过objdump定位问题的具体位置

尝试通过objdump定位问题的具体位置，失败。



### 通过版本号定位问题的具体位置

有没有这样的可能：packshop文件夹中或者kernel文件夹的修改点并没有被打包到新版本程序中。

于是笔者在weCI上查看该问题版本中，Dependinfo.xml文件里packshow文件夹的版本号和Full_Dependinfo.xml文件里kernel文件夹的版本号



**两张图**



并查看该次修改在svn上获得的版本号。



**图**



发现Dependinfo.xml文件里packshow文件夹的版本号大于该次修改的版本号，但Full_Dependinfo.xml文件里kernel文件夹的版本号却小于该次修改的版本号。也就是说，新版本程序中包含了packshop文件夹的修改点，却没有包含kernel文件夹的修改点。所以当在新版本程序的命令行中输入reboot命令时，驱动卸载命令确实是发出了，但该驱动使用的定时器资源却没有被释放，所以导致内核崩溃，重启失败。



### 导致版本号不一致的原因





## 问题解决

