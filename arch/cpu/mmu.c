#include <arch/cpu/mmu.h>

static uint32_t l1_table[4096]__attribute__((aligned(0x4000)));
static uint32_t l2_table_arr[(THREAD_COUNT + IDLE_THREAD_COUNT)][256]__attribute__((aligned(1024)));

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
    //kprintf("%x\n", section_entry);

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
    //kprintf("%x, %x\n", virt_addr, section_entry);
}

void mmu_l1_l2(void * virt_addr, void * l2_base_addr, bool pxn){
    unsigned int index = read_masked((unsigned int) virt_addr, 31, 20);
    uint32_t section_entry = 1;
    section_entry = write_masked(section_entry, (uint32_t) l2_base_addr>>10, 31, 10);
    if(pxn){
        section_entry |= 1<<2;
    }
    l1_table[index] = section_entry;
    //kprintf("%x, %x\n", virt_addr, section_entry);
}

void* l1_table_init(){
    uint32_t addr = 1048575;
    for(int i=0; i<4096; i++){
        mmu_l1_fault((void*) addr);
        //0x7E000000-0x3F000000
        //start 1008
        //15MB
        if(i==158){
            mmu_l1_section((void*) addr, (void*) addr, PERM_FULL_ACCESS, true, true);
        }
        if(i>=1008 && i<=1023){
            mmu_l1_section((void*) addr, (void*) addr, PERM_RW_NA, true, true);
        }
        addr = addr + ONE_MB;
    }

    //init section
    mmu_l1_section((void*) 1048575, (void*) 1048575, PERM_R_NA, true, true);
    // kernel stacks
    mmu_l1_section((void*) MAX_RAM_ADDR - ONE_MB, (void*) MAX_RAM_ADDR - ONE_MB, PERM_RW_NA, true, true);


    unsigned int* sizes[8] =    {kernel_text_size,    kernel_rodata_size,    kernel_data_size,    kernel_bss_size,    user_text_size,    user_rodata_size,    user_data_size,    user_bss_size};
    unsigned int* sections[8] = {kernel_text_section, kernel_rodata_section, kernel_data_section, kernel_bss_section, user_text_section, user_rodata_section, user_data_section, user_bss_section};
    unsigned int rights[8] =    {PERM_R_NA,           PERM_R_NA,             PERM_RW_NA,          PERM_RW_NA,         PERM_R_R,          PERM_R_R,            PERM_FULL_ACCESS,  PERM_FULL_ACCESS};
    bool pxn[8] =               {false,               true,                  true,                true,               true,              true,                true,              true};
    bool xn[8] =                {false,               true,                  true,                true,               false,             true,                true,              true};

    for(unsigned int sect=0; sect<sizeof(sizes)/sizeof(unsigned int); sect++){
        kprintf("--------- %u ---------\n", sect);
        for(int i=0; i<=(int) sizes[sect]/ONE_MB; i++){
            mmu_l1_section((void*) sections[sect] + i * ONE_MB, (void*) sections[sect] + i * ONE_MB, rights[sect], pxn[sect], xn[sect]);
            //kprintf("page head %x\n", sections[sect]);
            //kprintf("page base %x\n", (void*) sections[sect] + i * ONE_MB);
            //kprintf("page size %x\n", sizes[sect]);
        }
    }
    tcb_sp_init();    //hier ist noch ein translation fault

    for(int i=0; i<128; i++){
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

    for(int i=0; i<256; i++){
        int right = read_masked(l2_table_arr[0][i], 9, 9);
        right = right << 2;
        right = right + read_masked(l2_table_arr[0][i], 5, 4);
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

    return l1_table;
}

void tcb_sp_init(){
    for(int tcb=0; tcb<(THREAD_COUNT + IDLE_THREAD_COUNT); tcb++){
        mmu_l1_l2((void*) (THREAD_SP_BASE - (tcb * THREAD_SP_SIZE) - 1), l2_table_arr[tcb], true);
        for(int i=0; i<256; i++){
            mmu_l2_small((void*) (THREAD_SP_BASE - (tcb * THREAD_SP_SIZE) - (i * L2_SIZE) - 1), (void*)  (THREAD_SP_BASE - (tcb * THREAD_SP_SIZE) - (i * L2_SIZE) - 1), l2_table_arr[tcb], PERM_FULL_ACCESS, true);
        }
        mmu_l2_fault((void*) (THREAD_SP_BASE - (tcb * THREAD_SP_SIZE) - (255 * L2_SIZE) - 1), l2_table_arr[tcb]);
    }
}