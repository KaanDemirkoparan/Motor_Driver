/*
// TI File $Revision: /main/3 $
// Checkin $Date: Agu 1, 2017   13:45:43 $
//
// FILE:    f28003x_flash_lib_is_eabi.cmd
//
// TITLE:   Linker Command File For F280039 examples that run out of FLASH
//
//
//          Keep in mind that L0~L7 are protected by the code
//          security module.
//
//          What this means is in most cases you will want to move to
//          another memory map file which has more memory defined.
//
*/

MEMORY
{

   BEGIN             : origin = 0x00080000, length = 0x00000002
   BOOT_RSVD		 : origin = 0x00000002, length = 0x00000126

/* RAMLS0        	 : origin = 0x00008000, length = 0x00000800 */
/* RAMLS1        	 : origin = 0x00008800, length = 0x00000800 */
/* RAMLS2            : origin = 0x00009000, length = 0x00000800 */
/* RAMLS3            : origin = 0x00009800, length = 0x00000800 */
/* RAMLS4            : origin = 0x0000A000, length = 0x00000800 */
/* RAMLS5            : origin = 0x0000A800, length = 0x00000800 */
/* RAMLS6            : origin = 0x0000B000, length = 0x00000800 */
/* RAMLS7            : origin = 0x0000B800, length = 0x00000800 */
   RAMLSnP           : origin = 0x00008000, length = 0x00004000

/* RAMGS0            : origin = 0x0000C000, length = 0x00001000 */
/* RAMGS1            : origin = 0x0000D000, length = 0x00001000 */
   RAMGSnP           : origin = 0x0000C000, length = 0x00002000

   /* Flash sectors */
   /* BANK 0 */
/* FLASH_BANK0_SEC0  : origin = 0x080002, length = 0x000FFE */
   FLASHBANK0_BOOT	 : origin = 0x080002, length = 0x000FFE	/* remote update */

/* FLASH_BANK0_SEC1  : origin = 0x081000, length = 0x001000 */
/* FLASH_BANK0_SEC2  : origin = 0x082000, length = 0x001000 */
/* FLASH_BANK0_SEC3  : origin = 0x083000, length = 0x001000 */
/* FLASH_BANK0_SEC4  : origin = 0x084000, length = 0x001000 */
/* FLASH_BANK0_SEC5  : origin = 0x085000, length = 0x001000 */
/* FLASH_BANK0_SEC6  : origin = 0x086000, length = 0x001000 */
/* FLASH_BANK0_SEC7  : origin = 0x087000, length = 0x001000 */
/* FLASH_BANK0_SEC8  : origin = 0x088000, length = 0x001000 */
/* FLASH_BANK0_SEC9  : origin = 0x089000, length = 0x001000 */
/* FLASH_BANK0_SEC10 : origin = 0x08A000, length = 0x001000 */
/* FLASH_BANK0_SEC11 : origin = 0x08B000, length = 0x001000 */
/* FLASH_BANK0_SEC12 : origin = 0x08C000, length = 0x001000 */
/* FLASH_BANK0_SEC13 : origin = 0x08D000, length = 0x001000 */
/* FLASH_BANK0_SEC14 : origin = 0x08E000, length = 0x001000 */
   FLASHBANK0_CODE	: origin = 0x081000, length = 0x00E000	/* control code */

/* FLASH_BANK0_SEC15 : origin = 0x08F000, length = 0x001000 */
   FLASHBANK0_DATA 	: origin = 0x08F000, length = 0x001000	/* constant data */

   /* BANK 1 */
   FLASH_BANK1_SEC0  : origin = 0x090000, length = 0x001000

/* FLASH_BANK1_SEC1  : origin = 0x091000, length = 0x001000 */
/* FLASH_BANK1_SEC2  : origin = 0x092000, length = 0x001000 */
/* FLASH_BANK1_SEC3  : origin = 0x093000, length = 0x001000 */
/* FLASH_BANK1_SEC4  : origin = 0x094000, length = 0x001000 */
/* FLASH_BANK1_SEC5  : origin = 0x095000, length = 0x001000 */
/* FLASH_BANK1_SEC6  : origin = 0x096000, length = 0x001000 */
/* FLASH_BANK1_SEC7  : origin = 0x097000, length = 0x001000 */
/* FLASH_BANK1_SEC8  : origin = 0x098000, length = 0x001000 */
/* FLASH_BANK1_SEC9  : origin = 0x099000, length = 0x001000 */
/* FLASH_BANK1_SEC10 : origin = 0x09A000, length = 0x001000 */
/* FLASH_BANK1_SEC11 : origin = 0x09B000, length = 0x001000 */
/* FLASH_BANK1_SEC12 : origin = 0x09C000, length = 0x001000 */
/* FLASH_BANK1_SEC13 : origin = 0x09D000, length = 0x001000 */
/* FLASH_BANK1_SEC14 : origin = 0x09E000, length = 0x001000 */
   FLASHBANK1_CODE	 : origin = 0x091000, length = 0x00E000	/* control code */

/* FLASH_BANK1_SEC15 : origin = 0x09F000, length = 0x001000 */
   FLASHBANK1_DATA 	 : origin = 0x09F000, length = 0x001000	/* constant data */

  /* BANK 2 */
   FLASH_BANK2_SEC0  : origin = 0x0A0000, length = 0x001000

/* FLASH_BANK2_SEC1  : origin = 0x0A1000, length = 0x001000 */
/* FLASH_BANK2_SEC2  : origin = 0x0A2000, length = 0x001000 */
/* FLASH_BANK2_SEC3  : origin = 0x0A3000, length = 0x001000 */
/* FLASH_BANK2_SEC4  : origin = 0x0A4000, length = 0x001000 */
/* FLASH_BANK2_SEC5  : origin = 0x0A5000, length = 0x001000 */
/* FLASH_BANK2_SEC6  : origin = 0x0A6000, length = 0x001000 */
/* FLASH_BANK2_SEC7  : origin = 0x0A7000, length = 0x001000 */
/* FLASH_BANK2_SEC8  : origin = 0x0A8000, length = 0x001000 */
/* FLASH_BANK2_SEC9  : origin = 0x0A9000, length = 0x001000 */
/* FLASH_BANK2_SEC10 : origin = 0x0AA000, length = 0x001000 */
/* FLASH_BANK2_SEC11 : origin = 0x0AB000, length = 0x001000 */
/* FLASH_BANK2_SEC12 : origin = 0x0AC000, length = 0x001000 */
/* FLASH_BANK2_SEC13 : origin = 0x0AD000, length = 0x001000 */
/* FLASH_BANK2_SEC14 : origin = 0x0AE000, length = 0x001000 */
   FLASHBANK2_CODE 	 : origin = 0x0A1000, length = 0x00E000	/* constant data */

/* FLASH_BANK2_SEC15 : origin = 0x0AF000, length = 0x000FF0 */
// FLASH_BANK0_SEC15_RSVD     : origin = 0x0AFFF0, length = 0x000010  /* Reserve and do not use for code as per the errata advisory "Memory: Prefetching Beyond Valid Memory" */
   FLASHBANK2_DATA 	 : origin = 0x0AF000, length = 0x000FF0	/* constant data */

   BOOTROM           : origin = 0x003F8000, length = 0x00007FC0
   SECURE_ROM        : origin = 0x003F2000, length = 0x00006000

   RESET             : origin = 0x003FFFC0, length = 0x00000002


   RAMM0S          	 : origin = 0x00000128, length = 0x000002D8	/* stack */
   RAMM1D         	 : origin = 0x00000400, length = 0x000003F8	/* on-chip RAM block M1 */
// RAMM1_RSVD        : origin = 0x000007F8, length = 0x00000008 /* Reserve and do not use for code as per the errata advisory "Memory: Prefetching Beyond Valid Memory" */

/* RAMGS2            : origin = 0x0000E000, length = 0x00001000 */
   RAMGSC            : origin = 0x0000E000, length = 0x00000800
   RAMGSF            : origin = 0x0000E800, length = 0x00000800
/* RAMGS3            : origin = 0x0000F000, length = 0x00000FF8 */
   RAMGSD            : origin = 0x0000F000, length = 0x00000FF8
// RAMGS3_RSVD       : origin = 0x0000FFF8, length = 0x00000008 /* Reserve and do not use for code as per the errata advisory "Memory: Prefetching Beyond Valid Memory" */

}


