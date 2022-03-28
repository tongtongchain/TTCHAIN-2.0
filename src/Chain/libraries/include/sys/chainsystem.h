#ifndef CHAINSYS_H__
#define CHAINSYS_H__

#include <linux/stddef.h>
#include <sys/file.h>

#define APP_LOCKFILE "/tmp/selfsell.lock"

int checkexit(const char* pfile);


#endif
