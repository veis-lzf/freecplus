/*
 *  程序名：demo40.cpp，此程序演示freecplus框架中采用CDir类和CFile类处理数据文件的用法。
 *  作者：C语言技术网(www.freecplus.net) 日期：20190525
*/
#include "../_freecplus.h"

int main()
{
  CDir Dir;

  // 扫描/tmp/data目录下文件名匹配"surfdata_*.xml"的文件。
  if (Dir.OpenDir("/tmp/data","surfdata_*.xml")==false)
  {
    printf("Dir.OpenDir(/tmp/data) failed.\n"); return -1;
  }

  CFile File;

  while (Dir.ReadDir()==true)
  {
    printf("处理文件%s...",Dir.m_FullFileName);

    if (File.Open(Dir.m_FullFileName,"r")==false)
    {
      printf("failed.File.Open(%s) failed.\n",Dir.m_FullFileName); return -1;
    }

    // 这里可以插入读取数据文件的内容、解析xml字符串并把数据写入数据库的代码。
    // 读取文本数据用Fgets和FFGETS方法，读取二进制数据用Fread方法。
    // 具体的代码我就不写了。

    // 处理完文件中的数据后，关闭文件指针，并删除文件。
    File.CloseAndRemove();

    printf("ok\n");
  }
}

