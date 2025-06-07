#include <stdio.h>

#include "string.h"
#include "init.h"
#include "object.h"

int main(void) {
	fr_preinit();
	fr_init();
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
