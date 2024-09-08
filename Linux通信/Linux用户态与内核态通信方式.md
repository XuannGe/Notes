# Linux用户态与内核态通信方式

用户态和内核态之间的通信方式主要有以下几种：

**系统调用（System Call）**：这是最常见的方式，用户态程序通过系统调用接口（如open、read、write、fork等）请求内核执行特定的动作。系统调用是用户态和内核态之间进行交互的桥梁，它允许用户态程序请求内核提供服务。 

**信号（Signal）**：内核通过信号通知用户态进程发生了某些事件，用户态程序可以注册信号处理函数来响应特定的信号事件。例如，SIGTERM和SIGINT等信号就是用于通知进程终止或中断的信号。 

**共享内存（Share Memory）**：允许多个进程在它们的地址空间中共享一块内存区域，从而实现用户态和内核态之间的高效通信。这种方式避免了频繁的用户态和内核态切换，但也需要处理数据同步和一致性的问题。  

**IOCTL**：这是内核较早的一种用户态和内核态的交互方式。用户态程序通过命令的方式调用ioctl函数，然后内核态分发到对应驱动处理，最后将处理结果返回到用户态。  

**Netlink**：本质上是一种特殊的socket，用于内核与多种用户进程之间的消息传递系统。netlink支持内核与用户态之间的双向通信，是一种全双工通信方式。  

**procfs/sysfs**：在Linux中，procfs和sysfs是特殊的文件系统，用于内核与用户空间之间的信息交互。procfs提供了内核和进程的各种信息，而sysfs则提供了设备和驱动的信息。用户态程序可以通过读取这些文件系统中的文件来获取内核信息，也可以通过写入特定的文件来配置内核或驱动。 

每种通信方式都有其特定的应用场景和优缺点，开发者需要根据具体需求选择适合的通信方式。

实现用户态和内核态之间的通信通常涉及编写内核模块（在内核态运行）以及用户空间程序（在用户态运行）。下面将提供一个简单的例子，展示如何编写一个内核模块，它提供一个系统调用接口，以及一个用户空间程序来调用这个系统调用。



系统调用（System Call）
-----------------

首先，我们需要编写一个内核模块，它注册一个新的系统调用。这个系统调用将简单地返回传递给它的参数值。

```c
#include <linux/kernel.h>  
#include <linux/syscalls.h>  
#include <linux/unistd.h>  

#define __NR_my_syscall 333  
  
SYSCALL_DEFINE1(my_syscall, int, arg)  
{  
    printk(KERN_INFO "my_syscall called with arg %d\n", arg);  
    return arg;  
}  
  
static int __init syscall_example_init(void)  
{  
    
    if (syscall_register(__NR_my_syscall, (sys_call_ptr_t)my_syscall) < 0) {  
        printk(KERN_ALERT "Failed to register syscall!\n");  
        return -1;  
    }  
    printk(KERN_INFO "syscall_example module loaded\n");  
    return 0;  
}  
  

static void __exit syscall_example_exit(void)  
{  
    
    syscall_unregister( );  
    printk(KERN_INFO "syscall_example module unloaded\n");  
}  
  
module_init(syscall_example_init);  
module_exit(syscall_example_exit);  
  
MODULE_LICENSE("GPL");

```

编译这个内核模块需要Makefile文件：

```null

obj-m += syscall_example.o  
  
all:  
    make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules  
  
clean:  
    make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

```

在编译并加载这个模块后，新的系统调用就可以通过`__NR_my_syscall`号来调用了。

接下来，我们编写一个用户空间程序来调用这个新的系统调用。

```c
#include <unistd.h>  
#include <stdio.h>  
#include <sys/syscall.h>  
  

#define __NR_my_syscall 333  
  
int main() {  
    int arg = 42; 
    long result;  
  
    
    result = syscall(__NR_my_syscall, arg);  
    if (result >= 0) {  
        printf("my_syscall returned: %ld\n", result);  
    } else {  
        perror("my_syscall failed");  
    }  
  
    return 0;  
}
```

