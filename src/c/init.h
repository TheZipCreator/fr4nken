#ifdef __cplusplus
extern "C" {
#endif
#ifndef FR_INIT_GUARD
#define FR_INIT_GUARD

void fr_preinit(void);
void fr_init(void);
void fr_deinit(void);

#define FUNCS(NAME) void NAME##_preinit(void); void NAME##_init(void); void NAME##_deinit(void)

FUNCS(c);
FUNCS(cpp);
FUNCS(zig);
FUNCS(nim);
void fr_sdlLoop(void);

#undef FUNCS

#endif
#ifdef __cplusplus
}
#endif

