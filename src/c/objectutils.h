#ifdef __cplusplus
extern "C" {
#endif
#ifndef FR_OBJECTUTILS_GUARD
#define FR_OBJECTUTILS_GUARD

#include "errorhandling.h"
#include "string.h"
#include "value.h"
#include "object.h"

#ifdef __cplusplus
#define FR_METHOD(NAME) extern "C" fr_Value NAME(fr_Object *self, size_t nargs, const fr_Value *args)
#else
#define FR_METHOD(NAME) fr_Value NAME(fr_Object *self, size_t nargs, const fr_Value *args)
#endif
#define FR_GET_DATA(TYPE) TYPE *data = (TYPE *)self->data
#define FR_NARGS_EQUALS(LEN) FR_ASSERT_MSG(nargs == LEN, FR_UNDEFINED, "Expected exactly " #LEN " arguments.")
#define FR_NARGS_BETWEEN(MIN, MAX) FR_ASSERT_MSG(nargs >= MIN && nargs <= MAX, FR_UNDEFINED, "Expected between " #MIN " and " #MAX " arguments.")
#define _FR_GET(NAME, I, TYPE, TAG, ENUM) \
	TYPE NAME; \
	FR_ASSERT_MSG(args[(I)].type == ENUM, FR_UNDEFINED, "Expected type %s, got type %s at argument " #I ".", fr_typename(ENUM), fr_typename(args[(I)].type)); \
	NAME = args[(I)].TAG
#define FR_GET_INT(NAME, I) _FR_GET(NAME, I, int64_t, vint, FR_TYPE_INT)
#define FR_GET_FLOAT(NAME, I) _FR_GET(NAME, I, double, vfloat, FR_TYPE_FLOAT)
#define FR_GET_OBJECT(NAME, I) _FR_GET(NAME, I, fr_Object *, vobject, FR_TYPE_OBJECT)
#define _FR_GET_OBJECT(NAME, I, TYPE, TAG, TYPENAME) \
	TYPE *NAME; \
	do { \
		fr_Value v = args[(I)]; \
		FR_ASSERT_MSG(v.type == FR_TYPE_OBJECT, FR_UNDEFINED, "Expected type %s, got type %s at argument " #I ".", fr_typename(FR_TYPE_OBJECT), fr_typename(v.type)); \
		FR_ASSERT_MSG(v.vobject->dataTag == TAG, FR_UNDEFINED, "Expected " TYPENAME ", got something else at argument " #I "."); \
		NAME = (TYPE *)v.vobject->data; \
	} while(0)
#define FR_GET_STRING(NAME, I) _FR_GET_OBJECT(NAME, I, fr_String, fr_stringDataTag, "string") 
#ifdef __cplusplus
#define FR_GET_LIST(NAME, I) _FR_GET_OBJECT(NAME, I, fr4nken::ListData, fr_listDataTag, "string") 
#endif


#endif
#ifdef __cplusplus
}
#endif

