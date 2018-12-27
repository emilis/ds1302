/* Copyright (C) 2018 Emilis Dambauskas
   This file is part of the DS1302 Control Library.
   Written by Emilis Dambauskas <emilis.d@gmail.com>.

   The DS1302 Control Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The DS1302 Control Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the DS1302 Control Library; if not, see
   <http://www.gnu.org/licenses/>.

   As a special exception, if you link the code in this file with
   files compiled with a GNU compiler to produce an executable,
   that does not cause the resulting executable to be covered by
   the GNU Lesser General Public License.  This exception does not
   however invalidate any other reasons why the executable file
   might be covered by the GNU Lesser General Public License.
   This exception applies to code released by its copyright holders
   in files containing the exception.
*/

/// Begin lib ------------------------------------------------------------------

#ifndef _LIBDS1302_H
#define _LIBDS1302_H


/// Defines --------------------------------------------------------------------

/// Create the variable `ds1302_device`:
#define DS1302_setup(...) ds1302_device ds1302_device = ds1302_setup( __VA_ARGS__ )

/// Shorthands for using the variable `ds1302_device`:

#define DS1302_start_transfer(...) ds1302_start_transfer( ds1302_device, __VA_ARGS__ )
#define DS1302_stop_transfer(...) ds1302_stop_transfer( ds1302_device, __VA_ARGS__ )
#define DS1302_start_read(...) ds1302_start_read( ds1302_device, __VA_ARGS__ )
#define DS1302_start_write(...) ds1302_start_write( ds1302_device, __VA_ARGS__ )

#define DS1302_read_bit(...) ds1302_read_bit( ds1302_device, __VA_ARGS__ )
#define DS1302_read_byte(...) ds1302_read_byte( ds1302_device, __VA_ARGS__ )
#define DS1302_write_bit(...) ds1302_write_bit( ds1302_device, __VA_ARGS__ )
#define DS1302_write_byte(...) ds1302_write_byte( ds1302_device, __VA_ARGS__ )
#define DS1302_read_command(...) ds1302_read_command( ds1302_device, __VA_ARGS__ )
#define DS1302_write_command(...) ds1302_write_command( ds1302_device, __VA_ARGS__ )
#define DS1302_write_and_check(...) ds1302_write_and_check( ds1302_device, __VA_ARGS__ )

#define DS1302_read_seconds() ds1302_read_seconds( ds1302_device )
#define DS1302_read_minutes() ds1302_read_minutes( ds1302_device )
#define DS1302_read_hours() ds1302_read_hours( ds1302_device )
#define DS1302_read_mday() ds1302_read_mday( ds1302_device )
#define DS1302_read_month() ds1302_read_month( ds1302_device )
#define DS1302_read_weekday() ds1302_read_weekday( ds1302_device )
#define DS1302_read_year() ds1302_read_year( ds1302_device )
#define DS1302_read_clock_halt() ds1302_read_clock_halt( ds1302_device )
#define DS1302_read_24h_mode() ds1302_read_24h_mode( ds1302_device )
#define DS1302_read_pm() ds1302_read_pm( ds1302_device )
#define DS1302_read_write_protect() ds1302_read_write_protect( ds1302_device )

#define DS1302_write_seconds(...) ds1302_write_seconds( ds1302_device, __VA_ARGS__ )
#define DS1302_write_minutes(...) ds1302_write_minutes( ds1302_device, __VA_ARGS__ )
#define DS1302_write_hours(...) ds1302_write_hours( ds1302_device, __VA_ARGS__ )
#define DS1302_write_mday(...) ds1302_write_mday( ds1302_device, __VA_ARGS__ )
#define DS1302_write_month(...) ds1302_write_month( ds1302_device, __VA_ARGS__ )
#define DS1302_write_weekday(...) ds1302_write_weekday( ds1302_device, __VA_ARGS__ )
#define DS1302_write_year(...) ds1302_write_year( ds1302_device, __VA_ARGS__ )
#define DS1302_write_clock_halt(...) ds1302_write_clock_halt( ds1302_device, __VA_ARGS__ )
#define DS1302_write_write_protect(...) ds1302_write_write_protect( ds1302_device, __VA_ARGS__ )

