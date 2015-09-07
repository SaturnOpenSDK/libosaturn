/*
 * Copyright (c) 2012-2016 Israel Jacquez
 * See LICENSE for details.
 *
 * Israel Jacquez <mrkotfw@gmail.com>
 */

#include <sys/fd.h>
#include <sys/file.h>

#include <lib/memb.h>

/* There is only one process */
struct file_descriptor_entry {
        int32_t fde_id;
        const struct file *fde_file;
};

MEMB(file_descriptors, struct file_descriptor_entry, FD_MAX, 0);

/* File descriptor ID counter */
static uint32_t fd_unique = 0;

/*
 * Initialize file descriptor component.
 */
void
fd_init(void)
{
        fd_unique = 0;

        memb_init(&file_descriptors);
}

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
void *
fd_alloc(const struct file *file)
{
        /* Sanitize arguments */
        if (file == NULL) {
                /* XXX: errno = */
                return NULL;
        }

        /* Ensure all the file descriptor generic operations are
         * valid */
        if ((file->f_ops.fo_read == NULL) ||
            (file->f_ops.fo_poll == NULL) ||
            (file->f_ops.fo_close == NULL)) {
                return -1;
        }
        /* File offset in a new file descriptor must start at the 0th byte */
        if (file->f_offset != 0) {
                return -1;
        }

        struct file_descriptor_entry *fde;
        if ((fde = (struct file_descriptor_entry *)memb_alloc(
                        &file_descriptors)) == NULL) {
                /* There are no more file descriptors left */
                /* XXX: errno = EMFILE */
                return NULL;
        }

        fde->fde_id = ++fd_unique;
        fde->fde_file = file;

        /* XXX: errno = */

        return fde;
}

/*
 * Free a valid file descriptor.
 *
 * If successful, 0 is returned. Otherwise a -1 is returned for the
 * following cases:
 *
 *   - Bad file descriptor (EBADF);
 *   - File descriptor is in a bad state (EBADFD); or
 *   - Unable to free file descriptor (address not within bounds of the
 *     block pool).
 */
int
fd_free(const void *addr)
{
        if (addr == NULL) {
                /* XXX: errno = EBADF */
                return -1;
        }

        struct file_descriptor_entry *fde;
        fde = (struct file_descriptor_entry *)addr;

        if ((fde->fde_id < 0) && (fde->fde_id > fd_unique)) {
                /* XXX: errno = EBADF */
                return -1;
        }

        fde->fde_id = -1;
        fde->fde_file = NULL;

        if ((memb_free(&file_descriptors, fde)) < 0) {
                /* XXX: errno = */
                return -1;
        }

        /* XXX: errno = */

        return 0;
}
