# Syscall Library Documentation

The Syscall Library provides a basic interface to interact with kernel functions.
<br><br>


## void syscall_putc(char)

The given character is written out via the UART bus.
<br><br> 

## char syscall_getc()

This function check if the is a char in the ring buffer.
- if so, it will return it
- if not, it will put the thread waiting until a new char is received and return the char afterwards
<br><br>  

## void syscall_sleep(unsigned int)

This function puts the thread calling it into Waiting State for the amount of timer intervalls given in the argument.
If the argument is 0 the thread will continue without any pause.

**Note:** The sleep time given in the argument is a minimum. It is not guaranteed that the thread will be running directly after the time has passed.
<br><br>  

## void syscall_exit()

This function should be called for a clean implementation after a thread has finished.
It will end the thread and reset it.

**Note:** If this function is not called at the end of a thread it will be called automatically by the kernel.
<br><br> 

## bool syscall_thread_create(void (*func)(void *), const void*, unsigned int)

This function creates a new thread. The first arguments needs to be a pointer to the function that should be run by the thread.
The second argument is a pointer to the aruments for the function. The size of the arguments in bytes is given in the third argument.

After a successfull creation the function returns true.
If the is no free thread available in the system you will get false in return.
<br><br>  

## void undef_syscall()

This creates an undefined syscall exception which will cause a register dump and exit of the current thread.
<br><br>  