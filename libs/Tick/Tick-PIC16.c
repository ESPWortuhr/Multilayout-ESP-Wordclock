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
	T1CON = 0;
	TMR1L = 0;
	TMR1H = 0;
	// Configure timer interrupts
	PIR1bits.TMR1IF = 0;
	PIE1bits.TMR1IE = 1;
	INTCONbits.GIE = 1;
	INTCONbits.PEIE = 1;
	// Internal clock, 1:8 prescaler, timer enabled
	T1CON = 0x31;
}

uint32_t tick_get()
{
	tick_read_internal();
	return *((uint32_t *) & tickbuffer[0]);
}

void tick_update()
{
	if (PIR1bits.TMR1IF) {
		tickcnt++;
		PIR1bits.TMR1IF = 0;
	}
}

static void tick_read_internal()
{
	do {
		PIE1bits.TMR1IE = 1; // Enable interrupt
		asm("nop");
		PIE1bits.TMR1IE = 0; // Disable interrupt

		tickbuffer[0] = TMR1L;
		tickbuffer[1] = TMR1H;

		*((uint32_t*) & tickbuffer[2]) = tickcnt;
	} while (PIR1bits.TMR1IF);
	PIE1bits.TMR1IE = 1;
}
