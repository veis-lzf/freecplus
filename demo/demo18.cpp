/*
 *  程序名：demo18.cpp，此程序演示freecplus框架正则表达示MatchStr函数的使用。
 *  作者：C语言技术网(www.freecplus.net) 日期：20190525
*/
#include "../_freecplus.h"

int main()
{
  char filename[301];  
  STRCPY(filename,sizeof(filename),"_freecplus.h");
  
  // 以下代码将输出yes。
  if (MatchStr(filename,"*.h,*.cpp")==true) printf("yes\n");
  else printf("no\n");

  // 以下代码将输出yes。
  if (MatchStr(filename,"*.H")==true) printf("yes\n");
  else printf("no\n");

  // 以下代码将输出no。
  if (MatchStr(filename,"*.cpp")==true) printf("yes\n");
  else printf("no\n");
}

