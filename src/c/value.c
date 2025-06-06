#include "value.h"

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

