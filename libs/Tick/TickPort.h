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

#ifndef TICKPORT_H
#define TICKPORT_H


#if defined(PLIB_PIC16)

#include <xc.h>
#include <stdint.h>
#include "Config.h"
#define TICKS_PER_SECOND	((CONFIG_TIMING_MAIN_CLOCK + 128ull)/8ull)	// Timer with 1:8 prescaler

#elif defined(PLIB_PIC18)

#include <xc.h>
#include <stdint.h>
#include "Config.h"
#define TICKS_PER_SECOND	((CONFIG_TIMING_MAIN_CLOCK + 128ull)/256ull)	// Timer with 1:256 prescaler

#elif defined(PLIB_PIC24)

#include <xc.h>
#include <stdint.h>
#include "Config.h"
#define TICKS_PER_SECOND	((CONFIG_TIMING_MAIN_CLOCK + 128ull)/256ull)	// Timer with 1:256 prescaler

#elif defined(ARDUINO)

#include <Arduino.h>
#include <stdint.h>
#define TICKS_PER_SECOND		1000ull

#define tick_get() millis()

#endif

#endif
// End of Header file
