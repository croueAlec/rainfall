# rainfall

To setup rainfall, you have to download the ISO from the intra's subject page. Then create a virtual machine using your prefered software (in my case Virtual Box).

To create the VM, you can do this manually or just use 
```sh
./tools/setup_rainfall_vm.sh [iso path]
```

Or create the VM manually and add a port forwarding rule under `Settings > Network > Adapter 1`, set `Attached to` to `NAT` and create a `Port Forwarding` rule with both the `Host Port` and `Guest Port` set to **4242**.

After starting the VM you can log on using `ssh -p 4242 level0@127.0.0.1` and using the password `level0`. Then you're good to go.

You can log in as a specific user (level[0-9] or bonus[0-3]) using the following command
```sh
./tools/open_level.sh [user]
```

If the flag has not been found, it will not be printed.

 ## Introduction
 This is a project about binary exploitation, in particular **Buffer Overflow** attacks. 
 This introduction will serve as a well an *introduction* on a few concepts required to properly understand what a **buffer overflow** is and how we will exploit them.
 You will find below a few wikipedia pages, articles, and tools pertaining to this topic :

 ### Assembly, registers, and stack memory
 In a nutshell, [Assembly language](https://en.wikipedia.org/wiki/Assembly_language) is the last barrier between **Humans** and **machine code**, it is composed of **instructions** to tell the cpu what to do (push, pop, jmp...). It uses variables of up to **64 bits** stored in the cpu called **registers** to process operations (eax, ebp, esp...). They each have confusing 3 letter names and serve a specific purpose. For a more in-depth explanation see my [libasm readme](https://github.com/croueAlec/libasm).

 The [stack memory](https://en.wikipedia.org/wiki/Stack-based_memory_allocation) is used to store local variables (among other things). It is based on the [stack abstract data type](https://en.wikipedia.org/wiki/Stack_(abstract_data_type)) on a concept of **LIFO** (the last value you placed is the next one you will get out, Last In-First Out).
 Let's jump to the really relevant part. When a function calls another one it stores on the stack the adress of the instruction after the function call. For instance *Func A* calls *Func B* at instruction, *N*, the adress of *N+1* will be stored on the **stack**. Then *Func B*, will allocate it's variables on the **stack** right after the adress of *N+1*.
 
 A **buffer overflow** happens when we overwrite outside the variable's allocated space. In the right conditions we could overwrite the return adress located right before *Func B* on the **stack**. When done on purpose, this is called a **buffer overflow** attack. It can be used to redirect from *Func B* to **another function** or a **piece of code** injected by other means.


## Sources

[Stack memory introduction article](https://beta.hackndo.com/stack-introduction/) (in french).
[Endianness wikipedia page](https://en.wikipedia.org/wiki/Endianness).
[An infographic](https://github.com/croueAlec/ft_ssl/blob/main/Documentation/endianness.png) to better understand endianness (made by yours truly).

## Tools

[Dogbolt](https://dogbolt.org/) an online Binary decompiler.
[Godbolt](https://godbolt.org/) an online C to assembler translator.
[CodeConvert](https://www.codeconvert.ai/assembly-to-c-converter) an online **AI** assembly to C translator.

[Wiremask's buffer overflow finder](https://wiremask.eu/tools/buffer-overflow-pattern-generator/) this tool creates a pattern to be used as a basic overflow input to show where the return adress can be overwritten.

## Methodology

When we reach a new level, we will follow these basic steps and use the following commands.

`checksec --file <bin>` can be used to see what vulnerabilities the binary has.

`ll` can be used to see the binary's permissions. Most of the time it will have the permissions of level*N+1*. Meaning any command we can use in it will be used as if we were level*N+1*.

`gdb <bin>` can be used to follow the flow of execution step by step. It can also show the assembly code of a specific function using the `disass <function>` command. Moreover, using the `info function` command shows the functions used in the binary as well as their adresses.

`readelf -sW <bin> | grep FUNC` an alternative to gdb's `info function`.

`man <function>` to search for potential security issues we can exploit in any LIBC we find.

`objdump -SMintel --demangle <bin>` to get the full asm dump

The workflow will be as follows :
- Using `checksec` and `ll` we will analyse what is feasible with our binary.
- Using `gdb`'s `info function` or `readelf` we can find the functions used in the binary.
- With these functions now found, we can try to find known attack vectors to exploit using the `man` command.
- Then we can inspect the code using `objdump` to find a way to use this attack vector.
- Finally we can use the different tools mentionned above to apply this exploit.

We also have access to **Ghidra** to decompile binaries. To extract binaries from the vm, use
```sh
./tools/extract_binary.sh USER
```

Binaries can then be decompiled by loading them in **Ghidra** then analysing them.
The `main` function can be found using the `symbol tree` under `m`.
