/*
 * Copyright (c) 2012-2016 Israel Jacquez
 * See LICENSE for details.
 *
 * Israel Jacquez <mrkotfw@gmail.com>
 */

#ifndef _PATHNAME_NAME_H_
#define _PATHNAME_NAME_H_

#include <sys/vnode.h>
#include <sys/component_name.h>

void pathname_cache_init(void);
int pathname_cache_enter(const struct component_name *, struct vnode *);
int pathname_cache_lookup(const struct component_name *, struct vnode **);
int pathname_cache_evict(const struct component_name *);
void pathname_cache_evict_lru(void);

#endif /* !_PATHNAME_NAME_H_ */
