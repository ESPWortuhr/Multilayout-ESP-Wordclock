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

/**
 * This file has been prepared for Doxygen automatic documentation generation.
 */
#ifndef TICK_H
#define TICK_H

/*-------------------------------------------------------------*/
/*		Includes and dependencies			*/
/*-------------------------------------------------------------*/
#include "TickPort.h"

/*-------------------------------------------------------------*/
/*		Macros and definitions				*/
/*-------------------------------------------------------------*/
#define TICK_SECOND	((unsigned long long)TICKS_PER_SECOND)
#define TICK_MINUTE	((unsigned long long)TICKS_PER_SECOND*60ull)
#define TICK_HOUR	((unsigned long long)TICKS_PER_SECOND*3600ull)

/*-------------------------------------------------------------*/
/*		Typedefs enums & structs			*/
/*-------------------------------------------------------------*/

/*-------------------------------------------------------------*/
/*		Function prototypes				*/
/*-------------------------------------------------------------*/
/**
 * @brief Initializes the system tick service.
 *
 * This function prepares the time keeping facilities for non-blocking delays,
 * event scheduling, time calculation and even real time clock functions. The
 * underlying time keeping variable is 48 bit long so there is enough space to
 * count from miliseconds to days and weeks.
 */
void tick_init();

/**
 * @brief Gets the current system tick value.
 *
 * Reads the lower 32 bits of the system tick counter. You can compare this
 * value with constants to generate delays or use it to measure time intervals.
 * The resolution and maximun time span to measure depend on operating frequency
 * and timer architecture.
 *
 * @return The current system tick value (a 32 bit integer value)
 */
uint32_t tick_get();

/**
 * @brief Updates the tick counter variable.
 *
 * For platforms which tick updates need to be done outside the Tick library
 * assembly, such as the PIC16 architecture wich only has a single interrupt
 * vector.
 */
void tick_update();
#endif
// End of Header file
