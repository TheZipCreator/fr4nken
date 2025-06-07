#ifdef __cplusplus
extern "C" {
#endif
#ifndef FR_STRING_GUARD
#define FR_STRING_GUARD

#include "object.h"

/// Data tag for a string
extern uint32_t fr_stringDataTag;

/// Data inside a string object 
typedef struct fr_String {
	size_t len;
	char data[];
} fr_String;

/// Hashes a string
size_t fr_hashString(const fr_String *string);

/// Creates a string object (copies the string to the new object)
fr_Object *fr_newString(const char *string, size_t len);
/// Creates a string object from a C string
fr_Object *fr_newStringC(const char *string);
/// Creates an unowned string object
fr_Object *fr_newStringUnowned(const char *string, size_t len);
/// Creates an unowned string object from a C string
fr_Object *fr_newStringCUnowned(const char *string);

#endif
#ifdef __cplusplus
}
#endif

