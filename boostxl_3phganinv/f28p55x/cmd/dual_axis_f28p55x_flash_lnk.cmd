//#############################################################################
//
// FILE:    dual_axis_f28004x_flash_lnk_cpu1.cmd.cmd
//
//#############################################################################
// $Copyright:
// Copyright (C) 2017-2025 Texas Instruments Incorporated - http://www.ti.com/
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//   Redistributions of source code must retain the above copyright
//   notice, this list of conditions and the following disclaimer.
//
//   Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the
//   documentation and/or other materials provided with the
//   distribution.
//
//   Neither the name of Texas Instruments Incorporated nor the names of
//   its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// $
//#############################################################################
// In addition to this memory linker command file,
// add the header linker command file directly to the project.
// The header linker command file is required to link the
// peripheral structures to the proper locations within
// the memory map.
//
// For BIOS applications add:      F28004x_Headers_BIOS.cmd
// For nonBIOS applications add:   F28004x_Headers_nonBIOS.cmd
//
// The user must define CLA_C in the project linker settings if using the
// CLA C compiler
// Project Properties -> C2000 Linker -> Advanced Options -> Command File
// Preprocessing -> --define
#ifdef CLA_C
// Define a size for the CLA scratchpad area that will be used
// by the CLA compiler for local symbols and temps
// Also force references to the special symbols that mark the
// scratchpad are.
CLA_SCRATCHPAD_SIZE = 0x100;
--undef_sym=__cla_scratchpad_end
--undef_sym=__cla_scratchpad_start
#endif //CLA_C


MEMORY
{
   BEGIN            : origin = 0x080000, length = 0x000002  // Update the codestart location as needed

   BOOT_RSVD        : origin = 0x000002, length = 0x000126     /* Part of M0, BOOT rom will use this for stack */
   RAMM0            : origin = 0x000128, length = 0x0002D8
   RAMM1            : origin = 0x000400, length = 0x000400

   RAMLS0           : origin = 0x008000, length = 0x000800
   RAMLS1           : origin = 0x008800, length = 0x000800
   RAMLS2           : origin = 0x009000, length = 0x000800
   RAMLS3           : origin = 0x009800, length = 0x000800
   RAMLS4           : origin = 0x00A000, length = 0x000800
   RAMLS5           : origin = 0x00A800, length = 0x000800

   RAMLS6_7         : origin = 0x00B000, length = 0x001000
   // RAMLS6           : origin = 0x00B000, length = 0x000800
   // RAMLS7           : origin = 0x00B800, length = 0x000800

   RAMLS8           : origin = 0x014000, length = 0x002000  // When configured as CLA program use the address 0x4000
   RAMLS9           : origin = 0x016000, length = 0x002000  // When configured as CLA program use the address 0x6000

   // RAMLS8_CLA    : origin = 0x004000, length = 0x002000  // Use only if configured as CLA program memory
   // RAMLS9_CLA    : origin = 0x006000, length = 0x002000  // Use only if configured as CLA program memory

   RAMGS0           : origin = 0x00C000, length = 0x002000
   RAMGS1           : origin = 0x00E000, length = 0x002000
   RAMGS2           : origin = 0x010000, length = 0x002000
   RAMGS3           : origin = 0x012000, length = 0x002000

   /* Flash Banks (128 sectors each) */
   FLASH_BANK0     : origin = 0x080002, length = 0x1FFFE
   FLASH_BANK1     : origin = 0x0A0000, length = 0x20000
   FLASH_BANK2     : origin = 0x0C0000, length = 0x20000
   FLASH_BANK3     : origin = 0x0E0000, length = 0x20000
   FLASH_BANK4     : origin = 0x100000, length = 0x20000


   CLATOCPURAM      : origin = 0x001480,   length = 0x000080
   CPUTOCLARAM      : origin = 0x001500,   length = 0x000080
   CLATODMARAM      : origin = 0x001680,   length = 0x000080
   DMATOCLARAM      : origin = 0x001700,   length = 0x000080

   RESET            : origin = 0x3FFFC0, length = 0x000002
}