编译并运行用户空间程序：

```c
gcc -o user_program user_program.c  
sudo insmod syscall_example.ko  
./user_program                  
sudo rmmod syscall_example     
```



信号（Signal）
----------

这个示例，提供一个简单的C语言实现，用于在用户态和内核态之间通信，并使用信号来实现。这个示例将使用字符设备驱动程序和`sigaction`系统调用**内核态（字符设备驱动程序）**。

首先，我们需要创建一个简单的字符设备驱动程序。这个驱动程序将实现信号处理，以便用户态程序可以与内核态通信。

```c
#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>
#include<linux/signal.h>

#define DEVICE_NAME "my_device"
#define DEVICE_MAJOR 240
#define DEVICE_MINOR 0

static int my_device_open(struct inode *inode, struct file *file) {
    return 0;
}

static int my_device_release(struct inode *inode, struct file *file) {
    return 0;
}

static ssize_t my_device_write(struct file *file, const char __user *buf, size_t count, loff_t *f_pos) {
    int pid;

    if (copy_from_user(&pid, buf, sizeof(int))) {
        return -EFAULT;
    }

    struct siginfo info;
    memset(&info, 0, sizeof(struct siginfo));
    info.si_signo = SIGIO;
    info.si_code = SI_QUEUE;
    info.si_int = 42; 

    send_sig_info(SIGIO, &info, current);

    return count;
}

static struct file_operations my_device_fops = {
    .owner = THIS_MODULE,
    .open = my_device_open,
    .release = my_device_release,
    .write = my_device_write,
};

static struct cdev my_device_cdev;

static int __init my_device_init(void) {
    int ret;
    dev_t devno = MKDEV(DEVICE_MAJOR, DEVICE_MINOR);

    ret = register_chrdev_region(devno, 1, DEVICE_NAME);
    if (ret < 0) {
        printk(KERN_ERR "Failed to register device region\n");
        return ret;
    }

    cdev_init(&my_device_cdev, &my_device_fops);
    my_device_cdev.owner = THIS_MODULE;

    ret = cdev_add(&my_device_cdev, devno, 1);
    if (ret < 0) {
        printk(KERN_ERR "Failed to add device\n");
        unregister_chrdev_region(devno, 1);
        return ret;
    }

    printk(KERN_INFO "My device initialized\n");
    return 0;
}

static void __exit my_device_exit(void) {
    dev_t devno = MKDEV(DEVICE_MAJOR, DEVICE_MINOR);

    cdev_del(&my_device_cdev);
    unregister_chrdev_region(devno, 1);

    printk(KERN_INFO "My device exited\n");
}

module_init(my_device_init);
module_exit(my_device_exit);
MODULE_LICENSE("GPL");

```

**用户态（应用程序）**

接下来，我们需要创建一个简单的用户态应用程序，用于与内核态通信。这个应用程序将使用信号处理来接收来自内核态的信号。

```c
#include<stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include<signal.h>

#define DEVICE_NAME "/dev/my_device"

void signal_handler(int sig, siginfo_t *info, void *ucontext) {
    if (sig == SIGIO) {
        printf("Data from kernel: %d\n", info->si_int);
    }
}

int main() {
    int fd, pid;
    struct sigaction sa;

    memset(&sa, 0, sizeof(sa));
    sa.sa_sigaction = signal_handler;
    sa.sa_flags = SA_SIGINFO;

    if (sigaction(SIGIO, &sa, NULL) < 0) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    pid = getpid();

    fd = open(DEVICE_NAME, O_WRONLY);
    if (fd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    if (write(fd, &pid, sizeof(int)) < 0) {
        perror("write");
        exit(EXIT_FAILURE);
    }

    close(fd);

    while (1) {
        sleep(1);
    }

    return 0;
}
```

