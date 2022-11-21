#include <arch/cpu/interrupt_controller.h>

#define INTRPTCNTR_BASE_WITH_OFFSET (0x7E00B200 - 0x3F000000)

struct interrupt_register{
	unsigned int irq_basic_pending;
	unsigned int irq_pending_1;
	unsigned int irq_pending_2;
	unsigned int fiq_control;
	unsigned int enable_irqs_1; //irqs 0-31
	unsigned int enable_irqs_2; //irqs 32-63
	unsigned int enable_basic_irqs;
	unsigned int disable_irqs_1;
	unsigned int disable_irqs_2;
	unsigned int disable_basic_irqs;
};

static volatile
struct interrupt_register* const iregs = (struct interrupt_register*) (INTRPTCNTR_BASE_WITH_OFFSET);

void enable_basic_interrupts_disable_rest(void) {
	unsigned int select_all = 0xFFFFFFFF;
	
	//& => nur dieses bit
	iregs->enable_basic_irqs |= EN_ARM_TIMER_BASIC;
	iregs->enable_irqs_1 |= EN_SYSTIMER_C1_EN1; 	
	iregs->enable_irqs_2 |= EN_SYSTIMER_UART_EN2; 	
	
	//disablen von allem anderen
	iregs->disable_basic_irqs &= (select_all & ~EN_ARM_TIMER_BASIC);
	iregs->disable_irqs_1 &= (select_all & ~EN_SYSTIMER_C1_EN1);
	iregs->disable_irqs_2 &= (select_all & ~EN_SYSTIMER_UART_EN2);
}

unsigned int interrupt_pos(void) {
	if(iregs->irq_basic_pending & EN_ARM_TIMER_BASIC) {
		return (unsigned int) EN_ARM_TIMER_BASIC;
	}
	else if(iregs->irq_basic_pending & EN_SYSTIMER_C1_EN1) {
		return (unsigned int) EN_SYSTIMER_C1_EN1;
	}
	else if(iregs->irq_basic_pending & EN_SYSTIMER_UART_EN2) {
		return (unsigned int) EN_SYSTIMER_UART_EN2;
	}
	//oder irq_basic_pending ausgeben lassen ? in hex ?
	else {return iregs->irq_basic_pending;}
}

unsigned int pending_status(void) {
	return (unsigned int) iregs->irq_basic_pending;
}