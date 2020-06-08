/**************************************************************************************/
/*   程序名：_mysql.cpp，此程序是freecplus框架的C/C++操作mysql数据库的定义文件。      */
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

#include "_mysql.h"

connection::connection()
{ 
  m_conn = NULL;

  m_state = 0; 

  memset(&m_env,0,sizeof(LOGINENV));

  memset(&m_cda,0,sizeof(m_cda));

  m_cda.rc=-1;
  strncpy(m_cda.message,"database not open.",128);

  // 数据库种类
  memset(m_dbtype,0,sizeof(m_dbtype));
  strcpy(m_dbtype,"mysql");
}

connection::~connection()
{
  disconnect();
}

// 从connstr中解析username,password,tnsname
// "120.77.115.3","szidc","SZmb1601","lxqx",3306
void connection::setdbopt(char *connstr)
{
  memset(&m_env,0,sizeof(LOGINENV));

  char *bpos,*epos;

  bpos=epos=0;

  // ip
  bpos=connstr;
  epos=strstr(bpos,",");
  if (epos > 0) 
  {
    strncpy(m_env.ip,bpos,epos-bpos); 
  }else return;

  // user
  bpos=epos+1;
  epos=0;
  epos=strstr(bpos,",");
  if (epos > 0) 
  {
    strncpy(m_env.user,bpos,epos-bpos); 
  }else return;

  // pass
  bpos=epos+1;
  epos=0;
  epos=strstr(bpos,",");
  if (epos > 0) 
  {
    strncpy(m_env.pass,bpos,epos-bpos); 
  }else return;

  // dbname
  bpos=epos+1;
  epos=0;
  epos=strstr(bpos,",");
  if (epos > 0) 
  {
    strncpy(m_env.dbname,bpos,epos-bpos); 
  }else return;

  // port
  m_env.port=atoi(epos+1);
}

int connection::connecttodb(char *connstr,char *charset,unsigned int autocommitopt)
{
  // 如果已连接上数据库，就不再连接
  // 所以，如果想重连数据库，必须显示的调用disconnect()方法后才能重连
  if (m_state == 1) return 0;

  // 从connstr中解析username,password,tnsname
  setdbopt(connstr);

  memset(&m_cda,0,sizeof(m_cda));

  if ( (m_conn = mysql_init(NULL)) == NULL )
  {
    m_cda.rc=-1; strncpy(m_cda.message,"initialize mysql failed.\n",128); return -1;
  }

  if ( mysql_real_connect(m_conn,m_env.ip,m_env.user,m_env.pass,m_env.dbname,m_env.port, NULL, 0 ) == NULL )
  {
    m_cda.rc=mysql_errno(m_conn); strncpy(m_cda.message,mysql_error(m_conn),2000); mysql_close(m_conn); m_conn=NULL;  return -1;
  }

  // 设置事务模式，0-关闭自动提交，1-开启自动提交
  m_autocommitopt=autocommitopt;

  if ( mysql_autocommit(m_conn, m_autocommitopt ) != 0 )
  {
    m_cda.rc=mysql_errno(m_conn); strncpy(m_cda.message,mysql_error(m_conn),2000); mysql_close(m_conn); m_conn=NULL;  return -1;
  }

  // 设置字符集
  character(charset);

  m_state = 1;

  return 0;
}

// 设置字符集，要与数据库的一致，否则中文会出现乱码
void connection::character(char *charset)
{
  mysql_set_character_set(m_conn,charset);

  return;
}

int connection::disconnect()
{
  memset(&m_cda,0,sizeof(m_cda));

  if (m_state == 0) 
  { 
    m_cda.rc=-1; strncpy(m_cda.message,"database not open.",128); return -1;
  }

  rollback();

  mysql_close(m_conn); 

  m_conn=NULL;

  m_state = 0;    

  return 0;
}