这个示例，我们使用了`sigaction`系统调用来实现信号处理。用户态应用程序通过`sigaction`系统调用注册信号处理函数，当接收到信号时，信号处理函数将被调用。内核态驱动程序通过`send_sig_info`函数发送信号，用户态应用程序接收到信号后，信号处理函数将打印数据。



## 共享内存（Share Memory）

这个示例，提供一个简单的C语言实现，用于在用户态和内核态之间通信，并使用共享内存来实现。这个示例将使用字符设备驱动程序和`mmap`系统调用。

**内核态（字符设备驱动程序）**

首先，我们需要创建一个简单的字符设备驱动程序。这个驱动程序将实现共享内存的映射，以便用户态程序可以与内核态通信。

```c
#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/mm.h>
#include<linux/slab.h>

#define DEVICE_NAME "my_device"
#define DEVICE_MAJOR 240
#define DEVICE_MINOR 0

static int my_device_open(struct inode *inode, struct file *file) {
    return 0;
}

static int my_device_release(struct inode *inode, struct file *file) {
    return 0;
}

static int my_device_mmap(struct file *file, struct vm_area_struct *vma) {
    unsigned long size = vma->vm_end - vma->vm_start;
    void *data = kmalloc(size, GFP_KERNEL);

    if (!data) {
        return -ENOMEM;
    }

    if (remap_pfn_range(vma, vma->vm_start, virt_to_phys(data) >> PAGE_SHIFT, size, vma->vm_page_prot)) {
        kfree(data);
        return -EAGAIN;
    }

    file->private_data = data;
    return 0;
}

static struct file_operations my_device_fops = {
    .owner = THIS_MODULE,
    .open = my_device_open,
    .release = my_device_release,
    .mmap = my_device_mmap,
};

static struct cdev my_device_cdev;

static int __init my_device_init(void) {
    int ret;
    dev_t devno = MKDEV(DEVICE_MAJOR, DEVICE_MINOR);

    ret = register_chrdev_region(devno, 1, DEVICE_NAME);
    if (ret < 0) {
        printk(KERN_ERR "Failed to register device region\n");
        return ret;
    }

    cdev_init(&my_device_cdev, &my_device_fops);
    my_device_cdev.owner = THIS_MODULE;

    ret = cdev_add(&my_device_cdev, devno, 1);
    if (ret < 0) {
        printk(KERN_ERR "Failed to add device\n");
        unregister_chrdev_region(devno, 1);
        return ret;
    }

    printk(KERN_INFO "My device initialized\n");
    return 0;
}

static void __exit my_device_exit(void) {
    dev_t devno = MKDEV(DEVICE_MAJOR, DEVICE_MINOR);

    cdev_del(&my_device_cdev);
    unregister_chrdev_region(devno, 1);

    printk(KERN_INFO "My device exited\n");
}

module_init(my_device_init);
module_exit(my_device_exit);
MODULE_LICENSE("GPL");

```

**用户态（应用程序）**

接下来，我们需要创建一个简单的用户态应用程序，用于与内核态通信。这个应用程序将使用`mmap`系统调用来映射共享内存，并在共享内存中存储和读取数据。

```c
#include<stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define DEVICE_NAME "/dev/my_device"

int main() {
    int fd;
    void *data;
    size_t size = sysconf(_SC_PAGESIZE);

    fd = open(DEVICE_NAME, O_RDWR);
    if (fd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    data = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (data == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    
    *(int *)data = 42;

    
    printf("Data from kernel: %d\n", *(int *)data);

    munmap(data, size);
    close(fd);

    return 0;
}
```

这个示例，我们使用了`mmap`系统调用来实现共享内存。用户态应用程序通过`mmap`系统调用映射共享内存，并在共享内存中存储和读取数据。内核态驱动程序通过`remap_pfn_range`函数将物理内存映射到用户空间。



IOCTL
-----

这个示例，提供一个简单的C语言实现，用于在用户态和内核态之间通信，并使用`ioctl`系统调用。这个示例将使用字符设备驱动程序和`ioctl`系统调用。

**内核态（字符设备驱动程序）**

