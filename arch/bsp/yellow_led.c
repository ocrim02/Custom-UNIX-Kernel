#include <arch/bsp/yellow_led.h>

#define GPIO_BASE (0x7E200000 - 0x3F000000)
#define GREEN_LED_1 4
#define YELLOW_LED_1 5
#define RED_LED 6
#define YELLOW_LED_2 7
#define GREEN_LED_2 8
#define GPF_BITS 3

enum gpio_func {
	gpio_input = 0x0, 	// GPIO Pin is an input
	gpio_output = 0x1, 	// GPIO Pin is an output
};

struct gpio {
	unsigned int func[6];
	unsigned int unused0;
	unsigned int set[2];
	unsigned int unused1;
	unsigned int clr[2];
};

static volatile
struct gpio * const gpio_port = (struct gpio *)GPIO_BASE;


void light_row(void)
{
	/* Initialisieren */
	gpio_port->func[0] = gpio_output << (YELLOW_LED_1 * GPF_BITS);
	gpio_port->func[0] |= gpio_output << (YELLOW_LED_2 * GPF_BITS);
	gpio_port->func[0] |= gpio_output << (RED_LED * GPF_BITS);
	gpio_port->func[0] |= gpio_output << (GREEN_LED_2 * GPF_BITS);
	gpio_port->func[0] |= gpio_output << (GREEN_LED_1 * GPF_BITS);

	/* Anschalten */
	gpio_port->set[0] = 1 << GREEN_LED_1;
	gpio_port->set[0] = 1 << YELLOW_LED_1;
	gpio_port->set[0] = 1 << RED_LED;
	gpio_port->set[0] = 1 << YELLOW_LED_2;
	gpio_port->set[0] = 1 << GREEN_LED_2;
	
	gpio_port->clr[0] = 1 << GREEN_LED_1;
	gpio_port->clr[0] = 1 << YELLOW_LED_1;
	gpio_port->clr[0] = 1 << RED_LED;
	gpio_port->clr[0] = 1 << YELLOW_LED_2;
	gpio_port->clr[0] = 1 << GREEN_LED_2;
}