SECTIONS
{
   codestart		: > BEGIN,     		 	   	ALIGN(4)
   .reset           : > RESET, 				   	TYPE = DSECT

   GROUP
   {
       .TI.ramfunc {
#if defined(SFRA_ENABLE)
       -l sfra_f32_tmu_coff.lib<sfra_f32_tmu_collect.obj> (.text)
       -l sfra_f32_tmu_coff.lib<sfra_f32_tmu_inject.obj> (.text)
#endif
       }
       ramfuncs
       	/* Digital Controller Library functions */
       dclfuncs
       dcl32funcs
   }          LOAD = FLASHBANK0_CODE,
              RUN = RAMLSnP,
              LOAD_START(RamfuncsLoadStart),
              LOAD_SIZE(RamfuncsLoadSize),
              LOAD_END(RamfuncsLoadEnd),
              RUN_START(RamfuncsRunStart),
              RUN_SIZE(RamfuncsRunSize),
              RUN_END(RamfuncsRunEnd),
              ALIGN(8)

	ctrlfuncs : {
	            }
	          LOAD = FLASHBANK0_CODE,
              RUN = RAMLSnP,
              LOAD_START(loadStart_ctrlfuncs),
              LOAD_END(loadEnd_ctrlfuncs),
              LOAD_SIZE(loadSize_ctrlfuncs),
              RUN_START(runStart_ctrlfuncs),
              RUN_END(runEnd_ctrlfuncs),
              RUN_SIZE(runSize_ctrlfuncs),
              ALIGN(8)

   .text            : > FLASHBANK0_CODE,	ALIGN(8)
   .cinit           : > FLASHBANK0_CODE,	ALIGN(4)
   .switch          : > FLASHBANK0_CODE,	ALIGN(4)
   .cio				: > FLASHBANK0_CODE
   .pinit           : > FLASHBANK0_CODE,	ALIGN(4)
   .const           : > FLASHBANK0_CODE,  	ALIGN(4)
   .init_array      : > FLASHBANK0_CODE, 	ALIGN(4)

   .stack           : > RAMM0S
   .sysmem          : > RAMM0S
   .bss             : > RAMM1D
   .bss:output      : > RAMM1D
   .bss:cio         : > RAMM1D
   .data            : > RAMM1D

   est_data			: > RAMGSF
                        LOAD_START(loadStart_est_data),
                        LOAD_END(loadEnd_est_data),
                        LOAD_SIZE(loadSize_est_data)

        /*  Allocate IQ math areas: */
   IQmath           : > FLASHBANK0_CODE, 	ALIGN(8)
   IQmathTables     : > FLASHBANK0_CODE,	ALIGN(8)
}

