/*
 *  程序名：demo32.cpp，此程序演示freecplus框架中采用CDir类获取某目录及其子目录中的文件列表信息。
 *  作者：C语言技术网(www.freecplus.net) 日期：20190525
*/
#include "../_freecplus.h"

int main()
{
  CDir Dir;

  if (Dir.OpenDir("/tmp/root","*.h,*cpp",100,true,true)==false)
  { 
    printf("Dir.OpenDir(/tmp/root) failed.\n"); return -1; 
  }

  while(Dir.ReadDir()==true)
  {
    printf("filename=%s,mtime=%s,size=%d\n",Dir.m_FullFileName,Dir.m_ModifyTime,Dir.m_FileSize);
  }
}

