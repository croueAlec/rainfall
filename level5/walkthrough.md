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

This is usually mitigated using **Relocation Read-Only (RELRO)**, it forces the program to fill the **GOT** at the beginning and then sets it to **READONLY**.

Now, what did this `checksec` say again ? "*No RELRO*", huh, what a coinkydink!

[doc](https://www.isec.tugraz.at/wp-content/uploads/2020/07/04-exploits.pdf)