#define DS1302_write_date(...) ds1302_write_date( ds1302_device, __VA_ARGS__ )
#define DS1302_print_date() ds1302_print_date( ds1302_device )

/// These aliases are not necessary, but kept for consistency:
#define DS1302_decode_value ds1302_decode_value
#define DS1302_encode_value ds1302_encode_value


/// Includes -------------------------------------------------------------------

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>


/// Structs --------------------------------------------------------------------

typedef struct ds1302_device {

    uint8_t clk_pin	;
    uint8_t dat_pin	;
    uint8_t ce_pin	;
} ds1302_device;


/// Functions ------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

    extern ds1302_device	ds1302_setup(
                                uint8_t clk_pin,
                                uint8_t dat_pin,
                                uint8_t ce_pin
                            );

    extern void		ds1302_start_transfer(  ds1302_device d );
    extern void		ds1302_stop_transfer(   ds1302_device d );
    extern void		ds1302_start_read(      ds1302_device d );
    extern void		ds1302_start_write(     ds1302_device d );

    extern uint8_t  ds1302_read_bit(        ds1302_device d );
    extern uint8_t	ds1302_read_byte(       ds1302_device d );
    extern uint8_t  ds1302_write_bit(       ds1302_device d,    uint8_t bit );
    extern uint8_t  ds1302_write_byte(      ds1302_device d,    uint8_t byte );

    extern uint8_t	ds1302_read_command(    ds1302_device d,    uint8_t command );
    extern uint8_t	ds1302_write_command(
                        ds1302_device d,
                        uint8_t command,
                        uint8_t value
                    );
    extern uint8_t	ds1302_write_and_check(
                        ds1302_device d,
                        uint8_t command,
                        uint8_t value
                    );

    extern uint8_t  ds1302_check_range(
                        uint8_t min,
                        uint8_t max,
                        uint8_t value
                    );
    extern ds1302_device    ds1302_check_device( ds1302_device d );

    extern uint8_t	ds1302_decode_value(
                        uint8_t value,
                        uint8_t length,
                        uint8_t min,
                        uint8_t max
                    );
    extern uint8_t	ds1302_encode_value(
                        uint8_t value,
                        uint8_t min,
                        uint8_t max
                    );

    extern uint8_t	ds1302_read_seconds(        ds1302_device d );
    extern uint8_t	ds1302_read_minutes(        ds1302_device d );
    extern uint8_t	ds1302_read_hours(          ds1302_device d );
    extern uint8_t	ds1302_read_mday(           ds1302_device d );
    extern uint8_t	ds1302_read_month(          ds1302_device d );
    extern uint8_t	ds1302_read_weekday(        ds1302_device d );
    extern uint8_t	ds1302_read_year(           ds1302_device d );
    extern uint8_t	ds1302_read_clock_halt(     ds1302_device d );
    extern uint8_t	ds1302_read_24h_mode(	    ds1302_device d );
    extern uint8_t	ds1302_read_pm(		        ds1302_device d );
    extern uint8_t	ds1302_read_write_protect(	ds1302_device d );

    extern uint8_t	ds1302_write_seconds(       ds1302_device d,    uint8_t seconds );
    extern uint8_t	ds1302_write_minutes(       ds1302_device d,    uint8_t minutes );
    extern uint8_t	ds1302_write_hours(		    ds1302_device d,    uint8_t hours );
    extern uint8_t	ds1302_write_mday(          ds1302_device d,    uint8_t mday );
    extern uint8_t	ds1302_write_month(         ds1302_device d,    uint8_t month );
    extern uint8_t  ds1302_write_weekday(       ds1302_device d,    uint8_t weekday );
    extern uint8_t	ds1302_write_year(          ds1302_device d,	uint8_t year );
    extern uint8_t	ds1302_write_clock_halt(    ds1302_device d,	uint8_t ch );
    extern uint8_t	ds1302_write_write_protect( ds1302_device d,    uint8_t wp );

    extern uint8_t	ds1302_write_date(
                        ds1302_device d,
                        uint8_t year,
                        uint8_t month,
                        uint8_t mday,
                        uint8_t hours,
                        uint8_t minutes,
                        uint8_t seconds
                    );

#ifdef __cplusplus
}
#endif


/// End of lib -----------------------------------------------------------------

#endif // _LIBDS1302_H
