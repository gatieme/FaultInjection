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
	{ 0x146be775, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0xbf7bf975, __VMLINUX_SYMBOL_STR(unregister_jprobe) },
	{ 0xa8154d56, __VMLINUX_SYMBOL_STR(register_jprobe) },
	{ 0xcb6fa3da, __VMLINUX_SYMBOL_STR(remove_proc_entry) },
	{ 0xb35b1c13, __VMLINUX_SYMBOL_STR(proc_create_data) },
	{ 0x53027231, __VMLINUX_SYMBOL_STR(proc_mkdir) },
	{ 0xe914e41e, __VMLINUX_SYMBOL_STR(strcpy) },
	{ 0xe2d5255a, __VMLINUX_SYMBOL_STR(strcmp) },
	{ 0x28318305, __VMLINUX_SYMBOL_STR(snprintf) },
	{ 0xfd766812, __VMLINUX_SYMBOL_STR(unregister_kprobe) },
	{ 0x1fb91136, __VMLINUX_SYMBOL_STR(register_kprobe) },
	{ 0xf8e398fc, __VMLINUX_SYMBOL_STR(memstart_addr) },
	{ 0xa5cc83f6, __VMLINUX_SYMBOL_STR(init_task) },
	{ 0x98cf60b3, __VMLINUX_SYMBOL_STR(strlen) },
	{ 0x91715312, __VMLINUX_SYMBOL_STR(sprintf) },
	{ 0x20c55ae0, __VMLINUX_SYMBOL_STR(sscanf) },
	{ 0xfbc74f64, __VMLINUX_SYMBOL_STR(__copy_from_user) },
	{ 0xdcb764ad, __VMLINUX_SYMBOL_STR(memset) },
	{ 0x1b9aca3f, __VMLINUX_SYMBOL_STR(jprobe_return) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

