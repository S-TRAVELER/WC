
![wc](https://note.youdao.com/yws/public/resource/7a96c956afc5000fec65397c4cb7c628/xmlnote/7E687711661049E68F4DC0107872949F/10367)

软工作业: 现wc命令
基本功能列表：
```
wc.exe -c file.c     //返回文件 file.c 的字符数

wc.exe -w file.c    //返回文件 file.c 的词的数目  

wc.exe -l file.c      //返回文件 file.c 的行数
```

扩展功能：

    -s   递归处理目录下符合条件的文件。
    
    -a   返回更复杂的数据（代码行 / 空行 / 注释行）。

空行：本行全部是空格或格式控制字符，如果包括代码，则只有不超过一个可显示的字符，例如“{”。

代码行：本行包括多于一个字符的代码。

注释行：本行不是代码行，并且本行包括注释。一个有趣的例子是有些程序员会在单字符后面加注释：

    } //注释
在这种情况下，这一行属于注释行。

[file_name]: 文件或目录名，可以处理一般通配符。

高级功能：

 -x 参数。这个参数单独使用。如果命令行有这个参数，则程序会显示图形界面，用户可以通过界面选取单个文件，程序就会显示文件的字符数、行数等全部统计信息。

需求举例：
```
　　wc.exe -s -a *.c
```

返回当前目录及子目录中所有*.c 文件的代码行数、空行数、注释行数。
