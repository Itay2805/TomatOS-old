#ifndef KERNEL_CORE_PROCESS_THREAD_H
#define KERNEL_CORE_PROCESS_THREAD_H

// threading library (will probably use coperative threading)

#include <stdbool.h>

typedef void(*thread_entry_t)();

typedef struct thread {
	thread_entry_t entry;
	const char* name;
	int uid;
} thread_t;

typedef struct thread_tasklist {
	thread_t* list;
	int last_uid;
	bool something_in_fg;
} thread_tasklist_t;

#ifdef __cplusplus
extern "C" {
#endif

	bool thread_new(thread_t* thread, thread_tasklist_t* tasklist);
	bool thread_resume_all(thread_tasklist_t* tasklist);

#ifdef __cplusplus
}
#endif

#endif