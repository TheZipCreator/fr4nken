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

/// Creates a string object (copies the string to the new object)
fr_Object *fr_newString(const char *string, size_t len);

#endif
#ifdef __cplusplus
}
#endif

