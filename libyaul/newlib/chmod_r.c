#include <sys/reent.h>

#include <common.h>

int
_chmod_r(struct _reent *r __unused, const char *path __unused,
    short mode __unused)
{
        return 0;
}
