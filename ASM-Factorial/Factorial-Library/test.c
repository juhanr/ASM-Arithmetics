#include "faclib.h"
#include <stdio.h>

int main(int argc, char** argv, char** envp) {
	int x, fac;

	x = 0;
	fac = factorial(x);
	printf("%d! = %d\n", x, fac);

	x = 1;
	fac = factorial(x);
	printf("%d! = %d\n", x, fac);

	x = 2;
	fac = factorial(x);
	printf("%d! = %d\n", x, fac);

	x = 3;
	fac = factorial(x);
	printf("%d! = %d\n", x, fac);

	x = 7;
	fac = factorial(x);
	printf("%d! = %d\n", x, fac);

	x = 10;
	fac = factorial(x);
	printf("%d! = %d\n", x, fac);

	x = 12;
	fac = factorial(x);
	printf("%d! = %d\n", x, fac);

	x = -10;
	fac = factorial(x);
	printf("%d! = %d\n", x, fac);

	return 0;
}
