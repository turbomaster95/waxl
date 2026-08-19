#include <linux/module.h>
#include <linux/export-internal.h>
#include <linux/compiler.h>

MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xf11e289f, "register_filesystem" },
	{ 0x92997ed8, "_printk" },
	{ 0xe7561589, "get_tree_nodev" },
	{ 0x92eec939, "new_inode" },
	{ 0x782d76ab, "simple_inode_init_ts" },
	{ 0x67d45892, "simple_dir_inode_operations" },
	{ 0x804b2469, "simple_dir_operations" },
	{ 0x81f22829, "d_make_root" },
	{ 0x9c857d2b, "d_alloc_name" },
	{ 0xd997b10c, "d_add" },
	{ 0x0da02d67, "jiffies" },
	{ 0x656e4a6e, "snprintf" },
	{ 0x619cb7dd, "simple_read_from_buffer" },
	{ 0xbfc44ec2, "unregister_filesystem" },
	{ 0x00626420, "kill_anon_super" },
	{ 0x6d424e92, "simple_statfs" },
	{ 0xe59c9293, "module_layout" },
};

MODULE_INFO(depends, "");

