#ifdef __cplusplus
extern "C" {
#endif
#ifndef FR_OBJECT_GUARD
#define FR_OBJECT_GUARD

#include "hashtable.h"
#include "value.h"
#include <stdint.h>

typedef struct fr_Object fr_Object;

/// Method callback (takes an object pointer, the number of args, and the arguments themselves)
typedef fr_Value (*fr_Method)(fr_Object *self, size_t nargs, const fr_Value *args);
/// Callback to destroy object data
typedef void (*fr_DestroyData)(uint32_t tag, void *data);

/// Creates a unique data tag. This is to be used to detect whether `data` is what is expected
uint32_t fr_newDataTag(void);

XSTD_HASHTABLE_H(fr_MethodTable, fr_MethodTable, fr_MethodTableIter, fr_MethodTableIter, const char *, fr_Method);

/// An object. These are basically just some arbitrary data and a vtable.
struct fr_Object {
	/// Tag of the data
	uint32_t dataTag;
	/// Reference count
	uint32_t refcnt;
	/// Arbitrary user data contained in the object (may be NULL)
	void *data;
	/// Destroy data method (may be NULL)
	fr_DestroyData destroyData;
	/// Methods
	fr_MethodTable methods;
};

/// Creates an object, starting with reference count one
fr_Object *fr_new(uint32_t dataTag, void *data, fr_DestroyData destroyData);
/// Acquires an object reference, increasings its refcount
void fr_acquire(fr_Object *object);
/// Releases an object reference, destroying the object if necessary
void fr_release(fr_Object *object);
/// Destroys an object
void fr_destroy(fr_Object *object);
/// Adds a new method to an object
void fr_addMethod(fr_Object *object, const char *name, fr_Method method);
/// Checks if an object has a method
bool fr_hasMethod(fr_Object *object, const char *name);
/// Calls a method on an object
fr_Value fr_callMethod(fr_Object *object, const char *name, size_t nargs, const fr_Value *args);
/// Calls a method on an object using the following syntax in `fmt`:
/// u - undefined
/// n - null
/// i - int
/// f - float
/// b - bool
/// o - object
fr_Value fr_shortCall(fr_Object *object, const char *name, const char *fmt, ...);

XSTD_HASHTABLE_H(fr_Registry, fr_Registry, fr_RegistryIter, fr_RegistryIter, const char*, fr_Object *);
/// The global registry for global objects
extern fr_Registry fr_registry;
/// Utility function to get from the registry
fr_Object **fr_registry_get(const char *name);
/// Utility function to add to the registry
void fr_registry_put(const char *name, fr_Object *obj);



#endif
#ifdef __cplusplus
}
#endif

