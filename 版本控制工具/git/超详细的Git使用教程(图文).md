# 超详细的Git使用教程(图文)



### 一、Git概述

#### 1.1 Git历史

Git 诞生于一个极富纷争大举创新的年代。Linux 内核开源项目有着为数众多的参与者。 绝大多数的 Linux 内核维护工作都花在了提交补丁和保存归档的繁琐事务上（1991－2002年间）。 到 2002 年，整个项目组开始启用一个专有的分布式版本控制系统 BitKeeper 来管理和维护代码。  
到了 2005 年，开发 BitKeeper 的商业公司同 Linux 内核开源社区的合作关系结束，他们收回了 Linux 内核社区免费使用 BitKeeper 的权力。 这就迫使 Linux 开源社区（特别是 Linux 的缔造者 Linus Torvalds）基于使用 BitKeeper 时的经验教训，开发出自己的版本系统。  
他们对新的系统制订了若干目标：

*   速度
*   简单的设计
*   对非线性开发模式的强力支持（允许成千上万个并行开发的分支）
*   完全分布式
*   有能力高效管理类似 Linux 内核一样的超大规模项目（速度和数据量）



#### 1.2 Git与SVN对比

SVN是集中式版本控制系统，版本库是集中放在中央服务器的，而开发人员工作的时候，用的都是自己的电脑，所以首先要从中央服务器下载最新的版本，然后开发，开发完后，需要把自己开发的代码提交到中央服务器。

集中式版本控制工具缺点： 

- 服务器单点故障 
- 容错性差 
  ![](超详细的Git使用教程(图文).assets/561be95264a77ae1bddd698db7c890c8.png)

Git是分布式版本控制系统（Distributed Version Control System，简称 DVCS） ，分为两种类型的仓库：  

本地仓库和远程仓库。

本地仓库：是在开发人员自己电脑上的Git仓库  
远程仓库：是在远程服务器上的Git仓库

Clone：克隆，就是将远程仓库复制到本地  
Push：推送，就是将本地仓库代码上传到远程仓库  
Pull：拉取，就是将远程仓库代码下载到本地仓库  
![](超详细的Git使用教程(图文).assets/fa4c63f92108cca0984d0474699d1cb9.png)



#### 1.3 Git工作流程

工作流程如下：  
1．从远程仓库中克隆代码到本地仓库  
2．从本地仓库中checkout代码然后进行代码修改  
3．在提交前先将代码提交到暂存区  
4．提交到本地仓库。本地仓库中保存修改的各个历史版本  
5．修改完成后，需要和团队成员共享代码时，将代码push到远程仓库  
![](超详细的Git使用教程(图文).assets/7c537ec949ab678006f585b798a3a45e.png)



#### 1.4 Git下载与安装

下载地址： https://git-scm.com/download  
![](超详细的Git使用教程(图文).assets/b6885027263f83c0ed0544109eaed3f6.png)

下载完成后可以得到如下安装文件：  
![](超详细的Git使用教程(图文).assets/e17456749b66793ac1864fc5f3bcb656.png)

这里默认下载的是64位的软件

双击下载的安装文件来安装Git。  
一直下一步直到安装完成即可  
安装完成后在电脑桌面（也可以是其他目录）点击右键，如果能够看到如下两个菜单则说明Git安装成功。  
![](超详细的Git使用教程(图文).assets/a99c5a7d9c1d93b0c8c7e1bb2d757343.png)

**Git GUI：Git提供的图形界面工具  
Git Bash：Git提供的命令行工具**



### 二、Git代码托管服务

#### 2.1 常用的Git代码托管服务

前面我们已经知道了Git中存在两种类型的仓库，即本地仓库和远程仓库。那么我们如何搭建Git远程仓库呢？我们可以借助互联网上提供的一些代码托管服务来实现，其中比较常用的有GitHub、码云、GitLab等。

gitHub（ 地址：https://github.com/ ）是一个面向开源及私有软件项目的托管平台，因为只支持Git 作为唯一的版本库格式进行托管，故名gitHub

码云（地址： https://gitee.com/ ）是国内的一个代码托管平台，由于服务器在国内，所以相比于GitHub，码云速度会更快

GitLab （地址： https://about.gitlab.com/ ）是一个用于仓库管理系统的开源项目，使用Git作为代码管理工具，并在此基础上搭建起来的web服务

本次使用码云作为演示 码云在国内用起来网速比GitHub快



#### 2.2 在码云注册账号

