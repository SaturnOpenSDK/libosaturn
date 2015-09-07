/*
 * Copyright (c) 2012-2016 Israel Jacquez
 * See LICENSE for details.
 *
 * Israel Jacquez <mrkotfw@gmail.com>
 */

#ifndef _FD_H_
#define _FD_H_

#include <inttypes.h>

#include <sys/file.h>

#define FD_MAX  16 /* Maximum number of file descriptors in system */

void *fd_alloc(const struct file *);
int fd_free(const void *);

#endif /* _FD_H_ */