首先，我们需要创建一个简单的字符设备驱动程序。这个驱动程序将实现`ioctl`系统调用，以便用户态程序可以与内核态通信。

```c
#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>
#include<linux/ioctl.h>

#define DEVICE_NAME "my_device"
#define DEVICE_MAJOR 240
#define DEVICE_MINOR 0

#define IOCTL_CMD_GET_DATA _IOR('d', 1, int)
#define IOCTL_CMD_SET_DATA _IOW('d', 2, int)

static int my_device_open(struct inode *inode, struct file *file) {
    return 0;
}

static int my_device_release(struct inode *inode, struct file *file) {
    return 0;
}

static long my_device_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
    int data;

    switch (cmd) {
        case IOCTL_CMD_GET_DATA:
            data = 42; 
            if (copy_to_user((void *)arg, &data, sizeof(int))) {
                return -EFAULT;
            }
            break;
        case IOCTL_CMD_SET_DATA:
            if (copy_from_user(&data, (void *)arg, sizeof(int))) {
                return -EFAULT;
            }
            
            break;
        default:
            return -ENOTTY;
    }

    return 0;
}

static struct file_operations my_device_fops = {
    .owner = THIS_MODULE,
    .open = my_device_open,
    .release = my_device_release,
    .unlocked_ioctl = my_device_ioctl,
};

static struct cdev my_device_cdev;

static int __init my_device_init(void) {
    int ret;
    dev_t devno = MKDEV(DEVICE_MAJOR, DEVICE_MINOR);

    ret = register_chrdev_region(devno, 1, DEVICE_NAME);
    if (ret < 0) {
        printk(KERN_ERR "Failed to register device region\n");
        return ret;
    }

    cdev_init(&my_device_cdev, &my_device_fops);
    my_device_cdev.owner = THIS_MODULE;

    ret = cdev_add(&my_device_cdev, devno, 1);
    if (ret < 0) {
        printk(KERN_ERR "Failed to add device\n");
        unregister_chrdev_region(devno, 1);
        return ret;
    }

    printk(KERN_INFO "My device initialized\n");
    return 0;
}

static void __exit my_device_exit(void) {
    dev_t devno = MKDEV(DEVICE_MAJOR, DEVICE_MINOR);

    cdev_del(&my_device_cdev);
    unregister_chrdev_region(devno, 1);

    printk(KERN_INFO "My device exited\n");
}

module_init(my_device_init);
module_exit(my_device_exit);
MODULE_LICENSE("GPL");
```

**用户态（应用程序）**

接下来，我们需要创建一个简单的用户态应用程序，用于与内核态通信。这个应用程序将使用`ioctl`系统调用来读取数据。

```c
#include<stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define DEVICE_NAME "/dev/my_device"
#define IOCTL_CMD_GET_DATA _IOR('d', 1, int)
#define IOCTL_CMD_SET_DATA _IOW('d', 2, int)

int main() {
    int fd, data;

    fd = open(DEVICE_NAME, O_RDWR);
    if (fd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    if (ioctl(fd, IOCTL_CMD_GET_DATA, (unsigned long)&data) < 0) {
        perror("ioctl");
        exit(EXIT_FAILURE);
    }

    printf("Data from kernel: %d\n", data);

    close(fd);

    return 0;
}
```

这个示例，我们使用了`ioctl`系统调用来实现用户态和内核态之间的通信。用户态应用程序通过`ioctl`系统调用发送命令（在这个例子中是`IOCTL_CMD_GET_DATA`），内核态驱动程序根据命令执行相应的操作并返回数据。



Netlink
-------

这个示例，提供一个简单的C语言实现，用于在用户态和内核态之间通信，并使用netlink来实现。这个示例将使用内核模块和用户空间应用程序。

**内核态（内核模块）**

首先，我们需要创建一个简单的内核模块。这个模块将实现一个简单的netlink套接字，用于与用户空间通信。

