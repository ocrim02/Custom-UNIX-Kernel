# Memory Layout Documentation

## UBoot and Init
The first 1 MB is reserved for UBoot and the Init section.

## Kernel
All sections of the kernel (.text, .rodata, .data, .bss) get 1 MB each starting behind the first MB.

## User
All sections of the user (.utext, .urodata, .udata + .ubss) get 1 MB each starting (1 MB alingned) behind the kernel bss section.

## Process Space
Behind the user section is a reserved space for PROCCESS_COUNT - 1 times 1 MB for process data.

## Kernel Stacks
The kernel stack is positioned at the end of the 128 MB of available RAM. <br>
Each of the kernel stacks has a max size of 4 kB and is seperated by a guardpage at the top. (Not implemented yet)

## User Stacks
Each user-/threadstack has a max size of 1 MB but the last Page (top) is a guard page, so each stack has a usable size of 1020 kB. <br>
The user stack section starts at 127 MB and goes up from there (to smaller addresses).


