/*
 * print number of free blocks in filesystem.
 * Use: statfs <path>
 * Example:
 *   statfs /flash
 */


#include <rtthread.h>
#include <dfs.h>
#include <dfs_fs.h>

static int print_statfs(char *path)
{
    int32_t                retval = -RT_ERROR;
    struct statfs          fs_stat;
    struct dfs_filesystem *fs;

    fs = dfs_filesystem_lookup(path);
    if (fs == NULL)
    {
        rt_kprintf("file %s not found\r\n", path);
        return -RT_ERROR;
    }

    retval = dfs_statfs(fs->path, &fs_stat);
    if (retval == 0)
    {
        uint32_t bsize  = fs_stat.f_bsize;
        uint32_t blocks = fs_stat.f_blocks;
        uint32_t bfree  = fs_stat.f_bfree;
        uint32_t bavail = fs_stat.f_bavail;
        rt_kprintf("path: %s block size: %d blocks: %d free: %d available: %d\r\n", fs->path, bsize, blocks, bfree, bavail);
    }
    else
        rt_kprintf("path: %s no stats\r\n", path);
    return retval;
}

static int cmd_statfs(int argc, char **argv)
{
    int32_t retval = -RT_ERROR;
    char   *path   = RT_NULL;

    if (argc < 2)
    {
        rt_kprintf("%s <path>\r\n", argv[0]);
    }
    else
    {
        print_statfs(argv[1]);
    }
    return 0;
}

MSH_CMD_EXPORT_ALIAS(cmd_statfs, statfs, statfs <path> filesystem stats);
