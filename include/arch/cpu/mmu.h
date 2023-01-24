#ifndef MMU_H
#define MMU_H

#include <stdint.h>
#include <lib/utils.h>
#include <kernel/thread.h>
#include <arch/cpu/cpu.h>

#ifndef __LDS__
extern unsigned int kernel_text_section[];
extern unsigned int kernel_rodata_section[];
extern unsigned int kernel_data_section[];
extern unsigned int kernel_bss_section[];
extern unsigned int user_text_section[];
extern unsigned int user_rodata_section[];
extern unsigned int user_data_section[];
extern unsigned int user_bss_section[];

extern unsigned int kernel_text_size[];
extern unsigned int kernel_rodata_size[];
extern unsigned int kernel_data_size[];
extern unsigned int kernel_bss_size[];
extern unsigned int user_text_size[];
extern unsigned int user_rodata_size[];
extern unsigned int user_data_size[];
extern unsigned int user_bss_size[];
#endif

#define ONE_MB (1024*1024)


enum mmu_permission {
	PERM_NO_ACCESS         = 0b000,
	PERM_RW_NA             = 0b001,
	PERM_R_NA              = 0b101,
	PERM_R_R               = 0b111,
	PERM_RW_R              = 0b010,
	PERM_FULL_ACCESS       = 0b011
};


void mmu_l1_section(void * virt_addr, void * phy_addr, enum mmu_permission perm, bool pxn, bool xn);
void* l1_table_init();
void mmu_init(void* l1_base);
void clear_tlb();
void tcb_sp_init();
void mmu_l1_l2(void * virt_addr, void * l2_base_addr, bool pxn);
void mmu_l2_small(void * virt_addr, void * phy_addr, uint32_t l2_table[256], enum mmu_permission perm, bool xn);
void mmu_l2_fault(void * virt_addr, uint32_t l2_table[256]);
void mmu_l1_fault(void * virt_addr);

#endif