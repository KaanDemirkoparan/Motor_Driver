/*
// FILE:    f28002x_flash_lib_is_eabi.cmd
//
// TITLE:   Linker Command File For F280025 examples that run out of Flash
//
//
//          Keep in mind that L4, L5,L6 and L7 are protected by the code
//          security module.
//
//          What this means is in most cases you will want to move to
//          another memory map file which has more memory defined.
//
*/

MEMORY
{
   BEGIN           	  : origin = 0x00080000, length = 0x00000002
   BOOT_RSVD	      : origin = 0x00000002, length = 0x00000126

/* RAMLS5        	  : origin = 0x0000A800, length = 0x00000800 */
/* RAMLS6    		  : origin = 0x0000B000, length = 0x00000800 */
/* RAMLS7        	  : origin = 0x0000B800, length = 0x00000800 */
   RAMLS567    		  : origin = 0x0000A600, length = 0x00001A00

   /* Flash sectors */
   /* BANK 0 */
/* FLASHBANK0_SECT0	  : origin = 0x00080002, length = 0x00000FFE */
   FLASHBANK0_BOOT	  : origin = 0x00080002, length = 0x00000FFE	/* remote update */
/* FLASHBANK0_SECT1	  : origin = 0x00081000, length = 0x00001000 */
/* FLASHBANK0_SECT2	  : origin = 0x00082000, length = 0x00001000 */
/* FLASHBANK0_SECT3	  : origin = 0x00083000, length = 0x00001000 */
/* FLASHBANK0_SECT4	  : origin = 0x00084000, length = 0x00001000 */
/* FLASHBANK0_SECT5	  : origin = 0x00085000, length = 0x00001000 */
/* FLASHBANK0_SECT6	  : origin = 0x00086000, length = 0x00001000 */
/* FLASHBANK0_SECT7	  : origin = 0x00087000, length = 0x00001000 */
/* FLASHBANK0_SECT8	  : origin = 0x00088000, length = 0x00001000 */
/* FLASHBANK0_SECT9	  : origin = 0x00089000, length = 0x00001000 */
/* FLASHBANK0_SECT10  : origin = 0x0008A000, length = 0x00001000 */
/* FLASHBANK0_SECT11  : origin = 0x0008B000, length = 0x00001000 */
/* FLASHBANK0_SECT12  : origin = 0x0008C000, length = 0x00001000 */
/* FLASHBANK0_SECT13  : origin = 0x0008D000, length = 0x00001000 */
   FLASHBANK0_CODE	  : origin = 0x00081000, length = 0x0000D000	/* control code */

/* FLASHBANK0_SECT14  : origin = 0x0008E000, length = 0x00001000 */
   FLASHBANK0_DATA 	  : origin = 0x0008E000, length = 0x00001000	/* constant data */

   FLASHBANK0_PRMS    : origin = 0x0008F000, length = 0x000FF0
   FLASHBANK0_SEC15_RSVD	: origin = 0x08FFF0, length = 0x000010  /* Reserve and do not use for code as per the errata advisory "Memory: Prefetching Beyond Valid Memory" */

   BOOTROM			  : origin = 0x003F0000, length = 0x00008000
   BOOTROM_EXT		  : origin = 0x003F8000, length = 0x00007FC0
   RESET           	  : origin = 0x003FFFC0, length = 0x00000002

   RAMM0S          	  : origin = 0x00000128, length = 0x00000118	/* stack, on-chip RAM block M0 part */
   RAMM1D         	  : origin = 0x00000240, length = 0x000005B8	/* on-chip RAM block M0 part & M1 */
   RAMM1_RSVD         : origin = 0x000007F8, length = 0x00000008    /* Reserve and do not use for code as per the errata advisory "Memory: Prefetching Beyond Valid Memory" */

   RAMGS0     	   	  : origin = 0x0000C000, length = 0x000007F8
   RAMGS0_RSVD        : origin = 0x0000C7F8, length = 0x00000008    /* Reserve and do not use for code as per the errata advisory "Memory: Prefetching Beyond Valid Memory" */

   RAMLS4        	  : origin = 0x0000A000, length = 0x00000600	/* Reserve for FAST library */
}


