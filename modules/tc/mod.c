#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/fs_context.h>
#include <linux/pagemap.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Deva");
MODULE_DESCRIPTION("Modern Custom Pseudo-Filesystem");

#define MYFS_MAGIC 0x13371337

/* Superblock operations for our pseudo-fs */
static const struct super_operations myfs_ops = {
    .statfs = simple_statfs,
};

/* Read callback for /status */
static ssize_t status_read(struct file *filp, char __user *buf, size_t count, loff_t *ppos) {
    char msg[64];
    int len;

    len = snprintf(msg, sizeof(msg), "myfs status: ACTIVE | uptime jiffies: %lu\n", jiffies);
    return simple_read_from_buffer(buf, count, ppos, msg, len);
}

static const struct file_operations status_fops = {
    .read = status_read,
};

/* Populate root directory of our filesystem */
static int myfs_fill_super(struct super_block *sb, struct fs_context *fc) {
    struct inode *root_inode;
    struct inode *status_inode;
    struct dentry *root_dentry;
    struct dentry *status_dentry;

    sb->s_blocksize = PAGE_SIZE;
    sb->s_blocksize_bits = PAGE_SHIFT;
    sb->s_magic = MYFS_MAGIC;
    sb->s_op = &myfs_ops;

    /* 1. Create Root Directory Inode */
    root_inode = new_inode(sb);
    if (!root_inode) return -ENOMEM;

    root_inode->i_ino = 1;
    root_inode->i_mode = S_IFDIR | 0755;
    simple_inode_init_ts(root_inode);
    root_inode->i_op = &simple_dir_inode_operations;
    root_inode->i_fop = &simple_dir_operations;

    root_dentry = d_make_root(root_inode);
    if (!root_dentry) return -ENOMEM;
    sb->s_root = root_dentry;

    /* 2. Create '/status' File inside Root */
    status_dentry = d_alloc_name(root_dentry, "status");
    if (!status_dentry) return -ENOMEM;

    status_inode = new_inode(sb);
    if (!status_inode) return -ENOMEM;

    status_inode->i_ino = 2;
    status_inode->i_mode = S_IFREG | 0444; /* Read-only */
    simple_inode_init_ts(status_inode);
    status_inode->i_fop = &status_fops;

    d_add(status_dentry, status_inode);
    return 0;
}

/* Modern fs_context get_tree hook */
static int myfs_get_tree(struct fs_context *fc) {
    return get_tree_nodev(fc, myfs_fill_super);
}

static const struct fs_context_operations myfs_context_ops = {
    .get_tree = myfs_get_tree,
};

/* Initialize fs context */
static int myfs_init_fs_context(struct fs_context *fc) {
    fc->ops = &myfs_context_ops;
    return 0;
}

static struct file_system_type myfs_type = {
    .owner           = THIS_MODULE,
    .name            = "myfs",
    .init_fs_context = myfs_init_fs_context,
    .kill_sb         = kill_anon_super,
};

static int __init myfs_init(void) {
    int ret = register_filesystem(&myfs_type);
    if (ret == 0)
        pr_info("myfs: Pseudo-filesystem registered successfully.\n");
    return ret;
}

static void __exit myfs_exit(void) {
    unregister_filesystem(&myfs_type);
    pr_info("myfs: Pseudo-filesystem unregistered.\n");
}

module_init(myfs_init);
module_exit(myfs_exit);
