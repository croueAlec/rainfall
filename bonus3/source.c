#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* Decompiled using Ghidra */

void frame_dummy(void)
{
	return;
}

int main(int argc, char **argv)
{
	FILE	*fd = fopen("/home/user/end/.pass", "r");
	char	buffer[132];

	bzero(buffer, 132);

	if ((fd == (FILE *)0x0) || (argc != 2)) {
		return 255;

	} else {
		fread(buffer, 1, 66, fd);
		buffer[65] = 0;
		buffer[atoi(argv[1])] = 0;
		fread(&buffer[66], 1, 65, fd);
		fclose(fd);

		if (strcmp(buffer, argv[1]) == 0) {
			execl("/bin/sh", "sh", 0);
		} else {
			puts(&buffer[66]);
		}

		return 0;
	}
}
