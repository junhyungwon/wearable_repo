ARM C/C++ CODE GENERATION TOOLS
Release Notes
4.9.5
May 2012

===============================================================================
Contents
===============================================================================
1) Support Information
   1.1) Defect History
   1.2) Compiler Documentation Errata
   1.3) TI E2E Community
   1.4) Defect Tracking Database
2) Cortex-M4 Support
3) C99 Floating Point Routines
4) Execute Only Code (--embedded_constants=[on|off])
5) Constant Expressions in Memory Directives
6) SET_DATA_SECTION and SET_CODE_SECTION pragmas
7) Linker generated CRC tables
8) Compiler update for Silicon Erratum Cortex-R4#57 for
   TMS570LS3xxx


-------------------------------------------------------------------------------
1) Support Information
-------------------------------------------------------------------------------

1.1) Defect History
-------------------

The list of defects fixed in this release as well as known issues can
be found in the file DefectHistory.txt.

1.2) Compiler Documentation Errata
----------------------------------

Errata from the "ARM Optimizing Compiler User's Guide" and the
"ARM Assembly Language User's Guide" is available online at the
Texas Instruments Embedded Processors CG Wiki:

http://tiexpressdsp.com/wiki/index.php?title=Category:CGT

under the 'Compiler Documentation Errata' link.

This Wiki has been established to assist developers in using TI Embedded
Processor Software and Tools.  Developers are encouraged to dig through all
the articles.  Registered users can update missing or incorrect information.

1.3) TI E2E Community
---------------------

Questions concerning TI Code Generation Tools can be posted to the TI E2E
Community forums.  The "Development Tools" forum can be found at:

http://e2e.ti.com/support/development_tools/f/default.aspx

1.4) Defect Tracking Database
-----------------------------

Compiler defect reports can be tracked at the Development Tools bug
database, SDOWP. The log in page for SDOWP, as well as a link to create
an account with the defect tracking database is found at:

https://cqweb.ext.ti.com/pages/SDO-Web.html

A my.ti.com account is required to access this page.  To find an issue
in SDOWP, enter your bug id in the "Find Record ID" box once logged in.
To find tables of all compiler issues click the queries under the folder:

"Public Queries" -> "Development Tools" -> "TI C-C++ Compiler"

With your SDOWP account you can save your own queries in your
"Personal Queries" folder.

1.5) Compiler Documentation
---------------------------
Links to the current compiler documentation can be found here:
http://processors.wiki.ti.com/index.php/TI_Compiler_Information#Compiler_Manuals

-------------------------------------------------------------------------------
2.) Cortex-M4 Support

-------------------------------------------------------------------------------

This release includes support for the Cortex-M4 device as well as support
for the FPv4-SPD16 floating point extension. Included in this release:

1.) Assembly support for all Cortex-M4 instructions
2.) Code generation of applicable Cortex-M4 instructions from C/C++
3.) Assembly support for all FPv4-SPD16 instructions. This includes the 
    fused multiply-add instructions and the 16-bit <-> 32-bit floating point
    conversion instructions.
4.) Code generation of applicable FPv4-SPD16 instructions from C/C++

The new support is enabled through the use of two options. 

--silicon_version=7M4     - Enables support for the core Cortex-M4 instructions
--float_support=FPv4SPD16 - Enables support for the FPv4-SPD16 extension. 

The --abi=eabi option must be used when using floating point support. Floating
point support is turned on by default for Cortex-M4. If the user does not
want to use the floating point unit, the option --float_support=fpalib can
be specified on the command line.

-------------------------------------------------------------------------------
3.) C99 Floating Point routines

-------------------------------------------------------------------------------

The additional C99 floating point functions are included in this release.
This includes functions like tanf(), sinf(), powf(), ... The motivation
for these functions is to provide single precision floating point 
implementations of the RTS functions. This should improve execution time
on applications that use single precision floating point types (float).
To use the routines, the user must call the appropriate function. If a user
calls tan(), the double precision version will be used. The user must call
tanf() to obtain the benefits.

-------------------------------------------------------------------------------
4.) Execute Only Code (--embedded_constants=[on|off])