SECTIONS
{
   .reset           : > RESET, 				   	TYPE = DSECT
   codestart		: > BEGIN,     		 	   	ALIGN(4)


   GROUP
   {
#if defined(SFRA_ENABLE)
       .TI.ramfunc
       {
         -l sfra_f32_tmu_eabi.lib<sfra_f32_tmu_collect.obj> (.text)
         -l sfra_f32_tmu_eabi.lib<sfra_f32_tmu_inject.obj> (.text)
       }
#else
	   .TI.ramfunc
#endif
       ramfuncs
   }          LOAD = FLASHBANK0_CODE
              RUN = RAMLS567,
              LOAD_START(RamfuncsLoadStart),
              LOAD_SIZE(RamfuncsLoadSize),
              LOAD_END(RamfuncsLoadEnd),
              RUN_START(RamfuncsRunStart),
              RUN_SIZE(RamfuncsRunSize),
              RUN_END(RamfuncsRunEnd),
              ALIGN(8)

	ctrlfuncs : {
	            }
	          LOAD = FLASHBANK0_CODE
              RUN = RAMLS567,
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
   .bss             : > RAMM1D
   .bss:output      : > RAMM1D
   .bss:cio         : > RAMM1D
   .data            : > RAMM1D
   .sysmem          : > RAMM1D

   est_data     	: > RAMLS4
                        LOAD_START(loadStart_est_data),
                        LOAD_END(loadEnd_est_data),
                        LOAD_SIZE(loadSize_est_data)
}

SECTIONS
{
   prms_data		: > FLASHBANK0_DATA

   hal_data			:> 	RAMM1D
                        LOAD_START(loadStart_hal_data),
                        LOAD_END(loadEnd_hal_data),
                        LOAD_SIZE(loadSize_hal_data)

   foc_data			:> 	RAMM1D
                        LOAD_START(loadStart_foc_data),
                        LOAD_END(loadEnd_foc_data),
                        LOAD_SIZE(loadSize_foc_data)

   motor_data		:> 	RAMM1D
                        LOAD_START(loadStart_motor_data),
                        LOAD_END(loadEnd_motor_data),
                        LOAD_SIZE(loadSize_motor_data)

   user_data		:> 	RAMGS0
                        LOAD_START(loadStart_user_data),
                        LOAD_END(loadEnd_user_data),
                        LOAD_SIZE(loadSize_user_data)

   sys_data	    	:> 	RAMGS0
                        LOAD_START(loadStart_sys_data),
                        LOAD_END(loadEnd_sys_data),
                        LOAD_SIZE(loadSize_sys_data)

   vibc_data    	:> 	RAMGS0
                        LOAD_START(loadStart_vibc_data),
                        LOAD_END(loadEnd_vibc_data),
                        LOAD_SIZE(loadSize_vibc_data)

   dmaBuf_data    	:> 	RAMGS0
                        LOAD_START(loadStart_dmaBuf_data),
                        LOAD_END(loadEnd_dmaBuf_data),
                        LOAD_SIZE(loadSize_dmaBuf_data)

   datalog_data    	:> 	RAMGS0
                        LOAD_START(loadStart_datalog_data),
                        LOAD_END(loadEnd_datalog_data),
                        LOAD_SIZE(loadSize_datalog_data)

   SFRA_F32_Data   	:> 	RAMGS0
                        LOAD_START(loadStart_SFRA_F32_Data),
                        LOAD_END(loadEnd_SFRA_F32_Data),
                        LOAD_SIZE(loadSize_SFRA_F32_Data)
}

/*
//===========================================================================
// End of file.
//===========================================================================
*/
