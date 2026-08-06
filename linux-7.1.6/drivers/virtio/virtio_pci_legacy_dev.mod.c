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

KSYMTAB_FUNC(vp_legacy_probe, "");
SYMBOL_FLAGS(vp_legacy_probe, 0x01);
KSYMTAB_FUNC(vp_legacy_remove, "");
SYMBOL_FLAGS(vp_legacy_remove, 0x01);
KSYMTAB_FUNC(vp_legacy_get_features, "");
SYMBOL_FLAGS(vp_legacy_get_features, 0x01);
KSYMTAB_FUNC(vp_legacy_get_driver_features, "");
SYMBOL_FLAGS(vp_legacy_get_driver_features, 0x01);
KSYMTAB_FUNC(vp_legacy_set_features, "");
SYMBOL_FLAGS(vp_legacy_set_features, 0x01);
KSYMTAB_FUNC(vp_legacy_get_status, "");
SYMBOL_FLAGS(vp_legacy_get_status, 0x01);
KSYMTAB_FUNC(vp_legacy_set_status, "");
SYMBOL_FLAGS(vp_legacy_set_status, 0x01);
KSYMTAB_FUNC(vp_legacy_queue_vector, "");
SYMBOL_FLAGS(vp_legacy_queue_vector, 0x01);
KSYMTAB_FUNC(vp_legacy_config_vector, "");
SYMBOL_FLAGS(vp_legacy_config_vector, 0x01);
KSYMTAB_FUNC(vp_legacy_set_queue_address, "");
SYMBOL_FLAGS(vp_legacy_set_queue_address, 0x01);
KSYMTAB_FUNC(vp_legacy_get_queue_enable, "");
SYMBOL_FLAGS(vp_legacy_get_queue_enable, 0x01);
KSYMTAB_FUNC(vp_legacy_get_queue_size, "");
SYMBOL_FLAGS(vp_legacy_get_queue_size, 0x01);

SYMBOL_CRC(vp_legacy_probe, 0x3a1b2e54);
SYMBOL_CRC(vp_legacy_remove, 0x95506ac6);
SYMBOL_CRC(vp_legacy_get_features, 0x87228621);
SYMBOL_CRC(vp_legacy_get_driver_features, 0x94c34080);
SYMBOL_CRC(vp_legacy_set_features, 0xedaa2e26);
SYMBOL_CRC(vp_legacy_get_status, 0x25ebf024);
SYMBOL_CRC(vp_legacy_set_status, 0x0cbbba7c);
SYMBOL_CRC(vp_legacy_queue_vector, 0x3f91da70);
SYMBOL_CRC(vp_legacy_config_vector, 0xa53f20df);
SYMBOL_CRC(vp_legacy_set_queue_address, 0xae69e101);
SYMBOL_CRC(vp_legacy_get_queue_enable, 0x47e913e2);
SYMBOL_CRC(vp_legacy_get_queue_size, 0xb924ca47);

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xbb63fac7, "pci_iounmap" },
	{ 0x50fc948c, "pci_release_region" },
	{ 0xa78af5f3, "ioread32" },
	{ 0x4a453f53, "iowrite32" },
	{ 0x0b19b445, "ioread8" },
	{ 0x848d372e, "iowrite8" },
	{ 0x06a86bc1, "iowrite16" },
	{ 0x842c8e9d, "ioread16" },
	{ 0xa0197905, "dma_set_mask" },
	{ 0x7fc503f7, "_dev_warn" },
	{ 0x8882cbc5, "dma_set_coherent_mask" },
	{ 0xee3dcea2, "pci_request_region" },
	{ 0x2a01da19, "pci_iomap" },
	{ 0xa78e4b0d, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "B166D1C71CE630F00545238");
