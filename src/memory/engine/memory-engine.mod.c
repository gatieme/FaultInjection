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
	{ 0xc6c01fa, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x8574f4e9, __VMLINUX_SYMBOL_STR(single_open) },
	{ 0x754d539c, __VMLINUX_SYMBOL_STR(strlen) },
	{ 0x638fe045, __VMLINUX_SYMBOL_STR(unregister_kprobe) },
	{ 0x98ac23a8, __VMLINUX_SYMBOL_STR(single_release) },
	{ 0x512b1d19, __VMLINUX_SYMBOL_STR(register_kprobe) },
	{ 0x56d4ca56, __VMLINUX_SYMBOL_STR(seq_printf) },
	{ 0x5dfa304a, __VMLINUX_SYMBOL_STR(remove_proc_entry) },
	{ 0x1b9aca3f, __VMLINUX_SYMBOL_STR(jprobe_return) },
	{ 0x57793513, __VMLINUX_SYMBOL_STR(seq_read) },
	{ 0xb1bedeb7, __VMLINUX_SYMBOL_STR(register_jprobe) },
	{ 0xfb578fc5, __VMLINUX_SYMBOL_STR(memset) },
	{ 0xfd036dc2, __VMLINUX_SYMBOL_STR(proc_mkdir) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x20c55ae0, __VMLINUX_SYMBOL_STR(sscanf) },
	{ 0x4c9d28b0, __VMLINUX_SYMBOL_STR(phys_base) },
	{ 0x7771d10d, __VMLINUX_SYMBOL_STR(find_vma) },
	{ 0x1c25c0d3, __VMLINUX_SYMBOL_STR(init_task) },
	{ 0xe16b0d30, __VMLINUX_SYMBOL_STR(unregister_jprobe) },
	{ 0xdb7305a1, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
	{ 0xa13aece1, __VMLINUX_SYMBOL_STR(proc_create_data) },
	{ 0x9e2b878e, __VMLINUX_SYMBOL_STR(seq_lseek) },
	{ 0x8554426d, __VMLINUX_SYMBOL_STR(pv_mmu_ops) },
	{ 0x28318305, __VMLINUX_SYMBOL_STR(snprintf) },
	{ 0x4f6b400b, __VMLINUX_SYMBOL_STR(_copy_from_user) },
	{ 0xe914e41e, __VMLINUX_SYMBOL_STR(strcpy) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "C86C1C692BE5A1BACC6D532");
