/*
 *  程序名：demo21.cpp，此程序演示freecplus框架的统计字符串字数的函数。
 *  作者：C语言技术网(www.freecplus.net) 日期：20190525
*/
#include "../_freecplus.h"

int main()
{
  char buffer[301];  
  STRCPY(buffer,sizeof(buffer),"messi,10,true,30,68.5,2100000,Barcelona。");
  printf("words=%d\n",Words(buffer)); // 输出结果:words=40

  STRCPY(buffer,sizeof(buffer),"梅西,10,true,30,68.5,2100000,Barcelona。");
  printf("words=%d\n",Words(buffer)); // 输出结果:words=37
}

