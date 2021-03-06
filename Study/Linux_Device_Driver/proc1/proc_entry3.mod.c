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
	{ 0xca05c877, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0xda5a23c1, __VMLINUX_SYMBOL_STR(single_release) },
	{ 0xc3e1132a, __VMLINUX_SYMBOL_STR(seq_read) },
	{ 0x56e9db97, __VMLINUX_SYMBOL_STR(seq_lseek) },
	{ 0xd122ae20, __VMLINUX_SYMBOL_STR(remove_proc_entry) },
	{ 0xf0fdf6cb, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{ 0x8ca9f5d7, __VMLINUX_SYMBOL_STR(proc_create_data) },
	{ 0xa0657eb3, __VMLINUX_SYMBOL_STR(proc_mkdir) },
	{ 0xa05f372e, __VMLINUX_SYMBOL_STR(seq_printf) },
	{ 0x6fdd98fa, __VMLINUX_SYMBOL_STR(single_open) },
	{ 0xd093accf, __VMLINUX_SYMBOL_STR(PDE_DATA) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "515F3E135FB85FD5D238344");
