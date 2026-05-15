MEMORY
{
   BEGIN            : origin = 0x00000000, length = 0x00000002
   BOOT_RSVD        : origin = 0x00000002, length = 0x00000126

   RAMM0            : origin = 0x00000128, length = 0x000002D8
   RAMM1            : origin = 0x00000400, length = 0x000003F8
   // RAMM1_RSVD       : origin = 0x000007F8, length = 0x00000008 /* Reserve and do not use for code as per the errata advisory "Memory: Prefetching Beyond Valid Memory" */

   /*RAMLS0           : origin = 0x00008000, length = 0x00000800
   RAMLS1           : origin = 0x00008800, length = 0x00000800
   RAMLS2           : origin = 0x00009000, length = 0x00000800
   RAMLS3           : origin = 0x00009800, length = 0x00000800
   RAMLS4           : origin = 0x0000A000, length = 0x00000800
   RAMLS5           : origin = 0x0000A800, length = 0x00000800
   RAMLS6_7         : origin = 0x0000B000, length = 0x00001000*/
   RAMLS0_6         : origin = 0x00008000, length = 0x00003800
   RAMLS7           : origin = 0x0000B800, length = 0x00000800

   RAMGS0           : origin = 0x0000C000, length = 0x00001000
   RAMGS1           : origin = 0x0000D000, length = 0x00001000
   RAMGS2           : origin = 0x0000E000, length = 0x00001000
   RAMGS3           : origin = 0x0000F000, length = 0x00000FF8
   // RAMGS3_RSVD      : origin = 0x0000FFF8, length = 0x00000008 /* Reserve and do not use for code as per the errata advisory "Memory: Prefetching Beyond Valid Memory" */

   BOOTROM          : origin = 0x003F8000, length = 0x00007FC0
   SECURE_ROM       : origin = 0x003F2000, length = 0x00006000


   RESET            : origin = 0x003FFFC0, length = 0x00000002

   /* Flash sectors */
   /* BANK 0 */
   FLASH_BANK0_SEC0  : origin = 0x080002, length = 0x000FFE	
   FLASH_BANK0_SEC1  : origin = 0x081000, length = 0x001000
   FLASH_BANK0_SEC2  : origin = 0x082000, length = 0x001000
   FLASH_BANK0_SEC3  : origin = 0x083000, length = 0x001000
   FLASH_BANK0_SEC4  : origin = 0x084000, length = 0x001000
   FLASH_BANK0_SEC5  : origin = 0x085000, length = 0x001000
   FLASH_BANK0_SEC6  : origin = 0x086000, length = 0x001000
   FLASH_BANK0_SEC7  : origin = 0x087000, length = 0x001000
   FLASH_BANK0_SEC8  : origin = 0x088000, length = 0x001000
   FLASH_BANK0_SEC9  : origin = 0x089000, length = 0x001000
   FLASH_BANK0_SEC10 : origin = 0x08A000, length = 0x001000
   FLASH_BANK0_SEC11 : origin = 0x08B000, length = 0x001000
   FLASH_BANK0_SEC12 : origin = 0x08C000, length = 0x001000
   FLASH_BANK0_SEC13 : origin = 0x08D000, length = 0x001000
   FLASH_BANK0_SEC14 : origin = 0x08E000, length = 0x001000
   FLASH_BANK0_SEC15 : origin = 0x08F000, length = 0x001000

   /* BANK 1 */
   FLASH_BANK1_SEC0  : origin = 0x090000, length = 0x001000
   FLASH_BANK1_SEC1  : origin = 0x091000, length = 0x001000
   FLASH_BANK1_SEC2  : origin = 0x092000, length = 0x001000
   FLASH_BANK1_SEC3  : origin = 0x093000, length = 0x001000
   FLASH_BANK1_SEC4  : origin = 0x094000, length = 0x001000
   FLASH_BANK1_SEC5  : origin = 0x095000, length = 0x001000
   FLASH_BANK1_SEC6  : origin = 0x096000, length = 0x001000
   FLASH_BANK1_SEC7  : origin = 0x097000, length = 0x001000
   FLASH_BANK1_SEC8  : origin = 0x098000, length = 0x001000
   FLASH_BANK1_SEC9  : origin = 0x099000, length = 0x001000
   FLASH_BANK1_SEC10 : origin = 0x09A000, length = 0x001000
   FLASH_BANK1_SEC11 : origin = 0x09B000, length = 0x001000
   FLASH_BANK1_SEC12 : origin = 0x09C000, length = 0x001000
   FLASH_BANK1_SEC13 : origin = 0x09D000, length = 0x001000
   FLASH_BANK1_SEC14 : origin = 0x09E000, length = 0x001000
   FLASH_BANK1_SEC15 : origin = 0x09F000, length = 0x001000

  /* BANK 2 */
   FLASH_BANK2_SEC0  : origin = 0x0A0000, length = 0x001000
   FLASH_BANK2_SEC1  : origin = 0x0A1000, length = 0x001000
   FLASH_BANK2_SEC2  : origin = 0x0A2000, length = 0x001000
   FLASH_BANK2_SEC3  : origin = 0x0A3000, length = 0x001000
   FLASH_BANK2_SEC4  : origin = 0x0A4000, length = 0x001000
   FLASH_BANK2_SEC5  : origin = 0x0A5000, length = 0x001000
   FLASH_BANK2_SEC6  : origin = 0x0A6000, length = 0x001000
   FLASH_BANK2_SEC7  : origin = 0x0A7000, length = 0x001000
   FLASH_BANK2_SEC8  : origin = 0x0A8000, length = 0x001000
   FLASH_BANK2_SEC9  : origin = 0x0A9000, length = 0x001000
   FLASH_BANK2_SEC10 : origin = 0x0AA000, length = 0x001000
   FLASH_BANK2_SEC11 : origin = 0x0AB000, length = 0x001000
   FLASH_BANK2_SEC12 : origin = 0x0AC000, length = 0x001000
   FLASH_BANK2_SEC13 : origin = 0x0AD000, length = 0x001000
   FLASH_BANK2_SEC14 : origin = 0x0AE000, length = 0x001000
   FLASH_BANK2_SEC15 : origin = 0x0AF000, length = 0x000FF0

// FLASH_BANK0_SEC15_RSVD     : origin = 0x0AFFF0, length = 0x000010  /* Reserve and do not use for code as per the errata advisory "Memory: Prefetching Beyond Valid Memory" */

}


