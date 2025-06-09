#include "value.h"
#include "object.h"

const char *fr_typename(uint8_t type) {
	switch(type) {
		case FR_TYPE_UNDEFINED:
			return "undefined";
		case FR_TYPE_NULL:
			return "null";
		case FR_TYPE_INT:
			return "int";
		case FR_TYPE_FLOAT:
			return "float";
		case FR_TYPE_BOOL:
			return "bool";
		case FR_TYPE_OBJECT:
			return "object";
		default:
			return "unknown";
	}
}

fr_Value fr_makeClosure(fr_Object *obj, const char *method) {
	fr_acquire(obj);
	return (fr_Value){FR_TYPE_CLOSURE, .vclosure = {obj, method}};
}
fr_Value fr_callClosure(fr_Closure closure, size_t nargs, const fr_Value *args) {
	return fr_callMethod(closure.obj, closure.method, nargs, args);
}
