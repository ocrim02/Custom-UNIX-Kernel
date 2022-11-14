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
    stmdb sp!, {r0-r12}
    mrs r0, sp_usr
    mrs r1, lr_usr
    mrs r2, spsr
    mov r3, lr
    mov r4, #0
    mov r5, #0
    stmdb sp!, {r0-r5}

    mov r0, #1
    mov r1, sp
    bl  interrupt
    //go back //probably faulty
    ldmfd r0, {r0-r12,sp,pc} ^
    //cpsr = spsr_abt
    
_software_interrupt:
    //set lr pipeline offset
    sub lr, lr, #4
    stmdb sp!, {r0-r12}
    mrs r0, sp_usr
    mrs r1, lr_usr
    mrs r2, spsr
    mov r3, lr
    mov r4, #0
    mov r5, #0
    stmdb sp!, {r0-r5}

    mov r0, #2
    mov r1, sp
    bl  interrupt
    //go back //probably faulty
    ldmfd r0, {r0-r12,sp,pc} ^
    //cpsr = spsr_abt

_prefetch_abort:
    //set lr pipeline offset
    sub lr, lr, #4
    stmdb sp!, {r0-r12}
    mrs r0, sp_usr
    mrs r1, lr_usr
    mrs r2, spsr
    mov r3, lr
    mrc p15, 0, r4, c5, c0, 1       //IFSR Data Fault Status 
    mrc p15, 0, r5, c6, c0, 2       //IFAR Data Fault Address
    stmdb sp!, {r0-r5}

    mov r0, #3
    mov r1, sp
    bl  interrupt
    //go back //probably faulty
    ldmfd r0, {r0-r12,sp,pc} ^
    //cpsr = spsr_abt
    

_data_abort:
    //set lr pipeline offset
    sub lr, lr, #8
    stmdb sp!, {r0-r12}
    mrs r0, sp_usr
    mrs r1, lr_usr
    mrs r2, spsr
    mov r3, lr
    mrc p15, 0, r4, c5, c0, 0       //DFSR Data Fault Status 
    mrc p15, 0, r5, c6, c0, 0       //DFAR Data Fault Address
    stmdb sp!, {r0-r5}

    mov r0, #4
    mov r1, sp
    bl  interrupt
    //go back //probably faulty
    ldmfd r0, {r0-r12,sp,pc} ^
    //cpsr = spsr_abt

_not_used:


_irq:
    //set lr pipeline offset
    sub lr, lr, #8
    stmdb sp!, {r0-r12}
    mrs r0, sp_usr
    mrs r1, lr_usr
    mrs r2, spsr
    mov r3, lr
    mov r4, #0
    mov r5, #0
    stmdb sp!, {r0-r5}

    mov r0, #5
    mov r1, sp
    bl  interrupt
    //go back //probably faulty
    ldmfd r0, {r0-r12,sp,pc} ^
    //cpsr = spsr_abt

_fiq:
    