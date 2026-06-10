# level2

Functions found
|LIBC|user|
|-|-|
|printf|main|
|fflush|frame_dummy|
|gets||
|strdup||
|puts||

As we saw before `gets` can be exploited for it's capactiy to cause **buffer overflows**.
`printf` can be exploited in some specific cases, see the man.

```
BUGS
       Because sprintf() and vsprintf()  assume  an  arbitrarily  long  string,
       callers  must be careful not to overflow the actual space; this is often
       impossible to assure.  Note that the length of the strings  produced  is
       locale-dependent  and  difficult  to  predict.   Use  snprintf() and vs‐
       nprintf() instead (or asprintf(3) and vasprintf(3)).

       Code such as printf(foo); often indicates a bug, since foo may contain a
       % character.  If foo comes from untrusted user input, it may contain %n,
       causing the printf() call to write to memory  and  creating  a  security
       hole.
```

In our case, the second paragraph in particular could be exploited.

This time we will use **Ghidra** from the start.
After extracting our binary using [the extracter script](../tools/extract_binary.sh), open it and analyse it.

Once again, *frame_dummy* does nothing.
Main only calls the *p* function.

The *p* function has a call to *gets* in a buffer of 76 chars.
What's interesting is that this time, the value we'll overwrite is a variable.
We can see that a **logical and** is applied to this variable with the hexadecimal number `0xb0000000`.

If this **and** comparison matches, this value is then printed with printf.
What this means is that any address will be printed as long as it contains `0xb0000000`.
By convention on **32bits** machines, addresses that match the `0xb0000000` mask are located on the **stack memory**. This means that we have to write an address to heap memory.

We will now use a shellcode!
