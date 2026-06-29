#include <stdint.h>
#include <string.h>

typedef int undefined4;
typedef uint8_t byte;
typedef unsigned int uint;

/* Decompiled using Ghidra */

void greetuser_builtin(void)

{
	char local_4c [4];
	undefined4 local_48;
	char local_44 [64];
	
	if (language == 1) {
	local_4c[0] = 'H';
	local_4c[1] = 'y';
	local_4c[2] = 'v';
	local_4c[3] = -0x3d;
	local_48._0_1_ = -0x5c;
	local_48._1_1_ = -0x3d;
	local_48._2_1_ = -0x5c;
	local_48._3_1_ = ' ';
	builtin_strncpy(local_44,"päivää ",0xb);
	}
	else if (language == 2) {
	builtin_strncpy(local_4c,"Goed",4);
	local_48._0_1_ = 'e';
	local_48._1_1_ = 'm';
	local_48._2_1_ = 'i';
	local_48._3_1_ = 'd';
	builtin_strncpy(local_44,"dag!",4);
	local_44[4] = ' ';
	local_44[5] = '\0';
	}
	else if (language == 0) {
	builtin_strncpy(local_4c,"Hell",4);
	local_48._0_1_ = 'o';
	local_48._1_1_ = ' ';
	local_48._2_1_ = '\0';
	}
	strcat(local_4c,&stack0x00000004);
	puts(local_4c);
	return;
}

undefined4 main(int param_1,int param_2)

{
	undefined4 uVar1;
	int iVar2;
	char *pcVar3;
	undefined4 *puVar4;
	byte bVar5;
	char local_60 [40];
	char acStack_38 [36];
	char *local_14;
	
	bVar5 = 0;
	if (param_1 == 3) {
	pcVar3 = local_60;
	for (iVar2 = 0x13; iVar2 != 0; iVar2 = iVar2 + -1) {
		pcVar3[0] = '\0';
		pcVar3[1] = '\0';
		pcVar3[2] = '\0';
		pcVar3[3] = '\0';
		pcVar3 = pcVar3 + 4;
	}
	strncpy(local_60,*(char **)(param_2 + 4),0x28);
	strncpy(acStack_38,*(char **)(param_2 + 8),0x20);
	local_14 = getenv("LANG");
	if (local_14 != (char *)0x0) {
		iVar2 = memcmp(local_14,&DAT_0804873d,2);
		if (iVar2 == 0) {
		language = 1;
		}
		else {
		iVar2 = memcmp(local_14,&DAT_08048740,2);
		if (iVar2 == 0) {
			language = 2;
		}
		}
	}
	pcVar3 = local_60;
	puVar4 = (undefined4 *)&stack0xffffff50;
	for (iVar2 = 0x13; iVar2 != 0; iVar2 = iVar2 + -1) {
		*puVar4 = *(undefined4 *)pcVar3;
		pcVar3 = pcVar3 + ((uint)bVar5 * -2 + 1) * 4;
		puVar4 = puVar4 + (uint)bVar5 * -2 + 1;
	}
	uVar1 = greetuser();
	}
	else {
	uVar1 = 1;
	}
	return uVar1;
}

/* === readable version === */

int language = 0;

void greetuser(char *input)
{
	char	greeting [64];

	if (language == 1) {
		strncpy(greeting,"Hyvää päivää ", 19);			// Means 'Day' in Finnish as a short form of 'Good Day'
	} else if (language == 2) {
		strncpy(greeting,"Goedemiddag! ", 14);			// 'Goedemiddag!' Means good afternoon in Dutch
	} else if (language == 0) {
		strncpy(greeting,"Hello ", 7);					// 'Hello' ...
	}

	strcat(greeting, input);
	puts(greeting);

	return;
}

int main(int argc,int **argv)
{
	char buffer [76];
	char *language_env_var;

	if (argc == 3) {

		bzero(buffer, 76);

		strncpy(buffer, argv[1], 40);					// buffer will not be null terminated if argv[1] >= 40
		strncpy(buffer, argv[2], 32);
		
		language_env_var = getenv("LANG");							// English, the default
		if (language_env_var != NULL) {
			if (memcmp(language_env_var, "fi", 2) == 0) {		// Finnish, LANG=fi
				language = 1;
			} else {
				if (memcmp(language_env_var, "nl", 2) == 0) {	// Dutch, LANG=nl
					language = 2;
				}
			}
		}

		greetuser(buffer);
	} else {
		return 1;
	}
	return 0;
}
