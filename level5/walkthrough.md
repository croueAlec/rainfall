# level5

Would you like a [PLT](https://medium.com/swlh/binary-exploitation-attacking-dynamic-linking-57d00ae56a83)? 

Nothing of note with `ll`.
We will get back to the `checksec` output later.

Functions found using `gdb > info functions`
|LIBC|user|
|-|-|
|printf|main|
|fgets|frame_dummy|
|system|o|
|exit|n|


The `main()` function calls to the `n()` function. The `n()` function has a the usual calls to `fgets()` and `printf()`. The function `o()` has the call to the `system()` function that starts a shell as **level6**.

**Ghidra** tells us that the `n()` function does not return and calls `exit()` directly. We cannot overwrite the value of `$eip` to return elsewhere then.

In the previous levels, we had to mess with values in order to influence the behavior of the code below `printf()`, well it seems this time the only thing below is the `exit()` function call.
What if `exit()` did not exit anymore?

Our goal will be to change the address of the function that is linked to the symbol *exit*. Basically, we tell the program that `exit()` is actually `o()`.

But how? When a binary is executed and a function is called, the **Procedure Linkage Table (PLT)** asks the **Global Offset Table (GOT)** for the function's address. If it does not exist yet, the **PLT** will link it and write the address in the **GOT** for future reference.

This means that if the user was able to write to the **GOT**, they could edit the addresses there. But if the **PLT** can write on it, so can the user.

This is usually mitigated using **Relocation Read-Only (RELRO)**, it forces the program to populate the **GOT** at the beginning and then sets it to **READONLY**. It is not always used since it adds some CPU cycles at the beginning.

Now, what did this `checksec` say again ? "*No RELRO*", huh, what a coinkydink!

[doc](https://www.isec.tugraz.at/wp-content/uploads/2020/07/04-exploits.pdf)


Let's do our usual Buffer Location.
```sh
for i in $(seq 1 100); do (echo -n "i=$i  "; echo "AAAA %$i\$x" | ./level5) ; done
```

It is at index 4.
Now we have to find the address of the `o()` function.

```gdb
x o
0x80484a4 <o>:	0x83e58955
```
[function address] [function name] : [first bytes at address, usually the machine code]

So `o()`'s address is `0x080484a4`.

Now we have to find the location of the `exit()` function on the **GOT**.

we need to disassemble the `exit()` function, and examine the address of the function itself calls.
```gdb
(gdb) disass exit
Dump of assembler code for function exit@plt:
   0x080483d0 <+0>:	jmp    *0x8049838
   0x080483d6 <+6>:	push   $0x28
   0x080483db <+11>:	jmp    0x8048370
End of assembler dump.

(gdb) x 0x8049838
0x8049838 <exit@got.plt>:	0x080483d6
```

`0x8049838` indeed points to `exit()`'s location on the **GOT**, in injectable little endian, the location is `\x38\x98\x04\x08`. We now have to overwrite the value at this address with the address of `o()`.

The address of `o()` in decimal is **134513828**. So our padding must be this **134513828 - 4**. That's a lot to write. This time, let's not be lazy and use a cleaner way to inject our value.

|address of o|padding|targeting the 4th argument|
|-|-|-|
|\x38\x98\x04\x08|%d|%4$n|
