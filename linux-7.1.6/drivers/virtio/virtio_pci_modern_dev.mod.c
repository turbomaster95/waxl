#include <linux/module.h>
#include <linux/export-internal.h>
#include <linux/compiler.h>

MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.arch = MODULE_ARCH_INIT,
};

MODULE_INFO(intree, "Y");

KSYMTAB_FUNC(vp_modern_probe, "");
SYMBOL_FLAGS(vp_modern_probe, 0x01);
KSYMTAB_FUNC(vp_modern_remove, "");
SYMBOL_FLAGS(vp_modern_remove, 0x01);
KSYMTAB_FUNC(vp_modern_get_extended_features, "");
SYMBOL_FLAGS(vp_modern_get_extended_features, 0x01);
KSYMTAB_FUNC(vp_modern_get_driver_extended_features, "");
SYMBOL_FLAGS(vp_modern_get_driver_extended_features, 0x01);
KSYMTAB_FUNC(vp_modern_set_extended_features, "");
SYMBOL_FLAGS(vp_modern_set_extended_features, 0x01);
KSYMTAB_FUNC(vp_modern_generation, "");
SYMBOL_FLAGS(vp_modern_generation, 0x01);
KSYMTAB_FUNC(vp_modern_get_status, "");
SYMBOL_FLAGS(vp_modern_get_status, 0x01);
KSYMTAB_FUNC(vp_modern_set_status, "");
SYMBOL_FLAGS(vp_modern_set_status, 0x01);
KSYMTAB_FUNC(vp_modern_get_queue_reset, "");
SYMBOL_FLAGS(vp_modern_get_queue_reset, 0x01);
KSYMTAB_FUNC(vp_modern_set_queue_reset, "");
SYMBOL_FLAGS(vp_modern_set_queue_reset, 0x01);
KSYMTAB_FUNC(vp_modern_queue_vector, "");
SYMBOL_FLAGS(vp_modern_queue_vector, 0x01);
KSYMTAB_FUNC(vp_modern_config_vector, "");
SYMBOL_FLAGS(vp_modern_config_vector, 0x01);
KSYMTAB_FUNC(vp_modern_queue_address, "");
SYMBOL_FLAGS(vp_modern_queue_address, 0x01);
KSYMTAB_FUNC(vp_modern_set_queue_enable, "");
SYMBOL_FLAGS(vp_modern_set_queue_enable, 0x01);
KSYMTAB_FUNC(vp_modern_get_queue_enable, "");
SYMBOL_FLAGS(vp_modern_get_queue_enable, 0x01);
KSYMTAB_FUNC(vp_modern_set_queue_size, "");
SYMBOL_FLAGS(vp_modern_set_queue_size, 0x01);
KSYMTAB_FUNC(vp_modern_get_queue_size, "");
SYMBOL_FLAGS(vp_modern_get_queue_size, 0x01);
KSYMTAB_FUNC(vp_modern_get_num_queues, "");
SYMBOL_FLAGS(vp_modern_get_num_queues, 0x01);
KSYMTAB_FUNC(vp_modern_map_vq_notify, "");
SYMBOL_FLAGS(vp_modern_map_vq_notify, 0x01);
KSYMTAB_FUNC(vp_modern_avq_num, "");
SYMBOL_FLAGS(vp_modern_avq_num, 0x01);
KSYMTAB_FUNC(vp_modern_avq_index, "");
SYMBOL_FLAGS(vp_modern_avq_index, 0x01);

SYMBOL_CRC(vp_modern_probe, 0x718c1407);
SYMBOL_CRC(vp_modern_remove, 0x4e32be3d);
SYMBOL_CRC(vp_modern_get_extended_features, 0x2d4b423b);
SYMBOL_CRC(vp_modern_get_driver_extended_features, 0x37815758);
SYMBOL_CRC(vp_modern_set_extended_features, 0xa14e59a5);
SYMBOL_CRC(vp_modern_generation, 0x8cdba2fe);
SYMBOL_CRC(vp_modern_get_status, 0x7cce6be5);
SYMBOL_CRC(vp_modern_set_status, 0x0881a36a);
SYMBOL_CRC(vp_modern_get_queue_reset, 0x681480a3);
SYMBOL_CRC(vp_modern_set_queue_reset, 0xe103ea8e);
SYMBOL_CRC(vp_modern_queue_vector, 0x7ea35e03);
SYMBOL_CRC(vp_modern_config_vector, 0xe05fb8ba);
SYMBOL_CRC(vp_modern_queue_address, 0x6c831679);
SYMBOL_CRC(vp_modern_set_queue_enable, 0x27fdf370);
SYMBOL_CRC(vp_modern_get_queue_enable, 0xea520ebc);
SYMBOL_CRC(vp_modern_set_queue_size, 0xa1721dc1);
SYMBOL_CRC(vp_modern_get_queue_size, 0x84194c34);
SYMBOL_CRC(vp_modern_get_num_queues, 0xfd314cde);
SYMBOL_CRC(vp_modern_map_vq_notify, 0x294adea0);
SYMBOL_CRC(vp_modern_avq_num, 0x03a59815);
SYMBOL_CRC(vp_modern_avq_index, 0x68993bb6);

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xbb63fac7, "pci_iounmap" },
	{ 0x4ed1c44f, "pci_release_selected_regions" },
	{ 0xf4d4a49f, "pci_read_config_byte" },
	{ 0x1a2620f7, "pci_read_config_dword" },
	{ 0x0822e936, "_dev_err" },
	{ 0xf75bb31d, "pci_iomap_range" },
	{ 0x4a453f53, "iowrite32" },
	{ 0xa78af5f3, "ioread32" },
	{ 0x0b19b445, "ioread8" },
	{ 0x848d372e, "iowrite8" },
	{ 0x06a86bc1, "iowrite16" },
	{ 0x842c8e9d, "ioread16" },
	{ 0x7fc503f7, "_dev_warn" },
	{ 0xf9a482f9, "msleep" },
	{ 0x8f207018, "pci_find_capability" },
	{ 0xeb2ee9c3, "pci_find_next_capability" },
	{ 0x35ea8db8, "_dev_info" },
	{ 0xa0197905, "dma_set_mask" },
	{ 0x8882cbc5, "dma_set_coherent_mask" },
	{ 0x6c43a64b, "pci_request_selected_regions" },
	{ 0xa78e4b0d, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "882B45E07829A60E0153FDD");
