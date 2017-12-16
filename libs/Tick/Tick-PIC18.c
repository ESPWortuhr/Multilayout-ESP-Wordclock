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

	TMR0L = 0;
	TMR0H = 0;
	// Configure timer interrupt
	INTCON2bits.TMR0IP = 0;
	INTCONbits.TMR0IF = 0;
	INTCONbits.TMR0IE = 1;

	// Timer0 on, 16-bit, internal timer, 1:256 prescaler
	T0CON = 0x87;
}

uint32_t tick_get()
{
	tick_read_internal();
	return *((uint32_t *) & tickbuffer[0]);
}

void tick_update()
{
	if (INTCONbits.TMR0IF) {
		tickcnt++;
		INTCONbits.TMR0IF = 0;
	}
}

static void tick_read_internal()
{
	do {
		INTCONbits.TMR0IE = 1; // Enable interrupt
		asm("nop");
		INTCONbits.TMR0IE = 0; // Disable interrupt

		tickbuffer[0] = TMR0L;
		tickbuffer[1] = TMR0H;

		*((uint32_t*) & tickbuffer[2]) = tickcnt;
	} while (INTCONbits.TMR0IF);
	INTCONbits.TMR0IE = 1; // Enable interrupt
}
