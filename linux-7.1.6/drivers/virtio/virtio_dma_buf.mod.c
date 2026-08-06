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

KSYMTAB_FUNC(virtio_dma_buf_export, "");
SYMBOL_FLAGS(virtio_dma_buf_export, 0x00);
KSYMTAB_FUNC(virtio_dma_buf_attach, "");
SYMBOL_FLAGS(virtio_dma_buf_attach, 0x00);
KSYMTAB_FUNC(is_virtio_dma_buf, "");
SYMBOL_FLAGS(is_virtio_dma_buf, 0x00);
KSYMTAB_FUNC(virtio_dma_buf_get_uuid, "");
SYMBOL_FLAGS(virtio_dma_buf_get_uuid, 0x00);

SYMBOL_CRC(virtio_dma_buf_export, 0x735137b7);
SYMBOL_CRC(virtio_dma_buf_attach, 0x709f9537);
SYMBOL_CRC(is_virtio_dma_buf, 0x3e103eb3);
SYMBOL_CRC(virtio_dma_buf_get_uuid, 0x76d57763);

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xe67c2861, "dma_buf_export" },
	{ 0xa78e4b0d, "module_layout" },
};

MODULE_INFO(depends, "");

