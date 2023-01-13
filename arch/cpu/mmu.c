#include <arch/cpu/mmu.h>

static uint32_t l1_table[4096]__attribute__((aligned(0x4000)));
static uint32_t l2_table_arr[(THREAD_COUNT + IDLE_THREAD_COUNT)][256]__attribute__((aligned(1024)));

void mmu_l1_section(void * virt_addr, void * phy_addr, enum mmu_permission perm){
    unsigned int index = read_masked((unsigned int) virt_addr, 31, 20);

    uint32_t section_entry = 2;
    section_entry = write_masked(section_entry, (uint32_t) phy_addr>>20, 31, 20);
    section_entry = write_masked(section_entry, perm>>2, 15, 15);
    section_entry = write_masked(section_entry, perm, 11, 10);

    l1_table[index] = section_entry;
}

void mmu_l1_fault(void * virt_addr){
    unsigned int index = read_masked((unsigned int) virt_addr, 31, 20);
    uint32_t section_entry = 0;
    l1_table[index] = section_entry;
}

void mmu_l2_fault(void * virt_addr, uint32_t l2_table[256]){
    unsigned int index = read_masked((unsigned int) virt_addr, 19, 12);
    uint32_t section_entry = 0;
    l2_table[index] = section_entry;
}

void mmu_l2_small(void * virt_addr, void * phy_addr, uint32_t l2_table[256], enum mmu_permission perm){
    unsigned int index = read_masked((unsigned int) virt_addr, 19, 12);
    uint32_t section_entry = 2;
    section_entry = write_masked(section_entry, (uint32_t) phy_addr>>12, 19, 12);
    section_entry = write_masked(section_entry, perm>>2, 9, 9);
    section_entry = write_masked(section_entry, perm, 5, 4);
    l2_table[index] = section_entry;
}

void mmu_l1_l2(void * virt_addr, void * l2_base_addr){
    unsigned int index = read_masked((unsigned int) virt_addr, 31, 20);
    uint32_t section_entry = 1;
    section_entry = write_masked(section_entry, (uint32_t) l2_base_addr>>10, 31, 10);
    l1_table[index] = section_entry;
    //kprintf("%u, %x\n", index, section_entry);
}

void* l1_table_init(){
    uint32_t addr = 1048575;
    for(int i=0; i<4096; i++){
        mmu_l1_section((void*) addr, (void*) addr, PERM_FULL_ACCESS);
        addr = addr + 1048575;
    }
    tcb_sp_init();
    return l1_table;
}

void tcb_sp_init(){
    for(int tcb=0; tcb<(THREAD_COUNT + IDLE_THREAD_COUNT); tcb++){
        mmu_l1_l2((void*) (THREAD_SP_BASE - (tcb * THREAD_SP_SIZE)), l2_table_arr[tcb]);
        for(int i=0; i<254; i++){
            mmu_l2_small((void*) (THREAD_SP_BASE - (tcb * THREAD_SP_SIZE) - (i * L2_SIZE)),(void*)  (THREAD_SP_BASE - (tcb * THREAD_SP_SIZE) - (i * L2_SIZE)), l2_table_arr[tcb], PERM_FULL_ACCESS);
        }
        mmu_l2_fault((void*) (THREAD_SP_BASE - (tcb * THREAD_SP_SIZE) - (255 * L2_SIZE)), l2_table_arr[tcb]);
    }
}