/*
 *  程序名：demo34.cpp，此程序演示freecplus框架的文件操作函数的用法
 *  作者：C语言技术网(www.freecplus.net) 日期：20190525
*/
#include "../_freecplus.h"

int main()
{
  // 删除文件。
  if (REMOVE("/tmp/root/_freecplus.h")==false)
  {
    printf("REMOVE(/tmp/root/_freecplus.h) %d:%s\n",errno,strerror(errno));
  }

  // 重命名文件。
  if (RENAME("/tmp/root/freecplus.cpp","/tmp/root/aaa/bbb/ccc/freecplus.cpp")==false)
  {
    printf("RENAME(/tmp/root/freecplus.cpp) %d:%s\n",errno,strerror(errno));
  }
  
  // 复制文件。
  if (COPY("/freecplus/_freecplus.h","/tmp/root/aaa/bbb/ccc/_freecplus.h")==false)
  {
    printf("COPY(/freecplus/_freecplus.h) %d:%s\n",errno,strerror(errno));
  }

  // 获取文件的大小。
  printf("size=%d\n",FileSize("/freecplus/_freecplus.h"));

  // 重置文件的时间。
  UTime("/freecplus/_freecplus.h","2020-01-05 13:37:29");

  // 获取文件的时间。
  char mtime[21]; memset(mtime,0,sizeof(mtime));
  FileMTime("/freecplus/_freecplus.h",mtime,"yyyy-mm-dd hh24:mi:ss");
  printf("mtime=%s\n",mtime);   // 输出mtime=2020-01-05 13:37:29
}