```c
#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/netlink.h>
#include<linux/skbuff.h>
#include <net/sock.h>

#define NETLINK_USER 31

static void netlink_recv_msg(struct sk_buff *skb) {
    struct nlmsghdr *nlh = (struct nlmsghdr *)skb->data;
    int data = 42; 

    if (nlh->nlmsg_type == NLMSG_NOOP) {
        struct sk_buff *skb_out;
        struct nlmsghdr *nlh_out;
        int *data_out;

        skb_out = nlmsg_new(NLMSG_SPACE(sizeof(int)), GFP_ATOMIC);
        if (!skb_out) {
            printk(KERN_ERR "Failed to allocate new skb\n");
            return;
        }

        nlh_out = nlmsg_put(skb_out, 0, 0, NLMSG_DONE, sizeof(int));
        if (!nlh_out) {
            printk(KERN_ERR "Failed to put new message\n");
            kfree_skb(skb_out);
            return;
        }

        data_out = nlmsg_data(nlh_out);
        *data_out = data;

        netlink_unicast(skb_out, nlh->nlmsg_pid, MSG_DONTWAIT);
    }
}

static int __init netlink_init(void) {
    struct netlink_kernel_cfg cfg = {
        .input = netlink_recv_msg,
    };

    struct sock *sock = netlink_kernel_create(&init_net, NETLINK_USER, &cfg);
    if (!sock) {
        printk(KERN_ERR "Failed to create netlink socket\n");
        return -ENOMEM;
    }

    printk(KERN_INFO "Netlink initialized\n");
    return 0;
}

static void __exit netlink_exit(void) {
    netlink_kernel_release(netlink_kernel_create(&init_net, NETLINK_USER, NULL));
    printk(KERN_INFO "Netlink exited\n");
}

module_init(netlink_init);
module_exit(netlink_exit);
MODULE_LICENSE("GPL");

```

**用户态（应用程序）**

接下来，我们需要创建一个简单的用户态应用程序，用于与内核态通信。这个应用程序将使用netlink套接字来读取数据。

```c
#include<stdio.h>
#include <stdlib.h>
#include<string.h>
#include <unistd.h>
#include <sys/socket.h>
#include<linux/netlink.h>

#define NETLINK_USER 31

int main() {
    int sockfd, n;
    struct sockaddr_nl src_addr, dest_addr;
    struct msghdr msg;
    struct iovec iov;
    struct nlmsghdr *nlh;
    int data;
    char buffer[NLMSG_SPACE(sizeof(int))];

    sockfd = socket(AF_NETLINK, SOCK_RAW, NETLINK_USER);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&src_addr, 0, sizeof(src_addr));
    src_addr.nl_family = AF_NETLINK;
    src_addr.nl_pid = getpid();
    src_addr.nl_groups = 0;

    if (bind(sockfd, (struct sockaddr *)&src_addr, sizeof(src_addr)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.nl_family = AF_NETLINK;
    dest_addr.nl_pid = 0; 
    dest_addr.nl_groups = 0;

    nlh = (struct nlmsghdr *)buffer;
    nlh->nlmsg_len = NLMSG_SPACE(sizeof(int));
    nlh->nlmsg_type = NLMSG_NOOP;
    nlh->nlmsg_flags = 0;
    nlh->nlmsg_seq = 0;
    nlh->nlmsg_pid = src_addr.nl_pid;

    iov.iov_base = (void *)nlh;
    iov.iov_len = nlh->nlmsg_len;

    msg.msg_name = (void *)&dest_addr;
    msg.msg_namelen = sizeof(dest_addr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    msg.msg_control = NULL;
    msg.msg_controllen = 0;
    msg.msg_flags = 0;

    if (sendmsg(sockfd, &msg, 0) < 0) {
        perror("sendmsg");
        exit(EXIT_FAILURE);
    }

    n = recv(sockfd, buffer, sizeof(buffer), 0);
    if (n < 0) {
        perror("recv");
        exit(EXIT_FAILURE);
    }

    data = *(int *)NLMSG_DATA(nlh);
    printf("Data from kernel: %d\n", data);

    close(sockfd);

    return 0;
}
```

