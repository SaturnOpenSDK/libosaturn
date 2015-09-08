/*
 * Copyright (c) 2012-2016 Israel Jacquez
 * See LICENSE for details.
 *
 * Israel Jacquez <mrkotfw@gmail.com>
 */

#ifndef _FD_H_
#define _FD_H_

#include <inttypes.h>

#define FD_COUNT_MAX 16 /* Maximum number of file descriptors in system */

int32_t fd_alloc(void);
int32_t fd_free(int32_t);
struct file *fd_file(int32_t);

#endif /* _FD_H_ */
