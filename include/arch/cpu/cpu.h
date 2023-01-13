#ifndef _CPU_H_
#define _CPU_H_

#define MAX_RAM_ADDR (128 * 1024 * 1024)
#define L2_SIZE (1024 * 4)
#define GUARD_PAGE_SIZE (1024 * 4)
#define SP_SIZE (L2_SIZE + GUARD_PAGE_SIZE)
#define SYS_SP MAX_RAM_ADDR
#define IRQ_SP (MAX_RAM_ADDR - (SP_SIZE))
#define ABT_SP (MAX_RAM_ADDR - (SP_SIZE * 2))
#define UND_SP (MAX_RAM_ADDR - (SP_SIZE * 3))
#define SVC_SP (MAX_RAM_ADDR - (SP_SIZE * 4))

#define THREAD_SP_SIZE (1024 * 1024)
#define THREAD_SP_BASE (MAX_RAM_ADDR - (1024 * 1024))

#define USR_MODE (0x10)
#define FIQ_MODE (0x11)
#define IRQ_MODE (0x12)
#define SVC_MODE (0x13)
#define ABT_MODE (0x17)
#define UND_MODE (0x1b)
#define SYS_MODE (0x1f)

#endif