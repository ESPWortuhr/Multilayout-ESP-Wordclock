/*	Library for time management on various microcontrollers
	Copyright (C) 2014 Jesus Ruben Santa Anna Zamudio.

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.

	Author website: http://www.geekfactory.mx
	Author e-mail: ruben at geekfactory dot mx
 */

#include "Tick.h"

/* Internal counter for timer interrupts */
static volatile unsigned long tickcnt = 0;
/* Total counter 6 bytes including timer register value */
static unsigned char tickbuffer[6];

static void tick_read_internal();

void tick_init()
{
	// Prescaler bits
	T1CONbits.TCKPS = 3;
	// Initialize timer registers
	PR1 = 0xFFFF;
	TMR1 = 0;
	// Initialize timer interrupts
	IPC0bits.T1IP = 2;
	IFS0bits.T1IF = 0;
	IEC0bits.T1IE = 1;
	// Start timer
	T1CONbits.TON = 1;
}

uint32_t tick_get()
{
	tick_read_internal();
	return *((uint32_t *) &tickbuffer[0]);
}

static void tick_read_internal()
{
	do {
		uint32_t xTempTicks;

		IEC0bits.T1IE = 1; // Enable interrupt
		Nop();
		IEC0bits.T1IE = 0; // Disable interrupt

		((uint32_t*) tickbuffer)[0] = TMR1;

		xTempTicks = tickcnt;
		if (((uint32_t *) tickbuffer)[0] == 0xFFFFu)
			xTempTicks--;

		// Get high 4 bytes
		tickbuffer[2] = ((uint8_t *) & xTempTicks)[0];
		tickbuffer[3] = ((uint8_t *) & xTempTicks)[1];
		tickbuffer[4] = ((uint8_t *) & xTempTicks)[2];
		tickbuffer[5] = ((uint8_t *) & xTempTicks)[3];
	} while (IFS0bits.T1IF);
	IEC0bits.T1IE = 1; // Enable interrupt
}

void __attribute__((interrupt, auto_psv)) _T1Interrupt(void)
{
	// Increment internal high tick counter
	tickcnt++;
	// Reset interrupt flag
	IFS0bits.T1IF = 0;
}
