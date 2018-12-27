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

/// Defines --------------------------------------------------------------------

#define CE_OFF          digitalWrite( device.ce_pin, LOW )
#define CE_ON           digitalWrite( device.ce_pin, HIGH )

#define CLK_HI          digitalWrite( device.clk_pin, HIGH )
#define CLK_LO          digitalWrite( device.clk_pin, LOW )

#define DAT_HI          digitalWrite( device.dat_pin, HIGH )
#define DAT_INPUT       pinMode( device.dat_pin, INPUT )
#define DAT_LO          digitalWrite( device.dat_pin, LOW )
#define DAT_OUTPUT      pinMode( device.dat_pin, OUTPUT )
#define DAT_READ        digitalRead( device.dat_pin )

#define DELAY_1         delayMicroseconds( 1 )
#define DELAY_2         delayMicroseconds( 2 )
#define DELAY_5         delayMicroseconds( 5 )

#define DEVICE          ds1302_device device

/// Includes -------------------------------------------------------------------

#include "libds1302.h"
#include <wiringPi.h>


/// Functions ==================================================================

/// Setup ----------------------------------------------------------------------

ds1302_device ds1302_setup( uint8_t clk_pin, uint8_t dat_pin, uint8_t ce_pin ){

    ds1302_device device;

    device.clk_pin = clk_pin;
    device.dat_pin = dat_pin;
    device.ce_pin =  ce_pin;

    ds1302_check_device( device );

    wiringPiSetupGpio();

    pinMode( device.clk_pin, OUTPUT );
    pinMode( device.dat_pin, OUTPUT );
    pinMode( device.ce_pin,  OUTPUT );

    ds1302_stop_transfer( device );

    return device;
}

/// Mode change ----------------------------------------------------------------

void ds1302_start_transfer( DEVICE ){

    CE_ON;
    DELAY_5;
}

void ds1302_stop_transfer( DEVICE ){

	CLK_LO;
	CE_OFF;
	DAT_LO;
}

void ds1302_start_read( DEVICE ){

	DAT_INPUT;
	DELAY_1;
}

void ds1302_start_write( DEVICE ){

	DAT_OUTPUT;
}

/// Low-level ------------------------------------------------------------------

uint8_t ds1302_write_bit( DEVICE, uint8_t bit ){

	if( bit ){
		DAT_HI;
	} else {
		DAT_LO;
	}
	DELAY_1;
	CLK_HI;
	DELAY_2;
	DAT_LO;
	DELAY_1;
	CLK_LO;

	return bit;
}

uint8_t ds1302_read_bit( DEVICE ){

	uint8_t bit = 0;
	bit = DAT_READ;
	DELAY_1;
	CLK_HI;
	DELAY_1;
	CLK_LO;
	DELAY_1;

	return bit;
}

uint8_t ds1302_write_byte( DEVICE, uint8_t byte ){

	for( uint8_t i=0; i<8; i++ ){
		ds1302_write_bit( device, ( byte >> i ) & 1 );
	}

    return byte;
}

uint8_t ds1302_read_byte( DEVICE ){

	uint8_t byte = 0;

	for( uint8_t i=0, b=0; i<8; i++ ){
		b = ds1302_read_bit( device );
		byte |= b << i;
	}

	return byte;
}

/// Command functions ----------------------------------------------------------

uint8_t ds1302_read_command( DEVICE, uint8_t command ){

	uint8_t value;

	ds1302_start_transfer( device );
	ds1302_start_write( device );
	
	ds1302_write_byte( device, command | 0x01 );
	ds1302_start_read( device );

	value = ds1302_read_byte( device );

	ds1302_stop_transfer( device );

	return value;
}

uint8_t ds1302_write_command( DEVICE, uint8_t command, uint8_t value ){

	ds1302_start_transfer( device );
	ds1302_start_write( device );

	ds1302_write_byte( device, command & 0xFE );
	ds1302_write_byte( device, value );

	ds1302_stop_transfer( device );

	return value;
}

uint8_t ds1302_write_and_check( DEVICE, uint8_t command, uint8_t value ){

	uint8_t check_value;

	ds1302_write_command( device, command, value );
	check_value = ds1302_read_command( device, command | 0x01 );

	if( value != check_value ){
		printf( "Values don't match: 0x%x != 0x%x\n", value, check_value );
	}

	return value;
}


/// Check functions ------------------------------------------------------------

uint8_t ds1302_check_range( uint8_t min, uint8_t max, uint8_t value ){

    if( value < min ){
		printf(
            "ERROR: ds1302_check_range got value out of range: %d (should be >= %d)\n",
            value,
            min
        );
		exit( 1 );
    } else if( value > max ){
		printf(
            "ERROR: ds1302_check_range got value out of range: %d (should be <= %d)\n",
            value,
            max
        );
		exit( 1 );
    }

    return value;
}

ds1302_device ds1302_check_device( DEVICE ){

    ds1302_check_range( 2, 27, device.clk_pin );
    ds1302_check_range( 2, 27, device.dat_pin );
    ds1302_check_range( 2, 27, device.ce_pin );

    return device;
}

/// Encode / decode functions --------------------------------------------------

uint8_t ds1302_encode( uint8_t value ){

	return (( value / 10 ) << 4 ) | ( value % 10 );
}

