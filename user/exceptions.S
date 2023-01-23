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
    
.global wait_for_int
wait_for_int:
    wfi
    mov pc, lr

.global read_addr
read_addr:
    ldr r1, [r0]
    mov pc, lr

.global write_addr
write_addr:
    mov r0, #0
    mov pc, lr

.global jump_addr
jump_addr:
    bx r0
    mov pc, lr