#include "init.h"
#include "value.h"
#include "string.h"
#include "objectutils.h"

static FR_METHOD(makeString) {
	FR_NARGS_BETWEEN(0, 1);
	if(nargs == 0) {
		return FR_OBJECT(fr_newString(NULL, 0));
	}
	FR_GET_INT(len, 0);
	char buf[len];
	memset(buf, 0, len);
	return FR_OBJECT(fr_newString(buf, len));
}

void c_preinit(void) {
	fr_stringDataTag = fr_newDataTag();
	fr_Object *make = *fr_Registry_get(&fr_registry, "make");
	fr_addMethod(make, "string", &makeString);
}

void c_init(void) {
	
}

void fr_preinit(void) {
	// init registry
	fr_registry = fr_Registry_new(512);
	// create Make object
	fr_Object *make = fr_new(0, NULL, NULL);
	fr_Registry_put(&fr_registry, "make", make);
	// preinit
	c_preinit();
	cpp_preinit();
	zig_preinit();

}

void fr_init(void) {
	// make events object
	fr_Object *make = *fr_Registry_get(&fr_registry, "make");
	fr_Object *events = fr_callMethod(make, "map", 0, NULL).vobject;
	
	// init
	c_init();
	cpp_init();
	zig_init();

}
