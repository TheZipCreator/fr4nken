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
	fr_Object *make = *fr_registryGet("make");
	fr_addMethod(make, "string", &makeString);
}

void c_init(void) {}

void c_deinit(void) {}

void fr_preinit(void) {
	// init registry
	fr_registry = fr_RegistryTable_new(512);
	// create Make object
	fr_registryPut("make", fr_new(0, NULL, NULL));
	// preinit
	c_preinit();
	cpp_preinit();
	zig_preinit();
	nim_preinit();

}

fr_Value test_listen(fr_Object *self, size_t nargs, const fr_Value *args) {
	fr_Object *graphics = args[0].vobject;
	fr_shortCall(graphics, "color", "iiii", 255, 0, 0, 255);
	fr_shortCall(graphics, "line", "ffff", 50.0, 50.0, 100.0, 100.0);
	return FR_UNDEFINED;
}

void fr_init(void) {
	// make events object
	fr_Object *make = *fr_registryGet("make");
	fr_Object *postInit = fr_shortCall(make, "event", "").vobject;
	fr_registryPut("event_postInit", postInit);
	fr_registryPut("event_render", fr_shortCall(make, "event", "").vobject);
	fr_Object *listener = fr_new(0, NULL, NULL);
	fr_addMethod(listener, "on_render", &test_listen);
	fr_shortCall(*fr_registryGet("event_render"), "listen", "c", listener, "on_render");

	
	// init
	c_init();
	cpp_init();
	zig_init();
	nim_init();
	
	// call postInit event
	fr_shortCall(postInit, "trigger", "");

}
void fr_deinit(void) {
	// deinit
	nim_deinit();
	zig_deinit();
	cpp_deinit();
	c_deinit();
}
