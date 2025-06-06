#include "errorhandling.h"

#include <stdio.h>
#include <stdarg.h>

void fr_printError(const char *fmt, ...) {
	static char buf[1024];
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buf, 1024, fmt, ap);
	va_end(ap);
	printf("[ERROR] %s\n", buf);
}
