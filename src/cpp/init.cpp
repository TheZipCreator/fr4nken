#include "../c/object.h"
#include "../c/objectutils.h"
#include "list.hpp"
#include "map.hpp"

FR_METHOD(fr_makeList) {
	return FR_OBJECT(fr4nken::newList());
}
FR_METHOD(fr_makeMap) {
	return FR_OBJECT(fr4nken::newMap());
}

extern "C" void cpp_preinit(void) {
	fr_listDataTag = fr_newDataTag();
	fr_mapDataTag = fr_newDataTag();
	fr_Object *make = *fr_Registry_get(&fr_registry, "make");
	fr_addMethod(make, "list", &fr_makeList);
	fr_addMethod(make, "map", &fr_makeMap);
}
extern "C" void cpp_init(void) {
}
