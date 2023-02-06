#include <arch/cpu/mmu.h>

static uint32_t l1_table[4096]__attribute__((aligned(0x4000)));
static uint32_t l2_table_arr[(THREAD_COUNT + IDLE_THREAD_COUNT + PROCESS_COUNT)][256]__attribute__((aligned(1024)));


void mmu_l1_section(void * virt_addr, void * phy_addr, enum mmu_permission perm, bool pxn, bool xn){
    unsigned int index = read_masked((unsigned int) virt_addr, 31, 20);

    uint32_t section_entry = 2;
    section_entry = write_masked(section_entry, (uint32_t) phy_addr>>20, 31, 20);
    section_entry = write_masked(section_entry, perm>>2, 15, 15);
    section_entry = write_masked(section_entry, perm, 11, 10);
    if(pxn){
        section_entry |= 1;
    }
    if(xn){
        section_entry |= 1<<4;
    }
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


void mmu_l2_small(void * virt_addr, void * phy_addr, uint32_t l2_table[256], enum mmu_permission perm, bool xn){
    unsigned int index = read_masked((unsigned int) virt_addr, 19, 12);
    uint32_t section_entry = 2;
    section_entry = write_masked(section_entry, (uint32_t) phy_addr>>12, 19, 12);
    section_entry = write_masked(section_entry, perm>>2, 9, 9);
    section_entry = write_masked(section_entry, perm, 5, 4);
    if(xn){
        section_entry |= 1;
    }
    l2_table[index] = section_entry;
}


void mmu_l1_l2(void * virt_addr, void * l2_base_addr, bool pxn){
    unsigned int index = read_masked((unsigned int) virt_addr, 31, 20);
    uint32_t section_entry = 1;
    section_entry = write_masked(section_entry, (uint32_t) l2_base_addr>>10, 31, 10);
    if(pxn){
        section_entry |= 1<<2;
    }
    l1_table[index] = section_entry;
}


void* l1_table_init(){
    uint32_t addr = ONE_MB - 1;
    for(int i=0; i<4096; i++){
        mmu_l1_fault((void*) addr);
        if(i==158){
            mmu_l1_section((void*) addr, (void*) addr, FULL_ACCESS, true, true);
        }
        // MMIO
        if(i >= PERIPHERAL_BASE / ONE_MB && i <= (PERIPHERAL_BASE + PERIPHERAL_SIZE) / ONE_MB){
            mmu_l1_section((void*) addr, (void*) addr, RW_NA, true, true);
        }
        addr = addr + ONE_MB;
    }

    //init section
    mmu_l1_section((void*) 1048575, (void*) 1048575, R_NA, true, true);
    // kernel stacks
    mmu_l1_section((void*) MAX_RAM_ADDR - ONE_MB, (void*) MAX_RAM_ADDR - ONE_MB, RW_NA, true, true);


    unsigned int* sizes[7] =    {kernel_text_size,    kernel_rodata_size,    kernel_data_size,    kernel_bss_size,    user_text_size,    user_rodata_size,    user_data_size};
    unsigned int* sections[7] = {kernel_text_section, kernel_rodata_section, kernel_data_section, kernel_bss_section, user_text_section, user_rodata_section, user_data_section};
    unsigned int rights[7] =    {R_NA,                R_NA,                  RW_NA,               RW_NA,              R_R,               R_R,                 RW_NA};
    bool pxn[7] =               {false,               true,                  true,                true,               true,              true,                true};
    bool xn[7] =                {false,               true,                  true,                true,               false,             true,                true};

    for(unsigned int sect=0; sect<sizeof(sizes)/sizeof(unsigned int); sect++){
        for(int i=0; i<=(int) sizes[sect]/ONE_MB; i++){
            mmu_l1_section((void*) sections[sect] + i * ONE_MB, (void*) sections[sect] + i * ONE_MB, rights[sect], pxn[sect], xn[sect]);
        }
    }

    tcb_l2_init();

    return l1_table;
}


void tcb_l2_init(){
    for(int tcb=0; tcb<(THREAD_COUNT + IDLE_THREAD_COUNT); tcb++){
        mmu_l1_l2((void*) (THREAD_SP_BASE - (tcb * THREAD_SP_SIZE) - 1), l2_table_arr[tcb], true);
        for(int i=0; i<256; i++){
            mmu_l2_small((void*) (THREAD_SP_BASE - (tcb * THREAD_SP_SIZE) - (i * L2_SIZE) - 1), (void*)  (THREAD_SP_BASE - (tcb * THREAD_SP_SIZE) - (i * L2_SIZE) - 1), l2_table_arr[tcb], FULL_ACCESS, true);
        }
        mmu_l2_fault((void*) (THREAD_SP_BASE - (tcb * THREAD_SP_SIZE) - (255 * L2_SIZE) - 1), l2_table_arr[tcb]);
    }

    
}

void pid_create(unsigned int pid){
    kprintf("pid create\n");
    if(pid >= THREAD_COUNT){
        kprintf("Error: invalid pid!");
        return;
    }

    //Pid space
    for(int id=0; id<PROCESS_COUNT; id++){
        mmu_l1_l2((void*) ((unsigned int) user_data_section + (unsigned int)(id * ONE_MB)), l2_table_arr[(THREAD_COUNT + IDLE_THREAD_COUNT) + id], true);
        for(int i=0; i<256; i++){
            mmu_l2_small((void*) ((unsigned int) user_data_section + (unsigned int) (id * ONE_MB) + (unsigned int) (i * L2_SIZE)), (void*) ((unsigned int) user_data_section + (unsigned int) (id * ONE_MB) + (unsigned int) (i * L2_SIZE)), l2_table_arr[(THREAD_COUNT + IDLE_THREAD_COUNT) + id], RW_NA, true);
        }
    }
    clear_tlb();

    unsigned int* src = user_data_section;
    unsigned int* dest = pid_section + ONE_MB * pid;
    for(unsigned int i=0; i<ONE_MB/4; i++){
        unsigned int mem = *(unsigned int*)(src + 4*i);
        *(dest + 4*i) = mem;
    }
}

// -1 to block all processes
void switch_pid(int pid){
    kprintf("switch to pid %i\n", pid);
    for(int id=0; id<PROCESS_COUNT; id++){
        mmu_l1_l2((void*) ((unsigned int) user_data_section + (unsigned int) (id * ONE_MB)), l2_table_arr[(THREAD_COUNT + IDLE_THREAD_COUNT) + id], true);
        for(int i=0; i<256; i++){
            mmu_l2_small((void*) ((unsigned int) user_data_section + (unsigned int) (id * ONE_MB) + (unsigned int) (i * L2_SIZE)), (void*) ((unsigned int) user_data_section + (unsigned int) (id * ONE_MB) + (unsigned int) (i * L2_SIZE)), l2_table_arr[(THREAD_COUNT + IDLE_THREAD_COUNT) + id], RW_NA, true);
        }
    }

    if(pid >= 0 && pid<PROCESS_COUNT){
        mmu_l1_l2((void*) ((unsigned int) pid_section + (unsigned int) (pid * ONE_MB)), l2_table_arr[(THREAD_COUNT + IDLE_THREAD_COUNT)], true);
        mmu_l1_l2((void*) ((unsigned int) user_data_section), l2_table_arr[(THREAD_COUNT + IDLE_THREAD_COUNT) + pid + 1], true);
        for(int i=0; i<256; i++){
            mmu_l2_small((void*) ((unsigned int) user_data_section + (unsigned int) (i * L2_SIZE)), (void*) ((unsigned int) pid_section + (unsigned int) (pid * ONE_MB) + (unsigned int) (i * L2_SIZE)), l2_table_arr[(THREAD_COUNT + IDLE_THREAD_COUNT) + pid + 1], FULL_ACCESS, true);
        }
    }

    //stack rights

    clear_tlb();
}


void show_l2_table(uint32_t l2_table[256]){
    for(int i=0; i<256; i++){
        int right = read_masked(l2_table[i], 9, 9);
        right = right << 2;
        right = right + read_masked(l2_table[i], 5, 4);
        kprintf("%u 4KB, ", i);
        switch (right){
        case 0:
            kprintf("NO ACCESS\n");
            break;
        case 1:
            kprintf("RW_NA\n");
            break;
        case 5:
            kprintf("R_NA\n");
            break;
        case 7:
            kprintf("R_R\n");
            break;
        case 2:
            kprintf("RW_R\n");
            break;
        default:
            kprintf("FULL ACCESS\n");
            break;
        }
    }
}


void show_l1_table(unsigned int from, unsigned int to){
    for(unsigned int i=from; i<to; i++){
        int right = read_masked(l1_table[i], 15, 15);
        right = right << 2;
        right = right + read_masked(l1_table[i], 11, 10);
        kprintf("%u MB, ", i);
        if(read_masked(l1_table[i], 1, 0) == 1){
            kprintf("L2\n");
        }
        else{
            switch (right){
            case 0:
                kprintf("NO ACCESS\n");
                break;
            case 1:
                kprintf("RW_NA\n");
                break;
            case 5:
                kprintf("R_NA\n");
                break;
            case 7:
                kprintf("R_R\n");
                break;
            case 2:
                kprintf("RW_R\n");
                break;
            default:
                kprintf("FULL ACCESS\n");
                break;
            }
        }
    }
}