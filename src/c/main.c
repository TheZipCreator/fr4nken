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
	{
		fr_Object *make = *fr_Registry_get(&fr_registry, "make");
		fr_Object *list = fr_callMethod(make, "list", 0, NULL).vobject;
		for(int64_t i = 5; i < 15; i++) {
			fr_shortCall(list, "push", "i", i);
		}
		for(int64_t i = 0; i < fr_callMethod(list, "len", 0, NULL).vint; i++) {
			printf("[%lu] = %lu\n", i, fr_shortCall(list, "get", "i", i).vint);
		}
		fr_release(list);
	}
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
