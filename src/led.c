#include "led.h"
#include "config.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"
#ifdef RP2040ZERO
#include "ws2812.pio.h"
#endif

#ifdef PICO
#define PICO_LED_PIN 25
#endif

static uint smWs2813;
static uint offsetWs2813;

#ifdef RP2040ZERO
void ws2812_put_pixel(uint32_t pixel_grb) {
	pio_sm_put_blocking(pio1, smWs2813, pixel_grb << 8u);
}
void ws2812_put_rgb(uint8_t red, uint8_t green, uint8_t blue) {
	#ifdef INVERT_RED_GREEN
	uint32_t mask = (red << 16) | (green << 8) | (blue << 0);
	#elif
	uint32_t mask = (green << 16) | (red << 8) | (blue << 0);
	#endif
	ws2812_put_pixel(mask);
}
#endif

void led_init() {
	#ifdef RP2040ZERO
	offsetWs2813 = pio_add_program(pio1, &ws2812_program);
	smWs2813 = pio_claim_unused_sm(pio1, true);
	ws2812_program_init(pio1, smWs2813, offsetWs2813, 16, 800000, true);
	#endif
}

void led_output_sync_status(bool out_of_sync) {
	#ifdef PICO
	gpio_put(PICO_LED_PIN, !out_of_sync);
	#endif
	#ifdef RP2040ZERO
	if(out_of_sync) {
		ws2812_put_rgb(255, 0, 0);
		sleep_ms(25);
	} else {
		ws2812_put_rgb(0, 255, 0);
	}
	#endif
}

void led_blink_error(int amount) {
	/* ensure led is off */
	#ifdef PICO
	gpio_put(PICO_LED_PIN, false);
	#endif
	#ifdef RP2040ZERO
	ws2812_put_rgb(0, 0, 0);
	#endif
	sleep_ms(500);
	/* start blinking */
	for(int i = 0; i < amount; ++i) {
		#ifdef PICO
		gpio_put(PICO_LED_PIN, true);
		#endif
		#ifdef RP2040ZERO
		ws2812_put_rgb(255, 0, 0);
		#endif
		sleep_ms(500);
		#ifdef PICO
		gpio_put(PICO_LED_PIN, false);
		#endif
		#ifdef RP2040ZERO
		ws2812_put_rgb(0, 0, 0);
		#endif
		sleep_ms(500);
	}
}