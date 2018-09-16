# Processes in TomatOS

In TomatOS there are two types of processes, foreground and background, with the main differences are:
* Foreground processes have access to the Term syscalls 
* Background processes will not get any user input events (like key, char and so on)

there is also a core process/service called 'alive', it always have uid of 0 and runs under root.
That process basically manages all the processes running, unlike most processes it is running directly from the kernel.

the scheduling goes like this:
* If a process wants to pull an event and the event queue is empty, the process will be set to SUSPENDED
* Once every 10ms the scheduler will check if SUSPENDED processes have events in their queue, and if they do it will set them to NORMAL
* if there is a running process:
    * Check if the process ran for atleast `(1000 / process_count) + (priority * 100)` (higher priority, more execution time)
    * If it did run for that long, find a process to replace it
* If there are no running processes (for example the process was killed or is waiting for event)
    * Find process to replace it
* Scheduling score is calculated with `suspended_time + 100 * (priority + 1)`
* When a process is running its status is set to RUNNING
