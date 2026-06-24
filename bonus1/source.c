#include <string.h>
#include <stdlib.h>
#include <unistd.h>


/* Decompiled using Ghidra */

typedef int undefined4;
typedef char undefined1;

undefined4 main(undefined4 param_1,int param_2)

{
  undefined4 uVar1;
  undefined1 local_3c [40];
  int local_14;
  
  local_14 = atoi(*(char **)(param_2 + 4));
  if (local_14 < 10) {
    memcpy(local_3c,*(void **)(param_2 + 8),local_14 * 4);
    if (local_14 == 0x574f4c46) {
      execl("/bin/sh","sh",0);
    }
    uVar1 = 0;
  }
  else {
    uVar1 = 1;
  }
  return uVar1;
}

/* === readable and fixed version === */
/* For some reason, Ghidra and other decompiler suites messed the variables used by main 
   The variable `local_14` is located below `local_3c` and the variable `uVar1` has been added.
   I realised this after talking to other students that showed me their source.c files.
*/

int main(int argc, char **argv)
{
	int		check;
	char	buffer[40];

	check = atoi(argv[1]);
	if (check < 10) {
		memcpy(buffer, argv[2], check * 4);
		if (check == 0x574f4c46) {
			execl("/bin/sh", "sh", 0);
		}
		return 0;
	} else {
		return 1;
	}
}

void frame_dummy(void)
{
	return;
}
