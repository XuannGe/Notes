# Source Insight 4.0使用教程(快速入门)



Source Insight是一个功能非常强大的，面向项目的程序编辑器，具有针对C/C ++，C＃，Java，Objective-C等的内置动态分析。Source Insight可实现多文件代码中变量和函数的快速搜索，有利于代码的搜索与阅读，能够轻松地浏览和编辑代码，同时自动显相关信息，轻松跳转到函数的调用者或对变量的引用。



一，Source Insight的安装
-------------------

Source Insight的安装非常简单，按照提示一步一步安装就好。装包可以在[Source Insight的官方网站](https://www.sourceinsight.com/)获取，可以下载试用版免费使用30天。

运行Source Insight界面如下：

![](Source Insight 4.0使用教程(快速入门).assets/a281495c3aa087d4654d2773b4a62b63.png)



二，Source Insight的使用
-------------------

### 1，新建工程

选择 Project  → New Project 新建一个工程，输入工程名字和保存工程的路径，然后点击OK，如下图所示：

![](Source Insight 4.0使用教程(快速入门).assets/621662e5c02c43daffafc3158f599203.png)

然后，输入要编辑的程序源代码的路径，剩下的选项可以默认设置，然后点击OK，如下图所示：

![](Source Insight 4.0使用教程(快速入门).assets/f920ec464be31552af1820420c017982.png)

接下来，在我们新建工程以后，Source Insight 要求我们为所建的工程添加文件，我们可以通过接下来的这个窗口来为工程增加文件。选择需要添加的源代码，上一步中我们已经定位到源代码工程中，所以我们选择Add All，把所有文件添加到工程中，当然也可以自己选择添加，然后勾选上包含下级文件，点击OK，确定，然后关闭即可，如下图所示：

![](Source Insight 4.0使用教程(快速入门).assets/7ee78dd3d2ed1ef5000e1d361cf5759e.png)

需要注意，File Name框中显示的只是Source Insight所能支持的文件（.c  .cpp  .h等），对于ASM程序和其它文件将不会显示。我们可以通过勾选Show only known file types这一选项来选择显示其它类型的文件。

这样我们的新建工程完成就完成了，默认的会显示下图这三个小窗口，可以通过这三个按钮![](Source Insight 4.0使用教程(快速入门).assets/f7127fb1748e30c73854f56647d3e1c7.png)
来选择打开或者关闭，界面如下所示：

![](Source Insight 4.0使用教程(快速入门).assets/71a5da43eb72a09e856447ab96a54757.png)



### 2，认识工程以及源码阅读

当我们新建好工程之后，可以通过工程窗口来查看或者打开所添加的文件，打开工程中的process.c，左边弹出相应的源程序文本，同时，源程序的文本用不同的颜色表示代码的不同意义。

最左边部分为打开的源程序的结构和变量函数等信息，这个窗口方便我们观察源程序结构并且可以快速定位到变量和函数，并且用不同的图标来表示，如：

![](Source Insight 4.0使用教程(快速入门).assets/6849e46ca88a717b2d8acf93e0cc6e51.png)
表示头文件包含、条件编译等

![](Source Insight 4.0使用教程(快速入门).assets/00e394f0a84f19e249f33a6b17b3cc7e.png)
表示宏定义

![](Source Insight 4.0使用教程(快速入门).assets/32143927ee7a7328efc211d38e0a25d5.png)
表示变量

![](Source Insight 4.0使用教程(快速入门).assets/77c35ba0f745b6606bd3a4031f940d50.png)
表示函数

我们定位到process_start函数且选中process结构体变量时，下方Context窗口会显示结构体定义是在哪个文件的哪个位置定义的，双击久可以跳转到对应位置，右下边Relation窗口，也会显示结构体的结构，如下图所示：

![](Source Insight 4.0使用教程(快速入门).assets/a4868741965fa9850f5015586af2fbd8.png)



### 3，查找定位及Reference等功能

在进行源程序的阅读中，我们要查看函数变量的定义原型，可以右击选择Jump To Definition来定位，或者点击工具栏中的![](Source Insight 4.0使用教程(快速入门).assets/8f790b9bcf0a7995d75489e8898689df.png)
，当然也可以通过上面提到的Context窗口定位。

然后就是Source Insight中的Reference![](Source Insight 4.0使用教程(快速入门).assets/ad0c1049fe242f924883b5f55125d5c1.png)
功能，可以快速找出整个工程中所有的标记，并且在该行程序前加上红色双向箭头![](Source Insight 4.0使用教程(快速入门).assets/e619c15bbbb7425fe5c0c474eb9c0c54.png)
快速定位，如下图所示：![](Source Insight 4.0使用教程(快速入门).assets/697f7af4c50326dfa78a560b2f8c87b3.png)

我们可以通过点击![](Source Insight 4.0使用教程(快速入门).assets/e619c15bbbb7425fe5c0c474eb9c0c54.png)
展开或者收起Reference，也可以通过![](Source Insight 4.0使用教程(快速入门).assets/be8afb0e3f8429a9079780407ed9bc90.png)
这两个图标查看上一个标记或者下一个标记。通过Reference我们可以更好的阅读源程序。



## 原文链接

[Source Insight 4.0使用教程(快速入门)_source insight使用教程-CSDN博客](https://blog.csdn.net/qq_36986070/article/details/117170266)