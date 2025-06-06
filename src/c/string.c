#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

#include "string.h"
#include "objectutils.h"

uint32_t fr_stringDataTag;

static void String_destroy(uint32_t tag, void *data) {
	free(data);
}

#define BOUNDS_CHECK(I) FR_ASSERT_MSG(I >= 0 && I < data->len, FR_UNDEFINED, "String index %" PRId64 " out of bounds for string of length %zu", I, data->len)

static FR_METHOD(String_len) {
	FR_NARGS_EQUALS(0);
	FR_GET_DATA(fr_String);
	return FR_INT(data->len);
}
static FR_METHOD(String_get) {
	FR_NARGS_EQUALS(1);
	FR_GET_DATA(fr_String);
	FR_GET_INT(index, 0);
	BOUNDS_CHECK(index);
	return FR_INT(data->data[index]);
}
static FR_METHOD(String_put) {
	FR_NARGS_EQUALS(2);
	FR_GET_DATA(fr_String);
	FR_GET_INT(index, 0);
	FR_GET_INT(ch, 1);
	BOUNDS_CHECK(index);
	data->data[index] = ch;
	return FR_UNDEFINED;
}
static FR_METHOD(String_append) {
	FR_NARGS_EQUALS(1);
	FR_GET_DATA(fr_String);
	FR_GET_STRING(other, 0);
	self->data = data = realloc(data, sizeof(fr_String)+data->len+other->len);
	memcpy(data->data+data->len, other->data, other->len);
	return FR_UNDEFINED;
}
static FR_METHOD(String_sub) {
	FR_NARGS_EQUALS(2);
	FR_GET_DATA(fr_String);
	FR_GET_INT(first, 0);
	FR_GET_INT(second, 1);
	BOUNDS_CHECK(first);
	BOUNDS_CHECK(second);
	FR_ASSERT_MSG(second >= first, FR_UNDEFINED, "Second index in substring must be greater than first");
	return FR_OBJECT(fr_newString(data->data+first, second-first));
}

fr_Object *fr_newString(const char *string, size_t len) {
	fr_String *data = malloc(sizeof(fr_String)+len);
	memcpy(data->data, string, len);
	fr_Object *ret = fr_new(fr_stringDataTag, data, &String_destroy);
	fr_addMethod(ret, "len", &String_len);
	fr_addMethod(ret, "get", &String_get);
	fr_addMethod(ret, "put", &String_put);
	fr_addMethod(ret, "append", &String_append);
	fr_addMethod(ret, "sub", &String_sub);
	return ret;
}

