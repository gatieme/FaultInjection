#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
};

static const struct modversion_info ____versions[]
__attribute_used__
__attribute__((section("__versions"))) = {
	{ 0x2f74e661, "struct_module" },
	{ 0xe914e41e, "strcpy" },
	{ 0xe2d5255a, "strcmp" },
	{ 0xaf25400d, "snprintf" },
	{ 0x9f4a0621, "unregister_kprobe" },
	{ 0x9159868d, "register_kprobe" },
	{ 0x6226b9fa, "machine_to_phys_mapping" },
	{ 0xefe8e01c, "end_pfn" },
	{ 0x3ee649bc, "machine_to_phys_order" },
	{ 0x55526907, "xen_features" },
	{ 0x859204af, "sscanf" },
	{ 0x945bc6a7, "copy_from_user" },
	{ 0xde0bdcff, "memset" },
	{ 0x1d26aa98, "sprintf" },
	{ 0x1d713c6a, "register_jprobe" },
	{ 0x34b1a85, "create_proc_entry" },
	{ 0x6a1a0ab2, "proc_mkdir" },
	{ 0xc8739237, "unregister_jprobe" },
	{ 0xcb23ecbc, "remove_proc_entry" },
	{ 0x1b9aca3f, "jprobe_return" },
	{ 0xdd132261, "printk" },
	{ 0xea3d9a67, "init_task" },
};

static const char __module_depends[]
__attribute_used__
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "1A51B9DA7A61011341C2698");
