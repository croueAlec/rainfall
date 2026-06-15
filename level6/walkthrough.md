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


