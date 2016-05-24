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
	{ 0xf8e3dbd2, "struct_module" },
	{ 0xe914e41e, "strcpy" },
	{ 0xe2d5255a, "strcmp" },
	{ 0xaf25400d, "snprintf" },
	{ 0x9f4a0621, "unregister_kprobe" },
	{ 0x9159868d, "register_kprobe" },
	{ 0xddcc15b8, "do_mm_track_pte" },
	{ 0x61af69bd, "mm_tracking_struct" },
	{ 0x859204af, "sscanf" },
	{ 0x945bc6a7, "copy_from_user" },
	{ 0xde0bdcff, "memset" },
	{ 0x1d26aa98, "sprintf" },
	{ 0x1d713c6a, "register_jprobe" },
	{ 0xa1679700, "create_proc_entry" },
	{ 0x3039689b, "proc_mkdir" },
	{ 0xc8739237, "unregister_jprobe" },
	{ 0x39280472, "remove_proc_entry" },
	{ 0x1b9aca3f, "jprobe_return" },
	{ 0xdd132261, "printk" },
	{ 0x4de1f52, "init_task" },
};

static const char __module_depends[]
__attribute_used__
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "FA4870512AF73251772D168");
