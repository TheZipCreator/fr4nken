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
	// create Make object
	fr_Object *make = fr_new(0, NULL, NULL);
	fr_addMethod(make, "string", &makeString);
	fr_Registry_put(&fr_registry, "make", make);
}

void c_init(void) {
	
}
