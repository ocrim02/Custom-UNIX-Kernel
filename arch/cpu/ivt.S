#define UNDEFINED 1 // Undefined Instruction
#define	SVC 2 // Supervisor Call
#define PFABT 3 // Prefetch Abort
#define DABT 4 // Data Abort
#define IRQ 5 // Interrupt
#define FIQ 6 // Fast INterrupt
#define UNUSED 7  // Unused

.global _ivt
.balign 64
_ivt:
    b       reset
    ldr     pc, =_undefined_instruction
    ldr     pc, =_software_interrupt
    ldr     pc, =_prefetch_abort
    ldr     pc, =_data_abort
    ldr     pc, =_not_used
    ldr     pc, =_irq
    ldr     pc, =_fiq


.global set_ivt
set_ivt:
	ldr r0, =_ivt
	mcr p15, 0, r0, c12, c0, 0
    mov pc, lr


reset:
    b _start

_undefined_instruction:
    sub lr, lr, #4
    stmfd sp!, {r0-r12, lr}
    mrs r0, sp_usr
    mrs r1, lr_usr
    mrs r2, spsr
    mov r3, #0
    mov r4, #0
    stmfd sp!, {r0-r4}

    mov r0, #UNDEFINED
    mov r1, sp
    bl  exception
    
    //go back 
    ldmfd sp!, {r0-r4}
    ldmfd sp!, {r0-r12, pc}^
    
_software_interrupt:
    sub lr, lr, #4
    stmfd sp!, {r0-r12, lr}
    mrs r0, sp_usr
    mrs r1, lr_usr
    mrs r2, spsr
    mov r3, #0
    mov r4, #0
    stmfd sp!, {r0-r4}

    mov r0, #SVC
    mov r1, sp
    bl  exception
    
    //go back 
    ldmfd sp!, {r0-r4}
    ldmfd sp!, {r0-r12, pc}^

_prefetch_abort:
    sub lr, lr, #4
    stmfd sp!, {r0-r12, lr}
    mrs r0, sp_usr
    mrs r1, lr_usr
    mrs r2, spsr
    mrc p15, 0, r3, c5, c0, 1       //IFSR Data Fault Status 
    mrc p15, 0, r4, c6, c0, 2       //IFAR Data Fault Address
    stmfd sp!, {r0-r4}

    mov r0, #PFABT
    mov r1, sp
    bl  exception

    //go back 
    ldmfd sp!, {r0-r4}
    ldmfd sp!, {r0-r12, pc}^
    

_data_abort:
    sub lr, lr, #8
    stmfd sp!, {r0-r12, lr}
    mrs r0, sp_usr
    mrs r1, lr_usr
    mrs r2, spsr
    mrc p15, 0, r3, c5, c0, 0       //DFSR Data Fault Status 
    mrc p15, 0, r4, c6, c0, 0       //DFAR Data Fault Address
    stmfd sp!, {r0-r4}

    mov r0, #DABT
    mov r1, sp
    bl  exception

    //go back 
    ldmfd sp!, {r0-r4}
    ldmfd sp!, {r0-r12, pc}^

_not_used:
    sub lr, lr, #4
    stmfd sp!, {r0-r12, lr}
    mrs r0, sp_usr
    mrs r1, lr_usr
    mrs r2, spsr
    mov r3, #0
    mov r4, #0
    stmfd sp!, {r0-r4}

    mov r0, #UNUSED
    mov r1, sp
    bl  exception

    //go back 
    ldmfd sp!, {r0-r4}
    ldmfd sp!, {r0-r12, pc}^


_irq:
    sub lr, lr, #4
    stmfd sp!, {r0-r12, lr}
    mrs r0, sp_usr
    mrs r1, lr_usr
    mrs r2, spsr
    mrs r3, #0
    mov r4, #0
    stmfd sp!, {r0-r4}

    mov r0, #IRQ
    mov r1, sp
    bl  exception

    //go back 
    ldmfd sp!, {r0-r4}
    ldmfd sp!, {r0-r12, pc}^


_fiq:
    sub lr, lr, #4
    stmfd sp!, {r0-r12, lr}
    mrs r0, sp_usr
    mrs r1, lr_usr
    mrs r2, spsr
    mov r3, #0
    mov r4, #0
    stmfd sp!, {r0-r4}

    mov r0, #FIQ
    mov r1, sp
    bl  exception

    //go back 
    ldmfd sp!, {r0-r4}
    ldmfd sp!, {r0-r12, pc}^

    