#ifndef SYSCALL_TERM_H
#define SYSCALL_TERM_H

void syscall_term_init(void);

// used when the kernel wants to write something and needs
// to avoid a syscall
void term_kwrite(const char* text);
void term_kreset(void);

#endif
