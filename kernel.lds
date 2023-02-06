/*inspired from https://sourceware.org/binutils/docs-2.35/ld/region_005fALIAS.html#region_005fALIAS */

ENTRY(_start)
SECTIONS
{
	. = 0x00008000;
	.init : { *(.init) }

	. = ALIGN(0x00100000);
	kernel_text_section = .;
	.text : {
		build/arch/*.o(.text)
		build/kernel/*.o(.text)
		build/lib/utils.o(.text)
	}
	kernel_text_size = SIZEOF(.text);

	. = ALIGN(0x00100000);
	kernel_rodata_section = .;
	.rodata : {
		build/arch/*.o(.rodata .rodata.*)
		build/kernel/*.o(.rodata .rodata.*)
		build/lib/utils.o(.rodata .rodata.*)
	}
	kernel_rodata_size = SIZEOF(.rodata);

	. = ALIGN(0x00100000);
	kernel_data_section = .;
	.data : {
		build/arch/*.o(.data)
		build/kernel/*.o(.data)
		build/lib/utils.o(.data)
	}
	kernel_data_size = SIZEOF(.data);

	. = ALIGN(0x00100000);
	kernel_bss_section = .;
	.bss : {
		build/arch/*.o(.bss)
		build/kernel/*.o(.bss)
		build/lib/utils.o(.bss)
	}
	kernel_bss_size = SIZEOF(.bss);

	. = ALIGN(0x00100000);
	user_text_section = .;
	.utext : {
		build/user/*.o(.text)
	}
	user_text_size = SIZEOF(.utext);

	. = ALIGN(0x00100000);
	user_rodata_section = .;
	.urodata : {
		build/user/*.o(.rodata .rodata.*)
	}
	user_rodata_size = SIZEOF(.urodata);

	. = ALIGN(0x00100000);
	user_data_section = .;
	.udata : {
		build/user/*.o(.data)
		build/user/*.o(.bss)
	}
	user_data_size = SIZEOF(.udata);

	. = ALIGN(0x00100000);
	pid_section = .;
}