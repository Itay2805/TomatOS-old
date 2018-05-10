#ifndef API_PARALLEL
#define API_PARALLEL

#include <api/coroutine.h>

int parallel_wait_for_any(int count, coroutine_t* coros);
void parallel_wait_for_all(int count, coroutine_t* coros);

#endif
