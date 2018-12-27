/* Copyright (C) 2018 Emilis Dambauskas
   This file is part of the DS1302 Control Utility.
   Written by Emilis Dambauskas <emilis.d@gmail.com>.

   The DS1302 Control Utility is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The DS1302 Control Utility is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the DS1302 Control Utility; if not, see
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
/// Notes ----------------------------------------------------------------------

/// getenv( "NAME" ) != NULL
/// printf( "format", ... )
/// fprintf( stderr, "format", ... )
/// strcmp( str1, str2 ) == 0
/// strcasecmp( str1, str2 ) == 0


/// Includes -------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libds1302.h"


/// Defines --------------------------------------------------------------------

#define CLK_PIN_DEFAULT 2
#define DAT_PIN_DEFAULT 3
#define CE_PIN_DEFAULT  4

#define DEVICE          ds1302_device ds1302_device


/// Functions ------------------------------------------------------------------

uint8_t get_pin( char *pin_name, uint8_t default_value ){

    char *env_value;
    uint8_t pin_value;

    env_value = getenv( pin_name );

    if( env_value == NULL ){
        return default_value;
    }

    if( 1 == sscanf( env_value, "%hhu", &pin_value )){
        return pin_value;
    } else {
        printf( "Failed to get value of environment variable %s.", *pin_name );
        return default_value;
    }
}


int do_print_date( DEVICE ){

    return printf(
        "20%.2d-%.2d-%.2d %.2d:%.2d:%.2d",
        DS1302_read_year(),
        DS1302_read_month(),
        DS1302_read_mday(),
        DS1302_read_hours(),
        DS1302_read_minutes(),
        DS1302_read_seconds()
    );
}

int do_start( DEVICE ){

    return DS1302_write_clock_halt( 0 );
}

int do_stop( DEVICE ){

    return DS1302_write_clock_halt( 1 );
}

int do_read( DEVICE, int argc, char *argv[] ){

    if( strcmp( argv[2], "year" ) == 0 ){
        return printf( "20%.2d", DS1302_read_year() );
    } else {
        int value;
        value = (
            !strcmp( argv[2], "month" )
                ? DS1302_read_month()
            : !strcmp( argv[2], "day" )
                ? DS1302_read_mday()
            : !strcmp( argv[2], "weekday" )
                ? DS1302_read_weekday()
            : !strcmp( argv[2], "hour" )
                ? DS1302_read_hours()
            : !strcmp( argv[2], "hours" )
                ? DS1302_read_hours()
            : !strcmp( argv[2], "minutes" )
                ? DS1302_read_minutes()
            : !strcmp( argv[2], "seconds" )
                ? DS1302_read_seconds()
            /// TODO: add reading of addresses
            : -1
            /*
            case "read":
                uint8_t *address;
                sscanf( argv[2], "%x", address );
                return printf( "%x", DS1302_read_command( address ));
                break;
            */
        );
        if( value >= 0 ){
            return printf( "%.2d", value );
        } else {
            printf( "Unrecognized field name '%s'", argv[2] );
            exit( 1 );
        }
    }
}

int do_write( DEVICE, int argc, char *argv[] ){

    if( argc > 4 ){
        printf( "Too many arguments for write." );
        exit( 1 );
    } else if( argc < 4 ){
        printf( "Not enough arguments for write." );
        exit( 1 );
    } else if( 0 == strcmp( argv[2], "year" )){
        uint8_t year;
        if( 1 == sscanf( argv[3], "20%2hhu", &year )){
            return DS1302_write_year( year );
        } else {
            printf( "Failed to parse year." );
            exit( 1 );
        }
    } else {
        uint8_t value;

        if( 1 != sscanf( argv[3], "%2hhu", &value )){
            printf( "Failed to parse value." );
            exit( 1 );
        }

        int status;
        status = (
            !strcmp( argv[2], "month" )
                ? DS1302_write_month( value )
            : !strcmp( argv[2], "day" )
                ? DS1302_write_mday( value )
            : !strcmp( argv[2], "weekday" )
                ? DS1302_write_weekday( value )
            : !strcmp( argv[2], "hour" )
                ? DS1302_write_hours( value )
            : !strcmp( argv[2], "hours" )
                ? DS1302_write_hours( value )
            : !strcmp( argv[2], "minutes" )
                ? DS1302_write_minutes( value )
            : !strcmp( argv[2], "seconds" )
                ? DS1302_write_seconds( value )
                : -1
        );

        /// TODO: implement write bytes to addresses
        return status;
    }

    /*
    case "write";
        uint8_t *command;
        uint8_t *value;
        sscanf( argv[2], "%x", command );
        sscanf( argv[3], "%x", value );
        return DS1302_write_and_check( command, value );
        break;
    */
}

int do_write_date( DEVICE, int argc, char *argv[] ){

    uint8_t year, month, mday, hours, minutes, seconds;
    int count;

    count = sscanf(
        argv[1],
        "20%2hhu-%2hhu-%2hhu%*[T ]%2hhu:%2hhu:%2hhu",
        &year, &month, &mday, &hours, &minutes, &seconds
    );

    if( count == 6 ){
        return DS1302_write_date( year, month, mday, hours, minutes, seconds );
    } else {
        printf( "Failed to parse the given timestamp." );
        exit( 1 );
    }
}


/// Main -----------------------------------------------------------------------

int main( int argc, char *argv[] ){

    DEVICE = ds1302_setup(
        get_pin( "DS1302_CLK_PIN",  CLK_PIN_DEFAULT ),
        get_pin( "DS1302_DAT_PIN",  DAT_PIN_DEFAULT ),
        get_pin( "DS1302_CE_PIN",   CE_PIN_DEFAULT )
    );

    if( argc == 1 ){
        return do_print_date( ds1302_device );
    } else {
        return (
            !strcmp( argv[1], "start" )
                ? do_start( ds1302_device )
            : !strcmp( argv[1], "stop" )
                ? do_stop( ds1302_device )
            : !strcmp( argv[1], "read" )
                ? do_read( ds1302_device, argc, argv )
            : !strcmp( argv[1], "write" )
                ? do_write( ds1302_device, argc, argv )
            : argc == 2
                ? do_write_date( ds1302_device, argc, argv )
                : -1
        );
    }
}