int connection::rollback()
{ 
  memset(&m_cda,0,sizeof(m_cda));

  if (m_state == 0) 
  { 
    m_cda.rc=-1; strncpy(m_cda.message,"database not open.",128); return -1;
  }

  if ( mysql_rollback(m_conn ) != 0 )
  {
    m_cda.rc=mysql_errno(m_conn); strncpy(m_cda.message,mysql_error(m_conn),2000); mysql_close(m_conn); m_conn=NULL;  return -1;
  }

  return 0;    
}

int connection::commit()
{ 
  memset(&m_cda,0,sizeof(m_cda));

  if (m_state == 0) 
  { 
    m_cda.rc=-1; strncpy(m_cda.message,"database not open.",128); return -1;
  }

  if ( mysql_commit(m_conn ) != 0 )
  {
    m_cda.rc=mysql_errno(m_conn); strncpy(m_cda.message,mysql_error(m_conn),2000); mysql_close(m_conn); m_conn=NULL;  return -1;
  }

  return 0;
}

void connection::err_report()
{
  if (m_state == 0) 
  { 
    m_cda.rc=-1; strncpy(m_cda.message,"database not open.",128); return;
  }

  memset(&m_cda,0,sizeof(m_cda));

  m_cda.rc=-1;
  strncpy(m_cda.message,"call err_report failed.",128);

  m_cda.rc=mysql_errno(m_conn);

  strncpy(m_cda.message,mysql_error(m_conn),2000);

  return;
}

sqlstatement::sqlstatement()
{
  initial();
}

void sqlstatement::initial()
{
  m_state=0;

  m_handle=NULL;

  memset(&m_cda,0,sizeof(m_cda));

  memset(m_sql,0,sizeof(m_sql));

  m_cda.rc=-1;
  strncpy(m_cda.message,"sqlstatement not connect to connection.\n",128);
}

sqlstatement::sqlstatement(connection *conn)
{
  initial();

  connect(conn);
}

sqlstatement::~sqlstatement()
{
  disconnect();
}

int sqlstatement::connect(connection *conn)
{
  // 注意，一个sqlstatement在程序中只能连一个connection，不允许连多个connection
  // 所以，只要这个光标已打开，就不允许再次打开，直接返回成功
  if ( m_state == 1 ) return 0;
  
  memset(&m_cda,0,sizeof(m_cda));

  m_conn=conn;

  // 如果数据库连接类的指针为空，直接返回失败
  if (m_conn == 0)
  {
    m_cda.rc=-1; strncpy(m_cda.message,"database not open.\n",128); return -1;
  }
  
  // 如果数据库没有连接好，直接返回失败
  if (m_conn->m_state == 0)
  {
    m_cda.rc=-1; strncpy(m_cda.message,"database not open.\n",128); return -1;
  }

  if ( (m_handle=mysql_stmt_init(m_conn->m_conn)) == NULL)
  {
    err_report(); return m_cda.rc;
  }

  m_state = 1;  // 光标成功打开

  m_autocommitopt=m_conn->m_autocommitopt;

  return 0;
}

int sqlstatement::disconnect()
{
  if (m_state == 0) return 0;

  memset(&m_cda,0,sizeof(m_cda));

  mysql_stmt_close(m_handle);

  m_state=0;

  m_handle=NULL;

  memset(&m_cda,0,sizeof(m_cda));

  memset(m_sql,0,sizeof(m_sql));

  m_cda.rc=-1;
  strncpy(m_cda.message,"cursor not open.",128);

  return 0;
}

int connection::execute(const char *fmt,...)
{
  memset(m_sql,0,sizeof(m_sql));

  va_list ap;
  va_start(ap,fmt);
  vsnprintf(m_sql,10240,fmt,ap);
  va_end(ap);

  sqlstatement stmt(this);

  return stmt.execute(m_sql);
}

