/*
 *  程序名：demo52.cpp，此程序演示采用freecplus框架的Cftp类下载文件。
 *  作者：C语言技术网(www.freecplus.net) 日期：20190525
*/
#include "../_ftp.h"

int main(int argc,char *argv[])
{
  Cftp ftp;

  // 登录远程FTP服务器，请改为你自己服务器的ip地址。
  if (ftp.login("172.16.0.15:21","freecplus","freecpluspwd",FTPLIB_PASSIVE) == false)
  {
    printf("ftp.login(172.16.0.15:21(freecplus/freecpluspwd)) failed.\n"); return -1;
  }

  // 把服务器上的/home/freecplus/tmp/demo51.cpp下载到本地，存为/tmp/test/demo51.cpp。
  // 如果本地的/tmp/test目录不存在，就创建它。
  if (ftp.get("/home/freecplus/tmp/demo51.cpp","/tmp/test/demo51.cpp")==false)
  { printf("ftp.get() failed.\n"); return -1; }

  printf("get /home/freecplus/tmp/demo51.cpp ok.\n");  

  // 删除服务上的/home/freecplus/tmp/demo51.cpp文件。
  if (ftp.ftpdelete("/home/freecplus/tmp/demo51.cpp")==false) 
  { printf("ftp.ftpdelete() failed.\n"); return -1; }

  printf("delete /home/freecplus/tmp/demo51.cpp ok.\n");  

  // 删除服务器上的/home/freecplus/tmp目录，如果目录非空，删除将失败。
  if (ftp.rmdir("/home/freecplus/tmp")==false) { printf("ftp.rmdir() failed.\n"); return -1; }
}

