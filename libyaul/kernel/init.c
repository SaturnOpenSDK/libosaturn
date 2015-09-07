/*
 * Copyright (c) 2012-2016 Israel Jacquez
 * See LICENSE for details.
 *
 * Israel Jacquez <mrkotfw@gmail.com>
 */

void fd_init(void);
void mm_init(void);
void vfs_init(void);

/**
 * Initialize the kernel.
 */
void
init(void)
{
        mm_init();
        fd_init();
        vfs_init();
}
