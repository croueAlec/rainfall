# level6

Nothing of note with `ll` and `checksec`.

Functions found using `(gdb) info functions`
|LIBC|user|
|-|-|
|strcpy|main|
|malloc|frame_dummy|
|puts|n|
|system|m|

Examining **Ghidra**'s decompiling, we can see 3 interesting functions.
`main()` uses a malloc to allocate **64 bytes** to the *__dest* `char*`. Then **4 bytes** to the *puVar1* `void*`.

Then the address of `m()` is stored in *puVar1*.

The 4th argument of `*argv[]` is copied using `strcpy()` into *__dest*.

Finally, the `puVar1` function pointer is called, thus calling `m()`.


`strcpy()`'s man tells us that it does not check for the destination buffer size.
```sh
CAVEATS
       [...]
       If  the  destination  buffer  is not large enough, the behavior is unde‐
       fined.  See _FORTIFY_SOURCE in feature_test_macros(7).
```
This means that a 4th argv argument of any size will be copied into *__dest*. 

The `m()` function writes `"Nope"` on the terminal.
The `n()` function calls the `system()` function to print the contents of **level7**'s .pass file.

So from the allocated *__dest* we need to overwrite the address of `m()` stored inside *puVar1* to the address of `n()`.

We need to figure out the offset between *__dest* and *puVar1*'s allocated spaces. The smart way to do it would be to find their addresses and subtract them.

```sh
ltrace ./level6
[...]
malloc(64)                                       = 0x0804a008  # __dest
malloc(4)                                        = 0x0804a050  # puVar1
[...]
```
`0x0804a008 - 0x0804a050 == 0x42 == 0d72`
But the really smart way to do it would be to use the [buffer overflow pattern generator](https://wiremask.eu/tools/buffer-overflow-pattern-generator/?).

Either way, the decimal offset is 72. So we just need to write the address of `n()` obtained using `(gdb) p n`, then convert it to little endian **0x8048454**, `\x54\x84\x04\x08` and write it with our offset.

```sh
$ ./level6 $(python -c "print 'a' * 72 + '\x54\x84\x04\x08'")
f73dcb7a06f60e3ccc608990b0a046359d42a1a0489ffeefd0d9cb2d7c9cb82d
```

Great we got our flag, now we can go to the next level.
```sh
$ su level7
f73dcb7a06f60e3ccc608990b0a046359d42a1a0489ffeefd0d9cb2d7c9cb82d
```

NOTE:
The **heap** and the **stack** addresses grow towards each other. Stack address go down from high addresses to low ones, and heap addresses go up from low addresses to higher ones.
This is why even though the address of *__dest* is lower than *puVar1*'s it can be used as a buffer overflow.
