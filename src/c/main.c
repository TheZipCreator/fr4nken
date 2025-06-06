#include <stdio.h>

#include "string.h"
#include "init.h"
#include "object.h"

int main(void) {
	fr_registry = fr_Registry_new(512);
	// preinit
	c_preinit();
	cpp_preinit();
	zig_preinit();
	// init
	c_init();
	cpp_init();
	zig_init();
	// destroy registry
	{
		fr_RegistryIter iter = fr_Registry_iterator(&fr_registry);
		while(fr_Registry_next(&iter)) {
			fr_destroy(*iter.value);
		}
		fr_Registry_destroy(&fr_registry);
	}
	return 0;
}
