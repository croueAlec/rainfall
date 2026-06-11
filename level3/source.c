#include <stdio.h>

typedef void code;
typedef void undefined;

/* Decompiled using Ghidra */

void main(void)
{
  v();
  return;
}

void frame_dummy(void)
{
  return;
}


void FUN_08048380(void)
{
  (*(code *)(undefined *)0x0)();
  return;
}

void v(void)
{
  char local_20c [520];
  
  fgets(local_20c,0x200,stdin);				// 0x200 == 0d512
  printf(local_20c);
  if (m == 0x40) {							// 0x40 == 0d64
    fwrite("Wait what?!\n",1,0xc,stdout);	// 0xc == 0d12
    system("/bin/sh");
  }
  return;
}
