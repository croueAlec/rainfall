#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>

/* Decompiled by Ghidra */

int main(int argc, char **argv)
{
	int		value;
	char   *string;
	__gid_t group_id;
	__uid_t user_id;

	value = atoi(argv[1]);
	if (value == 0x1a7) {
		string = strdup("/bin/sh");
		group_id = getegid();
		user_id = geteuid();
		setresgid(group_id, group_id, group_id);
		setresuid(user_id, user_id, user_id);
		execv("/bin/sh", &string);
	} else {
		fwrite("No !\n", 1, 5, (FILE *)stderr);
	}
	return 0;
}