要想使用码云的相关服务，需要注册账号（地址： https://gitee.com/signup ）  
![](超详细的Git使用教程(图文).assets/921a06c04a2edf772ecce2541be6ccae.png)



#### 2.3 登录码云并创建Git远程仓库

注册完成后就可以使用刚刚注册的邮箱进行登录（地址： https://gitee.com/login ）  
![](超详细的Git使用教程(图文).assets/8fd31d38c7578ea6b77c8ce61496a112.png)

登录成功后就可以创建Git远程仓库  
![](超详细的Git使用教程(图文).assets/e90090822bf09ada97ffde08df7350e5.png)

![](超详细的Git使用教程(图文).assets/eac994b78a6df81eadd25a08bbdbba62.png)

创建完成后可以查看仓库信息  
![](超详细的Git使用教程(图文).assets/c5a5a6f649b569c913e6d5fe9a9f8b22.png)

每个Git远程仓库都会对应一个网络地址，可以点击克隆/下载按钮弹出窗口并点击复制按钮获得这个网络地址  
![](超详细的Git使用教程(图文).assets/709790f0c88e529014364f62a8c57e83.png)



#### 2.4 邀请其他用户成为仓库成员

前面已经在码云上创建了自己的远程仓库，目前仓库成员只有自己一个人（身份为管理员）。在企业实际开发中，一个项目往往是由多个人共同开发完成的，为了使多个参与者都有权限操作远程仓库，就需要邀请其他项目参与者成为当前仓库的成员。  
![](超详细的Git使用教程(图文).assets/bf1e56a738c6c23423cdcee34285bc23.png)



### 三、Git常用命令

先学习如下一些命令和概念：

*   环境配置
*   获取Git仓库
*   工作目录、暂存区以及版本库概念
*   Git工作目录下文件的两种状态
*   本地仓库操作
*   远程仓库的使用
*   分支
*   标签



#### 3.1 环境配置

当安装Git后首先要做的事情是设置用户名称和email地址。这是非常重要的，因为每次Git提交都会使用该用户信息

设置用户信息  

`git config --global user.name “itcast”  
git config --global user.email “kinggm520@163.com”`  

查看配置信息  

`git config --list  
git config user.name`

通过上面的命令设置的信息会保存在~/.gitconfig文件中



#### 3.2 获取Git仓库

要使用Git对我们的代码进行版本控制，首先需要获得Git仓库

获取Git仓库通常有两种方式：  

- 在本地初始化一个Git仓库  
- 从远程仓库克隆



##### 3.2.1在本地初始化一个Git仓库

执行步骤如下：

1.  在电脑的任意位置创建一个空目录（例如repo1）作为我们的本地Git仓库
2.  进入这个目录中，点击右键打开Git bash窗口
3.  执行命令`git init`

如果在当前目录中看到.git文件夹（此文件夹为隐藏文件夹）则说明Git仓库创建成功  
![](超详细的Git使用教程(图文).assets/d675e0dd8f51cac795c62b2aa4cf149b.png)



##### 3.2.2从远程仓库克隆

可以通过Git提供的命令从远程仓库进行克隆，将远程仓库克隆到本地  
命令形式为：`git clone 远程Git仓库地址`


![](超详细的Git使用教程(图文).assets/9137ca8affc3802297fe675c31f36b23.png)



#### 3.3工作目录、暂存区以及版本库概念

为了更好的学习Git，我们需要了解Git相关的一些概念，这些概念在后面的学习中会经常提到

版本库：前面看到的.git隐藏文件夹就是版本库，版本库中存储了很多配置信息、日志信息和文件版本信息等  
工作目录（工作区）：包含.git文件夹的目录就是工作目录，主要用于存放开发的代码  
暂存区：.git文件夹中有很多文件，其中有一个index文件就是暂存区，也可以叫做stage。暂存区是一个临时保存修改文件的地方  
![](超详细的Git使用教程(图文).assets/9749d10bf3949ce5b960f1475f6c5fca.png)

#### 3.4 Git工作目录下文件的两种状态

Git工作目录下的文件存在两种状态：  

- untracked 未跟踪（未被纳入版本控制）  
- tracked 已跟踪（被纳入版本控制）  
- Unmodified 未修改状态  
- Modified 已修改状态  
- Staged 已暂存状态

这些文件的状态会随着我们执行Git的命令发生变化



#### 3.5 本地仓库操作

`git status` 查看文件状态 
![](超详细的Git使用教程(图文).assets/ff2dc02f8cafe9c88da881dbeaa26bfa.png)

也可以使用`git status –s` 使输出信息更加简洁 
![](超详细的Git使用教程(图文).assets/d007588e6b9786693b1d03e3e200145b.png)

