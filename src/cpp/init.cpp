#include "../c/object.h"
#include "../c/objectutils.h"
#include "list.hpp"

FR_METHOD(fr_makeList) {
	return FR_OBJECT(fr4nken::newList());
}

extern "C" void cpp_preinit(void) {
	fr_listDataTag = fr_newDataTag();
}
extern "C" void cpp_init(void) {
	fr_Object *make = *fr_Registry_get(&fr_registry, "make");
	fr_addMethod(make, "list", &fr_makeList);
}
