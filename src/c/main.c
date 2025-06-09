#include <stdio.h>

#include "string.h"
#include "init.h"
#include "object.h"

int main(void) {
	fr_preinit();
	fr_init();
	fr_deinit();
	fr_sdlLoop();
	// destroy registry
	{
		fr_RegistryIter iter = fr_RegistryTable_iterator(&fr_registry);
		while(fr_RegistryTable_next(&iter)) {
			fr_destroy(*iter.value);
		}
		fr_RegistryTable_destroy(&fr_registry);
	}
	return 0;
}
