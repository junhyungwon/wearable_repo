ARM C/C++ CODE GENERATION TOOLS
LINKER GENERATED CRC 
4.9.5
May 2012


INTRODUCTION

The ARM linker provides the capability to generate CRC tables.
The linker will compute a CRC value for the relevant region at link time, 
and store that value in target memory such that it is accessible at boot/run 
time.  The application code can then compute the CRC for each region and 
ensure that the value matches the linker-computed value.

At this time, CRC generation will be implemented for load addresses only.

NOTE: Only the CRC algorithm names and identifiers specified in the RTS header
file crc_tbl.h are currently supported.  All other names and ID values are
reserved for future use.

RESTRICTIONS

1. There is one supported CRC algorithm for ARM. The details of the algorithm
   are available in the MCRC documentation. The algorithm is identified by
   the linker as TMS570_CRC64_ISO. This name will be used as the CRC algorithm 
   specifiers (see below). The intial value for the algorithm is 0.

2. The CRC may only be requested at final link time.

3. CRC may only be applied to initialized sections.


A NOTE ON THE TMS570_CRC64_ISO ALGORITHM

The MCRC module calculates CRC's on 64 bit chunks of data. This is      
accomplished by writing a long long value to two memory mapped          
registers. In C this looks like a normal write of a long long to memory.
The code generated to read/write a long long to memory is something     
like:                                                                   

    LDM R0, {R2, R3}                                                    
    STM R1, {R2, R3}                                                    

where R2 contains the most significant word and R3 contains the least   
significant word. So the most significant word is written to the low    
address and the least significant word is written to the high address.   
                                                                        
The CRC memory mapped registers are in the reverse order from how the   
compiler performs the store. The least significant word is mapped to the
low address and the most significant word is mapped to the high address.
                                                                        
This means that the words are actually swapped before performing the CRC
calculation. It also means that the calculated CRC value has the words  
swapped. The TMS570_CRC64_ISO algorithm takes these issues into         
consideration and performs the swap when calculating the CRC value. The 
computed CRC value stored in the table has the words swapped so the     
value is the same as it is in memory.                                   
                                                                        
For the end user, these details will hopefully be transparent. If the   
runtime CRC routine is written in C, the long long loads and stores will
be generated correctly. The DMA mode of the MCRC module will also work  
correctly.                                                              
                                                                        
Another issue with the algorithm is that it requires the runtime CRC    
calculation to be done with 64-bit chunks. The MCRC module allows       
smaller chunks of data, but the values are padded to 64-bits. The       
TMS570_CRC64_ISO algorithm does not perform any padding, so all CRC     
computations must be done with 64-bit values. The algorithm will        
automatically pad the end of the data with zero's if it does not end
on a 64-bit boundary.                                                    

PROCEDURES

For any section that should be verified with a CRC, the linker command file 
must be modified to include a new operator.  The operator (keyword) is as 
follows.  The specification of a CRC algorithm is optional.

   * crc_table(user_specified_table_name [, algorithm=XXX])

The linker will use the CRC algorithm from any specification given in a 
crc_table() operator.  If that specification is omitted, the default algorithm 
of TMS570_CRC64_ISO will be used.

At this point, the user can include calls in their application to a routine 
which will verify CRC values for relevant sections.  At this time, such a 
routine must be provided by the user.

The linker will include CRC table information in the .map file.  This will 
include the CRC values as well as the algorithm specifier used for the 
calculation.


EXAMPLE

The new table operator is similar in syntax to the copy table operator.

Linker command file:

SECTIONS
{
/* This section can be verified by a CRC check on the load address of the   */
/* section.  ".loadaddr_crc_needed" and "_load_tab1" are user specified     */
/* identifiers.  The keyword crc_table(name) tells the linker to save the   */
/* CRC value for the load address of the associated section in the CRC      */
/* table called name.                                                       */
.loadaddr_crc_needed: {a1.obj(.text)} crc_table(_load_tab1)

/* The CRC algorithm is specified in this case, and will override the       */
/* default value.                                                           */
.loadaddr_crc2   {b1.obj(.text)} load=SLOW_MEM, run=FAST_MEM, 
      crc_table(_load_tab2, algorithm=TMS570_CRC64_ISO)

/* This section can also be verified by a CRC check on the load address of  */
/* the section.  The same identifier, _load_tab1, is specified for this     */
/* table as for section .load_addr_crc_needed above.  The CRC checking      */
/* routine operates on the table name, so one call may verify the CRC       */
/* values for both of these sections.  Note that different CRC algorithm    */
/* IDs can occur in a single table.                                         */
.loadaddr_crc3  {d1.obj(.text)} crc_table(_load_tab1)

/* The CRC tables are created in the special section .TI.crctab, which can  */
/* be specifically placed if desired */
.TI.crctab: { } > CRCMEM
}


