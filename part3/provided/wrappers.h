#ifndef __WRAPPERS_H
#define __WRAPPERS_H

#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>

#define __NR_START_ELEVATOR 353
#define __NR_ISSUE_REQUEST 354
#define __NR_STOP_ELEVATOR 355

int start_elevator() {
	return syscall(__NR_START_ELEVATOR);
}

int issue_request(int type, int start, int dest) {
	return syscall(__NR_ISSUE_REQUEST, type, start, dest);
}

int stop_elevator() {
	return syscall(__NR_STOP_ELEVATOR);
}

#endif
