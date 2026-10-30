#include <stdio.h>
#include <time.h>

/* Decompiled using Ghidra */

char buffer[65] = {0};

int main(int argc, char **argv)
{
	char *string;
	void *ptr;
	char *string2;
	FILE *fd;

	string = malloc(8);
	string2 = malloc(8);
	ptr = malloc(8);
	ptr = &string2;

	strcpy(string, argv[1]);
	strcpy(ptr, argv[2]);
	fd = fopen("/home/user/level8/.pass", "r");
	fgets(buffer, 0x44, fd);   // 0x44 == 0x68
	puts("~~");
	return 0;
}

void m(void)
{
	time_t tVar1;

	tVar1 = time((time_t *)0x0);
	printf("%s - %d\n", buffer, tVar1);
	return;
}

/* WARNING: Removing unreachable block (ram,0x080484e8) */

void frame_dummy(void)
{
	return;
}
