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

KSYMTAB_FUNC(virtio_pci_admin_has_dev_parts, "");
SYMBOL_FLAGS(virtio_pci_admin_has_dev_parts, 0x01);
KSYMTAB_FUNC(virtio_pci_admin_mode_set, "");
SYMBOL_FLAGS(virtio_pci_admin_mode_set, 0x01);
KSYMTAB_FUNC(virtio_pci_admin_obj_create, "");
SYMBOL_FLAGS(virtio_pci_admin_obj_create, 0x01);
KSYMTAB_FUNC(virtio_pci_admin_obj_destroy, "");
SYMBOL_FLAGS(virtio_pci_admin_obj_destroy, 0x01);
KSYMTAB_FUNC(virtio_pci_admin_dev_parts_metadata_get, "");
SYMBOL_FLAGS(virtio_pci_admin_dev_parts_metadata_get, 0x01);
KSYMTAB_FUNC(virtio_pci_admin_dev_parts_get, "");
SYMBOL_FLAGS(virtio_pci_admin_dev_parts_get, 0x01);
KSYMTAB_FUNC(virtio_pci_admin_dev_parts_set, "");
SYMBOL_FLAGS(virtio_pci_admin_dev_parts_set, 0x01);
KSYMTAB_FUNC(virtio_pci_admin_has_legacy_io, "");
SYMBOL_FLAGS(virtio_pci_admin_has_legacy_io, 0x01);
KSYMTAB_FUNC(virtio_pci_admin_legacy_common_io_write, "");
SYMBOL_FLAGS(virtio_pci_admin_legacy_common_io_write, 0x01);
KSYMTAB_FUNC(virtio_pci_admin_legacy_device_io_write, "");
SYMBOL_FLAGS(virtio_pci_admin_legacy_device_io_write, 0x01);
KSYMTAB_FUNC(virtio_pci_admin_legacy_device_io_read, "");
SYMBOL_FLAGS(virtio_pci_admin_legacy_device_io_read, 0x01);
KSYMTAB_FUNC(virtio_pci_admin_legacy_common_io_read, "");
SYMBOL_FLAGS(virtio_pci_admin_legacy_common_io_read, 0x01);
KSYMTAB_FUNC(virtio_pci_admin_legacy_io_notify_info, "");
SYMBOL_FLAGS(virtio_pci_admin_legacy_io_notify_info, 0x01);

