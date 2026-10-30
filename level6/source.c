#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Decompiled using Ghidra */

void main(int argc, char **argv)
{
	void *func;
	char *__dest;

	__dest = malloc(0x40); // 64
	func = malloc(4);
	*func = m;
	strcpy(__dest, argv[1]);
	func();
	return;
}

void m(void)
{
	puts("Nope");
	return;
}

void n(void)
{
	system("/bin/cat /home/user/level7/.pass");
	return;
}

void frame_dummy(void)
{
	return;
}
