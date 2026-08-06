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

KSYMTAB_FUNC(virtio_check_driver_offered_feature, "");
SYMBOL_FLAGS(virtio_check_driver_offered_feature, 0x01);
KSYMTAB_FUNC(virtio_config_changed, "");
SYMBOL_FLAGS(virtio_config_changed, 0x01);
KSYMTAB_FUNC(virtio_config_driver_disable, "");
SYMBOL_FLAGS(virtio_config_driver_disable, 0x01);
KSYMTAB_FUNC(virtio_config_driver_enable, "");
SYMBOL_FLAGS(virtio_config_driver_enable, 0x01);
KSYMTAB_FUNC(virtio_add_status, "");
SYMBOL_FLAGS(virtio_add_status, 0x01);
KSYMTAB_FUNC(virtio_reset_device, "");
SYMBOL_FLAGS(virtio_reset_device, 0x01);
KSYMTAB_FUNC(__register_virtio_driver, "");
SYMBOL_FLAGS(__register_virtio_driver, 0x01);
KSYMTAB_FUNC(unregister_virtio_driver, "");
SYMBOL_FLAGS(unregister_virtio_driver, 0x01);
KSYMTAB_FUNC(register_virtio_device, "");
SYMBOL_FLAGS(register_virtio_device, 0x01);
KSYMTAB_FUNC(is_virtio_device, "");
SYMBOL_FLAGS(is_virtio_device, 0x01);
KSYMTAB_FUNC(unregister_virtio_device, "");
SYMBOL_FLAGS(unregister_virtio_device, 0x01);
KSYMTAB_FUNC(virtio_device_reset_prepare, "");
SYMBOL_FLAGS(virtio_device_reset_prepare, 0x01);
KSYMTAB_FUNC(virtio_device_reset_done, "");
SYMBOL_FLAGS(virtio_device_reset_done, 0x01);

SYMBOL_CRC(virtio_check_driver_offered_feature, 0x071cb357);
SYMBOL_CRC(virtio_config_changed, 0xc05a7e07);
SYMBOL_CRC(virtio_config_driver_disable, 0x197282ca);
SYMBOL_CRC(virtio_config_driver_enable, 0x08f37f9c);
SYMBOL_CRC(virtio_add_status, 0xd9535614);
SYMBOL_CRC(virtio_reset_device, 0xfc802eac);
SYMBOL_CRC(__register_virtio_driver, 0xbd733b91);
SYMBOL_CRC(unregister_virtio_driver, 0xc71f3d6f);
SYMBOL_CRC(register_virtio_device, 0x7693f61c);
SYMBOL_CRC(is_virtio_device, 0x379dc1a7);
SYMBOL_CRC(unregister_virtio_device, 0xeda37a4d);
SYMBOL_CRC(virtio_device_reset_prepare, 0x437db427);
SYMBOL_CRC(virtio_device_reset_done, 0xbe094d61);

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x19d798b9, "__preempt_count" },
	{ 0xd6742002, "preempt_schedule_thunk" },
	{ 0x5b7d6f6f, "driver_register" },
	{ 0x70951371, "virtio_break_device" },
	{ 0x6091797f, "synchronize_rcu" },
	{ 0x6829382b, "virtio_check_mem_acc_cb" },
	{ 0x7fc503f7, "_dev_warn" },
	{ 0x0822e936, "_dev_err" },
	{ 0x9c6febfc, "add_uevent_var" },
	{ 0x99f2d00a, "sysfs_emit_at" },
	{ 0xe783e261, "sysfs_emit" },
	{ 0xa5f885c6, "driver_unregister" },
	{ 0x904f4458, "device_unregister" },
	{ 0xcc031c39, "ida_free" },
	{ 0xbb59a756, "bus_register" },
	{ 0x62eec1d5, "panic" },
	{ 0xc30c57cd, "bus_unregister" },
	{ 0x551cf2a8, "ida_destroy" },
	{ 0x2f93d3b7, "device_initialize" },
	{ 0x1b8f87fc, "ida_alloc_range" },
	{ 0x54178729, "dev_set_name" },
	{ 0x514f5a69, "device_add" },
	{ 0xa78e4b0d, "module_layout" },
};

MODULE_INFO(depends, "virtio_ring");

