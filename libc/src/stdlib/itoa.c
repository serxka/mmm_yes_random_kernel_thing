#include <stdlib.h>

char *itoa(int num, char *buf, const unsigned int base)
{
	int i = 0;
	int is_negative = 0;
	int digits = 0;

	// check if it zero
	if (!num) {
		buf[i++] = '0';
		buf[i] = '\0';
		return buf;
	}

	// only do negative numbers with base 10, otherwise its assumed unsigned
	if ((num < 0) && (base == 10)) {
		is_negative = 1;
		num = -num;
	}

	// loop through and get the current digit while thanking the lords of ascii
	while (num) {
		int r = num%base;
		buf[i++] = (r>9) ? (r-10) + 'a' : r + '0';
		num = num/base;
		++digits;
	}

	// add negative if negative
	if (is_negative)
		buf[i++] = '-';

	// end the string
	buf[i] = '\0';

	// reverse the string
	int start = 0;
	int end = digits - 1;
	while (start < end) {
		int temp = buf[end];
		buf[end] = buf[start];
		buf[start] = temp;
		++start;
		--end;
	}

	return buf;
}