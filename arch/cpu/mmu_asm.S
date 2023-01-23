
.global mmu_init
mmu_init:
    //domain access
    mrc p15, 0, r1, c3, c0, 0
    and r1, #0xFFFFFFFC
    orr r1, #0x00000001
    mcr p15, 0, r1, c3, c0, 0       

    //set l1 table base
	mcr p15, 0, r0, c2, c0, 0

    //TTB Control Reg
    mrc p15, 0, r1, c2, c0, 2
	//change r0
    mov r1, #0
    //and r1, #0x7FFFFFFF
	mcr p15, 0, r1, c2, c0, 2

    //sctlr
    mrc p15, 0, r1, c1, c0, 0
	//enable mmu
    orr r1, #0x00000001
    //disable Cache
    and r1, #0xFFFFFFFB
	mcr p15, 0, r1, c1, c0, 0
    
    mov pc, lr


.global clear_tlb
clear_tlb:
    mov r1, #0
    mcr p15, 0, r1, c8, c7, 0
    mov pc, lr