Main routine to verify CRC values:

#include "crc_tbl.h"

extern CRC_TABLE load_tab1;
extern CRC_TABLE load_tab2;

main()
{

   /* Verify CRC value for .text sections of a1.obj and d1.obj.            */
   if (my_check_CRC(&load_tab1)) puts("OK");

   /* Verify CRC for .text section of b1.obj - uses CRC8_PRIME             */
   if (!my_check_CRC(&load_tab2))  ERROR;
}


INTERFACE

The CRC generation function uses a mechanism based on the copy table 
functionality.  Using the syntax shown above in the linker command file allows
specification of code/data sections that should have CRC values computed and 
stored in the run time image. 

The tables contain entries of the form (modified from the copy table header):

/*********************************************************/
/* crc_tbl.h                                             */
/* Specification of CRC table data structures which can  */
/* be automatically generated by the linker (using the   */ 
/* crc_table() operator in the LCF).                     */
/*********************************************************/
#include <stdint.h>         /* For uint16_t and uint32_t */

/*********************************************************/
/* CRC Algorithm IDs                                     */
/*********************************************************/
#define TMS570_CRC64_ISO 10

/*********************************************************/
/* CRC Record Data Structure                             */
/* NOTE: The actual size of this structure may vary by   */
/*       target and memory model.                        */
/*********************************************************/
typedef struct crc_record
{
uint64_t         crc_value;
uint32_t         crc_alg_ID;    /* CRC algorithm ID */
uint32_t         addr;          /* Starting address      */
uint32_t         size;          /* size of data in bytes */
uint32_t         padding;
} CRC_RECORD;


/*********************************************************/
/* CRC Table Data Structure                              */
/*********************************************************/
typedef struct crc_table
{
uint32_t rec_size;
uint32_t num_recs;
CRC_RECORD recs[1];
} CRC_TABLE;


A user-supplied routine to verify CRC values should take a table name and 
check the CRC values for all entries in the table.  An outline of such a 
routine follows.

/**************************************************************/
/* General purpose CRC check routine.  Given the address of a */
/* linker-generated CRC_TABLE data structure, verify the CRC  */
/* of all object components that are designated with the      */
/* corresponding LCF crc_table() operator.                    */
/**************************************************************/
#include <crc_tbl.h>

/*********************************************************/
/* CHECK_CRC() - returns 1 if CRCs match, 0 otherwise    */
/*********************************************************/
unsigned int check_crc(CRC_TABLE *tp)
{
   int i;

   for (i = 0; i < tp->num_recs; i++)
   {
      CRC_RECORD* crc_rec = &tp->recs[i];
 
      /**************************************************/
      /* COMPUTE CRC OF DATA STARTING AT crc_rec.addr   */
      /* FOR crc_rec.size UNITS.  USE                   */
      /* crc_rec.crc_alg_ID to select algorithm.        */
      /* COMPARE COMPUTED VALUE TO crc_rec.crc_value.   */
      /**************************************************/
   }  
   if all CRCs match, return 1;
   else return 0;
}


In summary, with a linker command file such as given above, an application 
can verify CRC values against the linker-computed values by accessing the 
CRC tables through the table names (linker generated symbols).  Each table can
contain multiple entries, such as _load_tab1, or a single entry.  If a table 
contains multiple entries, the order of the entries is unspecified.  Each entry 
consists of the CRC algorithm ID, the page and start address for the CRC 
calculation, the size of the data region, and the linker-generated CRC value.  
Specifically, _load_tab1 would contain the algorithm ID used for the text
section of a1.obj, the page and address of that .text section, its size, and 
its CRC value.  The same data for d1.obj would also be in _load_tab1.  This 
information will be provided in the linker .map file for debug purposes.

The tables themselves are grouped in an input section named .TI.crctab.  For 
example, the _load_tab1 table will be placed in an input section called 
.TI.crctab:_load_tab1.  The .TI.crctab section can be placed by the user in 
the output file like any other section.


GROUP And UNION Behavior

A crc_table operator can be placed on a GROUP or a UNION.  The linker lowers
the table specification onto the members of the GROUP or UNION.

UNION
{
    sec1: {} crc_table(tab1, algorithm=alg2)
    sec2:
} crc_table(tab2, algorithm=alg1)

Result: 
tab1 contains 1 entry:   (sec1, alg2)
tab2 contains 2 entries: (sec1, alg1), (sec2, alg1)

These entries can be placed into a single table, if desired.

UNION
{
    sec1: {} crc_table(tab1, algorithm=alg2)
    sec2:
} crc_table(tab1, algorithm=alg1)

Result: 
tab1 contains 3 entries: (sec1, alg2), (sec1, alg1), (sec2, alg1)


