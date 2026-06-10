#define stdout 1

/* Decompiled by Ghidra */

void main(void)
{
  char local_50 [76];
  
  gets(local_50);
  return;
}

void frame_dummy(void)
{
  return;
}

void run(void)
{
  fwrite("Good... Wait what?\n",1,0x13,stdout);
  system("/bin/sh");
  return;
}