SECTIONS
{
   prms_data 		: > FLASHBANK0_DATA

   hal_data			:> 	RAMM1D
                        LOAD_START(loadStart_hal_data),
                        LOAD_END(loadEnd_hal_data),
                        LOAD_SIZE(loadSize_hal_data)

   foc_data			:> 	RAMM1D
                        LOAD_START(loadStart_foc_data),
                        LOAD_END(loadEnd_foc_data),
                        LOAD_SIZE(loadSize_foc_data)

   motor_data		:> 	RAMGSC
                        LOAD_START(loadStart_motor_data),
                        LOAD_END(loadEnd_motor_data),
                        LOAD_SIZE(loadSize_motor_data)

   user_data		:> 	RAMGSC
                        LOAD_START(loadStart_user_data),
                        LOAD_END(loadEnd_user_data),
                        LOAD_SIZE(loadSize_user_data)

   sys_data	    	:> 	RAMGSC
                        LOAD_START(loadStart_sys_data),
                        LOAD_END(loadEnd_sys_data),
                        LOAD_SIZE(loadSize_sys_data)

   vibc_data    	:> 	RAMGSD
                        LOAD_START(loadStart_vibc_data),
                        LOAD_END(loadEnd_vibc_data),
                        LOAD_SIZE(loadSize_vibc_data)

   dmaBuf_data    	:> 	RAMGSD
                        LOAD_START(loadStart_dmaBuf_data),
                        LOAD_END(loadEnd_dmaBuf_data),
                        LOAD_SIZE(loadSize_dmaBuf_data)

   datalog_data    	:> 	RAMGSD
                        LOAD_START(loadStart_datalog_data),
                        LOAD_END(loadEnd_datalog_data),
                        LOAD_SIZE(loadSize_datalog_data)

   SFRA_F32_Data   	:> 	RAMGSD
                        LOAD_START(loadStart_SFRA_F32_Data),
                        LOAD_END(loadEnd_SFRA_F32_Data),
                        LOAD_SIZE(loadSize_SFRA_F32_Data)
}

/*
//===========================================================================
// End of file.
//===========================================================================
*/
