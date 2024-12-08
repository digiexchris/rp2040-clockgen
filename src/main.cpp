#include "hardware/clocks.h"
#include "hardware/pio.h"
#include "pico/stdlib.h"
#include "pwm.pio.h" // Auto-generated header for the PIO program

// Function to initialize the PIO program
void pwm_program_init(PIO pio, uint sm, uint offset, uint pin)
{
	// Configure the GPIO pin
	pio_sm_set_consecutive_pindirs(pio, sm, pin, 1, true);

	// Configure the state machine
	pio_sm_config c = pwm_program_get_default_config(offset);
	sm_config_set_sideset_pins(&c, pin);
	sm_config_set_clkdiv(&c, 1.0f); // Default clock divider (will be set later)
	sm_config_set_set_pins(&c, pin, 1);

	// Initialize the state machine
	pio_sm_init(pio, sm, offset, &c);

	// Enable the GPIO pin for PIO
	gpio_set_function(pin, GPIO_FUNC_PIO0);
}

int main()
{
	// Initialize the GPIO for standard output
	stdio_init_all();

	// Define the GPIO pin for the clock output
	const uint CLOCK_PIN = 2;

	// Set up the PIO program
	PIO pio = pio0; // Use PIO0
	uint sm = 0;	// Use state machine 0
	uint offset = pio_add_program(pio, &pwm_program);

	// Initialize the PIO program for the specified GPIO pin
	pwm_program_init(pio, sm, offset, CLOCK_PIN);

	// Calculate and set the clock divider for 5 MHz
	float clock_div = (float)clock_get_hz(clk_sys) / (5 * 2);
	pio_sm_set_clkdiv(pio, sm, clock_div);

	// Enable the state machine
	pio_sm_set_enabled(pio, sm, true);

	while (true)
	{
		// Infinite loop to keep the program running
		tight_loop_contents();
	}

	return 0;
}
