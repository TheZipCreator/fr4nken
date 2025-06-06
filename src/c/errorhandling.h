#ifdef __cplusplus
extern "C" {
#endif
#ifndef FR_ERROR_HANDLING_GUARD
#define FR_ERROR_HANDLING_GUARD

#include <stdbool.h>

/// Prints an error message
void fr_printError(const char *fmt, ...);

/// Does an assertion
#define FR_ASSERT(COND, RET) do { \
	if(!(COND)) { \
		fr_printError("Assertion failed: %s", #COND); \
		return RET; \
	} \
} while(0)

/// Does an assertion, printing a message if false
#define FR_ASSERT_MSG(COND, RET, FMT, ...) do { \
	if(!(COND)) { \
		fr_printError("%s" __VA_OPT__(,) __VA_ARGS__); \
		return RET; \
	} \
} while(0)

#endif
#ifdef __cplusplus
}
#endif

