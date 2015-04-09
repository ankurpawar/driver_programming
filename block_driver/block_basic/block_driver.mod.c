#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x9412fa01, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0xcc3511c7, __VMLINUX_SYMBOL_STR(blk_cleanup_queue) },
	{ 0x754eb105, __VMLINUX_SYMBOL_STR(put_disk) },
	{ 0x531947d0, __VMLINUX_SYMBOL_STR(del_gendisk) },
	{ 0xb5a459dc, __VMLINUX_SYMBOL_STR(unregister_blkdev) },
	{ 0xd2b09ce5, __VMLINUX_SYMBOL_STR(__kmalloc) },
	{ 0x71a50dbc, __VMLINUX_SYMBOL_STR(register_blkdev) },
	{ 0x999e8297, __VMLINUX_SYMBOL_STR(vfree) },
	{ 0x94d54b15, __VMLINUX_SYMBOL_STR(add_disk) },
	{ 0x28318305, __VMLINUX_SYMBOL_STR(snprintf) },
	{ 0xca36fc7e, __VMLINUX_SYMBOL_STR(alloc_disk) },
	{ 0x772a5200, __VMLINUX_SYMBOL_STR(blk_queue_logical_block_size) },
	{ 0x284cec41, __VMLINUX_SYMBOL_STR(blk_init_queue) },
	{ 0x593a99b, __VMLINUX_SYMBOL_STR(init_timer_key) },
	{ 0xd6ee688f, __VMLINUX_SYMBOL_STR(vmalloc) },
	{ 0xa638378a, __VMLINUX_SYMBOL_STR(__blk_end_request_all) },
	{ 0x5a9d3f64, __VMLINUX_SYMBOL_STR(__blk_end_request_cur) },
	{ 0x69acdf38, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0xa5f47d47, __VMLINUX_SYMBOL_STR(blk_fetch_request) },
	{ 0x91c14f28, __VMLINUX_SYMBOL_STR(check_disk_change) },
	{ 0xd5f2172f, __VMLINUX_SYMBOL_STR(del_timer_sync) },
	{ 0xbe2c0274, __VMLINUX_SYMBOL_STR(add_timer) },
	{ 0x7d11c268, __VMLINUX_SYMBOL_STR(jiffies) },
	{ 0xfb578fc5, __VMLINUX_SYMBOL_STR(memset) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xda3e43d1, __VMLINUX_SYMBOL_STR(_raw_spin_unlock) },
	{ 0xd52bf1ce, __VMLINUX_SYMBOL_STR(_raw_spin_lock) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "0AFD2DB1C96E7493BE6B1AC");
