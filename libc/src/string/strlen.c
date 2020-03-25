#include <string.h>

int strlen(const char *string)
{
	unsigned int len = 0;
	while (string[len])
		++len;
	return len;
}