uint8_t ds1302_decode( uint8_t length, uint8_t value ){

    uint8_t decoded = 0;

    decoded +=  value & 0x0f;

    switch( length ){
        case 0:	decoded =   0;      break;
        case 1:	decoded &=  0x01; 	break;
        case 2: decoded &=  0x03; 	break;
        case 3: decoded &=	0x07;	break;
        case 4: break;
        case 5:	decoded +=  (( value & 0x10 ) >> 4 ) * 10;  break;
        case 6:	decoded +=  (( value & 0x30 ) >> 4 ) * 10; 	break;
        case 7:	decoded +=  (( value & 0x70 ) >> 4 ) * 10;	break;
        case 8:	decoded +=  (( value & 0xf0 ) >> 4 ) * 10;	break;
        default:
                printf(
                    "ERROR: ds1302_decode length out of range: %d (should be <= 8)\n",
                    length
                );
                exit( 1 );
    }

    return decoded;
}

/// Read commands --------------------------------------------------------------

uint8_t ds1302_read_seconds( DEVICE ){

    return ds1302_check_range( 0, 59,
            ds1302_decode( 7,
                ds1302_read_command( device, 0x81 )));
}


uint8_t ds1302_read_minutes( DEVICE ){

    return ds1302_check_range( 0, 59,
            ds1302_decode( 7,
                ds1302_read_command( device, 0x83 )));
}

uint8_t ds1302_read_hours( DEVICE ){

    return ds1302_check_range( 0, 24,
            ds1302_decode( 6,
                ds1302_read_command( device, 0x85 )));
}

uint8_t ds1302_read_mday( DEVICE ){

    return ds1302_check_range( 1, 31,
            ds1302_decode( 6,
                ds1302_read_command( device, 0x87 )));
}

uint8_t ds1302_read_month( DEVICE ){

    return ds1302_check_range( 1, 12,
            ds1302_decode( 5,
                ds1302_read_command( device, 0x89 )));
}

uint8_t ds1302_read_weekday( DEVICE ){

    return ds1302_check_range( 1, 7,
            ds1302_decode( 3,
                ds1302_read_command( device, 0x8b )));
}

uint8_t ds1302_read_year( DEVICE ){

    return ds1302_check_range( 0, 99,
            ds1302_decode( 8,
                ds1302_read_command( device, 0x8d )));
}

uint8_t ds1302_read_clock_halt( DEVICE ){

    return ( 0x80 & ds1302_read_command( device, 0x81 )) >> 7;
}

uint8_t ds1302_read_24h_mode( DEVICE ){

    if(( 0x80 & ds1302_read_command( device, 0x85 )) >> 7 ){
        return 0;
    } else {
        return 1;
    }
}

uint8_t ds1302_read_pm( DEVICE ){

    return ( 0x20 & ds1302_read_command( device, 0x85 )) >> 5;
}

uint8_t ds1302_read_write_protect( DEVICE ){

    return ( 0x80 & ds1302_read_command( device, 0x8f )) >> 7;
}

/// Write commands -------------------------------------------------------------

uint8_t ds1302_write_seconds( DEVICE, uint8_t seconds ){

    ds1302_check_range( 0, 59, seconds );

    uint8_t clock_halt = 0x80 & ds1302_read_command( device, 0x81 );

    return ds1302_write_and_check(
            device,
            0x80,
            clock_halt | ds1302_encode( seconds )
    );
}

uint8_t ds1302_write_minutes( DEVICE, uint8_t minutes ){

    ds1302_check_range( 0, 59, minutes );

    return ds1302_write_and_check( device, 0x82, ds1302_encode( minutes ));
}

uint8_t ds1302_write_hours( DEVICE, uint8_t hours ){

    ds1302_check_range( 0, 24, hours );

    /// Always write hours in 24h format:
    return ds1302_write_and_check( device, 0x84, ds1302_encode( hours ));
}

uint8_t ds1302_write_mday( DEVICE, uint8_t mday ){

    ds1302_check_range( 1, 31, mday );

    return ds1302_write_and_check( device, 0x86, ds1302_encode( mday ));
}

uint8_t ds1302_write_month( DEVICE, uint8_t month ){

    ds1302_check_range( 1, 12, month );

    return ds1302_write_and_check( device, 0x88, ds1302_encode( month ));
}

uint8_t ds1302_write_weekday( DEVICE, uint8_t weekday ){

    ds1302_check_range( 1, 7, weekday );

    return ds1302_write_and_check( device, 0x8a, ds1302_encode( weekday ));
}

uint8_t ds1302_write_year( DEVICE, uint8_t year ){

    ds1302_check_range( 0, 99, year );

    return ds1302_write_and_check( device, 0x8c, ds1302_encode( year ));
}

uint8_t ds1302_write_clock_halt( DEVICE, uint8_t ch ){

    uint8_t seconds = ds1302_read_command( device, 0x81 ) & 0x7f;

    return ds1302_write_and_check( device, 0x80, seconds | ( ch & 0x01 ));
}

uint8_t ds1302_write_write_protect( DEVICE, uint8_t wp ){

    return ds1302_write_and_check( device, 0x8e, ( wp & 0x01 ) << 7 );
}

uint8_t ds1302_write_date(
    DEVICE,
    uint8_t year,
    uint8_t month,
    uint8_t mday,
    uint8_t hours,
    uint8_t minutes,
    uint8_t seconds
){
    ds1302_write_write_protect( device, 0 );
    ds1302_write_year( device, year );
    ds1302_write_month( device, month );
    ds1302_write_mday( device, mday );
    ds1302_write_hours( device, hours );
    ds1302_write_minutes( device, minutes );
    ds1302_write_seconds( device, seconds );

    return ( 0
        + year - ds1302_read_year( device )
        + month - ds1302_read_month( device )
        + mday - ds1302_read_mday( device )
        + hours - ds1302_read_hours( device )
        + minutes - ds1302_read_minutes( device )
        + seconds - ds1302_read_seconds( device )
    );
}