SECTIONS
{
   /* Setup for "boot to SARAM" mode:
      The codestart section (found in DSP28_CodeStartBranch.asm)
      re-directs execution to the start of user code.  */
   codestart        : > BEGIN
   .text            : >> FLASH_BANK2 | FLASH_BANK3, ALIGN(4)
   .cinit           : > FLASH_BANK4, ALIGN(4)
   .switch          : > FLASH_BANK4
   .reset           : > RESET,	TYPE = DSECT /* not used, */
   .stack           : > RAMM1

#if defined(__TI_EABI__)
   .init_array      : > FLASH_BANK4, ALIGN(4)
   .bss             : > RAMGS3, ALIGN(4)
   .bss:output      : > RAMGS3, ALIGN(4)
   .bss:cio         : > RAMGS3, ALIGN(4)
   .data            : > RAMGS3, ALIGN(4)
   .sysmem          : > RAMGS3, ALIGN(4)
   /* Initalized sections go in Flash */
   .const           : > FLASH_BANK4, ALIGN(4)
#else
   .pinit           : > FLASH_BANK4,   	PAGE = 0, ALIGN(4)
   .ebss            : > RAMGS3,    	   	PAGE = 1
   .esysmem         : > RAMGS3,        	PAGE = 1
   .cio             : > RAMGS3,       	PAGE = 1
   .econst          : > FLASH_BANK4,   	PAGE = 0, ALIGN(4)
#endif

#if defined(__TI_EABI__)
    .TI.ramfunc : {} LOAD = FLASH_BANK1,
                     RUN = RAMLS6_7,
                     LOAD_START(RamfuncsLoadStart),
                     LOAD_SIZE(RamfuncsLoadSize),
                     LOAD_END(RamfuncsLoadEnd),
                     RUN_START(RamfuncsRunStart),
                     RUN_SIZE(RamfuncsRunSize),
                     RUN_END(RamfuncsRunEnd),
					 ALIGN(4)
#else
    .TI.ramfunc : {} LOAD = FLASH_BANK1,
                     RUN = RAMLS6_7,
                     LOAD_START(RamfuncsLoadStart),
                     LOAD_SIZE(RamfuncsLoadSize),
                     LOAD_END(RamfuncsLoadEnd),
                     RUN_START(RamfuncsRunStart),
                     RUN_SIZE(RamfuncsRunSize),
                     RUN_END(RamfuncsRunEnd),
					 PAGE = 0, ALIGN(4)
#endif

#if defined(__TI_EABI__)
    /* CLA specific sections */
   Cla1Prog         : LOAD = FLASH_BANK1,
                      RUN = RAMLS4 | RAMLS5,
                      LOAD_START(Cla1funcsLoadStart),
                      LOAD_END(Cla1funcsLoadEnd),
                      RUN_START(Cla1funcsRunStart),
                      LOAD_SIZE(Cla1funcsLoadSize),
                      ALIGN(4)
#else
    /* CLA specific sections */
   Cla1Prog         : LOAD = FLASH_BANK1,
                      RUN = RAMLS4 | RAMLS5,
                      LOAD_START(Cla1funcsLoadStart),
                      LOAD_END(Cla1funcsLoadEnd),
                      RUN_START(Cla1funcsRunStart),
                      LOAD_SIZE(Cla1funcsLoadSize),
                      PAGE = 0, ALIGN(4)
#endif

   ClaData          : > RAMLS3, ALIGN=2

   Cla1ToCpuMsgRAM  : > CLATOCPURAM
   CpuToCla1MsgRAM  : > CPUTOCLARAM

    /* SFRA specific sections */
   SFRA_F32_Data    : > RAMGS3, ALIGN = 64
   sfra_data    	: > RAMGS3
   
#ifdef CLA_C
  /* CLA C compiler sections */
   //
   // Must be allocated to memory the CLA has write access to
   //
   CLAscratch       :
                        { *.obj(CLAscratch)
                        . += CLA_SCRATCHPAD_SIZE;
                        *.obj(CLAscratch_end) } >  RAMLS2,  ALIGN=2

   .scratchpad      : > RAMLS2
   .bss_cla		    : > RAMLS2
   .const_cla	    :  LOAD = FLASH_BANK0_SEC3,
                       RUN = RAMLS4 | RAMLS5,
                       RUN_START(Cla1ConstRunStart),
                       LOAD_START(Cla1ConstLoadStart),
                       LOAD_SIZE(Cla1ConstLoadSize),
  					   ALIGN(2)
#endif //CLA_C
}

//===========================================================================
// End of file.
//===========================================================================
