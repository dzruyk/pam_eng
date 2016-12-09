#include <time.h>

int
pe_getticks()
{
	int tpms = CLOCKS_PER_SEC / 1000;

	return clock() / tpms;
}

