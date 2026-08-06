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

KSYMTAB_FUNC(virtio_max_dma_size, "");
SYMBOL_FLAGS(virtio_max_dma_size, 0x01);
KSYMTAB_FUNC(virtqueue_add_sgs, "");
SYMBOL_FLAGS(virtqueue_add_sgs, 0x01);
KSYMTAB_FUNC(virtqueue_add_outbuf, "");
SYMBOL_FLAGS(virtqueue_add_outbuf, 0x01);
KSYMTAB_FUNC(virtqueue_add_outbuf_premapped, "");
SYMBOL_FLAGS(virtqueue_add_outbuf_premapped, 0x01);
KSYMTAB_FUNC(virtqueue_add_inbuf, "");
SYMBOL_FLAGS(virtqueue_add_inbuf, 0x01);
KSYMTAB_FUNC(virtqueue_add_inbuf_cache_clean, "");
SYMBOL_FLAGS(virtqueue_add_inbuf_cache_clean, 0x01);
KSYMTAB_FUNC(virtqueue_add_inbuf_ctx, "");
SYMBOL_FLAGS(virtqueue_add_inbuf_ctx, 0x01);
KSYMTAB_FUNC(virtqueue_add_inbuf_premapped, "");
SYMBOL_FLAGS(virtqueue_add_inbuf_premapped, 0x01);
KSYMTAB_FUNC(virtqueue_dma_dev, "");
SYMBOL_FLAGS(virtqueue_dma_dev, 0x01);
KSYMTAB_FUNC(virtqueue_kick_prepare, "");
SYMBOL_FLAGS(virtqueue_kick_prepare, 0x01);
KSYMTAB_FUNC(virtqueue_notify, "");
SYMBOL_FLAGS(virtqueue_notify, 0x01);
KSYMTAB_FUNC(virtqueue_kick, "");
SYMBOL_FLAGS(virtqueue_kick, 0x01);
KSYMTAB_FUNC(virtqueue_get_buf_ctx, "");
SYMBOL_FLAGS(virtqueue_get_buf_ctx, 0x01);
KSYMTAB_FUNC(virtqueue_get_buf, "");
SYMBOL_FLAGS(virtqueue_get_buf, 0x01);
KSYMTAB_FUNC(virtqueue_disable_cb, "");
SYMBOL_FLAGS(virtqueue_disable_cb, 0x01);
KSYMTAB_FUNC(virtqueue_enable_cb_prepare, "");
SYMBOL_FLAGS(virtqueue_enable_cb_prepare, 0x01);
KSYMTAB_FUNC(virtqueue_poll, "");
SYMBOL_FLAGS(virtqueue_poll, 0x01);
KSYMTAB_FUNC(virtqueue_enable_cb, "");
SYMBOL_FLAGS(virtqueue_enable_cb, 0x01);
KSYMTAB_FUNC(virtqueue_enable_cb_delayed, "");
SYMBOL_FLAGS(virtqueue_enable_cb_delayed, 0x01);
KSYMTAB_FUNC(virtqueue_detach_unused_buf, "");
SYMBOL_FLAGS(virtqueue_detach_unused_buf, 0x01);
KSYMTAB_FUNC(vring_interrupt, "");
SYMBOL_FLAGS(vring_interrupt, 0x01);
KSYMTAB_FUNC(vring_create_virtqueue, "");
SYMBOL_FLAGS(vring_create_virtqueue, 0x01);
KSYMTAB_FUNC(vring_create_virtqueue_map, "");
SYMBOL_FLAGS(vring_create_virtqueue_map, 0x01);
KSYMTAB_FUNC(virtqueue_resize, "");
SYMBOL_FLAGS(virtqueue_resize, 0x01);
KSYMTAB_FUNC(virtqueue_reset, "");
SYMBOL_FLAGS(virtqueue_reset, 0x01);
KSYMTAB_FUNC(vring_new_virtqueue, "");
SYMBOL_FLAGS(vring_new_virtqueue, 0x01);
KSYMTAB_FUNC(vring_del_virtqueue, "");
SYMBOL_FLAGS(vring_del_virtqueue, 0x01);
KSYMTAB_FUNC(vring_notification_data, "");
SYMBOL_FLAGS(vring_notification_data, 0x01);
KSYMTAB_FUNC(vring_transport_features, "");
SYMBOL_FLAGS(vring_transport_features, 0x01);
KSYMTAB_FUNC(virtqueue_get_vring_size, "");
SYMBOL_FLAGS(virtqueue_get_vring_size, 0x01);
KSYMTAB_FUNC(__virtqueue_break, "");
SYMBOL_FLAGS(__virtqueue_break, 0x01);
KSYMTAB_FUNC(__virtqueue_unbreak, "");
SYMBOL_FLAGS(__virtqueue_unbreak, 0x01);
KSYMTAB_FUNC(virtqueue_is_broken, "");
SYMBOL_FLAGS(virtqueue_is_broken, 0x01);
KSYMTAB_FUNC(virtio_break_device, "");
SYMBOL_FLAGS(virtio_break_device, 0x01);
KSYMTAB_FUNC(__virtio_unbreak_device, "");
SYMBOL_FLAGS(__virtio_unbreak_device, 0x01);
KSYMTAB_FUNC(virtqueue_get_desc_addr, "");
SYMBOL_FLAGS(virtqueue_get_desc_addr, 0x01);
KSYMTAB_FUNC(virtqueue_get_avail_addr, "");
SYMBOL_FLAGS(virtqueue_get_avail_addr, 0x01);
KSYMTAB_FUNC(virtqueue_get_used_addr, "");
SYMBOL_FLAGS(virtqueue_get_used_addr, 0x01);
KSYMTAB_FUNC(virtqueue_get_vring, "");
SYMBOL_FLAGS(virtqueue_get_vring, 0x01);
KSYMTAB_FUNC(virtqueue_map_alloc_coherent, "");
SYMBOL_FLAGS(virtqueue_map_alloc_coherent, 0x01);
KSYMTAB_FUNC(virtqueue_map_free_coherent, "");
SYMBOL_FLAGS(virtqueue_map_free_coherent, 0x01);
KSYMTAB_FUNC(virtqueue_map_page_attrs, "");
SYMBOL_FLAGS(virtqueue_map_page_attrs, 0x01);
KSYMTAB_FUNC(virtqueue_unmap_page_attrs, "");
SYMBOL_FLAGS(virtqueue_unmap_page_attrs, 0x01);
KSYMTAB_FUNC(virtqueue_map_single_attrs, "");
SYMBOL_FLAGS(virtqueue_map_single_attrs, 0x01);
KSYMTAB_FUNC(virtqueue_unmap_single_attrs, "");
SYMBOL_FLAGS(virtqueue_unmap_single_attrs, 0x01);
KSYMTAB_FUNC(virtqueue_map_mapping_error, "");
SYMBOL_FLAGS(virtqueue_map_mapping_error, 0x01);
KSYMTAB_FUNC(virtqueue_map_need_sync, "");
SYMBOL_FLAGS(virtqueue_map_need_sync, 0x01);
KSYMTAB_FUNC(virtqueue_map_sync_single_range_for_cpu, "");
SYMBOL_FLAGS(virtqueue_map_sync_single_range_for_cpu, 0x01);
KSYMTAB_FUNC(virtqueue_map_sync_single_range_for_device, "");
SYMBOL_FLAGS(virtqueue_map_sync_single_range_for_device, 0x01);

