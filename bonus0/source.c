#include <stdint.h>

typedef int undefined4;
typedef unsigned int uint;

typedef uint8_t byte

/* Decompiled using Ghidra */

undefined4 main(void)
{
	char local_3a [54];
	
	pp(local_3a);
	puts(local_3a);
	return 0;
}

void p(char *param_1,char *param_2)
{
	char *pcVar1;
	char local_100c [4104];
	
	puts(param_2);
	read(0,local_100c,0x1000);
	pcVar1 = strchr(local_100c,10);
	*pcVar1 = '\0';
	strncpy(param_1,local_100c,0x14);
	return;
}

void pp(char *param_1)
{
	char cVar1;
	uint uVar2;
	char *pcVar3;
	byte bVar4;
	char local_34 [20];
	char local_20 [20];
	
	bVar4 = 0;
	p(local_34,&DAT_080486a0);
	p(local_20,&DAT_080486a0);
	strcpy(param_1,local_34);
	uVar2 = 0xffffffff;
	pcVar3 = param_1;
	do {
	if (uVar2 == 0) break;
	uVar2 = uVar2 - 1;
	cVar1 = *pcVar3;
	pcVar3 = pcVar3 + (uint)bVar4 * -2 + 1;
	} while (cVar1 != '\0');
	(param_1 + (~uVar2 - 1))[0] = ' ';
	(param_1 + (~uVar2 - 1))[1] = '\0';
	strcat(param_1,local_20);
	return;
}

void frame_dummy(void)
{
	return;
}


/* === readable version === */

undefined4 main(void)
{
	char output_string[54];
	
	pp(output_string);
	puts(output_string);
	return 0;
}

void p(char *dest,char *prompt)
{
	char *null_byte_index;
	char main_buffer [4104];

	puts(prompt);
	read(0,main_buffer,4096);
	null_byte_index = strchr(main_buffer, '\n');
	*null_byte_index = '\0';
	strncpy(dest,main_buffer,20);
	return;
}

void pp(char *main_buffer)
{
	uint main_buffer;
	char buffer_a [20];
	char buffer_2 [20];

	p(buffer_a, " -");
	p(buffer_2, " -");
	strcpy(main_buffer,buffer_a);
	main_buffer = strlen(main_buffer);
	main_buffer[main_buffer] = ' ';
	main_buffer[main_buffer + 1] = '\0';
	strcat(main_buffer, buffer_2);
	return;
}

void frame_dummy(void)
{
	return;
}
