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
	{ 0xf7830f93, "alloc_workqueue_noprof" },
	{ 0x720a27a7, "__register_blkdev" },
	{ 0xbd733b91, "__register_virtio_driver" },
	{ 0xb5a459dc, "unregister_blkdev" },
	{ 0x8c03d20c, "destroy_workqueue" },
	{ 0x19d798b9, "__preempt_count" },
	{ 0x18dfe9fa, "virtqueue_disable_cb" },
	{ 0x680950b9, "virtqueue_get_buf" },
	{ 0x308fe497, "blk_mq_complete_request" },
	{ 0x6a87982e, "virtqueue_enable_cb" },
	{ 0x7d1c2163, "blk_mq_start_stopped_hw_queues" },
	{ 0xd6742002, "preempt_schedule_thunk" },
	{ 0x071cb357, "virtio_check_driver_offered_feature" },
	{ 0x14231a68, "mutex_lock" },
	{ 0x5ecea35b, "mutex_unlock" },
	{ 0x2f2c95c4, "flush_work" },
	{ 0x756a8cc2, "del_gendisk" },
	{ 0x685ca55f, "blk_mq_free_tag_set" },
	{ 0xfc802eac, "virtio_reset_device" },
	{ 0x037a0cba, "kfree" },
	{ 0xeff4af0e, "put_disk" },
	{ 0xcc031c39, "ida_free" },
	{ 0xe783e261, "sysfs_emit" },
	{ 0xb2fa093e, "blk_mq_map_queues" },
	{ 0x12c1714f, "blk_mq_map_hw_queues" },
	{ 0xa979975e, "virtqueue_kick_prepare" },
	{ 0x98bb9f0a, "virtqueue_notify" },
	{ 0xb320cc0e, "sg_init_one" },
	{ 0x94ea071e, "virtqueue_add_sgs" },
	{ 0xc71f3d6f, "unregister_virtio_driver" },
	{ 0x97651e6c, "vmemmap_base" },
	{ 0x7cd8d75e, "page_offset_base" },
	{ 0x274dd1a3, "sg_free_table_chained" },
	{ 0x5a666555, "blk_mq_end_request_batch" },
	{ 0x5d23f003, "blk_mq_requeue_request" },
	{ 0x169938c1, "__sysfs_match_string" },
	{ 0x37e288af, "queue_limits_commit_update_frozen" },
	{ 0x24eef9d8, "string_get_size" },
	{ 0x4ec1ae33, "_dev_notice" },
	{ 0x2ca12629, "set_capacity_and_notify" },
	{ 0x0822e936, "_dev_err" },
	{ 0x360682ec, "blk_mq_num_possible_queues" },
	{ 0x35ea8db8, "_dev_info" },
	{ 0x6a5cc518, "__kmalloc_noprof" },
	{ 0x656e4a6e, "snprintf" },
	{ 0x70646586, "blk_mq_unquiesce_queue" },
	{ 0x1b8f87fc, "ida_alloc_range" },
	{ 0xce09e194, "kmalloc_caches" },
	{ 0xc016d853, "__kmalloc_cache_noprof" },
	{ 0x9436e710, "mutex_init_generic" },
	{ 0x7e6633be, "blk_mq_alloc_tag_set" },
	{ 0x67a38d6b, "virtio_max_dma_size" },
	{ 0xee1fd493, "__blk_mq_alloc_disk" },
	{ 0xb0e602eb, "memmove" },
	{ 0xf4fb3a1a, "set_disk_ro" },
	{ 0x35af59f3, "device_add_disk" },
	{ 0x6d2f6011, "const_current_task" },
	{ 0x085f2944, "blk_mq_freeze_queue_nomemsave" },
	{ 0x043d7bd9, "blk_mq_quiesce_queue_nowait" },
	{ 0x8b75d0b5, "blk_mq_unfreeze_queue_nomemrestore" },
	{ 0x9e57859b, "blk_mq_alloc_request" },
	{ 0x790e835b, "blk_rq_map_kern" },
	{ 0x0d05d93e, "blk_execute_rq" },
	{ 0x84502a47, "blk_status_to_errno" },
	{ 0x76c0dab3, "blk_mq_free_request" },
	{ 0x754d539c, "strlen" },
	{ 0xc5b6f236, "queue_work_on" },
	{ 0x2306fb29, "blk_mq_end_request" },
	{ 0x43eadd1d, "blk_mq_complete_request_remote" },
	{ 0x4c9d28b0, "phys_base" },
	{ 0x1bee4974, "sg_alloc_table_chained" },
	{ 0x19d83fb9, "__blk_rq_map_sg" },
	{ 0x78efa431, "blk_mq_start_request" },
	{ 0x152426bc, "virtqueue_kick" },
	{ 0x7f2b218e, "blk_mq_stop_hw_queue" },
	{ 0xca86c1e9, "param_ops_uint" },
	{ 0xa78e4b0d, "module_layout" },
};

MODULE_INFO(depends, "virtio,virtio_ring");

MODULE_ALIAS("virtio:d00000002v*");
