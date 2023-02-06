#ifndef MMU_H
#define MMU_H

#include <stdint.h>
#include <lib/utils.h>
#include <kernel/thread.h>
#include <arch/cpu/cpu.h>

#ifndef __LDS__
extern unsigned int kernel_text_section[]__attribute__((weak));
extern unsigned int kernel_rodata_section[]__attribute__((weak));
extern unsigned int kernel_data_section[]__attribute__((weak));
extern unsigned int kernel_bss_section[]__attribute__((weak));
extern unsigned int user_text_section[]__attribute__((weak));
extern unsigned int user_rodata_section[]__attribute__((weak));
extern unsigned int user_data_section[]__attribute__((weak));
extern unsigned int pid_section[]__attribute__((weak));

extern unsigned int kernel_text_size[]__attribute__((weak));
extern unsigned int kernel_rodata_size[]__attribute__((weak));
extern unsigned int kernel_data_size[]__attribute__((weak));
extern unsigned int kernel_bss_size[]__attribute__((weak));
extern unsigned int user_text_size[]__attribute__((weak));
extern unsigned int user_rodata_size[]__attribute__((weak));
extern unsigned int user_data_size[]__attribute__((weak));
#endif

#define ONE_MB (1024*1024)
#define PERIPHERAL_BASE (0x7E000000 - 0x3F000000)
#define PERIPHERAL_SIZE 0xFFFFFF

enum mmu_permission {
	NO_ACCESS         = 0b000,
	RW_NA             = 0b001,
	R_NA              = 0b101,
	R_R               = 0b111,
	RW_R              = 0b010,
	FULL_ACCESS       = 0b011
};


void mmu_l1_section(void * virt_addr, void * phy_addr, enum mmu_permission perm, bool pxn, bool xn);
void* l1_table_init();
void mmu_init(void* l1_base);
void clear_tlb();
void tcb_l2_init();
void mmu_l1_l2(void * virt_addr, void * l2_base_addr, bool pxn);
void mmu_l2_small(void * virt_addr, void * phy_addr, uint32_t l2_table[256], enum mmu_permission perm, bool xn);
void mmu_l2_fault(void * virt_addr, uint32_t l2_table[256]);
void mmu_l1_fault(void * virt_addr);
void show_l1_table(unsigned int from, unsigned int to);
void show_l2_table(uint32_t l2_table[256]);
void switch_pid(int pid);
void pid_create(unsigned int pid);

#endif