/**************************************************************************************/
/*   程序名：_mysql.h，此程序是freecplus框架的C/C++操作MySQL数据库的声明文件。        */
/*   作者：C语言技术网(www.freecplus.net) 日期：20190525。                            */
/*                                                                                    */
/*   freecplus开源许可协议正文如下：                                                  */
/*     版权所有 (c) 2008-2020，码农有道，C语言技术网（www.freecplus.net）。           */
/*     特此向任何得到本软件副本或相关文档的人授权：被授权人有权使用、复制、修改、     */
/*   合并、出版、发布、散布、再授权和/或贩售软件及软件的副本，及授予被供应人同等      */
/*   权利，只需服从以下义务：                                                         */
/*     在软件和软件的所有副本中都必须包含以上版权声明和本许可声明。                   */
/*     如果违返，无论在本软件的使用上或其他买卖交易中是否涉及合同，都属于侵权行为，   */
/*   我们保留追究的权利。                                                             */
/*   freecplus开源许可协议正文结束。                                                  */
/**************************************************************************************/

#ifndef __MYSQL_H
#define __MYSQL_H

// C/C++库常用头文件
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <mysql.h>   // MySQL数据库接口函数的头文件

// MySQL登录环境
struct LOGINENV
{
  char ip[32];       // MySQL数据库的ip地址。
  int  port;         // MySQL数据库的通信端口。
  char user[32];     // 登录MySQL数据库的用户名。
  char pass[32];     // 登录MySQL数据库的密码。
  char dbname[51];   // 登录后，缺省打开的数据库。
};

struct CDA_DEF       // 调用MySQL接口函数执行的结果。
{
  int      rc;          // 返回值：0-成功，其它失败。
  unsigned long rpc;    // 如果是insert、update和delete，保存影响记录的行数，如果是select，保存结果集的行数。
  char     message[2048];  // 执行SQL语句如果失败，存放错误描述信息。
};

// MySQL数据库连接池类。
class connection
{
private:
  // 从connstr中解析ip,username,password,dbname,port。
  void setdbopt(char *connstr);

  // 设置字符集，要与数据库的一致，否则中文会出现乱码
  void character(char *charset);

  LOGINENV m_env; // 服务器环境句柄。

  char m_dbtype[21];   // 数据库种类，固定取值为"mysql"。
public:
  int m_state;         // 与数据库的连接状态，0-未连接，1-已连接。

  CDA_DEF m_cda;       // 数据库操作的结果或最后一次执行SQL语句的结果。

  char m_sql[10241];   // SQL语句的文本，最长不能超过10240字节。

  connection();        // 构造函数。
 ~connection();        // 析构函数。

  // 登录数据库。
  // connstr：数据库的登录参数，格式："ip,username,password,dbname,port"，
  // 例如："172.16.0.15,qxidc,qxidcpwd,qxidcdb,3306"。
  // charset：数据库的字符集，如"gbk"，必须与数据库保持一致，否则会出现中文乱码的情况。
  // autocommitopt：是否启用自动提交，0-不启用，1-启用，缺省是不启用。
  // 返回值：0-成功，其它失败，失败的代码在m_cda.rc中，失败的描述在m_cda.message中。
  int connecttodb(char *connstr,char *charset,unsigned int autocommitopt=0);

  // 提交事务。
  // 返回值：0-成功，其它失败，程序员一般不必关心返回值。
  int commit();

  // 回滚事务。
  // 返回值：0-成功，其它失败，程序员一般不必关心返回值。
  int  rollback();

  // 断开与数据库的连接。
  // 注意，断开与数据库的连接时，全部未提交的事务自动回滚。
  // 返回值：0-成功，其它失败，程序员一般不必关心返回值。
  int disconnect();

  // 执行SQL语句。
  // 如果SQL语句不需要绑定输入和输出变量（无绑定变量、非查询语句），可以直接用此方法执行。
  // 参数说明：这是一个可变参数，用法与printf函数相同。
  // 返回值：0-成功，其它失败，失败的代码在m_cda.rc中，失败的描述在m_cda.message中，
  // 如果成功的执行了非查询语句，在m_cda.rpc中保存了本次执行SQL影响记录的行数。
  // 程序员必须检查execute方法的返回值。
  // 在connection类中提供了execute方法，是为了方便程序员，在该方法中，也是用sqlstatement类来完成功能。
  int execute(const char *fmt,...);

  ////////////////////////////////////////////////////////////////////
  // 以下成员变量和函数，除了sqlstatement类，在类的外部不需要调用它。
  MYSQL     *m_conn;   // MySQL数据库连接句柄。
  int m_autocommitopt; // 自动提交标志，0-关闭自动提交；1-开启自动提交。
  void err_report();   // 获取错误信息。
  ////////////////////////////////////////////////////////////////////
};

// 执行SQL语句前绑定输入或输出变量个数的最大值，256是很大的了，可以根据实际情况调整。
#define MAXPARAMS  256

// 操作SQL语句类。
class sqlstatement
{
private:
  MYSQL_STMT *m_handle; // SQL语句句柄。
  
  MYSQL_BIND params_in[MAXPARAMS];  // 输入参数。
  unsigned long params_in_length[MAXPARAMS];
  my_bool params_in_is_null[MAXPARAMS];
  enum enum_field_types params_in_buffer_type[MAXPARAMS];
  MYSQL_BIND params_out[MAXPARAMS]; // 输出参数。
  unsigned maxbindin;
  
