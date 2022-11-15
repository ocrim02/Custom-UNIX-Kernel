//.extern interrupt_test

.global _ivt
.balign 32 //maybe 32
_ivt:
    b       reset
    ldr     pc, =_undefined_instruction
    ldr     pc, =_software_interrupt
    ldr     pc, =_prefetch_abort
    ldr     pc, =_data_abort
    ldr     pc, =_not_used
    ldr     pc, =_irq
    ldr     pc, =_fiq

.global create_prefetch_abort
create_prefetch_abort:
    bkpt #1
    mov pc, lr

.global create_undefined_instruction
create_undefined_instruction:
    udf
    mov pc, lr

.global create_supervisor_call
create_supervisor_call:
    svc #0
    mov pc, lr

.global create_data_abort
create_data_abort:
    mov r0, #0x01
    ldr r1, [r0]
    mov pc, lr


.global set_ivt
set_ivt:
	/* set IVT */
	ldr r0, =_ivt
	mcr p15, 0, r0, c12, c0, 0
    mov pc, lr

//Source: VALVERS.COM/open-software/raspberry-pi/bare-metal-programming-in-c-part-4/
.global enable_interrupts
enable_interrupts:
	mrs r0, cpsr
    bic r0, r0 , #0x80
    msr cpsr_c, r0
    cpsie i

    mov pc, lr

.global get_mode_regs
get_mode_regs:
    stmfd sp!, {lr}
    mov r1, r0
	bl get_processor_mode
    ldmfd sp!, {lr}

    msr cpsr_cxsf, r1
    mov r2, lr
    mov r3, sp
    mrs r4, spsr
    msr cpsr_cxsf, r0
    stmfd sp!, {r2-r4}

    mov r0, sp
    mov pc, lr

.global get_processor_mode
get_processor_mode:
    mrs r0, cpsr
    mov pc, lr


_undefined_instruction:
    //set lr pipeline offset
    sub lr, lr, #4
    stmfd sp!, {r0-r12, lr}
    mrs r0, sp_usr
    mrs r1, lr_usr
    mrs r2, spsr
    mov r3, #0
    mov r4, #0
    stmfd sp!, {r0-r4}

    mov r0, #1
    mov r1, sp
    bl  interrupt
    
    //go back 
    ldmfd sp!, {r0-r4}
    ldmfd sp!, {r0-r12, pc}^
    
_software_interrupt:
    //set lr pipeline offset
    sub lr, lr, #4
    stmfd sp!, {r0-r12, lr}
    mrs r0, sp_usr
    mrs r1, lr_usr
    mrs r2, spsr
    mov r3, #0
    mov r4, #0
    stmfd sp!, {r0-r4}

    mov r0, #2
    mov r1, sp
    bl  interrupt
    
    //go back 
    ldmfd sp!, {r0-r4}
    ldmfd sp!, {r0-r12, pc}^

_prefetch_abort:
    //set lr pipeline offset
    sub lr, lr, #4
    stmfd sp!, {r0-r12, lr}
    mrs r0, sp_usr
    mrs r1, lr_usr
    mrs r2, spsr
    mrc p15, 0, r3, c5, c0, 1       //IFSR Data Fault Status 
    mrc p15, 0, r4, c6, c0, 2       //IFAR Data Fault Address
    stmfd sp!, {r0-r4}

    mov r0, #3
    mov r1, sp
    bl  interrupt

    //go back 
    ldmfd sp!, {r0-r4}
    ldmfd sp!, {r0-r12, pc}^
    

_data_abort:
    //set lr pipeline offset
    sub lr, lr, #8
    stmfd sp!, {r0-r12, lr}
    mrs r0, sp_usr
    mrs r1, lr_usr
    mrs r2, spsr
    mrc p15, 0, r3, c5, c0, 0       //DFSR Data Fault Status 
    mrc p15, 0, r4, c6, c0, 0       //DFAR Data Fault Address
    stmfd sp!, {r0-r4}

    mov r0, #4
    mov r1, sp
    bl  interrupt

    //go back 
    ldmfd sp!, {r0-r4}
    ldmfd sp!, {r0-r12, pc}^

_not_used:


_irq:
    //set lr pipeline offset
    sub lr, lr, #4
    stmfd sp!, {r0-r12, lr}
    mrs r0, sp_usr
    mrs r1, lr_usr
    mrs r2, spsr
    mov r3, #0
    mov r4, #0
    stmfd sp!, {r0-r4}

    mov r0, #5
    mov r1, sp
    bl  interrupt

    //go back 
    ldmfd sp!, {r0-r4}
    ldmfd sp!, {r0-r12, pc}^



_fiq:
    