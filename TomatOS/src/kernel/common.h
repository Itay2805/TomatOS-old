
#ifdef _WIN32
#define PACKED
#define ASM(x)
#else
#define PACKED __attribute__((packed))
#define ASM(x) x
#endif
