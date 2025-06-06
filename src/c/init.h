#ifdef __cplusplus
extern "C" {
#endif
#ifndef FR_INIT_GUARD
#define FR_INIT_GUARD

void c_preinit(void);
void c_init(void);
void cpp_preinit(void);
void cpp_init(void);
void zig_preinit(void);
void zig_init(void);

#endif
#ifdef __cplusplus
}
#endif

