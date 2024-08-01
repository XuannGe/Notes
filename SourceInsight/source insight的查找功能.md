# source insight的查找功能



查找参数：
whole words only :          全字匹配查找
case sensitive  ：          区分大小写
project wide  :             项目内查找
include subdirectories :      包含子目录查找
skip inactive code :         跳过无效代码查找
skip comments  :          跳过注释查找
search only comments :     仅在注释在查找

 smart reference matching 智能参考匹配

touch files and cause recompile 触摸文件并导致重新编译

 

我们最常用的一种查找方式是选择Search/Lookup References或按Ctrl+/组合键再或者鼠标点 R 按钮，在弹出的Loopup References窗口进行查找操作。 在Search Method中有四种可选的查找方式：Simple String、Regular Expression、 Keyword Expression和Look Up Reference。其中Simple String是最普通的查找方式，可以查找文件中出现的任意字符或字符，甚至可以查找 _upap || u 这样的字符串，但是在工程较大时，查找过程会较慢。 Regular Expression查找方式将在后面讲述正则表达时会介绍到，这里暂时按下不表。 Keyword Expression和Look Up Reference查找的结果基本相同，但是显示方式略有差异。这两种方式都是使用SI预先建立的数据库，查找起来速度相当快。但通常这种速度只对在查找标识符时有明显效果。对于像函数名，变量名等的查找，强烈建议使用这两种方式进行查找。
Search Files
选择Search/Search Files或按Ctrl+Shift+F组合键，在弹出的Search Files窗口进行查找操作。 在File Name框中可以填入文件名或文件夹。注意当要查询的文件夹双包含子文件夹时，可以勾选Options中的Include Subdirectiories，实现对各层文件的递归搜索。
 Search Project
选择Search/Search Project，在弹出的Search Project窗口进行查找操作。操作与Loopup References几乎完全一致，它们各自保存上次搜索的配置。



## 原文链接

[source insight的查找功能](https://www.cnblogs.com/xiaohujian/p/12460508.html)