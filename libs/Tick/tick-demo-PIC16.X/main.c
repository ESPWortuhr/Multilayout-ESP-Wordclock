/************************* http://geekfactory.mx *******************************
 *
 * Demo for the "tick" library for time management, non-blocking delays and
 * event scheduling. In this program we toggle a led connected to the bit 0 on
 * A port, without the need to stop the program with a call to a delay function.
 *
 * Demostración de la librería "tick" para manejo de tiempos, retardos sin
 * bloqueo y programación de eventos. En este demo, se hace parpadear un led
 * conectado al puerto A en el bit 0, sin la necesidad de detener el programa
 * con una llamada a una función de retardo (delay).
 *
 * AUTHOR/AUTOR: Jesus Ruben Santa Anna Zamudio
 * MICROCONTROLLER/MICROCONTROLADOR: PIC16F88, PIC16F628
 * COMPILER/COMPILADOR: Microchip XC8 http://www.microchip.com/compilers
 *
 ********************************************************************************/
#include <xc.h>			// Encabezado para el compilador XC8
#include "../Tick.h"


/* Device specific configuration */
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON	// RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is MCLR)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EE Memory Code Protection bit (Code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off)
#pragma config CCPMX = RB0      // CCP1 Pin Selection bit (CCP1 function on RB0)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal External Switchover mode disabled)

void main()
{
	ANSEL = 0x00;
	TRISA = 0xFE;
	TRISB = 0xFF;

	// We operate on 32 bit ints
	uint32_t last_time = 0;

	// Initialize time count facilities
	tick_init();

	// Main loop
	for (;;) {
		// Check if more than half second (TICK_SECOND/2) has elapsed
		if (tick_get() - last_time >= TICK_SECOND / 2) {
			// Store the last time we toggled the led
			last_time = tick_get();
			// and do it...
			PORTAbits.RA0 ^= 1;
		}
		// Perform other tasks here inside the main loop
	}
}

/**
 * Interrupt vector implementation. Here we must update the internal counter on
 * the tick library.
 */
void interrupt isr(void)
{
	// Update the system counter
	tick_update();
	// Other interrupts can be handled here
}

