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

//(0en mit den ausgewählten bit verodern) und das gesamte Ding reinverunden ins Register

void enable_irq(unsigned int basic_irqs, unsigned int irqs_1, unsigned int irqs_2) {
	//& => nur dieses bit
	iregs->enable_basic_irqs |= basic_irqs;
	iregs->enable_irqs_1 |= irqs_1; 	
	iregs->enable_irqs_2 |= irqs_2; 	
}

void disable_irq(unsigned int basic_irqs, unsigned int irqs_1, unsigned int irqs_2) {
	//& => nur dieses bit
	iregs->disable_basic_irqs |= basic_irqs;
	iregs->disable_irqs_1 |= irqs_1;
	iregs->disable_irqs_2 |= irqs_2;
}

unsigned int irq_basis_pos(void) {
	return iregs->irq_basic_pending;
}

unsigned int irq_pend1_pos(void) {
	return (unsigned int) iregs->irq_pending_1;
}

unsigned int irq_pend2_pos(void) {
	return (unsigned int) iregs->irq_pending_2;
}