SECTIONS
{
   codestart        : > BEGIN ,   			ALIGN(4)
   .TI.ramfunc      : > RAMLS0_6

   .text            : > RAMLS0_6,   		ALIGN(8)
   .cinit           : > RAMLS0_6,    		ALIGN(4)
   .switch          : > RAMLS0_6,    		ALIGN(4)
   .reset           : > RESET,          	TYPE = DSECT /* not used, */
   .cio             : > RAMLS0_6
   .pinit           : > RAMLS0_6,			ALIGN(4)
   .const           : > RAMLS0_6,  			ALIGN(4)
   .init_array      : > RAMLS0_6, 			ALIGN(4)

   .stack           : > RAMM0
   .bss             : > RAMM1
   .bss:output      : > RAMM1
   .bss:cio         : > RAMM1
   .data            : > RAMM1
   .sysmem          : > RAMM1

/*
#if defined(__TI_EABI__)
   .bss             : > RAMLS0_6,              ALIGN(8)
   .bss:output      : > RAMLS0_6,              ALIGN(8)
   .init_array      : > RAMLS0_6,              ALIGN(8)
   .const           : > RAMLS0_6,              ALIGN(8)
   .data            : > RAMLS0_6,              ALIGN(8)
   .sysmem          : > RAMLS0_6,              ALIGN(8)
  .bss:cio          : > RAMLS0_6,              ALIGN(8)
#else
   .pinit           : > RAMLS0_6,              ALIGN(8)
   .ebss            : > RAMLS0_6,              ALIGN(8)
   .econst          : > RAMLS0_6,              ALIGN(8)
   .esysmem         : > RAMLS0_6,              ALIGN(8)
   .cio             : > RAMLS0_6,              ALIGN(8)
#endif
*/
    ramgs0 : > RAMGS0
    ramgs1 : > RAMGS1

   /* The following section definition are for SDFM examples */
   Filter1_RegsFile : > RAMLS7
   Filter2_RegsFile : > RAMLS7
   Filter3_RegsFile : > RAMLS7
   Filter4_RegsFile : > RAMLS7

    /*  Allocate IQ math areas: */
   IQmath           : > RAMLS7
   IQmathTables     : > RAMLS7
}


SECTIONS
{
    ctrl_data		      : > RAMGS0
	dmaBuf_data			  : > RAMGS0
    datalog_data	      : > RAMGS0
    graph_data		  	  : > RAMGS0
}


