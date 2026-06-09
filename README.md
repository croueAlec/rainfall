# rainfall
a repo for Rainfall, a 42School post-Common-Core Security project based on buffer overflow attacks

# rainfall

To setup rainfall, you have to download the ISO from the intra's subject page. Then create a virtual machine using your prefered software (in my case Virtual Box).

Before starting the virtual machine you should add a port forwarding rule under `Settings > Network > Adapter 1`, set `Attached to` to `NAT` and create a `Port Forwarding` rule with both the `Host Port` and `Guest Port` set to **4242**.

After starting the VM you can log on using `ssh -p 4242 level0@127.0.0.1` and using the password `level0`. Then you're good to go.

 ## Introduction
 This is a project about binary exploitation, in particular **Buffer Overflow** attacks. 
 This introduction will serve as a well an *introduction* on a few concepts required to properly understand what a **buffer overflow** is and how we will exploit them.
 You will find below a few wikipedia pages, articles, and tools pertaining to this topic :

 ### Assembly, registers, and stack memory
 In a nutshell, [Assembly language](https://en.wikipedia.org/wiki/Assembly_language) is the last barrier between **Humans** and **machine code**, it is composed of **instructions** to tell the cpu what to do (push, pop, jmp...). It uses variables of up to **64 bits** stored in the cpu called **registers** to process operations (eax, ebp, esp...). They each have confusing 3 letter names and serve a specific purpose. For a more in-depth explanation see my [libasm readme](https://github.com/croueAlec/libasm).

 The [stack memory](https://en.wikipedia.org/wiki/Stack-based_memory_allocation) is used to store local variables (among other things). It is based on the [stack abstract data type](https://en.wikipedia.org/wiki/Stack_(abstract_data_type)) on a concept of **LIFO** (the last value you placed is the next one you will get out, Last In-First Out).
 Let's jump to the really relevant part. When a function calls another one it stores on the stack the adress of the instruction after the function call. For instance *Func A* calls *Func B* at instruction, *N*, the adress of *N+1* will be stored on the **stack**. Then *Func B*, will allocate it's variables on the **stack** right after the adress of *N+1*.
 
 A **buffer overflow** happens when we overwrite outside the variable's allocated space. In the right conditions we could overwrite the return adress located right before *Func B* on the **stack**. When done on purpose, this is called a **buffer overflow** attack.


## Sources

## Tools
