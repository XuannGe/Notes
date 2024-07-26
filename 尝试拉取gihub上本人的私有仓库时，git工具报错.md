# 尝试拉取gihub上本人的私有仓库时，git工具报错



尝试拉取gihub上的本人的私有仓库时，git工具报错：



报错内容
----

```shell
$ git clone https://github.com/XuannGe/Notes.git
Cloning into 'Notes'...
fatal: unable to access 'https://github.com/XuannGe/Notes.git/': HTTP/2 stream 1 was not closed cleanly before end of the underlying stream
```



原因
---

git默认使用http/2.0协议，而github还是http/1.1 。



解决方案
-----

```shell
git config --global http.version HTTP/1.1
```



现场1
--

```shell
轩哥@LAPTOP-BE8ALCTH MINGW64 /d/Notes (master)
$ git config --global http.version HTTP/1.1

轩哥@LAPTOP-BE8ALCTH MINGW64 /d/Notes (master)
$ git clone https://github.com/XuannGe/Notes.git
Cloning into 'Notes'...
remote: Write access to repository not granted.
fatal: unable to access 'https://github.com/XuannGe/Notes.git/': The requested URL returned error: 403

```

此时http协议不正确的问题已经解决，再次报错是有其它原因



## 再次报错原因

很明显是仓库权限问题，先将本地仓库删除。

再重新拉取远程仓库到本地。

此时会要求你输入token，去github上申请一个即可。

输入token，便可成功下载仓库。

可以参考这篇文章：

[github 配置使用 personal access token 认证_github person token-CSDN博客](https://blog.csdn.net/qq_39218530/article/details/119809170)



## 现场2

```shell
轩哥@LAPTOP-BE8ALCTH MINGW64 /d/Notes (master)
$ git clone https://github.com/XuannGe/Notes.git
Cloning into 'Notes'...
warning: You appear to have cloned an empty repository.
```



## 参考原文

1.[【Git】unable to access https://github.com/xxx/xxx: HTTP/2 stream 1 was not closed cleanly before-CSDN博客](https://blog.csdn.net/m0_47406832/article/details/123044722)

2.[github 配置使用 personal access token 认证_github person token-CSDN博客](https://blog.csdn.net/qq_39218530/article/details/119809170)