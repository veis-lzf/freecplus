/*
 *  程序名：demo1.cpp，此程序演示freecplus框架中STRCPY函数的使用。
 *  作者：C语言技术网(www.freecplus.net) 日期：20190525
*/
#include "../_freecplus.h"

int main()
{
  char str[11];   // 字符串str的大小是11字节。

  STRCPY(str,sizeof(str),"freecplus");  // 待复制的内容没有超过str可以存放字符串的大小。
  printf("str=%s=\n",str);    // 出输结果是str=freecplus=
  STRCPY(str,sizeof(str),"www.freecplus.net");  // 待复制的内容超过了str可以存放字符串的大小。
  printf("str=%s=\n",str);    // 出输结果是str=www.freecp=
}

