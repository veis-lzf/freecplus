/*
 *  程序名：demo16.cpp，此程序演示freecplus框架中PickNumber函数的使用。
 *  作者：C语言技术网(www.freecplus.net) 日期：20190525
*/
#include "../_freecplus.h"

int main()
{
  char str[26];   // 字符串str的大小是11字节。

  STRCPY(str,sizeof(str),"iab+12.3xy");
  PickNumber(str,str,false,false);
  printf("str=%s=\n",str);    // 出输结果是str=123=

  STRCPY(str,sizeof(str),"iab+12.3xy");
  PickNumber(str,str,true,false);
  printf("str=%s=\n",str);    // 出输结果是str=+123=

  STRCPY(str,sizeof(str),"iab+12.3xy");
  PickNumber(str,str,true,true);
  printf("str=%s=\n",str);    // 出输结果是str=-12.3=
}