`git add` 将未跟踪的文件加入暂存区 
![](超详细的Git使用教程(图文).assets/2e324ecb684b47e7a7dc83d5d6e07b3d.png)

将新创建的文件加入暂存区后查看文件状态  
![](超详细的Git使用教程(图文).assets/7fae63d876ce07465751524c61221f35.png)

`git reset` 将暂存区的文件取消暂存 
![](超详细的Git使用教程(图文).assets/27db0ec0113f287efc1929a4264d8a25.png)

将文件取消暂存后查看文件状态 
![](超详细的Git使用教程(图文).assets/6bc2df6be6cee5904dd356bb42d6bb4c.png)

`git commit` 将暂存区的文件修改提交到本地仓库
![](超详细的Git使用教程(图文).assets/8170279b59a8ac05bdad172fa08a759f.png)

`git rm` 删除文件 
![](超详细的Git使用教程(图文).assets/7d9854899140127628cd2200e00ab65b.png)

删除文件后查看文件状态 
![](超详细的Git使用教程(图文).assets/13a3c3b2f26263ca12a55f7732f2e629.png)

上面删除的只是工作区的文件，需要提交到本地仓库  
![](超详细的Git使用教程(图文).assets/f1b9eabbe771442570f4d13e9505ab97.png)

将文件添加至忽略列表 
一般我们总会有些文件无需纳入Git 的管理，也不希望它们总出现在未跟踪文件列表。 通常都是些自动生成的文件，比如日志文件，或者编译过程中创建的临时文件等。 在这种情况下，我们可以在工作目录中创建一个名为 .gitignore 的文件（文件名称固定），列出要忽略的文件模式。下面是一个示例：

```shell
`# no .a files
*.a
# but do track lib.a, even though you're ignoring .a files above
!lib.a
# only ignore the TODO file in the current directory, not subdir/TODO
/TODO
# ignore all files in the build/ directory
build/
# ignore doc/notes.txt, but not doc/server/arch.txt
doc/*.txt
# ignore all .pdf files in the doc/ directory
doc/**/*.pdf` 
```



#### 3.6 远程仓库操作

前面执行的命令操作都是针对的本地仓库，本章节我们会学习关于远程仓库的一些操作，具体包括：

*   查看远程仓库
*   添加远程仓库
*   从远程仓库克隆
*   移除无效的远程仓库
*   从远程仓库中抓取与拉取
*   推送到远程仓库



**查看远程仓库**  
如果想查看已经配置的远程仓库服务器，可以运行 `git remote` 命令。 它会列出指定的每一个远程服务器的简写。 如果已经克隆了远程仓库，那么至少应该能看到 origin ，这是 Git 克隆的仓库服务器的默认名字 
![](超详细的Git使用教程(图文).assets/8fd34ca0e500703c05de29ba5ebd5911.png)

![](超详细的Git使用教程(图文).assets/6429a3cc6d160d228eeee87a1b6b7574.png)

**添加远程仓库**  
运行 `git remote add < shortname> < url>`

 添加一个新的远程 Git 仓库，同时指定一个可以引用的简写  
![](超详细的Git使用教程(图文).assets/ade48c79bb70029394dd20e498d7db33.png)

**从远程仓库克隆**  
如果你想获得一份已经存在了的 Git 仓库的拷贝，这时就要用到 git clone 命令。 Git 克隆的是该 Git 仓库服务器上的几乎所有数据（包括日志信息、历史记录等），而不仅仅是复制工作所需要的文件。 当你执行 git clone 命令的时候，默认配置下远程 Git 仓库中的每一个文件的每一个版本都将被拉取下来。  
克隆仓库的命令格式是 `git clone [url]`  
![](超详细的Git使用教程(图文).assets/a5d27e964eec39b5be5b72ad081fa818.png)

**移除无效的远程仓库**  
如果因为一些原因想要移除一个远程仓库 ，可以使用 git remote rm  
![](超详细的Git使用教程(图文).assets/1dfceb122d4007f27f07e6cd28ee622b.png)

注意：此命令只是从本地移除远程仓库的记录，并不会真正影响到远程仓库

**从远程仓库中抓取与拉取**  
`git fetch` 是从远程仓库获取最新版本到本地仓库，不会自动merge  
![](超详细的Git使用教程(图文).assets/26d01c682162637e97a5ee52b67f513b.png)

`git pull` 是从远程仓库获取最新版本并merge到本地仓库 
![](超详细的Git使用教程(图文).assets/a15c7b133d6c4735a2991ed7c1de99f7.png)

