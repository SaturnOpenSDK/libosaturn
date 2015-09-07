/*
 * Copyright (c) 2012-2016 Israel Jacquez
 * See LICENSE for details.
 *
 * Israel Jacquez <mrkotfw@gmail.com>
 */

#ifndef _FILE_H_
#define _FILE_H_

#include <inttypes.h>

#include <sys/vnode.h>

struct file {
        struct {
                int (*fo_read)(struct file *);
                int (*fo_poll)(struct file *);
                int (*fo_close)(struct file *);
        } f_ops;

        uint32_t f_offset; /* File offset in terms of bytes */
        void *f_data; /* Descriptor data */
};

#endif /* !_FILE_H_ */
