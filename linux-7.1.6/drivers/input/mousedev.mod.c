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

MODULE_INFO(intree, "Y");



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x14231a68, "mutex_lock" },
	{ 0x5d4747ee, "input_close_device" },
	{ 0x5ecea35b, "mutex_unlock" },
	{ 0xf5ff6d29, "mutex_lock_interruptible" },
	{ 0x611ee486, "input_open_device" },
	{ 0x19d798b9, "__preempt_count" },
	{ 0xfdd6f6de, "kill_fasync" },
	{ 0xd6742002, "preempt_schedule_thunk" },
	{ 0xa8f7cb55, "__wake_up" },
	{ 0x167e7f9d, "__get_user_1" },
	{ 0xab7d5430, "fasync_helper" },
	{ 0x3ae01e1c, "put_device" },
	{ 0x037a0cba, "kfree" },
	{ 0xfe487975, "init_wait_entry" },
	{ 0x01000e51, "schedule" },
	{ 0x93f6bff1, "prepare_to_wait_event" },
	{ 0x963e0acd, "finish_wait" },
	{ 0x6b10bee1, "_copy_to_user" },
	{ 0x306501f2, "cdev_device_del" },
	{ 0x5cf53ce2, "input_free_minor" },
	{ 0x518cbbd9, "input_unregister_handle" },
	{ 0x18a4132d, "misc_deregister" },
	{ 0x10c10c88, "input_unregister_handler" },
	{ 0x6091797f, "synchronize_rcu" },
	{ 0xfb19cbc4, "kmalloc_caches" },
	{ 0x5dacb437, "__kmalloc_cache_noprof" },
	{ 0xae89e13e, "stream_open" },
	{ 0xc0ff21c1, "input_get_new_minor" },
	{ 0x92997ed8, "_printk" },
	{ 0x9436e710, "mutex_init_generic" },
	{ 0xddece99e, "__init_waitqueue_head" },
	{ 0x3dd95a3d, "dev_set_name" },
	{ 0x297e75e6, "get_device" },
	{ 0xa2e6d6a3, "input_class" },
	{ 0x9cf47ad6, "device_initialize" },
	{ 0xf0cf6b8e, "cdev_init" },
	{ 0x3b3fc543, "cdev_device_add" },
	{ 0x1882b319, "input_register_handle" },
	{ 0xc2ec28c5, "input_register_handler" },
	{ 0xdffadfd2, "misc_register" },
	{ 0x0da02d67, "jiffies" },
	{ 0x7f02188f, "__msecs_to_jiffies" },
	{ 0xcdb4a9e6, "noop_llseek" },
	{ 0xae90285e, "param_ops_uint" },
	{ 0xa78e4b0d, "module_layout" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("input:b*v*p*e*-e*1,*2,*k*110,*r*0,*1,*a*m*l*s*f*w*");
MODULE_ALIAS("input:b*v*p*e*-e*1,*2,*k*r*8,*a*m*l*s*f*w*");
MODULE_ALIAS("input:b*v*p*e*-e*1,*3,*k*14A,*r*a*0,*1,*m*l*s*f*w*");
MODULE_ALIAS("input:b*v*p*e*-e*1,*3,*k*145,*r*a*0,*1,*18,*1C,*m*l*s*f*w*");
MODULE_ALIAS("input:b*v*p*e*-e*1,*3,*k*110,*r*a*0,*1,*m*l*s*f*w*");
