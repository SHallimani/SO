# SO
Shared Object
In this seession we are just going to understand and discuss on he Shared Libraries

I want to understand how the Shared Libraries will load and link with our application when running on the system.
Basic:



Libraries are loaded by ld.so (dynamic linker or run-time linker aka rtld, ld-linux.so.2 or ld-linux.so.*
in case of Linux; part of glibc). It is declared as "interpreter" (INTERP; .interp section) of all dynamic linked ELF binaries.
So, when you start program, Linux will start an ld.so (load into memory and jump to its entry point),
then ld.so will load your program into memory, prepare it and then run it.

You can also start dynamic program with
 --> /lib/ld-linux.so.2 ./your_program your_prog_params

ld.so does an actual open and mmap of all needed ELF files, both ELF file of your program and ELF files of all neeeded libraries
Also, it fills GOT and PLT tables and does relocations resolving (it writes addresses of functions from libraries to call sites, in many cases with indirect calls).

The typical load address of some library you can get with ldd utility.
It is actually a bash script, which sets a debug environment variable of ld.so (actually LD_TRACE_LOADED_OBJECTS=1 in case of glibc's rtld)
and starts a program. You even can also do it yourself without needs of the script
e.g. with using bash easy changing of environment variables for single run:
 --> LD_TRACE_LOADED_OBJECTS=1 /bin/echo

The ld.so will see this variable and will resolve all needed libraries and print load addresses of them.
But with this variable set, ld.so will not actually start a program (not sure about static constructors of program or libraries)
If the ASLR feature is disabled, load address will be the same most times
Modern Linuxes often has ASLR enabled, so to disable it, use echo 0 | sudo tee /proc/sys/kernel/randomize_va_space.

You can find offset of system function inside the libc.so with nm utility from binutils. I think, you should use nm -D /lib/libc.so or objdump -T /lib/libc.so and grep output.

Lets try with sample example
#include <stdio.h>
#include <dlfcn.h>

/**
 * It's like calling function from address
 * Example 1:
        int (*func)(void) = (int (*)(void))0x12345678;
        int x = func();

 * Example 2:
        ((int (*)(void))0xdeadbeef)();
 */
int main() {


        int (*self_fun)(const char *command);

        void *self = dlopen(NULL, RTLD_NOW);
        dlsym(self, "system"); // returns the pointer to the system() function

        printf("Self: %p\n", self);
        self_fun("echo 1 > /tmp/1.txt");

        dlclose(self);
        return 0;
}