void sqlstatement::err_report()
{
  // 注意，在该函数中，不可随意用memset(&m_cda,0,sizeof(m_cda))，否则会清空m_cda.rpc的内容
  if (m_state == 0)
  {
    m_cda.rc=-1; strncpy(m_cda.message,"cursor not open.\n",128); return;
  }
  
  memset(&m_conn->m_cda,0,sizeof(m_conn->m_cda));

  m_cda.rc=-1;
  strncpy(m_cda.message,"call err_report() failed.\n",128);

  m_cda.rc=mysql_stmt_errno(m_handle);

  snprintf(m_cda.message,2000,"%d,%s",m_cda.rc,mysql_stmt_error(m_handle));

  m_conn->err_report();

  return;
}

// 把字符串中的小写字母转换成大写，忽略不是字母的字符。
// 这个函数只在prepare方法中用到。
void MY__ToUpper(char *str)
{
  if (str == 0) return;

  if (strlen(str) == 0) return;

  int istrlen=strlen(str);

  for (int ii=0;ii<istrlen;ii++)
  {
    if ( (str[ii] >= 'a') && (str[ii] <= 'z') ) str[ii]=str[ii] - 32;
  }
}

// 删除字符串左边的空格。
// 这个函数只在prepare方法中用到。
void MY__DeleteLChar(char *str,const char chr)
{
  if (str == 0) return;
  if (strlen(str) == 0) return;

  char strTemp[strlen(str)+1];

  int iTemp=0;

  memset(strTemp,0,sizeof(strTemp));
  strcpy(strTemp,str);

  while ( strTemp[iTemp] == chr )  iTemp++;

  memset(str,0,strlen(str)+1);

  strcpy(str,strTemp+iTemp);

  return;
}

// 字符串替换函数
// 在字符串str中，如果存在字符串str1，就替换为字符串str2。
// 这个函数只在prepare方法中用到。
void MY__UpdateStr(char *str,const char *str1,const char *str2,bool bloop)
{
  if (str == 0) return;
  if (strlen(str) == 0) return;
  if ( (str1 == 0) || (str2 == 0) ) return;

  // 如果bloop为true并且str2中包函了str1的内容，直接返回，因为会进入死循环，最终导致内存溢出。
  if ( (bloop==true) && (strstr(str2,str1)>0) ) return;

  // 尽可能分配更多的空间，但仍有可能出现内存溢出的情况，最好优化成string。
  int ilen=strlen(str)*10;
  if (ilen<1000) ilen=1000;

  char strTemp[ilen];

  char *strStart=str;

  char *strPos=0;

  while (true)
  {
    if (bloop == true)
    {
      strPos=strstr(str,str1);
    }
    else
    {
      strPos=strstr(strStart,str1);
    }

    if (strPos == 0) break;

    memset(strTemp,0,sizeof(strTemp));
    strncpy(strTemp,str,strPos-str);
    strcat(strTemp,str2);
    strcat(strTemp,strPos+strlen(str1));
    strcpy(str,strTemp);

    strStart=strPos+strlen(str2);
  }
}

  
int sqlstatement::prepare(const char *fmt,...)
{
  memset(&m_cda,0,sizeof(m_cda));

  if (m_state == 0)
  {
    m_cda.rc=-1; strncpy(m_cda.message,"cursor not open.\n",128); return -1;
  }

  memset(m_sql,0,sizeof(m_sql));

  va_list ap;
  va_start(ap,fmt);
  vsnprintf(m_sql,10240,fmt,ap);
  va_end(ap);

  int ilen=strlen(m_sql);

  // 为了和oracle兼容，把:1,:2,:3...等替换成?
  for (int ii=0;ii<ilen;ii++)
  {
    if ( (m_sql[ii]==':') && (isdigit(m_sql[ii+1])!=0) )
    {
      m_sql[ii]='?';
      m_sql[ii+1]=' ';
      if (isdigit(m_sql[ii+2])!=0) m_sql[ii+2]=' ';
      if (isdigit(m_sql[ii+3])!=0) m_sql[ii+3]=' ';
    }
  }

  // 为了和oracle兼容
  // 把to_date规制成str_to_date。
  // 把to_char替换成date_format。
  if (strstr(m_sql,"str_to_date")==0) MY__UpdateStr(m_sql,"to_date","str_to_date",false);
  MY__UpdateStr(m_sql,"to_char","date_format",false);
  MY__UpdateStr(m_sql,"yyyy-mm-dd hh24:mi:ss","%Y-%m-%d %h:%i:%s",false);
  MY__UpdateStr(m_sql,"yyyymmddhh24miss","%Y%m%d%h%i%s",false);
  // 各位如果想兼容oracle和mysql更多的时间格式，可以在这里加代码。
  // 一定要把格式一一列出来，不能用"yyyy"替换"%Y"，因为在SQL语句的其它地方也可能存在"yyyy"。

  if (mysql_stmt_prepare(m_handle,m_sql,strlen(m_sql)) != 0)
  {
    err_report(); return m_cda.rc;
  }

  // 判断是否是查询语句，如果是，把m_sqltype设为0，其它语句设为1。
  m_sqltype=1;
  
  // 从待执行的SQL语句中截取30个字符，如果是以"select"打头，就认为是查询语句。
  char strtemp[31]; memset(strtemp,0,sizeof(strtemp)); strncpy(strtemp,m_sql,30);
  MY__ToUpper(strtemp); MY__DeleteLChar(strtemp,' ');
  if (strncmp(strtemp,"SELECT",6)==0)  m_sqltype=0;

  memset(params_in,0,sizeof(params_in));

  memset(params_out,0,sizeof(params_out));

  maxbindin=0;

  return 0;
}
  