  connection *m_conn;  // 数据库连接池指针。
  int m_sqltype;       // SQL语句的类型，0-查询语句；1-非查询语句。
  int m_autocommitopt; // 自动提交标志，0-关闭；1-开启。
  void err_report();   // 错误报告。
  void initial();      // 初始化成员变量。
public:
  int m_state;         // 与数据库连接池的绑定状态，0-未绑定，1-已绑定。

  char m_sql[10241];   // SQL语句的文本，最长不能超过10240字节。

  CDA_DEF m_cda;       // 执行SQL语句的结果。

  sqlstatement();      // 构造函数。
  sqlstatement(connection *conn);    // 构造函数，同时绑定数据库连接池。
 ~sqlstatement();      // 析构函数。

  // 绑定数据库连接池。
  // conn：数据库连接池connection对象的地址。
  // 返回值：0-成功，其它失败，只要conn参数是有效的，并且数据库的游标资源足够，connect方法不会返回失败。
  // 程序员一般不必关心connect方法的返回值。
  // 注意，每个sqlstatement只需要绑定一次，在绑定新的connection前，必须先调用disconnect方法。
  int connect(connection *conn);

  // 取消与数据库连接池的绑定。
  // 返回值：0-成功，其它失败，程序员一般不必关心返回值。
  int disconnect();

  // 准备SQL语句。
  // 参数说明：这是一个可变参数，用法与printf函数相同。
  // 返回值：0-成功，其它失败，程序员一般不必关心返回值。
  // 注意：如果SQL语句没有改变，只需要prepare一次就可以了。
  int prepare(const char *fmt,...);

  // 绑定输入变量的地址。
  // position：字段的顺序，从1开始，必须与prepare方法中的SQL的序号一一对应。
  // value：输入变量的地址，如果是字符串，内存大小应该是表对应的字段长度加1。
  // len：如果输入变量的数据类型是字符串，用len指定它的最大长度，建议采用表对应的字段长度。
  // 返回值：0-成功，其它失败，程序员一般不必关心返回值。
  // 注意：1）如果SQL语句没有改变，只需要bindin一次就可以了，2）绑定输入变量的总数不能超过256个。
  int bindin(unsigned int position,int    *value);
  int bindin(unsigned int position,long   *value);
  int bindin(unsigned int position,unsigned int  *value);
  int bindin(unsigned int position,unsigned long *value);
  int bindin(unsigned int position,float *value);
  int bindin(unsigned int position,double *value);
  int bindin(unsigned int position,char   *value,unsigned int len);

  // 绑定输出变量的地址。
  // position：字段的顺序，从1开始，与SQL的结果集一一对应。
  // value：输出变量的地址，如果是字符串，内存大小应该是表对应的字段长度加1。
  // len：如果输出变量的数据类型是字符串，用len指定它的最大长度，建议采用表对应的字段长度。
  // 返回值：0-成功，其它失败，程序员一般不必关心返回值。
  // 注意：1）如果SQL语句没有改变，只需要bindout一次就可以了，2）绑定输出变量的总数不能超过256个。
  int bindout(unsigned int position,int    *value);
  int bindout(unsigned int position,long   *value);
  int bindout(unsigned int position,unsigned int  *value);
  int bindout(unsigned int position,unsigned long *value);
  int bindout(unsigned int position,float *value);
  int bindout(unsigned int position,double *value);
  int bindout(unsigned int position,char   *value,unsigned int len);

  // 执行SQL语句。
  // 返回值：0-成功，其它失败，失败的代码在m_cda.rc中，失败的描述在m_cda.message中。
  // 如果成功的执行了非查询语句，在m_cda.rpc中保存了本次执行SQL影响记录的行数。
  // 程序员必须检查execute方法的返回值。
  int execute();

  // 执行SQL语句。
  // 如果SQL语句不需要绑定输入和输出变量（无绑定变量、非查询语句），可以直接用此方法执行。
  // 参数说明：这是一个可变参数，用法与printf函数相同。
  // 返回值：0-成功，其它失败，失败的代码在m_cda.rc中，失败的描述在m_cda.message中，
  // 如果成功的执行了非查询语句，在m_cda.rpc中保存了本次执行SQL影响记录的行数。
  // 程序员必须检查execute方法的返回值。
  int execute(const char *fmt,...);

  // 从结果集中获取一条记录。
  // 如果执行的SQL语句是查询语句，调用execute方法后，会产生一个结果集（存放在数据库的缓冲区中）。
  // next方法从结果集中获取一条记录，把字段的值放入已绑定的输出变量中。
  // 返回值：0-成功，1403-结果集已无记录，其它-失败，失败的代码在m_cda.rc中，失败的描述在m_cda.message中。
  // 返回失败的原因主要有两个：1）与数据库的连接已断开；2）绑定输出变量的内存太小。
  // 每执行一次next方法，m_cda.rpc的值加1。
  // 程序员必须检查next方法的返回值。
  int next();
};

/*
delimiter $$
drop function if exists to_null;

create function to_null(in_value varchar(10)) returns decimal
begin
if (length(in_value)=0) then
  return null;
else
  return in_value;
end if;
end;
$$
*/

#endif

