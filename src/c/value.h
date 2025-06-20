#ifdef __cplusplus
extern "C" {
#endif
#ifndef FR_VALUE_GUARD
#define FR_VALUE_GUARD

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/// Type of a value
enum {
	/// Returned when an operation is invalid (such as calling a nonexistent method)
	FR_TYPE_UNDEFINED,
	/// Null
	FR_TYPE_NULL,
	/// 64-bit integer
	FR_TYPE_INT,
	/// 64-bit float
	FR_TYPE_FLOAT,
	/// boolean
	FR_TYPE_BOOL,
	/// object
	FR_TYPE_OBJECT,
	// closure
	FR_TYPE_CLOSURE
};

typedef struct fr_Object fr_Object;

/// A closure
typedef struct fr_Closure {
	/// Object to call a method on
	fr_Object *obj;
	/// Name of method to call
	const char *method;
} fr_Closure;

/// A value, which is a tagged union of possible things.
typedef struct fr_Value {
	uint8_t type;
	union {
		int64_t vint;
		double vfloat;
		bool vbool;
		fr_Object *vobject;
		fr_Closure vclosure;
	};
} fr_Value;

/// Gets a string representing the name of a type
const char *fr_typename(uint8_t type);
/// Creates a closure
fr_Value fr_makeClosure(fr_Object *obj, const char *method);
/// Calls a closure
fr_Value fr_callClosure(fr_Closure closure, size_t nargs, const fr_Value *args);

// (implemented in zig)

/// Acquires a value
void fr_acquireV(fr_Value v);
/// Releases a value
void fr_releaseV(fr_Value v);
/// Hashes a value
size_t fr_hash(fr_Value v);
/// Checks if two values are equal
bool fr_equal(fr_Value a, fr_Value b);
/// Converts a value to a string
fr_Object *fr_toString(fr_Value v);


#define FR_UNDEFINED (fr_Value){.type = FR_TYPE_UNDEFINED}
#define FR_NULL (fr_Value){.type = FR_TYPE_NULL}
#define FR_INT(VALUE) (fr_Value){.type = FR_TYPE_INT, .vint = VALUE}
#define FR_FLOAT(VALUE) (fr_Value){.type = FR_TYPE_FLOAT, .vfloat = VALUE}
#define FR_BOOL(VALUE) (fr_Value){.type = FR_TYPE_BOOL, .vbool = VALUE}
#define FR_OBJECT(VALUE) (fr_Value){.type = FR_TYPE_OBJECT, .vobject = VALUE}
#define FR_CLOSURE(OBJECT, NAME) (fr_Value){.type = FR_TYPE_CLOSURE, .vclosure = {OBJECT, NAME}}

#endif
#ifdef __cplusplus
}
#endif

