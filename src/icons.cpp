#include "icons.h"

#include "leds.h"

static void led_set_pixels(int b, int first, int last) {
	for (int i = first; i <= last; i++)
		led_set_pixel(b, b, b, b, i);
}

// show the wlan symbol with individual brightness level for the rings
void show_icon_wlan(uint8_t r0, uint8_t r1, uint8_t r2) {
	led_clear();

	led_set_pixels(r0, 93, 93);
	led_set_pixels(r0, 81, 83);
	led_set_pixels(r0, 71, 71);

	led_set_pixels(r1, 63, 63);
	led_set_pixels(r1, 57, 57);
	led_set_pixels(r1, 46, 52);
	led_set_pixels(r1, 36, 40);

	led_set_pixels(r2, 33, 33);
	led_set_pixels(r2, 43, 43);
	led_set_pixels(r2, 22, 24);
	led_set_pixels(r2, 30, 32);
	led_set_pixels(r2, 12, 20);
	led_set_pixels(r2, 3, 7);

	led_show();
}

// show signal-strenght by using different brightness for the individual rings
void show_icon_wlan(int strength) {
	uint8_t r0, r1, r2;

	if (strength <= 0) {
		r0 = r1 = r2 = 0x40;
	} else if (strength <= 25) {
		r0 = r1 = r2 = 0x10;
	} else if (strength <= 50) {
		r0 = 0x80;
		r1 = r2 = 0x10;
	} else if (strength <= 75) {
		r0 = r1 = 0x80;
		r2 = 0x10;
	} else {
		r0 = r1 = r2 = 0x80;
	}

	show_icon_wlan(r0, r1, r2);
}

