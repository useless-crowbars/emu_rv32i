#include "my_stdlib.h"

#ifdef __riscv
#include <stdarg.h>
#define MAX_INT 2147483647

int abs(int j)
{
	return (j >= 0) ? j : -j;
}

char *strcpy(char *restrict dst, const char *restrict src)
{
	char *tmp = dst;
	while ((*dst++ = *src++) != '\0')
		;
	return tmp;
}

char *strncpy(char *dst, const char *restrict src, size_t dsize)
{
	char *tmp = dst;
	while (dsize-- > 0 && (*dst++ = *src++) != '\0')
		;
	if (*(dst - 1) != '\0')
		*dst = '\0';
	return tmp;
}

int atoi(const char *nptr)
{
	int result = 0;
	int sign = 1;

	while (*nptr == ' ' || *nptr == '\t' || *nptr == '\n' || *nptr == '\v' || *nptr == '\f' || *nptr == '\r') {
		nptr++;
	}

	if (*nptr == '-' || *nptr == '+') {
		if (*nptr == '-') {
			sign = -1;
		}
		nptr++;
	}

	while (*nptr >= '0' && *nptr <= '9') {
		result = result * 10 + (*nptr - '0');
		nptr++;
	}

	return sign * result;
}

///////////////////

void append_char(char *str, size_t *index, size_t size, char c)
{
	if (*index < size - 1) {
		str[*index] = c;
	}
	(*index)++;
}

void append_string(char *str, size_t *index, size_t size, const char *s)
{
	while (*s) {
		append_char(str, index, size, *s++);
	}
}

void append_int(char *str, size_t *index, size_t size, int value)
{
	char buffer[20];
	int pos = 0;
	int is_negative = value < 0;

	if (is_negative) {
		value = -value;
	}

	do {
		buffer[pos++] = '0' + (value % 10);
		value /= 10;
	} while (value > 0);

	if (is_negative) {
		buffer[pos++] = '-';
	}

	while (--pos >= 0) {
		append_char(str, index, size, buffer[pos]);
	}
}

int snprintf(char *str, size_t size, const char *restrict format, ...)
{
	va_list args;
	va_start(args, format);
	size_t index = 0;

	for (; *format; format++) {
		if (*format == '%') {
			format++;
			if (*format == 'd') {
				int value = va_arg(args, int);
				append_int(str, &index, size, value);
			} else if (*format == 's') {
				const char *value = va_arg(args, const char *);
				append_string(str, &index, size, value);
			} else if (*format == 'c') {
				char value = (char)va_arg(args, int);
				append_char(str, &index, size, value);
			} else {
				__asm__ __volatile__("ecall;");
				append_char(str, &index, size, '%');
				append_char(str, &index, size, *format);
			}
		} else {
			append_char(str, &index, size, *format);
		}
	}
	if (size > 0) {
		if (index < size) {
			str[index] = '\0';
		} else {
			str[size - 1] = '\0';
		}
	}
	va_end(args);
	return (int)index;
}

int sprintf(char *restrict str, const char *restrict format, ...)
{
	va_list args;
	va_start(args, format);
	size_t index = 0;

	for (; *format; format++) {
		if (*format == '%') {
			format++;
			if (*format == 'd') {
				int value = va_arg(args, int);
				append_int(str, &index, MAX_INT, value);
			} else if (*format == 's') {
				const char *value = va_arg(args, const char *);
				append_string(str, &index, MAX_INT, value);
			} else if (*format == 'c') {
				char value = (char)va_arg(args, int);
				append_char(str, &index, MAX_INT, value);
			} else {
				__asm__ __volatile__("ecall;");
				append_char(str, &index, MAX_INT, '%');
				append_char(str, &index, MAX_INT, *format);
			}
		} else {
			append_char(str, &index, MAX_INT, *format);
		}
	}

	str[index] = '\0';

	va_end(args);

	return (int)index;
	return 0;
}

///////

int tolower(int c)
{
	if (c >= 'A' && c <= 'Z') {
		return c + ('a' - 'A');
	}
	return c;
}

int toupper(int c)
{
	if (c >= 'a' && c <= 'z') {
		return c - ('a' - 'A');
	}
	return c;
}

int strcasecmp(const char *s1, const char *s2)
{
	while (*s1 && *s2) {
		char c1 = tolower((unsigned char)*s1);
		char c2 = tolower((unsigned char)*s2);

		if (c1 != c2) {
			return (unsigned char)c1 - (unsigned char)c2;
		}
		s1++;
		s2++;
	}
	return (unsigned char)tolower((unsigned char)*s1) - (unsigned char)tolower((unsigned char)*s2);
}

int strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2)) {
		s1++;
		s2++;
	}
	return *s1 - *s2;
}

int strncasecmp(const char *s1, const char *s2, size_t n)
{
	while (--n > 0 && *s1 && *s2) {
		char c1 = tolower((unsigned char)*s1);
		char c2 = tolower((unsigned char)*s2);

		if (c1 != c2) {
			return (unsigned char)c1 - (unsigned char)c2;
		}
		s1++;
		s2++;
	}
	return (unsigned char)tolower((unsigned char)*s1) - (unsigned char)tolower((unsigned char)*s2);
}

size_t strlen(const char *s)
{
	size_t length = 0;
	while (*s != '\0') {
		length++;
		s++;
	}
	return length;
}

void *memcpy(void *dest, const void *src, size_t n)
{
	char *d = (char *)dest;
	const char *s = (const char *)src;
	for (size_t i = 0; i < n; ++i) {
		d[i] = s[i];
	}
	return dest;
}

void *memset(void *s, int c, size_t n)
{
	unsigned char *p = (unsigned char *)s;
	unsigned char uc = (unsigned char)c;

	// Fill memory block with the specified value
	for (size_t i = 0; i < n; ++i) {
		p[i] = uc;
	}

	return s;
}

char *strcat(char *restrict dst, const char *restrict src)
{
	char *ptr = dst;
	while (*ptr) {
		ptr++;
	}
	while (*src) {
		*ptr++ = *src++;
	}
	*ptr = '\0';
	return dst;
}

void *alloca(size_t size)
{
	//__asm__ __volatile__("ecall");
	return NULL;
}

#endif __riscv
