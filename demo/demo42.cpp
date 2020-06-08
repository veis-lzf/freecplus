/*
 *  程序名：demo42.cpp，此程序演示采用freecplus框架的CLogFile类记录程序的运行日志。
 *  本程序修改demo40.cpp把输出的printf语句改为写日志文件。
 *  作者：C语言技术网(www.freecplus.net) 日期：20190525
*/
#include "../_freecplus.h"

int main()
{
  CLogFile logfile;

  // 打开日志文件，如果"/tmp/log"不存在，就创建它，但是要确保当前用户具备创建目录的权限。
  if (logfile.Open("/tmp/log/demo42.log")==false)
  { printf("logfile.Open(/tmp/log/demo42.log) failed.\n"); return -1; }

  logfile.Write("demo42程序开始运行。\n");

  CDir Dir;

  // 扫描/tmp/data目录下文件名匹配"surfdata_*.xml"的文件。
  if (Dir.OpenDir("/tmp/data","surfdata_*.xml")==false)
  { logfile.Write("Dir.OpenDir(/tmp/data) failed.\n"); return -1; }

  CFile File;

  while (Dir.ReadDir()==true)
  {
    logfile.Write("处理文件%s...",Dir.m_FullFileName);

    if (File.Open(Dir.m_FullFileName,"r")==false)
    { logfile.WriteEx("failed.File.Open(%s) failed.\n",Dir.m_FullFileName); return -1; }

    // 这里可以插入读取数据文件的内容、解析xml字符串并把数据写入数据库的代码。
    // 读取文本数据用Fgets和FFGETS方法，读取二进制数据用Fread方法。
    // 具体的代码我就不写了。

    // 处理完文件中的数据后，关闭文件指针，并删除文件。
    File.CloseAndRemove();

    logfile.WriteEx("ok\n");
  }

  logfile.Write("demo42程序运行结束。\n");
}