int sqlstatement::bindin(unsigned int position,int *value)
{
  if ( (position<1) || (position>=MAXPARAMS) || (position>m_handle->param_count) )
  {
    m_cda.rc=-1; strncpy(m_cda.message,"array bound.",128);
  }

  params_in[position-1].buffer_type = MYSQL_TYPE_LONG;
  params_in[position-1].buffer = value;
  params_in_buffer_type[position-1]=MYSQL_TYPE_LONG;

  if (position>maxbindin) maxbindin=position;

  return 0;
}

int sqlstatement::bindin(unsigned int position,long *value)
{
  if ( (position<1) || (position>=MAXPARAMS) || (position>m_handle->param_count) )
  {
    m_cda.rc=-1; strncpy(m_cda.message,"array bound.",128);
  }

  params_in[position-1].buffer_type = MYSQL_TYPE_LONGLONG;
  params_in[position-1].buffer = value;
  params_in_buffer_type[position-1]=MYSQL_TYPE_LONGLONG;

  if (position>maxbindin) maxbindin=position;

  return 0;
}

int sqlstatement::bindin(unsigned int position,unsigned int *value)
{
  if ( (position<1) || (position>=MAXPARAMS) || (position>m_handle->param_count) )
  {
    m_cda.rc=-1; strncpy(m_cda.message,"array bound.",128);
  }

  params_in[position-1].buffer_type = MYSQL_TYPE_LONG;
  params_in[position-1].buffer = value;
  params_in_buffer_type[position-1]=MYSQL_TYPE_LONG;

  if (position>maxbindin) maxbindin=position;

  return 0;
}

int sqlstatement::bindin(unsigned int position,unsigned long *value)
{
  if ( (position<1) || (position>=MAXPARAMS) || (position>m_handle->param_count) )
  {
    m_cda.rc=-1; strncpy(m_cda.message,"array bound.",128);
  }

  params_in[position-1].buffer_type = MYSQL_TYPE_LONGLONG;
  params_in[position-1].buffer = value;
  params_in_buffer_type[position-1]=MYSQL_TYPE_LONGLONG;

  if (position>maxbindin) maxbindin=position;

  return 0;
}

