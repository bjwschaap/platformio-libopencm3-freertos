#include "FreeRTOS.h"
#include "task.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

/*
 * Handler in case our application overflows the stack
 */
void vApplicationStackOverflowHook(
	TaskHandle_t xTask __attribute__((unused)),
    char *pcTaskName __attribute__((unused))) {

	for (;;);
}

/*
 * Task that toggles PC13, which is the LED
 */
static void task1(void *args __attribute__((unused))) {
    
    for (;;) {
		gpio_toggle(GPIOC, GPIO13);
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

/*
 * Main loop, this is where our program starts
 */
int main(void) {
    // Setup main clock, using external 8MHz crystal 
    rcc_clock_setup_in_hse_8mhz_out_72mhz();

    // Enable clock for GPIO channel C
    rcc_periph_clock_enable(RCC_GPIOC);

    // Set pinmode for PC13
	gpio_set_mode(
		GPIOC,
		GPIO_MODE_OUTPUT_2_MHZ,
		GPIO_CNF_OUTPUT_PUSHPULL,
		GPIO13);

	// Turn LED off
	gpio_set(GPIOC, GPIO13);

    // Tell FreeRTOS about our toggle task, and set it's stack and priority
	xTaskCreate(task1, "LED", 100, NULL, 2, NULL);

    // Start RTOS Task scheduler
	vTaskStartScheduler();

    // The task scheduler is blocking, so we should never come here...
	for (;;);
	return 0;
}
