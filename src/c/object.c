#include "object.h"
#include "string.h"
#include "errorhandling.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>

fr_RegistryTable fr_registry;

static size_t string_hash(const char *str) {
	// djb2 by Dan Bernstein
	size_t hash = 5381;
	int32_t c;
	while((c = *str++))
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */	
	return hash;
}
#define string_eq(a, b) (strcmp((a), (b)) == 0)

XSTD_HASHTABLE_C(fr_MethodTable, fr_MethodTable, fr_MethodTableIter, fr_MethodTableIter, const char *, fr_Method, string_hash, string_eq);
XSTD_HASHTABLE_C(fr_RegistryTable, fr_RegistryTable, fr_RegistryIter, fr_RegistryIter, const char *, fr_Object *, string_hash, string_eq);

uint32_t fr_newDataTag(void) {
	static uint32_t last_id = 1;
	return last_id++;
}

fr_Object *fr_new(uint32_t dataTag, void *data, fr_DestroyData destroyData) {
	fr_Object *ret = malloc(sizeof(fr_Object));
	ret->dataTag = dataTag;
	ret->data = data;
	ret->destroyData = destroyData;
	ret->refcnt = 1;
	ret->methods = fr_MethodTable_new(32);
	return ret;
}
fr_Object *fr_newUnowned(uint32_t dataTag, void *data, fr_DestroyData destroyData) {
	fr_Object *ret = fr_new(dataTag, data, destroyData);
	ret->refcnt--;
	return ret;
}

void fr_acquire(fr_Object *object) {
	object->refcnt++;
}

void fr_release(fr_Object *object) {
	// shouldn't happen, but just to make sure...
	FR_ASSERT(object->refcnt != 0,);

	if(--object->refcnt != 0)
		return;
	fr_destroy(object);
}
void fr_destroy(fr_Object *object) {
	if(object->destroyData != NULL)
		object->destroyData(object->dataTag, object->data);
	fr_MethodTable_destroy(&object->methods);
	free(object);
}

void fr_addMethod(fr_Object *object, const char *name, fr_Method method) {
	fr_MethodTable_put(&object->methods, name, method);
}

bool fr_hasMethod(fr_Object *object, const char *name) {
	return fr_MethodTable_get(&object->methods, name) != NULL;	
}

fr_Value fr_callMethod(fr_Object *object, const char *name, size_t nargs, const fr_Value *args) {
	fr_Method *meth = fr_MethodTable_get(&object->methods, name);
	if(meth == NULL) {
		fr_printError("Attempt to call non-existent method '%s'", name);
		return (fr_Value){FR_TYPE_UNDEFINED};
	}
	return (*meth)(object, nargs, args);
}
#include <stdio.h>

fr_Value fr_shortCall(fr_Object *object, const char *name, const char *fmt, ...) {
	size_t len = strlen(fmt);
	fr_Value args[len];
	size_t i = 0;
	va_list ap;
	va_start(ap, fmt);
	while(true) {
		char c = *fmt;
		if(c == '\0')
			break;
		switch(c) {
			case 'u':
				args[i++] = FR_UNDEFINED;
				break;
			case 'n':
				args[i++] = FR_NULL;
				break;
			case 'i':
				args[i++] = FR_INT(va_arg(ap, int64_t));
				break;
			case 'f':
				args[i++] = FR_FLOAT(va_arg(ap, double));
				break;
			case 'b':
				args[i++] = FR_BOOL(va_arg(ap, int));
				break;
			case 'o':
				args[i++] = FR_OBJECT(va_arg(ap, fr_Object *));
				break;
			case 'c': {
				fr_Object *o = va_arg(ap, fr_Object *);
				const char *name = va_arg(ap, const char*);
				args[i++] = FR_CLOSURE(o, name);
				break;
			}
			default:
				fr_printError("Unsupported shortCall character '%c' encountered.", c);
				return FR_UNDEFINED;
		}
		fmt++;
	}
	va_end(ap);
	return fr_callMethod(object, name, len, args);
}

fr_DataInfo fr_getDataInfo(fr_Object *obj) {
	return (fr_DataInfo){obj->dataTag, obj->data};
}

fr_Object **fr_registryGet(const char *name) {
	return fr_RegistryTable_get(&fr_registry, name);
}
void fr_registryPut(const char *name, fr_Object *obj) {
	fr_RegistryTable_put(&fr_registry, name, obj);
}