int sqlstatement::bindin(unsigned int position,char *value,unsigned int len)
{
  if ( (position<1) || (position>=MAXPARAMS) || (position>m_handle->param_count) )
  {
    m_cda.rc=-1; strncpy(m_cda.message,"array bound.",128);
  }

  params_in[position-1].buffer_type = MYSQL_TYPE_VAR_STRING;
  params_in[position-1].buffer = value;
  params_in[position-1].buffer_length = len;
  params_in[position-1].length=&params_in_length[position-1];
  params_in[position-1].is_null=&params_in_is_null[position-1];
  params_in_buffer_type[position-1]=MYSQL_TYPE_VAR_STRING;

  if (position>maxbindin) maxbindin=position;

  return 0;
}

int sqlstatement::bindin(unsigned int position,float *value)
{
  if ( (position<1) || (position>=MAXPARAMS) || (position>m_handle->param_count) )
  {
    m_cda.rc=-1; strncpy(m_cda.message,"array bound.",128);
  }

  params_in[position-1].buffer_type = MYSQL_TYPE_FLOAT;
  params_in[position-1].buffer = value;
  params_in_buffer_type[position-1]=MYSQL_TYPE_FLOAT;

  if (position>maxbindin) maxbindin=position;

  return 0;
}

int sqlstatement::bindin(unsigned int position,double *value)
{
  if ( (position<1) || (position>=MAXPARAMS) || (position>m_handle->param_count) )
  {
    m_cda.rc=-1; strncpy(m_cda.message,"array bound.",128);
  }

  params_in[position-1].buffer_type = MYSQL_TYPE_DOUBLE;
  params_in[position-1].buffer = value;
  params_in_buffer_type[position-1]=MYSQL_TYPE_DOUBLE;

  if (position>maxbindin) maxbindin=position;

  return 0;
}

///////////////////
int sqlstatement::bindout(unsigned int position,int *value)
{
  if ( (position<1) || (position>=MAXPARAMS) || (position>m_handle->field_count) )
  {
    m_cda.rc=-1; strncpy(m_cda.message,"array bound.",128);
  }

  params_out[position-1].buffer_type = MYSQL_TYPE_LONG;
  params_out[position-1].buffer = value;

  return 0;
}

int sqlstatement::bindout(unsigned int position,long *value)
{
  if ( (position<1) || (position>=MAXPARAMS) || (position>m_handle->field_count) )
  {
    m_cda.rc=-1; strncpy(m_cda.message,"array bound.",128);
  }

  params_out[position-1].buffer_type = MYSQL_TYPE_LONGLONG;
  params_out[position-1].buffer = value;

  return 0;
}

int sqlstatement::bindout(unsigned int position,unsigned int *value)
{
  if ( (position<1) || (position>=MAXPARAMS) || (position>m_handle->field_count) )
  {
    m_cda.rc=-1; strncpy(m_cda.message,"array bound.",128);
  }

  params_out[position-1].buffer_type = MYSQL_TYPE_LONG;
  params_out[position-1].buffer = value;

  return 0;
}

int sqlstatement::bindout(unsigned int position,unsigned long *value)
{
  if ( (position<1) || (position>=MAXPARAMS) || (position>m_handle->field_count) )
  {
    m_cda.rc=-1; strncpy(m_cda.message,"array bound.",128);
  }

  params_out[position-1].buffer_type = MYSQL_TYPE_LONGLONG;
  params_out[position-1].buffer = value;

  return 0;
}

int sqlstatement::bindout(unsigned int position,char *value,unsigned int len)
{
  if ( (position<1) || (position>=MAXPARAMS) || (position>m_handle->field_count) )
  {
    m_cda.rc=-1; strncpy(m_cda.message,"array bound.",128);
  }

  params_out[position-1].buffer_type = MYSQL_TYPE_VAR_STRING;
  params_out[position-1].buffer = value;
  params_out[position-1].buffer_length = len;

  return 0;
}