**注意：**如果当前本地仓库不是从远程仓库克隆，而是本地创建的仓库，并且仓库中存在文件，此时再从远程仓库拉取文件的时候会报错**（fatal: refusing to merge unrelated histories ）**，解决此问题可以在`git pull`命令后加入参数`–allow-unrelated-histories` 
当执行git中的“`git pull origin master –allow-unrelated-histories`”命令时，会出现“ couldn’t find remote ref –allow-unrelated-histories”的错误，  
输入如下命令即可解决：  
`git pull --rebase origin master`

**将本地分支与远程分支关联**

`git branch --set-upstream-to origin/master master` 

**推送到远程仓库**  
当你想分享你的代码时，可以将其推送到远程仓库。 命令形式：`git push [remote-name] [branch-name]`  
![](超详细的Git使用教程(图文).assets/5642b76f058365d3f3e546687cfe5bdb.png)



#### 3.7 Git分支

几乎所有的版本控制系统都以某种形式支持分支。 使用分支意味着你可以把你的工作从开发主线上分离开来，以免影响开发主线。Git 的master分支并不是一个特殊分支。 它跟其它分支没有区别。 之所以几乎每一个仓库都有 master 分支，是因为git init 命令默认创建它，并且大多数人都懒得去改动它。  
在本章节我们会学习到关于分支的相关命令，具体如下：

*   查看分支
*   创建分支
*   切换分支
*   推送至远程仓库分支
*   合并分支
*   删除分支



**查看分支**

*   列出所有本地分支  
    `git branch`
*   列出所有远程分支  
    `git branch -r`
*   列出所有本地分支和远程分支 
    `git branch -a`

![](超详细的Git使用教程(图文).assets/c5f583ce7ecddde5987c7d5d2848d0cf.png)

**创建分支**  
![](超详细的Git使用教程(图文).assets/dcd1380af9b847b194b7a474161c2e3b.png)

**切换分支**  
![](超详细的Git使用教程(图文).assets/08797146184ba91c95c49d475de54015.png)

**推送至远程仓库分支**  
![](超详细的Git使用教程(图文).assets/d7fed95c71e914277c216bb2b30d913c.png)

![](超详细的Git使用教程(图文).assets/da1166f89fc3388c696a590dcf61bc98.png)

**合并分支**  
![](超详细的Git使用教程(图文).assets/956417771f1093899e300a63dad10f9f.png)

有时候合并操作不会如此顺利。 如果你在两个不同的分支中，对同一个文件的同一个部分进行了不同的修改，Git 就没办法合并它们，同时会提示文件冲突。此时需要我们打开冲突的文件并修复冲突内容，最后执行git add命令来标识冲突已解决  
![](超详细的Git使用教程(图文).assets/c7f7330f989bdb16e1e1e9f49ca6d9a7.png)

**删除分支**  
![](超详细的Git使用教程(图文).assets/2388e3692cf9fad3deaa2a95e3dc1be1.png)

如果要删除的分支中进行了一些开发动作，此时执行上面的删除命令并不会删除分支，如果坚持要删除此分支，可以将命令中的-d参数改为-D 
![](超详细的Git使用教程(图文).assets/e7358447047c473e8ef675a2b9989588.png)

**综合应用**  
前面我们已经学习完成了Git分支相关的命令，本章节我们通过一个现实中的工作场景来对这些命令进行一个综合练习。

工作场景如下：  
开发某个网站。  
为实现某个新的需求，创建一个分支。  
在这个分支上开展工作。  
正在此时，你突然接到一个电话说有个很严重的问题需要紧急修补。 你将按照如下方式来处理：  
切换到你的线上分支（production branch）。  
为这个紧急任务新建一个分支，并在其中修复它。  
在测试通过之后，切换回线上分支，然后合并这个修补分支，最后将改动推送到线上分支。  
切换回你最初工作的分支上，继续工作。



#### 3.8 Git标签

像其他版本控制系统（VCS）一样，Git 可以给历史中的某一个提交打上标签，以示重要。 比较有代表性的是人们会使用这个功能来标记发布结点（v1.0 、v1.2等）。标签指的是某个分支某个特定时间点的状态。通过标签，可以很方便的切换到标记时的状态。

**下面将学习：** 

*   列出已有的标签
*   创建新标签
*   将标签推送至远程仓库
*   检出标签
*   删除标签

```shell
列出已有的标签
# 列出所有tag
$ git tag
# 查看tag信息
$ git show [tag]
```

