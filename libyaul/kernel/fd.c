/*
 * Copyright (c) 2012-2016 Israel Jacquez
 * See LICENSE for details.
 *
 * Israel Jacquez <mrkotfw@gmail.com>
 */

#include <stdlib.h>

#include <sys/fd.h>
#include <sys/file.h>

static struct {
        uint32_t fdt_allocated; /* Number of file descriptors allocated */
        int32_t fdt_next; /* Next available file descriptor */

        struct {
                bool fdte_used;
                struct file fdte_file;
        } fdt_entries[FD_COUNT_MAX];
} file_descriptor_table = {
        .fdt_allocated = 0,
        .fdt_next = -1,
        .fdt_entries = {
                .fdte_used = false,
                .fdte_used = false,
                .fdte_used = false,
                .fdte_used = false,
                .fdte_used = false,
                .fdte_used = false,
                .fdte_used = false,
                .fdte_used = false,
                .fdte_used = false,
                .fdte_used = false,
                .fdte_used = false,
                .fdte_used = false,
                .fdte_used = false,
                .fdte_used = false,
                .fdte_used = false,
                .fdte_used = false
        }
};

/*
 * Allocate a file descriptor.
 *
 * If successful, a pointer to a file descriptor is returned. Otherwise,
 * NULL is returned for the following cases:
 *
 *   - There are too many files open (not enough file descriptors) (EMFILE);
 *   - The pointer to file structure is invalid (EFAULT); or
 *   - The pointer to file structure has not been properly initialized
 */
uint32_t
fd_alloc(void)
{
        /* XXX: errno = */

        if (file_descriptor_table.fdt_allocated == FD_COUNT_MAX) {
                /* There are no more file descriptors left */
                /* XXX: errno = EMFILE */
                return -1;
        }

        uint32_t fd;
        fd = file_descriptor_table.fdt_next;
        if (fd < 0) {
                for (fd = 0; fd < FD_COUNT_MAX; fd++) {
                        if (!file_descriptor_table.fdt_entries[fd].ftde_used) {
                                break;
                        }
                }
        }

        file_descriptor_table.fdt_entries[fd].ftde_used = true;
        file_descriptor_table.fdt_allocated++;

        return fd;
}

/*
 * Free a valid file descriptor.
 *
 * If successful, 0 is returned. Otherwise a -1 is returned for the
 * following cases:
 *
 *   - Bad file descriptor (EBADF); or
 *   - File descriptor is in a bad state (EBADFD)
 */
int32_t
fd_free(int32_t fd)
{
        /* XXX: errno */

        if ((fd < 0) && (fd >= FD_COUNT_MAX)) {
                /* XXX: errno = EBADF */
                return -1;
        }

        if (!file_descriptor_table.fdt_entries[fd].ftde_used) {
                /* XXX: errno = */
                return -1;
        }

        file_descriptor_table.fdt_next = fd;

        file_descriptor_table.fdt_entries[fd].ftde_used = false;
        file_descriptor_table.fdt_allocated--;

        return 0;
}

/*
 * Retrieve file structure from file descriptor.
 *
 * If successful, a pointer to file structure is returned. Otherwise
 * NULL is returned for the following cases:
 *
 *   - Bad file descriptor (EBADF); or
 *   - File descriptor is in a bad state (EBADFD)
 */
struct file *
fd_file(int32_t fd)
{
        /* XXX: errno */

        if ((fd < 0) && (fd >= FD_COUNT_MAX)) {
                /* XXX: errno = EBADF */
                return NULL;
        }

        if (!file_descriptor_table.fdt_entries[fd].ftde_used) {
                /* XXX: errno = */
                return NULL;
        }

        return &file_descriptor_table.fdt_entries[fd].ftde_file;
}
