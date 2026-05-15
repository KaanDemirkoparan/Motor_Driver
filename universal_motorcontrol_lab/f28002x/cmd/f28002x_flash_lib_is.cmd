
MEMORY
{
    RAMM0S                    : origin = 0x000128, length = 0x000158
    RAMM1D                    : origin = 0x000280, length = 0x000578
    RAMLS4                    : origin = 0x00A000, length = 0x000800
    RAMLS567                  : origin = 0x00A800, length = 0x001800
    RAMGS0                    : origin = 0x00C000, length = 0x0007F8
    FLASHBANK0_BOOT           : origin = 0x080000, length = 0x001000
    FLASHBANK0_CODE           : origin = 0x081000, length = 0x00D000
    FLASHBANK0_DATA           : origin = 0x08E000, length = 0x001000
    FLASHBANK0_PRMS           : origin = 0x08F000, length = 0x000FF0
    RESET                     : origin = 0x3FFFC0, length = 0x000002
}


SECTIONS
{

    .reset               : >  RESET, TYPE = DSECT /* not used, */
    codestart            : >  0x080000
    .text                : >  FLASHBANK0_CODE,
                              ALIGN(8)

   GROUP
   {
    .TI.ramfunc
     ramfuncs
     dclfuncs
     dcl32funcs
    }
    						  LOAD >  FLASHBANK0_CODE,
                              RUN  >  RAMLS567,
                              LOAD_START(RamfuncsLoadStart),
                              LOAD_END(RamfuncsLoadEnd),
                              LOAD_SIZE(RamfuncsLoadSize),
                              RUN_START(RamfuncsRunStart),
                              RUN_END(RamfuncsRunEnd),
                              RUN_SIZE(RamfuncsRunSize),
                              ALIGN(8)
    ctrlfuncs            :    LOAD >  FLASHBANK0_CODE,
                              RUN  >  RAMLS567,
                              LOAD_START(loadStart_ctrlfuncs),
                              LOAD_END(loadEnd_ctrlfuncs),
                              LOAD_SIZE(loadSize_ctrlfuncs),
                              RUN_START(runStart_ctrlfuncs),
                              RUN_END(runEnd_ctrlfuncs),
                              RUN_SIZE(runSize_ctrlfuncs),
                              ALIGN(8)
    .binit                : >  FLASHBANK0_DATA,
                              ALIGN(8)
    .cinit                : >  FLASHBANK0_DATA,
                              ALIGN(8)
    .stack                : >  RAMM0S
    .init_array           : >  FLASHBANK0_CODE,
                              ALIGN(8)
    .bss                  : >  RAMM1D
    .const                : >  FLASHBANK0_CODE,
                              ALIGN(8)
    .data                 : >  RAMM1D
    .switch               : >  FLASHBANK0_CODE,
                              ALIGN(8)
    .sysmem               : >  RAMM1D

    est_data             : >  RAMLS4,
                              LOAD_START(loadStart_est_data),
                              LOAD_END(loadEnd_est_data),
                              LOAD_SIZE(loadSize_est_data)

    hal_data             : >  RAMM1D,
                              LOAD_START(loadStart_hal_data),
                              LOAD_END(loadEnd_hal_data),
                              LOAD_SIZE(loadSize_hal_data)

    user_data            : >  RAMM1D,
                              LOAD_START(loadStart_user_data),
                              LOAD_END(loadEnd_user_data),
                              LOAD_SIZE(loadSize_user_data)

    foc_data             : >  RAMM1D,
                              LOAD_START(loadStart_foc_data),
                              LOAD_END(loadEnd_foc_data),
                              LOAD_SIZE(loadSize_foc_data)

    sys_data             : >  RAMM1D,
                              LOAD_START(loadStart_sys_data),
                              LOAD_END(loadEnd_sys_data),
                              LOAD_SIZE(loadSize_sys_data)

    vibc_data            : >  RAMGS0,
                              LOAD_START(loadStart_vibc_data),
                              LOAD_END(loadEnd_vibc_data),
                              LOAD_SIZE(loadSize_vibc_data)

    dmaBuf_data          : >  RAMGS0,
                              LOAD_START(loadStart_dmaBuf_data),
                              LOAD_END(loadEnd_dmaBuf_data),
                              LOAD_SIZE(loadSize_dmaBuf_data)

    datalog_data         : >  RAMGS0,
                              LOAD_START(loadStart_datalog_data),
                              LOAD_END(loadEnd_datalog_data),
                              LOAD_SIZE(loadSize_datalog_data)

    SFRA_F32_Data        : >  RAMGS0,
                              LOAD_START(loadStart_SFRA_F32_Data),
                              LOAD_END(loadEnd_SFRA_F32_Data),
                              LOAD_SIZE(loadSize_SFRA_F32_Data)
}

/*
//===========================================================================
// End of file.
//===========================================================================
*/
