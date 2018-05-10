#include <api/parallel.h>

#include <api/os.h>

#include <stddef.h>
#include <memory.h>

static int run_until_limit(int count, coroutine_t* coros, int limit) {
	int living = count;
	uint32_t* filters = malloc(count * sizeof(int));
	event_t eventData;
	eventData.type = 0;
	while (true) {
		for (int n = 0; n < count; n++) {
			coroutine_t* coro = &coros[n];
			if (coroutine_status(coro) != COROUTINE_STATUS_DEAD) {
				if (filters[n] == NULL || filters[n] - 1 == eventData.type || eventData.type == EVENT_TERMINATE) {
					void* filter = coroutine_resume(coro, &eventData);
					filters[n] = filter;
					if (coroutine_status(coro) == COROUTINE_STATUS_DEAD) {
						--living;
						if (living <= limit) {
							return n;
						}
					}
				}
			}
		}
		eventData = os_pull_event_raw(NULL);
	}
	return 0;
}

int parallel_wait_for_any(int count, coroutine_t* coros) {
	return run_until_limit(count, coros, count - 1);
}

void parallel_wait_for_all(int count, coroutine_t* coros) {
	run_until_limit(count, coros, 0);
}