-------------------------------------------------------------------------------

By default the compiler will embed constants in functions. These constants
can include literals, addresses, strings, etc. This is a problem if the user
wants to prevent reads from a memory region that contains only executable code.
To enable the user to generate "execute only code", the compiler provides the
--embedded_constants=[on|off] option. If the option is not specified, it is
assumed to be on. The option is only available on Cortex-M devices.

-------------------------------------------------------------------------------
5.) Constant expressions in memory range specifiers

-------------------------------------------------------------------------------

This feature extends the functionality of the memory region construct
in linker command files.

Memory range origin and length can now use expressions of integer 
constants with below operators:
      binary op: * / % + - << >> == != < <= > >= & | && || 
      unary op:  - ~ !
    
Expressions are evaluated using standard C operator precedence rules.
    
No checking is done for overflow or underflow, however, expressions are 
evaluated using a larger integer type.

Preprocess directive #define constants can be used in place of integer 
constants. Global symbols can not be used in Memory Directive 
expressions.

Three new address operators have been added for referencing memory range 
properties from prior memory range entries:

        START(MR)  Returns start address for previously defined memory range.
        SIZE(MR)   Returns size of previously defined memory range.
        END(MR)    Returns end address for previously defined memory range.

Example linker command file:

#define ROM_START 0x00000020
#define ROM_LEN   0x00100000

MEMORY
{
   ROM1 (RX): origin = ROM_START length = ROM_LEN - 0x8000
   ROM2 (RW): origin = END(ROM1) length = ROM_LEN - SIZE(ROM1)
}

-------------------------------------------------------------------------------
6.) SET_DATA_SECTION and SET_CODE_SECTION pragmas.

-------------------------------------------------------------------------------
Two new pragmas have been added to the compiler, SET_DATA_SECTION and 
SET_CODE_SECTION. These pragmas can be used to set the section for all
declarations below the pragma. 

Example:

#pragma SET_DATA_SECTION("mydata")
int x;
int y;
#pragma SET_DATA_SECTION()

In the example above, x and y will both be put in the section mydata. To
reset the current section to the default used by the compiler, a blank
paramater should be passed to the pragma. An easy way to think of the pragma
is that it is like applying the CODE_SECTION or DATA_SECTION pragma to all
symbols below it.

The pragmas apply to both declarations and definitions. If applied to a
declaration and not the definition, the pragma that is active at the
declaration is used to set the section for that symbol. Here is an example:

#pragma SET_CODE_SECTION("func1")
extern void func1();
#pragma SET_CODE_SECTION()
...
void func1() { ... } 

In this example func1 will end up in section func1. If conflicting sections are
specified at the declaration and definition, a diagnostic will be issued.

The current CODE_SECTION and DATA_SECTION pragmas and GCC attributes can be
used to override the SET_CODE_SECTION and SET_DATA_SECTION pragmas. For example:

#pragma DATA_SECTION(x, "x_data")
#pragma SET_DATA_SECTION("mydata")
int x;
int y;
#pragma SET_DATA_SECTION()

In this example, x will be placed in x_data and y will be placed in mydata. No
diagnostic is issued for this case.

The pragmas work for both C and C++. In C++, the pragmas are ignored for
templates and for implictly created objects, such as implicit constructors
and virtual function tables.

-------------------------------------------------------------------------------
7.) Linker generated CRC tables

-------------------------------------------------------------------------------
Please refer to the LINKER_GENERATED_CRC_README.txt file for more information
on this feature.

-------------------------------------------------------------------------------
8.) Compiler update for Silicon Erratum Cortex-R4#57 for TMS570LS3xxx
-------------------------------------------------------------------------------

Please reference the Gladiator errata SPNZ181, Cortex-R4#57
http://www.ti.com/lit/er/spnz181a/spnz181a.pdf

The --arm_vmrs_si_workaround=on|off option was added to prevent a conditional
VMRS instruction from being generated by the compiler.  By default, the option
is enabled (on).  A warning will be emitted if a developer attempts to add a
conditional VMRS instruction to the assembly file when the option is enabled.
The developer can turn the option off using --arm_vmrs_si_workaround=off.