这个示例，我们使用了netlink套接字来实现用户态和内核态之间的通信。用户态应用程序通过netlink套接字发送请求，内核态驱动程序根据请求执行相应的操作并返回数据。



procfs/sysfs
------------

这个示例提供一个简单的C语言实现，用于在用户态和内核态之间通信，并使用procfs来实现读写。这个示例将使用内核模块和用户空间应用程序。

**内核态（内核模块）**

首先，我们需要创建一个简单的内核模块。这个模块将实现一个简单的procfs文件，用于与用户空间通信。

```c
#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/proc_fs.h>
#include<linux/seq_file.h>

#define PROC_FILE_NAME "my_proc_file"

static int data = 42; 

static int my_proc_show(struct seq_file *m, void *v) {
    seq_printf(m, "%d\n", data);
    return 0;
}

static int my_proc_open(struct inode *inode, struct file *file) {
    return single_open(file, my_proc_show, NULL);
}

static ssize_t my_proc_write(struct file *file, const char __user *buf, size_t count, loff_t *f_pos) {
    char kbuf[16];
    int value;

    if (count > sizeof(kbuf) - 1) {
        return -EINVAL;
    }

    if (copy_from_user(kbuf, buf, count)) {
        return -EFAULT;
    }

    kbuf[count] = '\0';
    if (sscanf(kbuf, "%d", &value) != 1) {
        return -EINVAL;
    }

    data = value;
    return count;
}

static const struct file_operations my_proc_fops = {
    .owner = THIS_MODULE,
    .open = my_proc_open,
    .read = seq_read,
    .write = my_proc_write,
    .llseek = seq_lseek,
    .release = single_release,
};

static int __init my_proc_init(void) {
    proc_create(PROC_FILE_NAME, 0666, NULL, &my_proc_fops);
    printk(KERN_INFO "My proc file initialized\n");
    return 0;
}

static void __exit my_proc_exit(void) {
    remove_proc_entry(PROC_FILE_NAME, NULL);
    printk(KERN_INFO "My proc file exited\n");
}

module_init(my_proc_init);
module_exit(my_proc_exit);
MODULE_LICENSE("GPL");

```

**用户态（应用程序）**

接下来，我们需要创建一个简单的用户态应用程序，用于与内核态通信。这个应用程序将使用procfs文件来读取和写入数据。

```c
#include<stdio.h>
#include <stdlib.h>
#include<string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PROC_FILE_PATH "/proc/my_proc_file"

int main() {
    int fd, n;
    char buffer[256];

    
    fd = open(PROC_FILE_PATH, O_RDONLY);
    if (fd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    n = read(fd, buffer, 255);
    if (n < 0) {
        perror("read");
        exit(EXIT_FAILURE);
    }

    buffer[n] = '\0';
    printf("Data from kernel: %s\n", buffer);

    close(fd);

    
    fd = open(PROC_FILE_PATH, O_WRONLY);
    if (fd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    n = write(fd, "24\n", 3);
    if (n < 0) {
        perror("write");
        exit(EXIT_FAILURE);
    }

    close(fd);

    
    fd = open(PROC_FILE_PATH, O_RDONLY);
    if (fd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    n = read(fd, buffer, 255);
    if (n < 0) {
        perror("read");
        exit(EXIT_FAILURE);
    }

    buffer[n] = '\0';
    printf("Data from kernel: %s\n", buffer);

    close(fd);

    return 0;
}

```

在这个示例中，我们使用了procfs文件来实现用户态和内核态之间的读写通信。用户态应用程序通过procfs文件读取和写入数据，内核态驱动程序将数据读取到内核空间并写入文件。



## 参考文章

1.  [Linux用户态/内核态通信方式汇总 - 付时凡 - 博客园 (cnblogs.com)](https://www.cnblogs.com/adam-ma/p/18084237)