int sqlstatement::bindout(unsigned int position,float *value)
{
  if ( (position<1) || (position>=MAXPARAMS) || (position>m_handle->field_count) )
  {
    m_cda.rc=-1; strncpy(m_cda.message,"array bound.",128);
  }

  params_out[position-1].buffer_type = MYSQL_TYPE_FLOAT;
  params_out[position-1].buffer = value;

  return 0;
}

int sqlstatement::bindout(unsigned int position,double *value)
{
  if ( (position<1) || (position>=MAXPARAMS) || (position>m_handle->field_count) )
  {
    m_cda.rc=-1; strncpy(m_cda.message,"array bound.",128);
  }

  params_out[position-1].buffer_type = MYSQL_TYPE_DOUBLE;
  params_out[position-1].buffer = value;

  return 0;
}

int sqlstatement::execute()
{
  memset(&m_cda,0,sizeof(m_cda));

  if (m_state == 0)
  {
    m_cda.rc=-1; strncpy(m_cda.message,"cursor not open.\n",128); return -1;
  }

  if ( (m_handle->param_count>0) && (m_handle->bind_param_done == 0))
  {
    if (mysql_stmt_bind_param(m_handle,params_in) != 0)
    {
      err_report(); return m_cda.rc;
    }
  }

  if ( (m_handle->field_count>0) && (m_handle->bind_result_done == 0) )
  {
    if (mysql_stmt_bind_result(m_handle,params_out) != 0)
    {
      err_report(); return m_cda.rc;
    }
  }
  
  for (int ii=0;ii<maxbindin;ii++)
  {
    if (params_in_buffer_type[ii] == MYSQL_TYPE_VAR_STRING )
    {
      if (strlen((char *)params_in[ii].buffer)==0) 
      {
        //params_in[ii].buffer_type = MYSQL_TYPE_NULL;
        params_in_is_null[ii]=true;
      }
      else 
      {
        //params_in[ii].buffer_type = MYSQL_TYPE_VAR_STRING;
        params_in_is_null[ii]=false;
        params_in_length[ii]=strlen((char *)params_in[ii].buffer);
      }
    }
  }

  if (mysql_stmt_execute(m_handle) != 0)
  {
    err_report(); return m_cda.rc;
  }
  
  // 如果不是查询语句，就获取影响记录的行数
  if (m_sqltype == 1) 
  { 
    m_cda.rpc=m_handle->affected_rows;
    m_conn->m_cda.rpc=m_cda.rpc;
  }
    
  return 0;
}

int sqlstatement::execute(const char *fmt,...)
{
  char strtmpsql[10241];
  memset(strtmpsql,0,sizeof(strtmpsql));

  va_list ap;
  va_start(ap,fmt);
  vsnprintf(strtmpsql,10240,fmt,ap);
  va_end(ap);

  if (prepare(strtmpsql) != 0) return m_cda.rc;

  return execute();
}

int sqlstatement::next()
{
  // 注意，在该函数中，不可随意用memset(&m_cda,0,sizeof(m_cda))，否则会清空m_cda.rpc的内容
  if (m_state == 0)
  {
    m_cda.rc=-1; strncpy(m_cda.message,"cursor not open.\n",128); return -1;
  }
  
  // 如果语句未执行成功，直接返回失败。
  if (m_cda.rc != 0) return m_cda.rc;
  
  // 判断是否是查询语句，如果不是，直接返回错误
  if (m_sqltype != 0)
  {
    m_cda.rc=-1; strncpy(m_cda.message,"no recordset found.\n",128); return -1;
  }
  
  int ret=mysql_stmt_fetch(m_handle);

  if (ret==0) 
  { 
    m_cda.rpc++; return 0; 
  }
 
  if (ret==1) 
  {
    err_report(); return m_cda.rc;
  }

  if (ret==MYSQL_NO_DATA) return MYSQL_NO_DATA;

  if (ret==MYSQL_DATA_TRUNCATED) 
  {
    m_cda.rpc++; return 0;
  }
  
  return 0;
}