```shell
创建新标签
# 新建一个tag
$ git tag [tagName]`

```

![](超详细的Git使用教程(图文).assets/337c9717d05527c13e9bf4a34097c47f.png)

**将标签推送至远程仓库**

```shell
# 提交指定tag
$ git push [remote] [tag]
```

![](超详细的Git使用教程(图文).assets/d09bb4bac4066a3ae6a8dc60062bf908.png)

**检出标签**

```shell
# 新建一个分支，指向某个tag
$ git checkout -b [branch] [tag]
```

![](超详细的Git使用教程(图文).assets/49ee7f9eaff738decf19ab37ee39cac8.png)

**删除标签**

```shell
# 删除本地tag
$ git tag -d [tag]
# 删除远程tag
$ git push origin :refs/tags/[tag]`
```

![](超详细的Git使用教程(图文).assets/c680d6f015b5816f40f9e82feb5453e7.png)

![](超详细的Git使用教程(图文).assets/bd7e11667da93b063c552f6e2459a90f.png)



### 四、在IDEA中使用Git

#### 4.1 在IDEA中配置Git

安装好IntelliJ IDEA后，如果Git安装在默认路径下，那么idea会自动找到git的位置，如果更改了Git的安装位置则需要手动配置下Git的路径。  
选择File→Settings打开设置窗口，找到Version Control下的git选项：  
![](超详细的Git使用教程(图文).assets/05e358e8bcc9a29cb674ff6676b14578.png)

选择git的安装目录后可以点击“Test”按钮测试是否正确配置  
下图表示正确  
![](超详细的Git使用教程(图文).assets/36460f7816ba2d85880ccfe31365c826.png)



#### 4.2 在IDEA中使用Git

本章节我们会学习在IDEA中使用Git进行版本管理，具体包括：

*   在IDEA中创建工程并将工程添加至Git
*   将文件添加到暂存区
*   提交文件
*   将代码推送到远程仓库
*   从远程仓库克隆工程到本地
*   从远程拉取代码
*   版本对比
*   创建分支
*   切换分支
*   分支合并

**在IDEA中创建工程并将工程添加至Git**  
![](超详细的Git使用教程(图文).assets/c8b725c0cf19c2e4c2e4cb3114dd777c.png)

将项目添加至Git管理后，可以从IDEA的工具栏上看到Git操作的按钮

**将文件添加到暂存区**  
![](超详细的Git使用教程(图文).assets/82b1fa19f385a88c46876e5264d9a6b3.png)

**提交文件**  
![](超详细的Git使用教程(图文).assets/a5cf7f86236844a37d94f8d93899847c.png)

![](超详细的Git使用教程(图文).assets/6a179f7d9bd4346ae32fb53407c3cfe4.png)

**将代码推送到远程仓库**  
![](超详细的Git使用教程(图文).assets/0a688a87b99fcbafc5442266b13fd834.png)

**从远程仓库克隆工程到本地**  
![](超详细的Git使用教程(图文).assets/918d2a0e598b0d5b3f15ece95d26faaa.png)

**从远程拉取代码**  
![](超详细的Git使用教程(图文).assets/a6fd26ed35b8a28882a4bc7f2d80ad7f.png)

![](超详细的Git使用教程(图文).assets/0d85c537c552e96582cf30e2e8aa5f58.png)

**版本对比**  
在代码页面右键  
![](超详细的Git使用教程(图文).assets/03b86181ba972b24643c5a2e0395819a.png)

**创建分支**  
![](超详细的Git使用教程(图文).assets/bb5c82586dac78bb67e81fad7b0bfd69.png)

![](超详细的Git使用教程(图文).assets/11747d9de98e00564aeb860912ea4d65.png)

**切换分支**  
![](超详细的Git使用教程(图文).assets/c5072a5d124fc7684b7e9fdc0a856230.png)

![](超详细的Git使用教程(图文).assets/fb634d990faa0e7c8eefc80975a4d98f.png)

**分支合并**  
![](超详细的Git使用教程(图文).assets/debe72fb2466fbd2b1f9647a6eb11f93.png)

![](超详细的Git使用教程(图文).assets/1ffd4c023e2da3664432374a8754829b.png)

### 最后新手难免犯错误 如果代码提交错误了怎么办

[参考我的另一篇文章 如何回滚代码  
https://blog.csdn.net/qq_37883866/article/details/114375872](https://blog.csdn.net/qq_37883866/article/details/114375872)



## 原文链接

[超详细的Git使用教程(图文)-CSDN博客](https://blog.csdn.net/qq_37883866/article/details/105349257)