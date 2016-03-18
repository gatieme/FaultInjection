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
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x14522340, "module_layout" },
	{ 0x25ec1b28, "strlen" },
	{ 0x7e5e301f, "unregister_kprobe" },
	{ 0x93260715, "register_kprobe" },
	{ 0x1a6d6e4f, "remove_proc_entry" },
	{ 0x1b9aca3f, "jprobe_return" },
	{ 0x3c2c5af5, "sprintf" },
	{ 0xb94db510, "register_jprobe" },
	{ 0xde0bdcff, "memset" },
	{ 0xed113e22, "proc_mkdir" },
	{ 0xea147363, "printk" },
	{ 0x42224298, "sscanf" },
	{ 0xb4390f9a, "mcount" },
	{ 0xd258dfc9, "init_task" },
	{ 0x3a3f86d, "unregister_jprobe" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x6d6b15ff, "create_proc_entry" },
	{ 0xe52947e7, "__phys_addr" },
	{ 0x897a5a81, "pv_mmu_ops" },
	{ 0x9edbecae, "snprintf" },
	{ 0x3302b500, "copy_from_user" },
	{ 0xe914e41e, "strcpy" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "B1ACBA74DBDC446E48AD242");

static const struct rheldata _rheldata __used
__attribute__((section(".rheldata"))) = {
	.rhel_major = 6,
	.rhel_minor = 7,
	.rhel_release = 572,
};
