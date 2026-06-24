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
