

.global get_mode_regs
get_mode_regs:
    stmfd sp!, {lr}
    add r9, r0, #0xc0
	bl get_processor_mode
    mov r10, r0
    ldmfd sp!, {lr}

    msr cpsr_c, r9
    mov r2, lr
    mov r3, sp
    mrs r4, spsr
    msr cpsr_c, r10
    stmfd sp!, {r2-r4}

    mov r0, sp
    mov pc, lr

.global get_processor_mode
get_processor_mode:
    mrs r0, cpsr
    mov pc, lr