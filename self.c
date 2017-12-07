#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>

/**
 * It's like calling function from address
 * Example 1:
	int (*func)(void) = (int (*)(void))0x12345678;
	int x = func();

 * Example 2:
	((int (*)(void))0xdeadbeef)();

 * Compilation:
	gcc self.c -ldl
 */

int main() {

	char *error;
	int (*self_fun)(const char *command);

	void *self = dlopen("libc-2.23.so", RTLD_NOW);
	if (!self) {
		fprintf (stderr, "%s\n", dlerror());
		exit(1);
	}
	dlerror();    /* Clear any existing error */

	self_fun = dlsym(self, "system"); // returns the pointer to the system() function
	if ((error = dlerror()) != NULL) {
		fprintf (stderr, "%s\n", error);
		exit(1);
	}

	self_fun("echo HelloWorld--GoodMorning > /tmp/1.txt");

	dlclose(self);
	return 0;
}
