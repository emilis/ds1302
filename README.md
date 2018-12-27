# ds1302(1) -- an interface to DS1302 real-time-clock component

## SYNOPSIS

**ds1302**
**ds1302** _'1999-12-31 23:59:59'_
**ds1302** **start**
**ds1302** **stop**
**ds1302** **read** [ _year_ | _month_ | _day_ | _weekday_ | _hours_ | _minutes_ | _seconds_ ]
**ds1302** **write** [ _year_ | _month_ | _day_ | _weekday_ | _hours_ | _minutes_ | _seconds_ ] _VALUE_

## DESCRIPTION

**ds1302** is an utility to control a DS1302 real-time-clock component.  

## EXAMPLES

Some exmamples

## OVERVIEW

Many options are available to **ds1302** in order to give as much flexibility as possible to the user.

## EXIT STATUS

0   Successful program execution.

1   Usage, syntax or environment error.

2   Operational error.

## ENVIRONMENT

## FILES

_/etc/ds1302.conf_
DS1302 wiring configuration file.

## HISTORY

2018 Created by Emilis Dambauskas (emilis.d@gmail.com).
