# **一、freecplus框架介绍**

**freecplus框架是UNIX平台下C/C++程序开发的业务层基础框架，由C语言技术网组织开发、维护。其目的是为C/C++程序员供免费的、开源的程序库。freecplus框架与C++标准库结合使用，可以极大的提高程序员的开发效率。**

**freecplus框架的初衷是为C/C++程序员提供功能强大、简单易用的函数和类，把程序员从纷繁复杂的C/C++底层代码中解放出来，程序员将更关注软件功能和业务逻辑的实现。**

**freecplus框架中采用了第三方的开源库，如ftplib，并对这些库做了封装，形成更简单易用的类库。**

**在致力开源的C/C++程序员的努力下，freecplus将不断的优化、完善和扩展，希望有更多的优秀程序员加入。**

# **二、freecplus开源许可协议**

**freecplus开源许可协议正文如下：**

**版权所有 (c) 2008-2020，码农有道，C语言技术网（www.freecplus.net）。**

**特此向任何得到本软件副本或相关文档的人授权：被授权人有权使用、复制、修改、合并、出版、发布、散布、再授权和/或贩售软件及软件的副本，及授予被供应人同等权利，只需服从以下义务：**

**在软件和软件的所有副本中都必须包含以上版权声明和本许可声明。**

**如果违返，无论在本软件的使用上或其他买卖交易中是否涉及合同，都属于侵权行为，我们保留追究的权利。**

**freecplus开源许可协议正文结束。**

# **三、freecplus框架内容**

**按照实现的功能，freecplus框架有12个分类。**

**![image.png](https://www.freecplus.net/runoobFiles/ueditor/image/20200315/1584280841172036028.png)**

## **1、字符串操作**

**字符串操作函数和类，包括：**

**1）安全的字符串复制函数；**

**2）安全的字符串拼接函数；**

**3）安全的格式化输出到字符串的函数；**

**4）删除字符串左边、右边和两边指定字符的函数；**

**5）字符串大小写转换函数；**

**6）字符串替换函数；**

**7）从字符串中提取数字的函数；**

**8）正则表达式函数；**

**9）字符串拆分的类；**

**10）   统计字符串字数的函数。**

## **2、xml解析**

**从xml格式的字符串中解析出字段。**

## **3、日期时间**

**1）日期时间的获取、转换和运算；**

**2）把整数表示的时间转换为字符串表示的时间；**

**3）把字符串表示的时间转换为整数表示的时间；**

**4）时间运算；**

**5）精确到微秒的计时器。**

## **4、目录操作**

**1）创建目录以及子目录；**

**2）递归获取目录以及子目录中的文件信息。**

## **5、文件操作**

**1）打开、读取、删除、重命名、复制文件；**

**2）获取文件的大小和时间；**

**3）重置文件的时间；**

**4）常用的文件操作方法类。**

## **6、日志文件**

**把C/C++服务程序记录程序运行和数据处理的日志记录到文件中，包括日志文件的创建、切换、备份等。**

## **7、参数文件**

**加载C/C++服务程序运行参数。**

## **8、tcp网络通信**

**1）封装了socket系列函数，实现tcp客户端，包括报文的接收和发送；**

**2）封装了socket系列函数，实现tcp服务端，包括报文的接收和发送；**

**3）解决了tcp报文粘包的问题；**

**4）通信可以设置超时机制。**

## **9、ftp客户端**

**采用ftp协议，实现ftp客户端的常用功能。**

## **10、Oracle数据库操作**

**封装了OCI（Oracle Call Interface）函数库，实现对Oracle数据库的操作。**

## **11、MySQL数据库操作**

**封装了MySQL提供的mysqlclient函数库，实现对MySQL数据库的操作。**

## **12、PostgreSQL数据库操作**

**封装了PostgreSQL提供的pq函数库，实现对PostgreSQL数据库的操作。**

# **四、应用举例**

**C/C++语言操作数据库是一个技术难点，做过这方面开发程序员深有体会，尤其是Oracle数据库，Pro\*C的效率低下，代码冗长，OCI功能强大，但是难以驾驭。在freecplus框架中，把Oracle的OCI函数封装成了connection和sqlstatement两个类，操作Oracle数据库如探囊取物。**

**以下示例（createtable.cpp）是采用freecplus框架创建表的代码。**

```
/*
 *  程序名：createtable.cpp，此程序演示freecplus框架操作Oracle数据库（创建表）。
 *  作者：C语言技术网(www.freecplus.net) 日期：20190525
*/
#include "_ooci.h"   // freecplus框架操作Oracle的头文件。
 
int main(int argc,char *argv[])
{
  connection conn; // 数据库连接类
 
  // 登录数据库，返回值：0-成功，其它-失败。
  // 失败代码在conn.m_cda.rc中，失败描述在conn.m_cda.message中。
  if (conn.connecttodb("scott/tiger@snorcl11g_198","Simplified Chinese_China.ZHS16GBK")!=0)
  {
    printf("connect database failed.\n%s\n",conn.m_cda.message); return -1;
  }
 
  sqlstatement stmt(&conn); // 操作SQL语句的对象。
 
  // 准备创建表的SQL语句。
  // 超女表girls，超女编号id，超女姓名name，体重weight，报名时间btime，超女说明memo，超女图片pic。
  stmt.prepare("\
    create table girls(id    number(10),\
                       name  varchar2(30),\
                       weight   number(8,2),\
                       btime date,\
                       memo  clob,\
                       pic   blob,\
                       primary key (id))");
  // prepare方法不需要判断返回值。
 
  // 执行SQL语句，一定要判断返回值，0-成功，其它-失败。
  // 失败代码在stmt.m_cda.rc中，失败描述在stmt.m_cda.message中。
  if (stmt.execute() != 0)
  {
    printf("stmt.execute() failed.\n%s\n%s\n",stmt.m_sql,stmt.m_cda.message); return -1;
  }
 
  printf("create table girls ok.\n");
}
```

**运行效果**

**![image.png](https://www.freecplus.net/runoobFiles/ueditor/image/20200315/1584280926357030219.png)**

# **五、文档和下载**

**本文简单介绍了freecplus框架的基本情况。**

**更多文档请访问“C语言技术网->freecplus框架”栏目。**

**源代码下载请访问“C语言技术网->资源下载”栏目。**

# **六、版权声明**

**C语言技术网原创文章，转载请说明文章的来源、作者和原文的链接。**

**来源：C语言技术网（**[**www.freecplus.net**](http://www.freecplus.net/)**）**

**作者：码农有道**

**如果文章有错别字，或者内容有错误，或其他的建议和意见，请您联系我们指正，非常感谢！！！**