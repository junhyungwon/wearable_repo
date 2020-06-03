/******************************************************************************/
/* lnk_msp430l092.cmd - LINKER COMMAND FILE FOR LINKING MSP430L092 PROGRAMS     */
/*                                                                            */
/*   Usage:  lnk430 <obj files...>    -o <out file> -m <map file> lnk.cmd     */
/*           cl430  <src files...> -z -o <out file> -m <map file> lnk.cmd     */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/* These linker options are for command line linking only.  For IDE linking,  */
/* you should set your linker options in Project Properties                   */
/* -c                                               LINK USING C CONVENTIONS  */
/* -stack  0x0100                                   SOFTWARE STACK SIZE       */
/* -heap   0x0100                                   HEAP AREA SIZE            */
/*                                                                            */
/*----------------------------------------------------------------------------*/


/****************************************************************************/
/* SPECIFY THE SYSTEM MEMORY MAP                                            */
/****************************************************************************/

MEMORY
{
    SFR                     : origin = 0x0000, length = 0x0010
    PERIPHERALS_8BIT        : origin = 0x0010, length = 0x00F0
    PERIPHERALS_16BIT       : origin = 0x0100, length = 0x0100
    RAM                     : origin = 0x2380, length = 0x0080
    INFOA                   : origin = 0x1C00, length = 0x0060
    FLASH                   : origin = 0x1C80, length = 0x0700
    INT00                   : origin = 0x1C60, length = 0x0002
    INT01                   : origin = 0x1C62, length = 0x0002
    INT02                   : origin = 0x1C64, length = 0x0002
    INT03                   : origin = 0x1C66, length = 0x0002
    INT04                   : origin = 0x1C68, length = 0x0002
    INT05                   : origin = 0x1C6A, length = 0x0002
    INT06                   : origin = 0x1C6C, length = 0x0002
    INT07                   : origin = 0x1C6E, length = 0x0002
    INT08                   : origin = 0x1C70, length = 0x0002
    INT09                   : origin = 0x1C72, length = 0x0002
    INT10                   : origin = 0x1C74, length = 0x0002
    INT11                   : origin = 0x1C76, length = 0x0002
    INT12                   : origin = 0x1C78, length = 0x0002
    INT13                   : origin = 0x1C7A, length = 0x0002
    INT14                   : origin = 0x1C7C, length = 0x0002
    RESET                   : origin = 0x1C7E, length = 0x0002
}

/****************************************************************************/
/* SPECIFY THE SECTIONS ALLOCATION INTO MEMORY                              */
/****************************************************************************/

SECTIONS
{
    .bss       : {} > RAM                /* GLOBAL & STATIC VARS              */
    .sysmem    : {} > RAM                /* DYNAMIC MEMORY ALLOCATION AREA    */
    .stack     : {} > RAM (HIGH)         /* SOFTWARE SYSTEM STACK             */

    .text      : {} > FLASH              /* CODE                              */
    .cinit     : {} > FLASH              /* INITIALIZATION TABLES             */
    .const     : {} > FLASH              /* CONSTANT DATA                     */
    .cio       : {} > RAM                /* C I/O BUFFER                      */

    .pinit     : {} > FLASH              /* C++ CONSTRUCTOR TABLES            */

    .infoA     : {} > INFOA              /* MSP430 INFO FLASH MEMORY SEGMENTS */

    .int00   : {} > INT00                /* MSP430 INTERRUPT VECTORS          */
    .int01   : {} > INT01
    .int02   : {} > INT02
    .int03   : {} > INT03
    .int04   : {} > INT04
    .int05   : {} > INT05
    .int06   : {} > INT06
    .int07   : {} > INT07
    .int08   : {} > INT08
    .int09   : {} > INT09
    .int10   : {} > INT10
    .int11   : {} > INT11
    .int12   : {} > INT12
    .int13   : {} > INT13
    .int14   : {} > INT14
    .reset   : {} > RESET              /* MSP430 RESET VECTOR               */ 
}

/****************************************************************************/
/* INCLUDE PERIPHERALS MEMORY MAP                                           */
/****************************************************************************/

-l msp430l092.cmd