SYMBOL_CRC(virtio_pci_admin_has_dev_parts, 0x7f4547cf);
SYMBOL_CRC(virtio_pci_admin_mode_set, 0xb47d8df1);
SYMBOL_CRC(virtio_pci_admin_obj_create, 0x7923a3c7);
SYMBOL_CRC(virtio_pci_admin_obj_destroy, 0x5c6ea42a);
SYMBOL_CRC(virtio_pci_admin_dev_parts_metadata_get, 0x64d89e2b);
SYMBOL_CRC(virtio_pci_admin_dev_parts_get, 0x50976a1b);
SYMBOL_CRC(virtio_pci_admin_dev_parts_set, 0x141f4fbd);
SYMBOL_CRC(virtio_pci_admin_has_legacy_io, 0xefc4b305);
SYMBOL_CRC(virtio_pci_admin_legacy_common_io_write, 0xc517dbc2);
SYMBOL_CRC(virtio_pci_admin_legacy_device_io_write, 0x16ae5c78);
SYMBOL_CRC(virtio_pci_admin_legacy_device_io_read, 0xadc8aa78);
SYMBOL_CRC(virtio_pci_admin_legacy_common_io_read, 0xc2bbeb1f);
SYMBOL_CRC(virtio_pci_admin_legacy_io_notify_info, 0x32569b91);

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xc1514a3b, "free_irq" },
	{ 0xa78af5f3, "ioread32" },
	{ 0xe150d7b5, "vring_transport_features" },
	{ 0x990e6260, "pci_find_ext_capability" },
	{ 0x6a87982e, "virtqueue_enable_cb" },
	{ 0x7cce6be5, "vp_modern_get_status" },
	{ 0x6786e75e, "pci_enable_device" },
	{ 0x681480a3, "vp_modern_get_queue_reset" },
	{ 0x294adea0, "vp_modern_map_vq_notify" },
	{ 0xa53f20df, "vp_legacy_config_vector" },
	{ 0x4a453f53, "iowrite32" },
	{ 0x2684d82d, "virtqueue_detach_unused_buf" },
	{ 0x6a5cc518, "__kmalloc_noprof" },
	{ 0x0cb72472, "virtqueue_is_broken" },
	{ 0xf7620863, "virtqueue_get_avail_addr" },
	{ 0x84194c34, "vp_modern_get_queue_size" },
	{ 0x656e4a6e, "snprintf" },
	{ 0x9cbcaf26, "complete" },
	{ 0x95506ac6, "vp_legacy_remove" },
	{ 0xf9dd9922, "__init_swait_queue_head" },
	{ 0xae69e101, "vp_legacy_set_queue_address" },
	{ 0x4e32be3d, "vp_modern_remove" },
	{ 0x25ebf024, "vp_legacy_get_status" },
	{ 0x842c8e9d, "ioread16" },
	{ 0xf4944dc5, "__pci_register_driver" },
	{ 0xea0985dd, "vring_create_virtqueue" },
	{ 0x037a0cba, "kfree" },
	{ 0xb320cc0e, "sg_init_one" },
	{ 0x06a86bc1, "iowrite16" },
	{ 0x0881a36a, "vp_modern_set_status" },
	{ 0x3390b00e, "pci_irq_vector" },
	{ 0x96af2530, "get_device" },
	{ 0x03a59815, "vp_modern_avq_num" },
	{ 0x47934c12, "pci_unregister_driver" },
	{ 0xa891a13d, "vring_notification_data" },
	{ 0x6c831679, "vp_modern_queue_address" },
	{ 0x1a2620f7, "pci_read_config_dword" },
	{ 0xff05fa13, "vring_interrupt" },
	{ 0x848d372e, "iowrite8" },
	{ 0x68993bb6, "vp_modern_avq_index" },
	{ 0x9c99819b, "put_device" },
	{ 0xc05a7e07, "virtio_config_changed" },
	{ 0x0cbbba7c, "vp_legacy_set_status" },
	{ 0x2d4b423b, "vp_modern_get_extended_features" },
	{ 0xea520ebc, "vp_modern_get_queue_enable" },
	{ 0xe05fb8ba, "vp_modern_config_vector" },
	{ 0xbfcb8eb4, "virtqueue_get_vring_size" },
	{ 0x718c1407, "vp_modern_probe" },
	{ 0x8f207018, "pci_find_capability" },
	{ 0xe523ad75, "synchronize_irq" },
	{ 0xeda37a4d, "unregister_virtio_device" },
	{ 0x3f91da70, "vp_legacy_queue_vector" },
	{ 0x8e0b62bd, "virtqueue_get_desc_addr" },
	{ 0x0822e936, "_dev_err" },
	{ 0x92d5838e, "request_threaded_irq" },
	{ 0xa14e59a5, "vp_modern_set_extended_features" },
	{ 0x47e913e2, "vp_legacy_get_queue_enable" },
	{ 0xa1721dc1, "vp_modern_set_queue_size" },
	{ 0x3a1b2e54, "vp_legacy_probe" },
	{ 0xeffc8e1e, "pci_device_is_present" },
	{ 0x18dfe9fa, "virtqueue_disable_cb" },
	{ 0x7ea35e03, "vp_modern_queue_vector" },
	{ 0xe103ea8e, "vp_modern_set_queue_reset" },
	{ 0x8cdba2fe, "vp_modern_generation" },
	{ 0xbb63fac7, "pci_iounmap" },
	{ 0x94ea071e, "virtqueue_add_sgs" },
	{ 0xbe094d61, "virtio_device_reset_done" },
	{ 0xd6742002, "preempt_schedule_thunk" },
	{ 0x0b19b445, "ioread8" },
	{ 0x7fc503f7, "_dev_warn" },
	{ 0x1e5b32cf, "pci_alloc_irq_vectors_affinity" },
	{ 0x70951371, "virtio_break_device" },
	{ 0xe4e2aede, "pci_set_master" },
	{ 0xd49bd803, "wait_for_completion" },
	{ 0xedaa2e26, "vp_legacy_set_features" },
	{ 0x680950b9, "virtqueue_get_buf" },
	{ 0x3c3ff9fd, "sprintf" },
	{ 0xeb2ee9c3, "pci_find_next_capability" },
	{ 0x19d798b9, "__preempt_count" },
	{ 0xc25903b0, "virtqueue_get_used_addr" },
	{ 0x152426bc, "virtqueue_kick" },
	{ 0xbef427c9, "param_ops_bool" },
	{ 0x437db427, "virtio_device_reset_prepare" },
	{ 0xc016d853, "__kmalloc_cache_noprof" },
	{ 0x7693f61c, "register_virtio_device" },
	{ 0xb924ca47, "vp_legacy_get_queue_size" },
	{ 0x87228621, "vp_legacy_get_features" },
	{ 0xac2031d0, "pci_disable_device" },
	{ 0x76729ac5, "pci_irq_get_affinity" },
	{ 0xf4d4a49f, "pci_read_config_byte" },
	{ 0xfd314cde, "vp_modern_get_num_queues" },
	{ 0x9f10d1c0, "pci_free_irq_vectors" },
	{ 0x27fdf370, "vp_modern_set_queue_enable" },
	{ 0x969f179f, "vring_del_virtqueue" },
	{ 0xf9a482f9, "msleep" },
	{ 0xce09e194, "kmalloc_caches" },
	{ 0xa78e4b0d, "module_layout" },
};

MODULE_INFO(depends, "virtio_ring,virtio_pci_modern_dev,virtio_pci_legacy_dev,virtio");

MODULE_ALIAS("pci:v00001AF4d*sv*sd*bc*sc*i*");

MODULE_INFO(srcversion, "CB894C6F22F2C3BDBFEDE7B");
