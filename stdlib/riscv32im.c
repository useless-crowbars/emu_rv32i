#include "riscv32im.h"

#ifdef __riscv
long __divdi3(long a, long b)
{
	if (b == 0) {
		return 0;
	}

	int sign = (a < 0) ^ (b < 0) ? -1 : 1;

	unsigned long ua = a < 0 ? -a : a;
	unsigned long ub = b < 0 ? -b : b;

	unsigned long quotient = 0;
	while (ua >= ub) {
		ua -= ub;
		quotient++;
	}

	return sign * quotient;
}

int __divsi3(int a, int b)
{
	if (b == 0) {
		return 0;
	}

	int sign = (a < 0) ^ (b < 0) ? -1 : 1;

	unsigned int ua = a < 0 ? -a : a;
	unsigned int ub = b < 0 ? -b : b;

	unsigned int quotient = 0;
	while (ua >= ub) {
		ua -= ub;
		quotient++;
	}

	return sign * quotient;
}

int __modsi3(int a, int b)
{
	if (b == 0) {
		return 0;
	}

	int sign = (a < 0) ? -1 : 1;

	unsigned int ua = a < 0 ? -a : a;
	unsigned int ub = b < 0 ? -b : b;

	while (ua >= ub) {
		ua -= ub;
	}

	return sign * ua;
}

int __mulsi3(int a, int b)
{
	int result = 0;
	int sign = 1;

	if (a < 0) {
		a = -a;
		sign = -sign;
	}
	if (b < 0) {
		b = -b;
		sign = -sign;
	}

	while (b > 0) {
		if (b & 1) {
			result += a;
		}
		a <<= 1;
		b >>= 1;
	}

	if (sign < 0) {
		result = -result;
	}

	return result;
}

long __muldi3(long a, long b)
{
	long result = 0;
	int sign = (a < 0) ^ (b < 0) ? -1 : 1;
	unsigned long ua = a < 0 ? -a : a;
	unsigned long ub = b < 0 ? -b : b;

	while (ub > 0) {
		if (ub & 1) {
			result += ua;
		}
		ua <<= 1;
		ub >>= 1;
	}

	return sign * result;
}

unsigned int __udivsi3(unsigned int a, unsigned int b)
{
	if (b == 0) {
		return 0;
	}

	unsigned int quotient = 0;
	unsigned int remainder = 0;

	for (int i = 31; i >= 0; --i) {
		remainder <<= 1;
		remainder |= (a >> i) & 1;

		if (remainder >= b) {
			remainder -= b;
			quotient |= 1 << i;
		}
	}

	return quotient;
}

#endif
