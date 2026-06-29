# bonus3

Nothing of note with `ll`.

`Checksec` tells us **NX enabled**, this means No-Execute.
When enabled, the stack becomes split in two parts, one part is used to store executable code and the other is for readable program data.

Functions found using `(gdb) info functions`
|LIBC|user|
|-|-|
|strcmp|main|
|fclose|frame_dummy|
|fread||
|puts||
|fopen||
|execl||
|atoi||

Can you guess what `frame_dummy()` does? Right, nothing...

The `main()` function does a bunch of things with the *end user*'s .pass file and `argv[1]`, as well as the `atoi()` function. But this is all just a **red herring**.

We need to focus on the `execl()` function that opens a shell for us.
There are two requirements needed to reach it. Having one argument, and have that argument be equal to `buffer[132]`.

The `""` argument gives a length of **0 bytes** to `atoi()` and thus sets the `buffer[132]` string to an empty string. These two empty strings are identical and therefore pass the two checks.

```sh
./bonus3 ""
$ whoami
end
$ cat /home/user/end/.pass
3321b6f81659f9a71c76616f606e4b50189cecfea611393d5d649f75e157353c
```

```sh
su end
3321b6f81659f9a71c76616f606e4b50189cecfea611393d5d649f75e157353c
```

```sh
end@RainFall:~$ cat end 
Congratulations graduate!
```
