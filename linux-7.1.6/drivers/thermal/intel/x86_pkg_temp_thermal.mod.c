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
	{ 0x7dc80830, "thermal_zone_device_priv" },
	{ 0xdb0da161, "intel_tcc_get_temp" },
	{ 0x90de0452, "platform_thermal_package_notify" },
	{ 0x8a7cb9c4, "platform_thermal_package_rate_control" },
	{ 0x4f15eb97, "__cpuhp_remove_state" },
	{ 0x037a0cba, "kfree" },
	{ 0x6a5cc518, "__kmalloc_noprof" },
	{ 0x7712d0a9, "x86_match_cpu" },
	{ 0xbd48ea6a, "__max_dies_per_package" },
	{ 0x39081193, "__max_logical_packages" },
	{ 0x27a4ad5d, "__cpuhp_setup_state" },
	{ 0xa1186518, "intel_tcc_get_tjmax" },
	{ 0x4efb655f, "cpu_info" },
	{ 0xffeedf6a, "delayed_work_timer_fn" },
	{ 0xf9ddb5d9, "timer_init_key" },
	{ 0x6dd966e3, "thermal_zone_device_register_with_trips" },
	{ 0x83a6a894, "thermal_zone_device_enable" },
	{ 0x6bf63930, "thermal_zone_device_unregister" },
	{ 0x19d798b9, "__preempt_count" },
	{ 0xd6742002, "preempt_schedule_thunk" },
	{ 0x14231a68, "mutex_lock" },
	{ 0x5ecea35b, "mutex_unlock" },
	{ 0xf5f214b9, "thermal_clear_package_intr_status" },
	{ 0xf017c1a0, "thermal_zone_device_update" },
	{ 0x7f02188f, "__msecs_to_jiffies" },
	{ 0xb6c4379f, "system_percpu_wq" },
	{ 0xb2fcb56d, "queue_delayed_work_on" },
	{ 0x9fa7184a, "cancel_delayed_work_sync" },
	{ 0x8d83f57a, "param_ops_int" },
	{ 0xa78e4b0d, "module_layout" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("cpu:type:x86,ven0000fam*mod*:feature:*01C6*");
