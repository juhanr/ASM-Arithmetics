#include "combinatorics.h"
#include <stdio.h>

int main(int argc, char** argv, char** envp) {
	int n, r, c;
	n = 8;
	r = 5;

	// n!
	c = fac(n);
	printf("fac(%d) = %d\n", n, c);

	// Permutations without repetition
	c = perm(n,r);
	printf("perm(%d,%d) = %d\n", n, r, c);

	// Permutations with repetition
	c = permrep(n,r);
	printf("permrep(%d,%d) = %d\n", n, r, c);

	// Combinations without repetition
	c = comb(n,r);
	printf("comb(%d,%d) = %d\n", n, r, c);

	// Combinations with repetition
	c = combrep(n,r);
	printf("combrep(%d,%d) = %d\n", n, r, c);

	return 0;
}