SYMBOL_CRC(virtio_max_dma_size, 0x67a38d6b);
SYMBOL_CRC(virtqueue_add_sgs, 0x94ea071e);
SYMBOL_CRC(virtqueue_add_outbuf, 0x33aad9b3);
SYMBOL_CRC(virtqueue_add_outbuf_premapped, 0x0519d7f1);
SYMBOL_CRC(virtqueue_add_inbuf, 0xfd37d513);
SYMBOL_CRC(virtqueue_add_inbuf_cache_clean, 0xd0078f63);
SYMBOL_CRC(virtqueue_add_inbuf_ctx, 0xe0bd646a);
SYMBOL_CRC(virtqueue_add_inbuf_premapped, 0x7e878774);
SYMBOL_CRC(virtqueue_dma_dev, 0x153fb90e);
SYMBOL_CRC(virtqueue_kick_prepare, 0xa979975e);
SYMBOL_CRC(virtqueue_notify, 0x98bb9f0a);
SYMBOL_CRC(virtqueue_kick, 0x152426bc);
SYMBOL_CRC(virtqueue_get_buf_ctx, 0xb464c9c2);
SYMBOL_CRC(virtqueue_get_buf, 0x680950b9);
SYMBOL_CRC(virtqueue_disable_cb, 0x18dfe9fa);
SYMBOL_CRC(virtqueue_enable_cb_prepare, 0x8fbbdef9);
SYMBOL_CRC(virtqueue_poll, 0xdafe7e24);
SYMBOL_CRC(virtqueue_enable_cb, 0x6a87982e);
SYMBOL_CRC(virtqueue_enable_cb_delayed, 0x7c56e795);
SYMBOL_CRC(virtqueue_detach_unused_buf, 0x2684d82d);
SYMBOL_CRC(vring_interrupt, 0xff05fa13);
SYMBOL_CRC(vring_create_virtqueue, 0xea0985dd);
SYMBOL_CRC(vring_create_virtqueue_map, 0xf7a04625);
SYMBOL_CRC(virtqueue_resize, 0x29dcb940);
SYMBOL_CRC(virtqueue_reset, 0x5e5fc91c);
SYMBOL_CRC(vring_new_virtqueue, 0xdcb89141);
SYMBOL_CRC(vring_del_virtqueue, 0x969f179f);
SYMBOL_CRC(vring_notification_data, 0xa891a13d);
SYMBOL_CRC(vring_transport_features, 0xe150d7b5);
SYMBOL_CRC(virtqueue_get_vring_size, 0xbfcb8eb4);
SYMBOL_CRC(__virtqueue_break, 0x175371db);
SYMBOL_CRC(__virtqueue_unbreak, 0x1a33d7d3);
SYMBOL_CRC(virtqueue_is_broken, 0x0cb72472);
SYMBOL_CRC(virtio_break_device, 0x70951371);
SYMBOL_CRC(__virtio_unbreak_device, 0x93344592);
SYMBOL_CRC(virtqueue_get_desc_addr, 0x8e0b62bd);
SYMBOL_CRC(virtqueue_get_avail_addr, 0xf7620863);
SYMBOL_CRC(virtqueue_get_used_addr, 0xc25903b0);
SYMBOL_CRC(virtqueue_get_vring, 0x037e6b0a);
SYMBOL_CRC(virtqueue_map_alloc_coherent, 0xe3ed3e15);
SYMBOL_CRC(virtqueue_map_free_coherent, 0x1111eae9);
SYMBOL_CRC(virtqueue_map_page_attrs, 0x411d74f1);
SYMBOL_CRC(virtqueue_unmap_page_attrs, 0x49f64cb8);
SYMBOL_CRC(virtqueue_map_single_attrs, 0x1cce64dd);
SYMBOL_CRC(virtqueue_unmap_single_attrs, 0xeeaf26cc);
SYMBOL_CRC(virtqueue_map_mapping_error, 0x812606e0);
SYMBOL_CRC(virtqueue_map_need_sync, 0x00b74eee);
SYMBOL_CRC(virtqueue_map_sync_single_range_for_cpu, 0x8b408ef4);
SYMBOL_CRC(virtqueue_map_sync_single_range_for_device, 0x614ec372);

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x4c9d28b0, "phys_base" },
	{ 0x7cd8d75e, "page_offset_base" },
	{ 0x19d798b9, "__preempt_count" },
	{ 0xd6742002, "preempt_schedule_thunk" },
	{ 0x6a5cc518, "__kmalloc_noprof" },
	{ 0x037a0cba, "kfree" },
	{ 0x4b67f21a, "dma_alloc_attrs" },
	{ 0x9698bd29, "dma_free_attrs" },
	{ 0xc591d53d, "dma_map_page_attrs" },
	{ 0x97651e6c, "vmemmap_base" },
	{ 0x50a87ed1, "dma_unmap_page_attrs" },
	{ 0xc31db0ce, "is_vmalloc_addr" },
	{ 0xcb4a7e69, "__dma_need_sync" },
	{ 0xb1778371, "__dma_sync_single_for_cpu" },
	{ 0xa643ef05, "__dma_sync_single_for_device" },
	{ 0x93d34196, "dma_max_mapping_size" },
	{ 0x037ad6f1, "alloc_pages_exact_noprof" },
	{ 0x7fc503f7, "_dev_warn" },
	{ 0xeb44339a, "free_pages_exact" },
	{ 0xce09e194, "kmalloc_caches" },
	{ 0xc016d853, "__kmalloc_cache_noprof" },
	{ 0x0822e936, "_dev_err" },
	{ 0xa78e4b0d, "module_layout" },
};

MODULE_INFO(depends, "");

