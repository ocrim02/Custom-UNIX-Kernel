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

_undefined_instruction:
    //set lr pipeline offset
    sub lr, lr, #4
    stmfd sp!, {r0-r12,sp,lr}

    mov r0, #1
    mov r1, sp
    bl  interrupt
    //go back
    ldmfd r0, {r0-r12,sp,pc} ^
    
_software_interrupt:
    //set lr pipeline offset
    sub lr, lr, #4
    stmfd sp!, {r0-r12,sp,lr}

    mov r0, #2
    mov r1, sp
    bl  interrupt
    //go back
    ldmfd r0, {r0-r12,sp,pc} ^

_prefetch_abort:
    //set lr pipeline offset
    sub lr, lr, #4
    stmfd sp!, {r0-r12,sp,lr}

    mov r0, #3
    mov r1, sp
    bl  interrupt
    //go back
    ldmfd r0, {r0-r12,sp,pc} ^
    

_data_abort:
    //set lr pipeline offset
    sub lr, lr, #8
    stmfd sp!, {r0-r12,sp,lr}

    mov r0, #4
    mov r1, sp
    bl  interrupt
    //go back
    ldmfd r0, {r0-r12,sp,pc} ^

_not_used:
    //set lr pipeline offset
    sub lr, lr, #4
    stmfd sp!, {r0-r12,sp,lr}

    mov r0, #1
    mov r1, sp
    bl  interrupt
    //go back
    ldmfd r0, {r0-r12,sp,pc} ^

_irq:
    //set lr pipeline offset
    sub lr, lr, #8
    stmfd sp!, {r0-r12,sp,lr}

    mov r0, #5
    mov r1, sp
    bl  interrupt
    //go back
    ldmfd r0, {r0-r12,sp,pc} ^

_fiq:
    //set lr pipeline offset
    sub lr, lr, #8
    stmfd sp!, {r0-r12,sp,lr}

    mov r0, #1
    mov r1, sp
    bl  interrupt
    //go back
    ldmfd r0, {r0-r12,sp,pc} ^