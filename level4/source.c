#include <stdio.h>

/* Decompiled with Ghidra */

void main(void)
{
  n();
  return;
}

void frame_dummy(void)
{
  return;
}

void n(void)
{
  char local_20c [520];
  
  fgets(local_20c,0x200,stdin);						// 0x200 == 0d512
  p(local_20c);
  if (m == 0x1025544) {								// 0x1025544 == 0d16930116
    system("/bin/cat /home/user/level5/.pass");
  }
  return;
}

void p(char *param_1)
{
  printf(param_1);
  return;
}
