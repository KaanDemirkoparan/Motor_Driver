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


//! \file   \solutions\universal_motorcontrol_lab\f28p65x\drivers\include\hal.h
//! \brief  Contains public interface to various functions related
//!         to the HAL object
//!


#ifndef HAL_H
#define HAL_H


//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
//
//! \defgroup HAL HAL
//! @{
//
//*****************************************************************************

// the includes
#include "userParams.h"


// platforms
#include "hal_obj.h"

#include "pwmdac.h"
#include "svgen_current.h"

// the globals

// Watch window interface to the drv8323/drv8353/DRV8316 SPI
#if defined(BSXL8323RS_REVA) || defined(BSXL8353RS_REVA)
extern DRVIC_VARS_t drvicVars_M1;
#endif  // BSXL8323RS_REVA | BSXL8353RS_REVA

extern HAL_Handle    halHandle;
extern HAL_Obj       hal;

extern volatile uint16_t mtrPIEIER;
extern volatile uint16_t mtrIER;

#ifdef _FLASH
extern uint32_t loadStart_ctrlfuncs;
extern uint32_t loadEnd_ctrlfuncs;
extern uint32_t loadSize_ctrlfuncs;
extern uint32_t runStart_ctrlfuncs;
extern uint32_t runEnd_ctrlfuncs;
extern uint32_t runSize_ctrlfuncs;
#endif  // _FLASH

extern uint32_t loadStart_est_data;
extern uint32_t loadEnd_est_data;
extern uint32_t loadSize_est_data;

extern uint32_t loadStart_hal_data;
extern uint32_t loadEnd_hal_data;
extern uint32_t loadSize_hal_data;

extern uint32_t loadStart_user_data;
extern uint32_t loadEnd_user_data;
extern uint32_t loadSize_user_data;

extern uint32_t loadStart_foc_data;
extern uint32_t loadEnd_foc_data;
extern uint32_t loadSize_foc_data;

extern uint32_t loadStart_sys_data;
extern uint32_t loadEnd_sys_data;
extern uint32_t loadSize_sys_data;

extern uint32_t loadStart_vibc_data;
extern uint32_t loadEnd_vibc_data;
extern uint32_t loadSize_vibc_data;

extern uint32_t loadStart_dmaBuf_data;
extern uint32_t loadEnd_dmaBuf_data;
extern uint32_t loadSize_dmaBuf_data;

extern uint32_t loadStart_datalog_data;
extern uint32_t loadEnd_datalog_data;
extern uint32_t loadSize_datalog_data;

extern uint32_t loadStart_SFRA_F32_Data;
extern uint32_t loadEnd_SFRA_F32_Data;
extern uint32_t loadSize_SFRA_F32_Data;

// **************************************************************************
// the defines
//
// 50MHz LSPCLK frequency based on the DEVICE_SYSCLK_FREQ and
#define C28X_LSPCLK_PRESCALE        SYSCTL_LSPCLK_PRESCALE_4
#define C28X_LSPCLK_FREQ            (DEVICE_SYSCLK_FREQ / 4)

// DAC SPI Baud Rate
#define DACS_SPI_BITRATE           (2000000L)         // 2MHz


//! Trip Zones all interrupt
//!
#define HAL_TZFLAG_INTERRUPT_ALL    EPWM_TZ_INTERRUPT_DCBEVT2 |                \
                                    EPWM_TZ_INTERRUPT_DCBEVT1 |                \
                                    EPWM_TZ_INTERRUPT_DCAEVT2 |                \
                                    EPWM_TZ_INTERRUPT_DCAEVT1 |                \
                                    EPWM_TZ_INTERRUPT_OST |                    \
                                    EPWM_TZ_INTERRUPT_CBC

#define HAL_TZSEL_SIGNALS_ALL       EPWM_TZ_SIGNAL_CBC1 |                      \
                                    EPWM_TZ_SIGNAL_CBC2 |                      \
                                    EPWM_TZ_SIGNAL_CBC3 |                      \
                                    EPWM_TZ_SIGNAL_CBC4 |                      \
                                    EPWM_TZ_SIGNAL_CBC5 |                      \
                                    EPWM_TZ_SIGNAL_CBC6 |                      \
                                    EPWM_TZ_SIGNAL_DCAEVT2 |                   \
                                    EPWM_TZ_SIGNAL_DCBEVT2 |                   \
                                    EPWM_TZ_SIGNAL_OSHT1 |                     \
                                    EPWM_TZ_SIGNAL_OSHT2 |                     \
                                    EPWM_TZ_SIGNAL_OSHT3 |                     \
                                    EPWM_TZ_SIGNAL_OSHT4 |                     \
                                    EPWM_TZ_SIGNAL_OSHT5 |                     \
                                    EPWM_TZ_SIGNAL_OSHT6 |                     \
                                    EPWM_TZ_SIGNAL_DCAEVT1 |                   \
                                    EPWM_TZ_SIGNAL_DCBEVT1

//! \brief Defines the PWM frequency for PWMDAC
//!
#define HA_PWMDAC_FREQ_KHZ         100.0f

//! \brief Defines the comparator number for current protection
//!
#define HAL_NUM_CMPSS_CURRENT       6

//------------------------------------------------------------------------------
// DMA for Datalog
#define DMA_DATALOG1_BASE       DMA_CH3_BASE
#define DMA_DATALOG2_BASE       DMA_CH4_BASE
#define DMA_DATALOG3_BASE       DMA_CH5_BASE
#define DMA_DATALOG4_BASE       DMA_CH6_BASE

// Install the boostxlPak or EVM on site 1 (near emulator) on launchPad
//! \ Motor 1
#define MTR1_PWM_U_BASE         EPWM5_BASE
#define MTR1_PWM_V_BASE         EPWM4_BASE
#define MTR1_PWM_W_BASE         EPWM6_BASE

// XBAR-EPWM
#define MTR1_XBAR_TRIP_ADDRL1   XBAR_O_OUT7MUX0TO15CFG
#define MTR1_XBAR_TRIP_ADDRL2   XBAR_O_OUT7MUX16TO31CFG
#define MTR1_XBAR_TRIP_ADDRH1   XBAR_O_OUT7MUX32TO47CFG // created for p65x
#define MTR1_XBAR_TRIP_ADDRH2   XBAR_O_OUT7MUX48TO63CFG // created for p65x

#define MTR1_XBAR_INPUT1        XBAR_INPUT1             // unchanged updated p65x
#define MTR1_TZ_OSHT1           EPWM_TZ_SIGNAL_OSHT1    // unchanged updated p65x

#define MTR1_XBAR_TRIP          XBAR_TRIP7              // unchanged updated p65x
#define MTR1_DCTRIPIN           EPWM_DC_COMBINATIONAL_TRIPIN7 // unchanged updated p65x


#define MTR1_CMPSS_DACH_VALUE   (2048 + 1024 + 512)     // unchanged updated p65x
#define MTR1_CMPSS_DACL_VALUE   (2048 - 1024 - 512)     // unchanged updated p65x

//! \brief Defines the PWM deadband falling edge delay count (system clocks)
#define MTR1_PWM_DBFED_CNT      10           // 50ns

//! \brief Defines the PWM deadband rising edge delay count (system clocks)
#define MTR1_PWM_DBRED_CNT      10           // 50ns

// interrupt
#define MTR1_PWM_INT_BASE       MTR1_PWM_U_BASE         // EPWM1

//------------------------------------------------------------------------------
#define COM_CAN_BASE                    CANA_BASE

#define COM_CANRX_GPIO                  5
#define COM_CANTX_GPIO                  4

#define COM_CANRX_GPIO_PIN_CONFIG       GPIO_5_CANA_RX
#define COM_CANTX_GPIO_PIN_CONFIG       GPIO_4_CANA_TX

#define COM_INT_CAN                     INT_CANA0

#define GUI_SCI_BASE                    SCIB_BASE

#define GUI_SCI_SCIRX_GPIO              55
#define GUI_SCI_SCITX_GPIO              38

#define GUI_SCI_SCIRX_PIN_CONFIG        GPIO_55_SCIB_RX
#define GUI_SCI_SCITX_PIN_CONFIG        GPIO_38_SCIB_TX

#if defined(CMD_SWITCH_EN)
#define MTR1_CMD_SWITCH_GPIO    104
#define MTR1_CMD_STATE_GPIO     105
#endif  // CMD_SWITCH_EN

#if defined(SFRA_ENABLE)
#define GUI_LED_GPIO                    25  // ? TODO determine what this is
#define GUI_LED_GPIO_GPIO_PIN_CONFIG    GPIO_25_GPIO25  // TODO determine what this is
#endif

//! \brief Defines the QEP for encoder
#define MTR1_QEP_BASE           EQEP1_BASE

#define MTR1_HALL_U_GPIO        24
#define MTR1_HALL_V_GPIO        79
#define MTR1_HALL_W_GPIO        103

#define MTR1_CAP_U_BASE         ECAP1_BASE
#define MTR1_CAP_V_BASE         ECAP2_BASE
#define MTR1_CAP_W_BASE         ECAP3_BASE

#define MTR1_CAP_U_XBAR         XBAR_INPUT4
#define MTR1_CAP_V_XBAR         XBAR_INPUT5
#define MTR1_CAP_W_XBAR         XBAR_INPUT6

#define MTR1_CAP_U_INSEL        ECAP_INPUT_INPUTXBAR4
#define MTR1_CAP_V_INSEL        ECAP_INPUT_INPUTXBAR5
#define MTR1_CAP_W_INSEL        ECAP_INPUT_INPUTXBAR6

#if defined(BSXL8323RS_REVA) || defined(BSXL8353RS_REVA)
//! \brief Defines the gpio for enabling Power Module
#define MTR1_GATE_EN_GPIO       42

//! \brief Defines the gpio for the nFAULT of Power Module
#define MTR1_PM_nFAULT_GPIO     72

//! \brief Defines the gpio for setting mode
#define MTR1_GATE_MODE_GPIO     14      // only for DRV8353RH

//! \brief Defines the gpio for setting gain
#define MTR1_GATE_GAIN_GPIO     23      // only for DRV8353RH

//! \brief Defines the gpio for calibration
#define MTR1_GATE_CAL_GPIO      133      // only for DRV8353RH

#define MTR1_SPI_BASE           SPIA_BASE           // N/A

//------------------------------------------------------------------------------
// ADC
// Three-shunt
#define MTR1_ADC_TRIGGER_SOC        ADC_TRIGGER_EPWM5_SOCA  // EPWM5_SOCA
#define MTR1_ADC_I_SAMPLEWINDOW     14
#define MTR1_ADC_V_SAMPLEWINDOW     20

#define MTR1_IU_ADC_BASE        ADCB_BASE           // ADCB-B7* -SOC0-PPB1
#define MTR1_IV_ADC_BASE        ADCA_BASE           // ADCA-A7* -SOC0-PPB1
#define MTR1_IW_ADC_BASE        ADCC_BASE           // ADCC-C4* -SOC0-PPB1

#define MTR1_IU_ADCRES_BASE     ADCBRESULT_BASE
#define MTR1_IV_ADCRES_BASE     ADCARESULT_BASE
#define MTR1_IW_ADCRES_BASE     ADCCRESULT_BASE

#define MTR1_IU_ADC_CH_NUM      ADC_CH_ADCIN7
#define MTR1_IV_ADC_CH_NUM      ADC_CH_ADCIN7
#define MTR1_IW_ADC_CH_NUM      ADC_CH_ADCIN4

#define MTR1_IU_ADC_SOC_NUM     ADC_SOC_NUMBER0
#define MTR1_IV_ADC_SOC_NUM     ADC_SOC_NUMBER0
#define MTR1_IW_ADC_SOC_NUM     ADC_SOC_NUMBER0

#define MTR1_IU_ADC_PPB_NUM     ADC_PPB_NUMBER1
#define MTR1_IV_ADC_PPB_NUM     ADC_PPB_NUMBER1
#define MTR1_IW_ADC_PPB_NUM     ADC_PPB_NUMBER1

// CMPSS
#define MTR1_CMPSS_U_BASE_H     CMPSS7_BASE     // ADCB-B7* - HP - CMPSS7 sel2
#define MTR1_CMPSS_U_BASE_L     CMPSS7_BASE     // ADCB-B7* - LP - CMPSS7 sel2

#define MTR1_CMPSS_V_BASE_H     CMPSS9_BASE     // ADCA-A7* - HP - CMPSS9 sel2
#define MTR1_CMPSS_V_BASE_L     CMPSS4_BASE     // ADCA-A7* - LP - CMPSS4 sel3

#define MTR1_CMPSS_W_BASE_H     CMPSS5_BASE     // ADCC-C4* - HP - CMPSS5 sel0
#define MTR1_CMPSS_W_BASE_L     CMPSS5_BASE     // ADCC-C4* - LP - CMPSS5 sel0

// CMPSS selection
#define MTR1_IU_CMPHP_SEL       ASYSCTL_CMPHPMUX_SELECT_7    // ADCB-B7* - HP - CMPSS7 sel2
#define MTR1_IU_CMPLP_SEL       ASYSCTL_CMPLPMUX_SELECT_7    // ADCB-B7* - LP - CMPSS7 sel2

#define MTR1_IV_CMPHP_SEL       ASYSCTL_CMPHPMUX_SELECT_9    // ADCA-A7* - HP    - CMPSS9 sel2
#define MTR1_IV_CMPLP_SEL       ASYSCTL_CMPLPMUX_SELECT_4    // ADCA-A7* - LP    - CMPSS4 sel3

#define MTR1_IW_CMPHP_SEL       ASYSCTL_CMPHPMUX_SELECT_5    // ADCC-C4* - HP - CMPSS5 sel0
#define MTR1_IW_CMPLP_SEL       ASYSCTL_CMPLPMUX_SELECT_5    // ADCC-C4* - LP - CMPSS5 sel0

// Analog Subsystem Mux select
#define MTR1_IU_CMPHP_MUX       2                            // ADCB-B7* - HP - CMPSS7 sel2
#define MTR1_IU_CMPLP_MUX       2                            // ADCB-B7* - LP - CMPSS7 sel2

#define MTR1_IV_CMPHP_MUX       2                            // ADCA-A7* - HP    - CMPSS9 sel2
#define MTR1_IV_CMPLP_MUX       3                            // ADCA-A7* - LP    - CMPSS4 sel3

#define MTR1_IW_CMPHP_MUX       0                            // ADCC-C4* - HP - CMPSS5 sel0
#define MTR1_IW_CMPLP_MUX       0                            // ADCC-C4* - LP - CMPSS5 sel0

// ePWM XBAR MUX select
#define MTR1_IU_XBAR_EPWM_MUX_H     XBAR_EPWM_MUX12_CMPSS7_CTRIPH   // CMPSS7-HP
#define MTR1_IU_XBAR_EPWM_MUX_L     XBAR_EPWM_MUX13_CMPSS7_CTRIPL   // CMPSS7-LP
#define MTR1_IV_XBAR_EPWM_MUX_H     XBAR_EPWM_MUX58_CMPSS9_CTRIPH   // CMPSS9-HP
#define MTR1_IV_XBAR_EPWM_MUX_L     XBAR_EPWM_MUX07_CMPSS4_CTRIPL   // CMPSS4-LP
#define MTR1_IW_XBAR_EPWM_MUX_H     XBAR_EPWM_MUX08_CMPSS5_CTRIPH   // CMPSS5-HP
#define MTR1_IW_XBAR_EPWM_MUX_L     XBAR_EPWM_MUX09_CMPSS5_CTRIPL   // CMPSS5-LP

#define MTR1_IU_XBAR_MUX_H      XBAR_MUX12          // CMPSS7-HP
#define MTR1_IU_XBAR_MUX_L      XBAR_MUX13          // CMPSS7-LP
#define MTR1_IV_XBAR_MUX_H      XBAR_MUX58          // CMPSS9-HP
#define MTR1_IV_XBAR_MUX_L      XBAR_MUX07          // CMPSS4-LP
#define MTR1_IW_XBAR_MUX_H      XBAR_MUX08          // CMPSS5-HP
#define MTR1_IW_XBAR_MUX_L      XBAR_MUX09          // CMPSS5-LP

//V U, V, W
#define MTR1_VU_ADC_BASE        ADCC_BASE               // ADCA-A14/B14/*C14 -SOC1
#define MTR1_VV_ADC_BASE        ADCA_BASE               // ADCA-A4*          -SOC1
#define MTR1_VW_ADC_BASE        ADCB_BASE               // ADCB-B4*          -SOC1

#define MTR1_VU_ADCRES_BASE     ADCCRESULT_BASE
#define MTR1_VV_ADCRES_BASE     ADCARESULT_BASE
#define MTR1_VW_ADCRES_BASE     ADCBRESULT_BASE

#define MTR1_VU_ADC_CH_NUM      ADC_CH_ADCIN14
#define MTR1_VV_ADC_CH_NUM      ADC_CH_ADCIN4
#define MTR1_VW_ADC_CH_NUM      ADC_CH_ADCIN4

#define MTR1_VU_ADC_SOC_NUM     ADC_SOC_NUMBER1
#define MTR1_VV_ADC_SOC_NUM     ADC_SOC_NUMBER1
#define MTR1_VW_ADC_SOC_NUM     ADC_SOC_NUMBER1

// VDC, POT
#define MTR1_POT_ADC_BASE       ADCC_BASE               // ADCC-C0*  -SOC2
#define MTR1_VDC_ADC_BASE       ADCB_BASE               // ADCB-B0*  -SOC2-INT1

#define MTR1_POT_ADCRES_BASE    ADCCRESULT_BASE
#define MTR1_VDC_ADCRES_BASE    ADCBRESULT_BASE

#define MTR1_POT_ADC_CH_NUM     ADC_CH_ADCIN0
#define MTR1_VDC_ADC_CH_NUM     ADC_CH_ADCIN0

#define MTR1_POT_ADC_SOC_NUM    ADC_SOC_NUMBER2
#define MTR1_VDC_ADC_SOC_NUM    ADC_SOC_NUMBER2

//------------------------------------------------------------------------------
// interrupt
// VDC
#define MTR1_ADC_INT_BASE       MTR1_VDC_ADC_BASE       // ADCB-B0*-SOC2-INT1
#define MTR1_ADC_INT_NUM        ADC_INT_NUMBER1
#define MTR1_ADC_INT_SOC        MTR1_VDC_ADC_SOC_NUM

#define MTR1_PIE_INT_NUM        INT_ADCB1
#define MTR1_INT_ACK_GROUP      INTERRUPT_ACK_GROUP1    // ADCB_B0*-SOC2-INT1-CPU_INT1
// end of BSXL8323RS_REVA || BSXL8353RS_REVA

//------------------------------------------------------------------------------
#elif defined(BSXL3PHGAN_REVA)
//! \brief Defines the gpio for enabling Power Module
#define MTR1_GATE_EN_GPIO       12 // nEN-uC

//! \brief Defines the gpio for the nFAULT of Power Module
#define MTR1_PM_nFAULT_GPIO     14 // OT

//------------------------------------------------------------------------------
// ADC
// Three-shunt
#define MTR1_ADC_TRIGGER_SOC        ADC_TRIGGER_EPWM5_SOCA  // EPWM5_SOCA
#define MTR1_ADC_I_SAMPLEWINDOW     14
#define MTR1_ADC_V_SAMPLEWINDOW     20

#define MTR1_IU_ADC_BASE        ADCA_BASE           // ADCA-A7* -SOC0-PPB1
#define MTR1_IV_ADC_BASE        ADCB_BASE           // ADCB-B7* -SOC0-PPB1
#define MTR1_IW_ADC_BASE        ADCC_BASE           // ADCC-C7* -SOC0-PPB1

#define MTR1_IU_ADCRES_BASE     ADCARESULT_BASE
#define MTR1_IV_ADCRES_BASE     ADCBRESULT_BASE
#define MTR1_IW_ADCRES_BASE     ADCCRESULT_BASE

#define MTR1_IU_ADC_CH_NUM      ADC_CH_ADCIN7
#define MTR1_IV_ADC_CH_NUM      ADC_CH_ADCIN7
#define MTR1_IW_ADC_CH_NUM      ADC_CH_ADCIN7

#define MTR1_IU_ADC_SOC_NUM     ADC_SOC_NUMBER0
#define MTR1_IV_ADC_SOC_NUM     ADC_SOC_NUMBER0
#define MTR1_IW_ADC_SOC_NUM     ADC_SOC_NUMBER0

#define MTR1_IU_ADC_PPB_NUM     ADC_PPB_NUMBER1
#define MTR1_IV_ADC_PPB_NUM     ADC_PPB_NUMBER1
#define MTR1_IW_ADC_PPB_NUM     ADC_PPB_NUMBER1

// CMPSS
#define MTR1_CMPSS_U_BASE_H     CMPSS9_BASE     // ADCA-A7* - HP - CMPSS9 sel2
#define MTR1_CMPSS_U_BASE_L     CMPSS4_BASE     // ADCA-A7* - LP - CMPSS4 sel3

#define MTR1_CMPSS_V_BASE_H     CMPSS7_BASE     // ADCA-A7* - HP - CMPSS9 sel2
#define MTR1_CMPSS_V_BASE_L     CMPSS7_BASE     // ADCA-A7* - LP - CMPSS4 sel2

#define MTR1_CMPSS_W_BASE_H     CMPSS11_BASE    // ADCC-C4* - HP - CMPSS5 sel1
#define MTR1_CMPSS_W_BASE_L     CMPSS11_BASE    // ADCC-C4* - LP - CMPSS5 sel1

// CMPSS selection
#define MTR1_IU_CMPHP_SEL       ASYSCTL_CMPHPMUX_SELECT_9
#define MTR1_IU_CMPLP_SEL       ASYSCTL_CMPLPMUX_SELECT_4

#define MTR1_IV_CMPHP_SEL       ASYSCTL_CMPHPMUX_SELECT_7
#define MTR1_IV_CMPLP_SEL       ASYSCTL_CMPLPMUX_SELECT_7

#define MTR1_IW_CMPHP_SEL       ASYSCTL_CMPHPMUX_SELECT_11
#define MTR1_IW_CMPLP_SEL       ASYSCTL_CMPLPMUX_SELECT_11

// Analog Subsystem Mux select
#define MTR1_IU_CMPHP_MUX       2
#define MTR1_IU_CMPLP_MUX       3

#define MTR1_IV_CMPHP_MUX       2
#define MTR1_IV_CMPLP_MUX       2

#define MTR1_IW_CMPHP_MUX       1
#define MTR1_IW_CMPLP_MUX       1

// ePWM XBAR MUX select
#define MTR1_IU_XBAR_EPWM_MUX_H     XBAR_EPWM_MUX58_CMPSS9_CTRIPH   // CMPSS9-HP
#define MTR1_IU_XBAR_EPWM_MUX_L     XBAR_EPWM_MUX07_CMPSS4_CTRIPL   // CMPSS4-LP
#define MTR1_IV_XBAR_EPWM_MUX_H     XBAR_EPWM_MUX12_CMPSS7_CTRIPH   // CMPSS7-HP
#define MTR1_IV_XBAR_EPWM_MUX_L     XBAR_EPWM_MUX13_CMPSS7_CTRIPL   // CMPSS7-LP
#define MTR1_IW_XBAR_EPWM_MUX_H     XBAR_EPWM_MUX62_CMPSS11_CTRIPH  // CMPSS11-HP
#define MTR1_IW_XBAR_EPWM_MUX_L     XBAR_EPWM_MUX63_CMPSS11_CTRIPL  // CMPSS11-LP

#define MTR1_IU_XBAR_MUX_H      XBAR_MUX58
#define MTR1_IU_XBAR_MUX_L      XBAR_MUX07
#define MTR1_IV_XBAR_MUX_H      XBAR_MUX12
#define MTR1_IV_XBAR_MUX_L      XBAR_MUX13
#define MTR1_IW_XBAR_MUX_H      XBAR_MUX62
#define MTR1_IW_XBAR_MUX_L      XBAR_MUX63

//V U, V, W
#define MTR1_VU_ADC_BASE        ADCA_BASE               // ADCA-A4* -SOC1
#define MTR1_VV_ADC_BASE        ADCB_BASE               // ADCA-B4* -SOC1
#define MTR1_VW_ADC_BASE        ADCC_BASE               // ADCB-C4* -SOC1

#define MTR1_VU_ADCRES_BASE     ADCARESULT_BASE
#define MTR1_VV_ADCRES_BASE     ADCBRESULT_BASE
#define MTR1_VW_ADCRES_BASE     ADCCRESULT_BASE

#define MTR1_VU_ADC_CH_NUM      ADC_CH_ADCIN4
#define MTR1_VV_ADC_CH_NUM      ADC_CH_ADCIN4
#define MTR1_VW_ADC_CH_NUM      ADC_CH_ADCIN4

#define MTR1_VU_ADC_SOC_NUM     ADC_SOC_NUMBER1
#define MTR1_VV_ADC_SOC_NUM     ADC_SOC_NUMBER1
#define MTR1_VW_ADC_SOC_NUM     ADC_SOC_NUMBER1

// VDC
#define MTR1_VDC_ADC_BASE       ADCA_BASE               // ADC-A14*/B14/C14  -SOC2-INT1

#define MTR1_VDC_ADCRES_BASE    ADCARESULT_BASE

#define MTR1_VDC_ADC_CH_NUM     ADC_CH_ADCIN14

#define MTR1_VDC_ADC_SOC_NUM    ADC_SOC_NUMBER2

//------------------------------------------------------------------------------
// interrupt
// VDC
#define MTR1_ADC_INT_BASE       MTR1_VDC_ADC_BASE       // ADC-A14*/B14/C14  -SOC2-INT1
#define MTR1_ADC_INT_NUM        ADC_INT_NUMBER1
#define MTR1_ADC_INT_SOC        MTR1_VDC_ADC_SOC_NUM

#define MTR1_PIE_INT_NUM        INT_ADCA1
#define MTR1_INT_ACK_GROUP      INTERRUPT_ACK_GROUP1    // ADC-A14*/B14/C14  -SOC2-INT1
// end of BSXL3PHGAN_REVA

#elif defined(DRV8300DRGE_EVM)
//------------------------------------------------------------------------------
// ADC
// Three-shunt
#define MTR1_ADC_TRIGGER_SOC        ADC_TRIGGER_EPWM5_SOCA  // EPWM5_SOCA
#define MTR1_ADC_I_SAMPLEWINDOW     14
#define MTR1_ADC_V_SAMPLEWINDOW     20

//------------------------------------------------------------------------------
// ADC
// Three-shunt
#define MTR1_IU_ADC_BASE        ADCC_BASE               // ADC-C7
#define MTR1_IV_ADC_BASE        ADCB_BASE               // ADC-B7
#define MTR1_IW_ADC_BASE        ADCA_BASE               // ADC-A7

#define MTR1_IU_ADCRES_BASE     ADCCRESULT_BASE
#define MTR1_IV_ADCRES_BASE     ADCBRESULT_BASE
#define MTR1_IW_ADCRES_BASE     ADCARESULT_BASE

#define MTR1_IU_ADC_CH_NUM      ADC_CH_ADCIN7
#define MTR1_IV_ADC_CH_NUM      ADC_CH_ADCIN7
#define MTR1_IW_ADC_CH_NUM      ADC_CH_ADCIN7

#define MTR1_IU_ADC_SOC_NUM     ADC_SOC_NUMBER0         // SOC0-PPB1
#define MTR1_IV_ADC_SOC_NUM     ADC_SOC_NUMBER0         // SOC0-PPB1
#define MTR1_IW_ADC_SOC_NUM     ADC_SOC_NUMBER0         // SOC0-PPB1

#define MTR1_IU_ADC_PPB_NUM     ADC_PPB_NUMBER1
#define MTR1_IV_ADC_PPB_NUM     ADC_PPB_NUMBER1
#define MTR1_IW_ADC_PPB_NUM     ADC_PPB_NUMBER1

// CMPSS
#define MTR1_CMPSS_U_BASE_H     CMPSS9_BASE     // ADCA-A7* - HP - CMPSS9 sel2
#define MTR1_CMPSS_U_BASE_L     CMPSS4_BASE     // ADCA-A7* - LP - CMPSS4 sel3

#define MTR1_CMPSS_V_BASE_H     CMPSS7_BASE     // ADCA-A7* - HP - CMPSS9 sel2
#define MTR1_CMPSS_V_BASE_L     CMPSS7_BASE     // ADCA-A7* - LP - CMPSS4 sel2

#define MTR1_CMPSS_W_BASE_H     CMPSS11_BASE    // ADCC-C4* - HP - CMPSS5 sel1
#define MTR1_CMPSS_W_BASE_L     CMPSS11_BASE    // ADCC-C4* - LP - CMPSS5 sel1

// CMPSS selection
#define MTR1_IU_CMPHP_SEL       ASYSCTL_CMPHPMUX_SELECT_9
#define MTR1_IU_CMPLP_SEL       ASYSCTL_CMPLPMUX_SELECT_4

#define MTR1_IV_CMPHP_SEL       ASYSCTL_CMPHPMUX_SELECT_7
#define MTR1_IV_CMPLP_SEL       ASYSCTL_CMPLPMUX_SELECT_7

#define MTR1_IW_CMPHP_SEL       ASYSCTL_CMPHPMUX_SELECT_11
#define MTR1_IW_CMPLP_SEL       ASYSCTL_CMPLPMUX_SELECT_11

// Analog Subsystem Mux select
#define MTR1_IU_CMPHP_MUX       2
#define MTR1_IU_CMPLP_MUX       3

#define MTR1_IV_CMPHP_MUX       2
#define MTR1_IV_CMPLP_MUX       2

#define MTR1_IW_CMPHP_MUX       1
#define MTR1_IW_CMPLP_MUX       1

// ePWM XBAR MUX select
#define MTR1_IU_XBAR_EPWM_MUX_H     XBAR_EPWM_MUX58_CMPSS9_CTRIPH   // CMPSS9-HP
#define MTR1_IU_XBAR_EPWM_MUX_L     XBAR_EPWM_MUX07_CMPSS4_CTRIPL   // CMPSS4-LP
#define MTR1_IV_XBAR_EPWM_MUX_H     XBAR_EPWM_MUX12_CMPSS7_CTRIPH   // CMPSS7-HP
#define MTR1_IV_XBAR_EPWM_MUX_L     XBAR_EPWM_MUX13_CMPSS7_CTRIPL   // CMPSS7-LP
#define MTR1_IW_XBAR_EPWM_MUX_H     XBAR_EPWM_MUX62_CMPSS11_CTRIPH  // CMPSS11-HP
#define MTR1_IW_XBAR_EPWM_MUX_L     XBAR_EPWM_MUX63_CMPSS11_CTRIPL  // CMPSS11-LP

#define MTR1_IU_XBAR_MUX_H      XBAR_MUX58
#define MTR1_IU_XBAR_MUX_L      XBAR_MUX07
#define MTR1_IV_XBAR_MUX_H      XBAR_MUX12
#define MTR1_IV_XBAR_MUX_L      XBAR_MUX13
#define MTR1_IW_XBAR_MUX_H      XBAR_MUX62
#define MTR1_IW_XBAR_MUX_L      XBAR_MUX63

//V U, V, W
#define MTR1_VU_ADC_BASE        ADCC_BASE               // ADC-C14
#define MTR1_VV_ADC_BASE        ADCA_BASE               // ADC-A4
#define MTR1_VW_ADC_BASE        ADCB_BASE               // ADC-B4

#define MTR1_VU_ADCRES_BASE     ADCCRESULT_BASE
#define MTR1_VV_ADCRES_BASE     ADCARESULT_BASE
#define MTR1_VW_ADCRES_BASE     ADCBRESULT_BASE

#define MTR1_VU_ADC_CH_NUM      ADC_CH_ADCIN14
#define MTR1_VV_ADC_CH_NUM      ADC_CH_ADCIN4
#define MTR1_VW_ADC_CH_NUM      ADC_CH_ADCIN4

#define MTR1_VU_ADC_SOC_NUM     ADC_SOC_NUMBER1
#define MTR1_VV_ADC_SOC_NUM     ADC_SOC_NUMBER1
#define MTR1_VW_ADC_SOC_NUM     ADC_SOC_NUMBER1

// VDC
#define MTR1_VDC_ADC_BASE       ADCC_BASE               // ADC-C4*  -SOC2-INT1

#define MTR1_VDC_ADCRES_BASE    ADCCRESULT_BASE

#define MTR1_VDC_ADC_CH_NUM     ADC_CH_ADCIN4

#define MTR1_VDC_ADC_SOC_NUM    ADC_SOC_NUMBER2

//------------------------------------------------------------------------------
// interrupt
// VDC
#define MTR1_ADC_INT_BASE       MTR1_VDC_ADC_BASE       // ADC-A14*/B14/C14  -SOC2-INT1
#define MTR1_ADC_INT_NUM        ADC_INT_NUMBER1
#define MTR1_ADC_INT_SOC        MTR1_VDC_ADC_SOC_NUM

#define MTR1_PIE_INT_NUM        INT_ADCC1
#define MTR1_INT_ACK_GROUP      INTERRUPT_ACK_GROUP1    // ADC-A14*/B14/C14  -SOC2-INT1
// end of DRV8300DRGE_EVM

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#else   // Not select a kit
#error Not select a kit and define the symbols in hal.h
#endif   // Not select a kit

// **************************************************************************
// the typedefs
//------------------------------------------------------------------------------
//! \brief Defines the function to turn LEDs off
//!
#define HAL_turnLEDOff              HAL_setGPIOHigh

//! \brief Defines the function to turn LEDs on
//!
#define HAL_turnLEDOn               HAL_setGPIOLow

//! \brief Defines the function to toggle LEDs
//!
#define HAL_toggleLED               HAL_toggleGPIO

//! \brief Enumeration for the LED numbers
//!
#if defined(BSXL8323RS_REVA) || defined(BSXL8353RS_REVA) \
|| defined(BSXL3PHGAN_REVA) || defined(DRV8300DRGE_EVM)
#define HAL_GPIO_LED1C      12   //!< GPIO pin number for LaunchPad LED 1
#define HAL_GPIO_LED2C      13   //!< GPIO pin number for LaunchPad LED 2
#define HAL_GPIO_LED1B      15   //!< GPIO pin number for BoostxlPak LED 1
#define HAL_GPIO_LED2B      31   //!< GPIO pin number for BoostxlPak LED 2
// BSXL8323RS_REVA || BSXL8353RS_REVA || BSXL3PHGAN_REVA
#else
#error Not defined GPIOs for LED & Debug in hal.h
#endif  //

//! \brief Enumeration for the sensor types
//!
typedef enum
{
    HAL_SENSORTYPE_CURRENT = 0,  //!< Enumeration for current sensor
    HAL_SENSORTYPE_VOLTAGE = 1   //!< Enumeration for voltage sensor
} HAL_SensorType_e;

//! \brief Enumeration for the QEP setup
//!
typedef enum
{
    HAL_QEP_QEP1=0,  //!< Select QEP1
    HAL_QEP_QEP2=1   //!< Select QEP2
} HAL_QEPSelect_e;

//! \brief Enumeration for the CPU Timer
//!
typedef enum
{
    HAL_CPU_TIMER0 = 0,  //!< Select CPU Timer0
    HAL_CPU_TIMER1 = 1,  //!< Select CPU Timer1
    HAL_CPU_TIMER2 = 2   //!< Select CPU Timer2
} HAL_CPUTimerNum_e;

// **************************************************************************
// the function prototypes

// the interrupt ISR for motor control
extern __interrupt void motor1CtrlISR(void);

// CLA Tasks
extern __interrupt void task_initModules(void);
extern __interrupt void task_mainISR(void);
extern __interrupt void task_mainLoop(void);
extern __interrupt void cla1Task4(void);
extern __interrupt void cla1Task5(void);
extern __interrupt void cla1Task6(void);
extern __interrupt void cla1Task7(void);
extern __interrupt void cla1Task8(void);
extern __interrupt void cla_EST_run_BackgroundTask(void);


//! \brief     Acknowledges an interrupt from the ADC so that another ADC
//!            interrupt can happen again.
//! \param[in] handle     The hardware abstraction layer (HAL) handle
static inline void HAL_ackMtr1ADCInt(void)
{
    // clear the ADC interrupt flag
    ADC_clearInterruptStatus(MTR1_ADC_INT_BASE, MTR1_ADC_INT_NUM);

    // Acknowledge interrupt from PIE group
    Interrupt_clearACKGroup(MTR1_INT_ACK_GROUP);

    return;
} // end of HAL_ackADCInt() function


//! \brief      Disables global interrupts
//! \param[in]  handle  The hardware abstraction layer (HAL) handle
extern void HAL_disableGlobalInts(HAL_Handle handle);


//! \brief      Enables the ADC interrupts
//! \details    Enables the ADC interrupt in the PIE, and CPU.  Enables the 
//!             interrupt to be sent from the ADC peripheral.
//! \param[in]  handle  The hardware abstraction layer (HAL) handle
extern void HAL_enableCtrlInts(HAL_Handle handle);


//! \brief      Enables the ADC interrupts without CPU interrupts
//! \details    Enables the ADC interrupts to only trigger CLA, and without
//!             interrupting the CPU
//! \param[in]  handle  The hardware abstraction layer (HAL) handle
extern void HAL_enableADCIntsToTriggerCLA(HAL_Handle handle);


//! \brief      Enables the debug interrupt
//! \details    The debug interrupt is used for the real-time debugger.  It is
//!             not needed if the real-time debugger is not used.  Clears
//!             bit 1 of ST1.
//! \param[in]  handle  The hardware abstraction layer (HAL) handle
extern void HAL_enableDebugInt(HAL_Handle handle);


//! \brief     Enables global interrupts
//! \param[in] handle  The hardware abstraction layer (HAL) handle
extern void HAL_enableGlobalInts(HAL_Handle handle);


//! \brief     Gets the PWM duty cycle times
//! \param[in] handle       The hardware abstraction layer (HAL) handle
//! \param[in] pDutyCycles  A pointer to memory for the duty cycle durations
static inline void
HAL_getDutyCycles(HAL_MTR_Handle handle,uint16_t *pDutyCycles)
{
  HAL_MTR_Obj *obj = (HAL_MTR_Obj *)handle;

  pDutyCycles[0] = EPWM_getCounterCompareValue(obj->pwmHandle[0],
                                               EPWM_COUNTER_COMPARE_A);
  pDutyCycles[1] = EPWM_getCounterCompareValue(obj->pwmHandle[1],
                                               EPWM_COUNTER_COMPARE_A);
  pDutyCycles[2] = EPWM_getCounterCompareValue(obj->pwmHandle[2],
                                               EPWM_COUNTER_COMPARE_A);
  return;
} // end of HAL_getDutyCycles() function


//! \brief     Gets the number of current sensors
//! \param[in] handle  The hardware abstraction layer (HAL) handle
//! \return    The number of current sensors
static inline uint16_t HAL_getNumCurrentSensors(HAL_MTR_Handle handle)
{
  HAL_MTR_Obj *obj = (HAL_MTR_Obj *)handle;
  
  return(obj->numCurrentSensors);
} // end of HAL_getNumCurrentSensors() function


//! \brief     Gets the number of voltage sensors
//! \param[in] handle  The hardware abstraction layer (HAL) handle
//! \return    The number of voltage sensors
static inline uint16_t HAL_getNumVoltageSensors(HAL_MTR_Handle handle)
{
  HAL_MTR_Obj *obj = (HAL_MTR_Obj *)handle;

  return(obj->numVoltageSensors);
} // end of HAL_getNumVoltageSensors() function

//! \brief     Gets the pwm enable status
//! \param[in]  handle  The hardware abstraction layer (HAL) handle
//! \return    The pwm enable
static inline bool HAL_getPwmEnableStatus(HAL_MTR_Handle handle)
{
  HAL_MTR_Obj *obj = (HAL_MTR_Obj *)handle;

  return(obj->flagEnablePWM);
} // end of HAL_getPwmStatus() function


//! \brief     Get the period of EPWM time-base module
//! \param[in] handle  The hardware abstraction layer (HAL) handle
//! \return    The periode of EPWM time-base module
static inline uint16_t
HAL_getTimeBasePeriod(HAL_MTR_Handle handle)
{
    HAL_MTR_Obj *obj = (HAL_MTR_Obj *)handle;

    return(EPWM_getTimeBasePeriod(obj->pwmHandle[0]));
} // end of HAL_getTimeBasePeriod() function


//! \brief      Configures the fault protection logic
//! \details    Sets up the trip zone inputs so that when a comparator
//!             signal from outside the micro-controller trips a fault,
//!             the EPWM peripheral blocks will force the
//!             power switches into a high impedance state.
//! \param[in]  handle  The hardware abstraction layer (HAL) handle
extern void HAL_setupMtrFaults(HAL_MTR_Handle handle);

//! \brief      Initializes the hardware abstraction layer (HAL) object
//! \details    Initializes all handles to the microcontroller peripherals.
//!             Returns a handle to the HAL object.
//! \param[in]  pMemory   A pointer to the memory for the hardware abstraction layer object
//! \param[in]  numBytes  The number of bytes allocated for the hardware abstraction layer object, bytes
//! \return     The hardware abstraction layer (HAL) object handle
extern HAL_Handle HAL_init(void *pMemory,const size_t numBytes);


//! \brief      Initializes the hardware abstraction layer (HAL) object
//! \details    Initializes all handles to the microcontroller peripherals.
//!             Returns a handle to the HAL_MTR object.
//! \param[in]  pMemory   A pointer to the memory for the hardware abstraction layer object
//! \param[in]  numBytes  The number of bytes allocated for the hardware abstraction layer object, bytes
//! \return     The hardware abstraction layer (HAL_MTR) object handle
extern HAL_MTR_Handle HAL_MTR1_init(void *pMemory, const size_t numBytes);


//! \brief      Initializes the interrupt vector table
//! \details    Points the ISR to the function mainISR.
//! \param[in]  handle  The hardware abstraction layer (HAL) handle
static inline void HAL_initIntVectorTable(HAL_Handle handle)
{
    // Motor_1->ADCA_INT1
    Interrupt_register(MTR1_PIE_INT_NUM, &motor1CtrlISR);

    return;
} // end of HAL_initIntVectorTable() function

//! \brief      Reads the ADC data with offset
//! \details    Reads in the ADC result registers and scales the values
//!             according to the settings in user_m1.h or user_m2.h.
//!             The structure gAdcData holds three phase voltages,
//!             three line currents, and one DC bus voltage.
//! \param[in]  handle    The hardware abstraction layer (HAL) handle
//! \param[in]  pADCData  A pointer to the ADC data buffer
static inline void
HAL_readMtr1ADCData(HAL_ADCData_t *pADCData)
{
    float32_t value;

    // convert phase A current
    value = (float32_t)ADC_readPPBResult(MTR1_IU_ADCRES_BASE, MTR1_IU_ADC_PPB_NUM);
    pADCData->I_A.value[0] = value * pADCData->current_sf;

    // convert phase B current
    value = (float32_t)ADC_readPPBResult(MTR1_IV_ADCRES_BASE, MTR1_IV_ADC_PPB_NUM);
    pADCData->I_A.value[1] = value * pADCData->current_sf;

    // convert phase C current
    value = (float32_t)ADC_readPPBResult(MTR1_IW_ADCRES_BASE, MTR1_IW_ADC_PPB_NUM);
    pADCData->I_A.value[2] = value * pADCData->current_sf;

#if defined(MOTOR1_FAST)
    // convert phase A voltage
    value = (float32_t)ADC_readResult(MTR1_VU_ADCRES_BASE, MTR1_VU_ADC_SOC_NUM);
    pADCData->V_V.value[0] = value * pADCData->voltage_sf;

    // convert phase B voltage
    value = (float32_t)ADC_readResult(MTR1_VV_ADCRES_BASE, MTR1_VV_ADC_SOC_NUM);
    pADCData->V_V.value[1] = value * pADCData->voltage_sf;

    // convert phase C voltage
    value = (float32_t)ADC_readResult(MTR1_VW_ADCRES_BASE, MTR1_VW_ADC_SOC_NUM);
    pADCData->V_V.value[2] = value * pADCData->voltage_sf;
#endif  // MOTOR1_FAST

    // convert dc bus voltage
    value = (float32_t)ADC_readResult(MTR1_VDC_ADCRES_BASE, MTR1_VDC_ADC_SOC_NUM);
    pADCData->VdcBus_V = value * pADCData->dcBusvoltage_sf;

#if defined(CMD_POT_EN)
    // read POT adc value
    pADCData->potAdc = ADC_readResult(MTR1_POT_ADCRES_BASE, MTR1_POT_ADC_SOC_NUM);
#endif  // CMD_POT_EN
    return;
} // end of HAL_readMtr1ADCData() functions

//! \brief     Reads the timer count
//! \param[in] handle       The hardware abstraction layer (HAL) handle
//! \param[in] timerNumber  The timer number, 0,1 or 2
//! \return    The timer count
static inline uint32_t
HAL_readTimerCnt(HAL_Handle handle,const uint16_t timerNumber)
{
    HAL_Obj *obj = (HAL_Obj *)handle;

    uint32_t timerCnt = CPUTimer_getTimerCount(obj->timerHandle[timerNumber]);

    return(timerCnt);
} // end of HAL_readTimerCnt() function

//! \brief     Sets the GPIO pin high
//! \param[in] handle      The hardware abstraction layer (HAL) handle
//! \param[in] gpioNumber  The GPIO number
static inline void HAL_setGPIOHigh(HAL_Handle handle,const uint32_t gpioNumber)
{

  // set GPIO high
  GPIO_writePin(gpioNumber, 1);

  return;
} // end of HAL_setGPIOHigh() function


//! \brief     Read the GPIO pin
//! \param[in] handle      The hardware abstraction layer (HAL) handle
//! \param[in] gpioNumber  The GPIO number
//! \return    The GPIO pin
static inline uint32_t
HAL_readGPIOData(HAL_Handle handle,const uint32_t gpioNumber)
{
    uint32_t gpioPinData;

    // set GPIO high
    gpioPinData = GPIO_readPin(gpioNumber);

    return(gpioPinData);
} // end of HAL_readGPIOData() function


//! \brief     Sets the GPIO pin low
//! \param[in] handle      The hardware abstraction layer (HAL) handle
//! \param[in] gpioNumber  The GPIO number
static inline void HAL_setGPIOLow(HAL_Handle handle,const uint32_t gpioNumber)
{
    // set GPIO low
    GPIO_writePin(gpioNumber, 0);

    return;
} // end of HAL_setGPIOLow() function


//! \brief     Sets the value of the internal DAC of the high comparator
//! \param[in] handle      The hardware abstraction layer (HAL) handle
//! \param[in] cmpssNumber The CMPSS number
//! \param[in] dacValue    The DAC value of the high comparator
static inline void
HAL_setCMPSSDACValueHigh(HAL_MTR_Handle handle,
                         const uint16_t cmpssNumber, uint16_t dacValue)
{
    HAL_MTR_Obj *obj = (HAL_MTR_Obj *)handle;

    // set GPIO low
    CMPSS_setDACValueHigh(obj->cmpssHandle[cmpssNumber], dacValue);

    return;
} // end of HAL_setCMPSSDACValueHigh() function


//! \brief     Sets the value of the internal DAC of the low comparator
//! \param[in] handle      The hardware abstraction layer (HAL) handle
//! \param[in] cmpssNumber The CMPSS number
//! \param[in] dacValue    The DAC value of the low comparator
static inline void
HAL_setCMPSSDACValueLow(HAL_MTR_Handle handle,
                        const uint16_t cmpssNumber, uint16_t dacValue)
{
  HAL_MTR_Obj *obj = (HAL_MTR_Obj *)handle;

  // set GPIO low
  CMPSS_setDACValueLow(obj->cmpssHandle[cmpssNumber], dacValue);

  return;
} // end of HAL_setCMPSSDACValueLow() function

//! \brief     Sets the number of voltage sensors
//! \param[in] handle             The hardware abstraction layer (HAL) handle
//! \param[in] numVoltageSensors  The number of voltage sensors
static inline void
HAL_setNumVoltageSensors(HAL_MTR_Handle handle,const uint16_t numVoltageSensors)
{
  HAL_MTR_Obj *obj = (HAL_MTR_Obj *)handle;

  obj->numVoltageSensors = numVoltageSensors;

  return;
} // end of HAL_setNumVoltageSensors() function

//! \brief     Sets the number of current sensors
//! \param[in] handle             The hardware abstraction layer (HAL) handle
//! \param[in] numCurrentSensors  The number of current sensors
static inline void
HAL_setNumCurrentSensors(HAL_MTR_Handle handle,const uint16_t numCurrentSensors)
{
  HAL_MTR_Obj *obj = (HAL_MTR_Obj *)handle;

  obj->numCurrentSensors = numCurrentSensors;

  return;
} // end of HAL_setNumCurrentSensors() function

//! \brief      Sets the hardware abstraction layer parameters
//! \details    Sets up the microcontroller peripherals.  Creates all of the scale
//!             factors for the ADC voltage and current conversions.  Sets the initial
//!             offset values for voltage and current measurements.
//! \param[in]  handle       The hardware abstraction layer (HAL) handle
extern void HAL_setParams(HAL_Handle handle);

//! \brief      Sets the hardware abstraction layer parameters
//! \details    Sets up the microcontroller peripherals.  Creates all of the scale
//!             factors for the ADC voltage and current conversions.  Sets the initial
//!             offset values for voltage and current measurements.
//! \param[in]  handle       The hardware abstraction layer (HAL) handle
extern void HAL_MTR_setParams(HAL_MTR_Handle handle, USER_Params *pUserParams);

//! \brief      Sets up the ADCs (Analog to Digital Converters)
//! \param[in]  handle  The hardware abstraction layer (HAL) handle
extern void HAL_setupADCs(HAL_Handle handle);

#if defined(BUFDAC_MODE)
//! \brief      Sets up the DACs (Digital to Analog Converters)
//! \param[in]  handle  The hardware abstraction layer (HAL) handle
extern void HAL_setupDACs(HAL_Handle handle);
#endif  // BUFDAC_MODE

#if defined(MOTOR1_HALL) && defined(CMD_CAP_EN)
#error HALL and CMD_CAP can't be enabled at the same time
#elif defined(MOTOR1_HALL)
//! \brief      Sets up the CAP (Capture Subsystems)
//! \param[in]  handle  The hardware abstraction layer (HAL) handle
extern void HAL_setupCAPs(HAL_MTR_Handle handle);

//! \brief      Sets up the CAP (Capture Subsystems)
//! \param[in]  handle  The hardware abstraction layer (HAL) handle
extern void HAL_resetCAPTimeStamp(HAL_MTR_Handle handle);

//! \brief     Read the CAP counters
//! \param[in] handle      The hardware abstraction layer (HAL) handle
//! \return    The CAP counters
static inline uint32_t HAL_calcCAPCount(HAL_MTR_Handle handle)
{
    HAL_MTR_Obj    *obj = (HAL_MTR_Obj *)handle;
    uint16_t  cnt;
    uint32_t capSumCount = 0;

    for(cnt = 0; cnt < 3; cnt++)
    {
        capSumCount += ECAP_getEventTimeStamp(obj->capHandle[cnt], ECAP_EVENT_2);
        capSumCount += ECAP_getEventTimeStamp(obj->capHandle[cnt], ECAP_EVENT_3);
    }

    return(capSumCount);
}
#elif defined(CMD_CAP_EN)
//! \brief      Sets up the CAP (Capture Subsystems)
//! \param[in]  handle  The hardware abstraction layer (HAL) handle
extern void HAL_setupCAPs(HAL_MTR_Handle handle);

//! \brief      Sets up the CAP (Capture Subsystems)
//! \param[in]  handle  The hardware abstraction layer (HAL) handle
extern void HAL_resetCAPTimeStamp(HAL_MTR_Handle handle);

//! \brief     Read the CAP counters
//! \param[in] handle      The hardware abstraction layer (HAL) handle
//! \return    The CAP counters
static inline uint32_t HAL_calcCAPCount(HAL_MTR_Handle handle)
{
    HAL_MTR_Obj    *obj = (HAL_MTR_Obj *)handle;

    uint32_t capSumCount = ECAP_getEventTimeStamp(obj->capHandle, ECAP_EVENT_1);
    capSumCount += ECAP_getEventTimeStamp(obj->capHandle, ECAP_EVENT_2);
    capSumCount += ECAP_getEventTimeStamp(obj->capHandle, ECAP_EVENT_3);
    capSumCount += ECAP_getEventTimeStamp(obj->capHandle, ECAP_EVENT_4);

    return(capSumCount);
}
#endif  // MOTOR1_HALL || CMD_CAP_EN

//! \brief      Sets up the CMPSSs (Comparator Subsystems)
//! \param[in]  handle  The hardware abstraction layer (HAL) handle
extern void HAL_setupCMPSSs(HAL_MTR_Handle handle);

//! \brief      Sets up the clocks
//! \details    Sets up the micro-controller's main oscillator
//! \param[in]  handle  The hardware abstraction layer (HAL) handle
extern void HAL_setupClks(HAL_Handle handle);

//! \brief     Sets up the GPIO (General Purpose I/O) pins
//! \param[in] handle  The hardware abstraction layer (HAL) handle
extern void HAL_setupGPIOs(HAL_Handle handle);

//! \brief     Sets up the FLASH.
//! \param[in] handle  The hardware abstraction layer (HAL) handle
extern void HAL_setupFlash(HAL_Handle handle);

//! \brief     Sets up the CLA
//! \param[in] handle  The hardware abstraction layer (HAL) handle
extern void HAL_setupCLA(HAL_Handle handle);

//! \brief     Sets up the PIE (Peripheral Interrupt Expansion)
//! \param[in] handle  The hardware abstraction layer (HAL) handle
extern void HAL_setupPIE(HAL_Handle handle);

#if defined(MOTOR1_ENC)
//! \brief     Sets up the QEP peripheral
//! \param[in] handle  The hardware abstraction layer (HAL) handle
extern void HAL_setupQEP(HAL_MTR_Handle handle);
#endif  // MOTOR1_ENC

//! \brief     Sets up the SCIA
//! \param[in] handle  The hardware abstraction layer (HAL) handle
extern void HAL_setupSCIA(HAL_Handle handle);

//! \brief     Sets up the I2CA
//! \param[in] handle  The hardware abstraction layer (HAL) handle
extern void HAL_setupI2CA(HAL_Handle halHandle);

#if defined(BSXL8323RS_REVA) || defined(BSXL8353RS_REVA)
//! \brief      Enables the 8323rs/8353rs/8316rs device
//! \details    Provides the correct timing to enable the drv8320
//! \param[in]  handle  The hardware abstraction layer (HAL) handle
extern void HAL_enableDRV(HAL_MTR_Handle handle);

//! \brief     Sets up the GATE object
//! \param[in] handle       The hardware abstraction layer (HAL) handle
extern void HAL_setupGate(HAL_MTR_Handle handle);

//! \brief     Writes data to the driver
//! \param[in] handle         The hardware abstraction layer (HAL) handle
//! \param[in] drv8323Vars  SPI variables
void HAL_writeDRVData(HAL_MTR_Handle handle, DRVIC_VARS_t *drvicVars);

//! \brief     Reads data from the driver
//! \param[in] handle         The hardware abstraction layer (HAL) handle
//! \param[in] drv8323Vars  SPI variables
void HAL_readDRVData(HAL_MTR_Handle handle, DRVIC_VARS_t *drvicVars);

//! \brief     Sets up the SPI interface for the driver
//! \param[in] handle         The hardware abstraction layer (HAL) handle
//! \param[in] drv8323Vars  SPI variables
extern void HAL_setupDRVSPI(HAL_MTR_Handle handle, DRVIC_VARS_t *drvicVars);

//! \brief     Sets up the SPI
//! \param[in] handle  The hardware abstraction layer (HAL) handle
extern void HAL_setupSPI(HAL_MTR_Handle handle);
       // BSXL8323RS_REVA || BSXL8353RS_REVA
#elif defined(BSXL3PHGAN_REVA)
//! \brief      Enables the gate driver
//! \details    Provides the correct timing to enable the gate driver
//! \param[in]  handle  The hardware abstraction layer (HAL) handle
extern void HAL_enableDRV(HAL_MTR_Handle handle);
// BSXL3PHGAN_REVA
#endif  // Declare HAL_setupGate and HAL_enableDRV

//! \brief     Sets up the CPU timer for time base
//! \param[in] handle          The hardware abstraction layer (HAL) handle
//! \param[in] systemFreq_MHz  The system frequency, MHz
extern void HAL_setupTimeBaseTimer(HAL_Handle handle,
                                   const float32_t timeBaseFreq_Hz);

//! \brief     Sets up the CPU timer for ADC trigger
//! \param[in] handle          The hardware abstraction layer (HAL) handle
//! \param[in] systemFreq_MHz  The system frequency, MHz
extern void HAL_setupADCTriggerTimer(HAL_Handle handle,
                                     const float32_t adcTriggerFreq_Hz);

//! \brief     Sets up the timers for CPU usage diagnostics
//! \param[in] handle          The hardware abstraction layer (HAL) handle
extern void HAL_setupCPUUsageTimer(HAL_Handle handle);

//! \brief     Sets up the timers
//! \param[in] handle          The hardware abstraction layer (HAL) handle
//! \param[in] cpuTimerNumber  The CPU timer number
static inline void
HAL_clearCPUTimerFlag(HAL_Handle halHandle, const uint16_t cpuTimerNumber)
{
    HAL_Obj   *obj = (HAL_Obj *)halHandle;

    CPUTimer_clearOverflowFlag(obj->timerHandle[cpuTimerNumber]);

    return;
}   // end of HAL_clearTimerFlag() function


//! Sets the CPU timer period count.
//! \param base is the base address of the timer module.
//! \param timerCount is the CPU timer timer count.
static inline void HAL_setCPUTimerCount(HAL_Handle halHandle, uint32_t timerCount)
{
    HAL_Obj   *obj = (HAL_Obj *)halHandle;

    //
    // Load the MSB timer Count
    //
    HWREG(obj->timerHandle[1] + CPUTIMER_O_TIM) = timerCount;

    return;
}

//! \brief     Gets CPU Timer status
//! \param[in] handle          The hardware abstraction layer (HAL) handle
//! \param[in] cpuTimerNumber  The CPU timer number
static inline bool
HAL_getCPUTimerStatus(HAL_Handle halHandle, const uint16_t cpuTimerNumber)
{
    HAL_Obj   *obj = (HAL_Obj *)halHandle;

    return (CPUTimer_getTimerOverflowStatus(obj->timerHandle[cpuTimerNumber]));
}

//! \brief     Sets up the DMA
//! \param[in] N/A
extern void HAL_setupDMA(void);


#if defined(DATALOGF4_EN) || defined(DATALOGI4_EN) || defined(DATALOGF2_EN)
//! \brief     Sets up the DMA for datalog
//! \param[in] handle          The hardware abstraction layer (HAL) handle
//! \param[in] dmaChNumber     The DMC Channel Number
//! \param[in] destAddr    The Datalog buffer dest address
//! \param[in] srcAddr     The Datalog buffer src address
void HAL_setupDMAforDLOG(HAL_Handle handle, const uint16_t dmaChNum,
                     const void *destAddr, const void *srcAddr);

//! \brief     Force trig the DMA channel for datalog
//! \param[in] handle          The hardware abstraction layer (HAL) handle
//! \param[in] dmaChNumber     The DMC Channel Number
static inline void
HAL_trigDMAforDLOG(HAL_Handle handle, const uint16_t DMAChNum)
{
    HAL_Obj *obj = (HAL_Obj *)handle;

    DMA_startChannel(obj->dmaChHandle[DMAChNum]);

    DMA_forceTrigger(obj->dmaChHandle[DMAChNum]);

    return;
} // end of HAL_trigDlogWithDMA() function
#endif  // DATALOGF4_EN || DATALOGF2_EN || DATALOGI4_EN

//! \brief     Toggles the GPIO pin
//! \param[in] handle      The hardware abstraction layer (HAL) handle
//! \param[in] gpioNumber  The GPIO number
static inline void HAL_toggleGPIO(HAL_Handle handle,const uint32_t gpioNumber)
{

    // set GPIO high
    GPIO_togglePin(gpioNumber);

    return;
} // end of HAL_toggleGPIO() function

//! \brief     Writes DAC data to the PWM comparators for DAC output
//! \param[in] handle    The hardware abstraction layer (HAL) handle
//! \param[in] pPWMDACData  The pointer to the DAC data
void HAL_setPWMDACParameters(HAL_Handle handle, HAL_PWMDACData_t *pPWMDACData);


//! \brief      Clear assigned memory
//! \param[in]  The memory start address
//! \param[in]  The memory size
void HAL_clearDataRAM(void *pMemory, uint16_t lengthMemory);

//! \brief     Reads PWM period register
//! \param[in] handle     The hardware abstraction layer (HAL) handle
//! \param[in] pwmNumber  The PWM number
//! \return    The PWM period value
static inline uint16_t
HAL_readPWMPeriod(HAL_MTR_Handle handle,const uint16_t pwmNumber)
{
  HAL_MTR_Obj *obj = (HAL_MTR_Obj *)handle;

  // the period value to be returned
  uint16_t pwmPeriodValue;

  pwmPeriodValue = EPWM_getTimeBasePeriod(obj->pwmHandle[pwmNumber]);

  return(pwmPeriodValue);
} // end of HAL_readPWMPeriod() function

//! \brief     Writes PWM data to the PWM comparators for motor control
//! \param[in] handle    The hardware abstraction layer (HAL) handle
//! \param[in] pPWMData  The pointer to the PWM data
static inline void
HAL_writePWMData(HAL_MTR_Handle handle, HAL_PWMData_t *pPWMData)
{
    HAL_MTR_Obj *obj = (HAL_MTR_Obj *)handle;

    float32_t period = (float32_t)(EPWM_getTimeBasePeriod(obj->pwmHandle[0]));

    uint16_t pwmCnt;

    for(pwmCnt=0; pwmCnt<3; pwmCnt++)
    {
      // compute the value
        float32_t V_pu = -pPWMData->Vabc_pu.value[pwmCnt];      // Negative
        float32_t V_sat_pu = __fsat(V_pu, 0.5, -0.5);           // -0.5~0.5
        float32_t V_sat_dc_pu = V_sat_pu + 0.5;                 // 0~1.0
        pPWMData->cmpValue[pwmCnt]  = (int16_t)(V_sat_dc_pu * period);  //

        if(pPWMData->cmpValue[pwmCnt] < pPWMData->minCMPValue)
        {
            pPWMData->cmpValue[pwmCnt] = pPWMData->minCMPValue;
        }

        // write the PWM data value
        EPWM_setCounterCompareValue(obj->pwmHandle[pwmCnt],
                                    EPWM_COUNTER_COMPARE_A,
                                    pPWMData->cmpValue[pwmCnt]);

        EPWM_setCounterCompareValue(obj->pwmHandle[pwmCnt],
                                    EPWM_COUNTER_COMPARE_B,
                                    pPWMData->cmpValue[pwmCnt]);
    }

    return;
} // end of HAL_writePWMData() function


//! \brief      Enables the PWM devices for motor control
//! \details    Turns on the outputs of the EPWM peripheral which will allow
//!             the power switches to be controlled.
//! \param[in]  handle  The hardware abstraction layer (HAL) handle
static inline void HAL_enablePWM(HAL_MTR_Handle handle)
{
    HAL_MTR_Obj *obj = (HAL_MTR_Obj *)handle;

    // Clear any comparator digital filter output latch
    CMPSS_clearFilterLatchHigh(obj->cmpssHandle[0]);
    CMPSS_clearFilterLatchLow(obj->cmpssHandle[0]);

    CMPSS_clearFilterLatchHigh(obj->cmpssHandle[1]);
    CMPSS_clearFilterLatchLow(obj->cmpssHandle[1]);

    CMPSS_clearFilterLatchHigh(obj->cmpssHandle[2]);
    CMPSS_clearFilterLatchLow(obj->cmpssHandle[2]);

    CMPSS_clearFilterLatchHigh(obj->cmpssHandle[3]);
    CMPSS_clearFilterLatchLow(obj->cmpssHandle[3]);

    CMPSS_clearFilterLatchHigh(obj->cmpssHandle[4]);
    CMPSS_clearFilterLatchLow(obj->cmpssHandle[4]);

    CMPSS_clearFilterLatchHigh(obj->cmpssHandle[5]);
    CMPSS_clearFilterLatchLow(obj->cmpssHandle[5]);

    // Clear any Trip Zone flag
    EPWM_clearTripZoneFlag(obj->pwmHandle[0], HAL_TZFLAG_INTERRUPT_ALL);
    EPWM_clearTripZoneFlag(obj->pwmHandle[1], HAL_TZFLAG_INTERRUPT_ALL);
    EPWM_clearTripZoneFlag(obj->pwmHandle[2], HAL_TZFLAG_INTERRUPT_ALL);

#if defined(BSXL3PHGAN_REVA)
    GPIO_writePin(obj->gateEnableGPIO, 0);
#endif   // BSXL3PHGAN_REVA

    obj->flagEnablePWM = true;

    return;
} // end of HAL_enablePWM() function


//! \brief      Enables the PWM for braking
//! \details    Turns on the outputs of the EPWM peripheral which will allow
//!             the power switches to be controlled.
//! \param[in]  handle  The hardware abstraction layer (HAL) handle
static inline void HAL_enableBrakePWM(HAL_MTR_Handle handle)
{
    HAL_MTR_Obj *obj = (HAL_MTR_Obj *)handle;
    uint16_t  cnt;

#if defined(BSXL3PHGAN_REVA)
    for(cnt=0; cnt<3; cnt++)
    {
        // setup the Action-qualifier Continuous Software Force Register (AQCSFRC)
         EPWM_setActionQualifierContSWForceAction(obj->pwmHandle[cnt],
                                                  EPWM_AQ_OUTPUT_A,
                                                  EPWM_AQ_SW_OUTPUT_LOW);

        // setup the Action-qualifier Continuous Software Force Register (AQCSFRC)
         EPWM_setActionQualifierContSWForceAction(obj->pwmHandle[cnt],
                                                  EPWM_AQ_OUTPUT_B,
                                                  EPWM_AQ_SW_OUTPUT_HIGH);

         // setup the Dead-Band Generator Control Register (DBCTL)
         EPWM_setDeadBandDelayMode(obj->pwmHandle[cnt], EPWM_DB_RED, false);
         EPWM_setDeadBandDelayMode(obj->pwmHandle[cnt], EPWM_DB_FED, false);
    }

    GPIO_writePin(obj->gateEnableGPIO, 0);
#else   // !BSXL3PHGAN_REVA
    for(cnt=0; cnt<3; cnt++)
    {
        // setup the Action-qualifier Continuous Software Force Register (AQCSFRC)
         EPWM_setActionQualifierContSWForceAction(obj->pwmHandle[cnt],
                                                  EPWM_AQ_OUTPUT_A,
                                                  EPWM_AQ_SW_OUTPUT_LOW);

        // setup the Action-qualifier Continuous Software Force Register (AQCSFRC)
         EPWM_setActionQualifierContSWForceAction(obj->pwmHandle[cnt],
                                                  EPWM_AQ_OUTPUT_B,
                                                  EPWM_AQ_SW_OUTPUT_HIGH);

         // setup the Dead-Band Generator Control Register (DBCTL)
         EPWM_setDeadBandDelayMode(obj->pwmHandle[cnt], EPWM_DB_RED, false);
         EPWM_setDeadBandDelayMode(obj->pwmHandle[cnt], EPWM_DB_FED, false);
    }
#endif  // !BSXL3PHGAN_REVA

    obj->flagEnablePWM = false;

    return;
} // end of HAL_enableBrakePWM() function


//! \brief      Enables the PWM for braking
//! \details    Turns on the outputs of the EPWM peripheral which will allow
//!             the power switches to be controlled.
//! \param[in]  handle  The hardware abstraction layer (HAL) handle
static inline void HAL_exitBrakeResetPWM(HAL_MTR_Handle handle)
{
    HAL_MTR_Obj *obj = (HAL_MTR_Obj *)handle;
    uint16_t  cnt;

#if defined(BSXL3PHGAN_REVA)
    for(cnt=0; cnt<3; cnt++)
    {
        // setup the Dead-Band Generator Control Register (DBCTL)
        EPWM_setDeadBandDelayMode(obj->pwmHandle[cnt], EPWM_DB_RED, true);
        EPWM_setDeadBandDelayMode(obj->pwmHandle[cnt], EPWM_DB_FED, true);

        // setup the Action-qualifier Continuous Software Force Register (AQCSFRC)
         EPWM_setActionQualifierContSWForceAction(obj->pwmHandle[cnt],
                                                  EPWM_AQ_OUTPUT_A,
                                                  EPWM_AQ_SW_DISABLED);

        // setup the Action-qualifier Continuous Software Force Register (AQCSFRC)
         EPWM_setActionQualifierContSWForceAction(obj->pwmHandle[cnt],
                                                  EPWM_AQ_OUTPUT_B,
                                                  EPWM_AQ_SW_DISABLED);
    }

    GPIO_writePin(obj->gateEnableGPIO, 0);
    // BSXL3PHGAN_REVA
#else   // !BSXL3PHGAN_REVA
    for(cnt=0; cnt<3; cnt++)
    {
        // setup the Dead-Band Generator Control Register (DBCTL)
        EPWM_setDeadBandDelayMode(obj->pwmHandle[cnt], EPWM_DB_RED, true);
        EPWM_setDeadBandDelayMode(obj->pwmHandle[cnt], EPWM_DB_FED, true);

        // setup the Action-qualifier Continuous Software Force Register (AQCSFRC)
         EPWM_setActionQualifierContSWForceAction(obj->pwmHandle[cnt],
                                                  EPWM_AQ_OUTPUT_A,
                                                  EPWM_AQ_SW_DISABLED);

        // setup the Action-qualifier Continuous Software Force Register (AQCSFRC)
         EPWM_setActionQualifierContSWForceAction(obj->pwmHandle[cnt],
                                                  EPWM_AQ_OUTPUT_B,
                                                  EPWM_AQ_SW_DISABLED);
    }
#endif  // !BSXL3PHGAN_REVA

    obj->flagEnablePWM = false;

    return;
} // end of HAL_enableBrakePWM() function

//! \brief      clear fault status of motor control
//! \details
//! \param[in]  handle  The hardware abstraction layer (HAL) handle
static inline void HAL_clearMtrFaultStatus(HAL_MTR_Handle handle)
{
    HAL_MTR_Obj *obj = (HAL_MTR_Obj *)handle;

    // Clear any comparator digital filter output latch
    CMPSS_clearFilterLatchHigh(obj->cmpssHandle[0]);
    CMPSS_clearFilterLatchLow(obj->cmpssHandle[0]);

    CMPSS_clearFilterLatchHigh(obj->cmpssHandle[1]);
    CMPSS_clearFilterLatchLow(obj->cmpssHandle[1]);

    CMPSS_clearFilterLatchHigh(obj->cmpssHandle[2]);
    CMPSS_clearFilterLatchLow(obj->cmpssHandle[2]);

    CMPSS_clearFilterLatchHigh(obj->cmpssHandle[3]);
    CMPSS_clearFilterLatchLow(obj->cmpssHandle[3]);

    CMPSS_clearFilterLatchHigh(obj->cmpssHandle[4]);
    CMPSS_clearFilterLatchLow(obj->cmpssHandle[4]);

    CMPSS_clearFilterLatchHigh(obj->cmpssHandle[5]);
    CMPSS_clearFilterLatchLow(obj->cmpssHandle[5]);

    // Clear any Trip Zone flag
    EPWM_clearTripZoneFlag(obj->pwmHandle[0], HAL_TZFLAG_INTERRUPT_ALL);
    EPWM_clearTripZoneFlag(obj->pwmHandle[1], HAL_TZFLAG_INTERRUPT_ALL);
    EPWM_clearTripZoneFlag(obj->pwmHandle[2], HAL_TZFLAG_INTERRUPT_ALL);

    return;
} // end of HAL_clearMtrFaultStatus() function

//! \brief      Disables the PWM device for motor control
//! \details    Turns off the outputs of the EPWM peripherals which will put
//!             the power switches into a high impedance state.
//! \param[in]  handle  The hardware abstraction layer (HAL) handle
static inline void HAL_disablePWM(HAL_MTR_Handle handle)
{
  HAL_MTR_Obj *obj = (HAL_MTR_Obj *)handle;

  EPWM_forceTripZoneEvent(obj->pwmHandle[0], EPWM_TZ_FORCE_EVENT_OST);
  EPWM_forceTripZoneEvent(obj->pwmHandle[1], EPWM_TZ_FORCE_EVENT_OST);
  EPWM_forceTripZoneEvent(obj->pwmHandle[2], EPWM_TZ_FORCE_EVENT_OST);

#if defined(BSXL3PHGAN_REVA)
    GPIO_writePin(obj->gateEnableGPIO, 1);
#endif  // BSXL3PHGAN_REVA


  obj->flagEnablePWM = false;

  return;
} // end of HAL_disablePWM() function

//! \brief     Sets up the PWMs (Pulse Width Modulators)
//! \param[in] handle          The hardware abstraction layer (HAL) handle
extern void HAL_setupPWMs(HAL_MTR_Handle handle);

//! \brief     Sets up the PWMs (Pulse Width Modulators)
//! \param[in] handle          The hardware abstraction layer (HAL) handle
static inline uint16_t HAL_getMtrTripFaults(HAL_MTR_Handle handle)
{
    HAL_MTR_Obj *obj = (HAL_MTR_Obj *)handle;
    uint16_t tripFault = 0;

    tripFault = (EPWM_getTripZoneFlagStatus(obj->pwmHandle[0]) &
            (EPWM_TZ_FLAG_OST | EPWM_TZ_FLAG_DCAEVT1 | EPWM_TZ_FLAG_DCAEVT2)) |
                    (EPWM_getTripZoneFlagStatus(obj->pwmHandle[1]) &
            (EPWM_TZ_FLAG_OST | EPWM_TZ_FLAG_DCAEVT1 | EPWM_TZ_FLAG_DCAEVT2)) |
                    (EPWM_getTripZoneFlagStatus(obj->pwmHandle[2]) &
            (EPWM_TZ_FLAG_OST | EPWM_TZ_FLAG_DCAEVT1 | EPWM_TZ_FLAG_DCAEVT2));

    return(tripFault);
}

//! \brief     Sets up the PWMs (Pulse Width Modulators)
//! \param[in] handle          The hardware abstraction layer (HAL) handle
extern void HAL_setMtrCMPSSDACValue(HAL_MTR_Handle handle,
                               const uint16_t dacValH, const uint16_t dacValL);

#if defined(MOTOR1_OVM)
//! \brief     Set trigger point in the middle of the low side pulse
//! \param[in] handle    The hardware abstraction layer (HAL) handle
//! \param[in] ignoreShunt  The low side shunt that should be ignored
//! \param[in] midVolShunt  The middle length of output voltage
static inline void HAL_setTrigger(HAL_MTR_Handle handle, HAL_PWMData_t *pPWMData,
                                  const SVGENCURRENT_IgnoreShunt_e ignoreShunt,
                                  const SVGENCURRENT_VmidShunt_e midVolShunt)
{
    HAL_MTR_Obj *obj = (HAL_MTR_Obj *)handle;

    int16_t pwmNum = midVolShunt;
    int16_t pwmCMPA = EPWM_getCounterCompareValue(obj->pwmHandle[pwmNum],
                                                   EPWM_COUNTER_COMPARE_A);

    int16_t pwmSOCCMP = 5;

    if(ignoreShunt == SVGENCURRENT_USE_ALL)
    {
        // Set up event source for ADC trigger
        EPWM_setADCTriggerSource(obj->pwmHandle[0],
                                 EPWM_SOC_A,
                                 EPWM_SOC_TBCTR_D_CMPC);
    }
    else
    {
        pwmSOCCMP = pwmCMPA - pPWMData->deadband - pPWMData->noiseWindow;

        if(pwmSOCCMP <= 0)
        {
            pwmSOCCMP = 5;

            // Set up event source for ADC trigger
            EPWM_setADCTriggerSource(obj->pwmHandle[0],
                                     EPWM_SOC_A,
                                     EPWM_SOC_TBCTR_U_CMPC);
        }
        else
        {
            pwmSOCCMP = 5;

            // Set up event source for ADC trigger
            EPWM_setADCTriggerSource(obj->pwmHandle[0],
                                     EPWM_SOC_A,
                                     EPWM_SOC_TBCTR_D_CMPC);
        }

    }

    //
    pPWMData->socCMP = pwmSOCCMP;

    // write the PWM data value  for ADC trigger
    EPWM_setCounterCompareValue(obj->pwmHandle[0],
                                EPWM_COUNTER_COMPARE_C,
                                pwmSOCCMP);
    return;
} // end of HAL_setTrigger() function
#endif  // MOTOR1_OVM

//! \brief     Set trigger point in the middle of the low side pulse
//! \param[in] handle    The hardware abstraction layer (HAL) handle
//! \param[in] deadband     The setting deadband for mosfet gate driver
//! \param[in] noisewindow  The noise window
//! \param[in] adcSample_us The adc sample time
extern void HAL_setTriggerPrams(HAL_PWMData_t *pPWMData,
                                const float32_t systemFreq_MHz, const float32_t deadband_us,
                                const float32_t noiseWindow_us, const float32_t adcSample_us);





//! \brief     Sets up the gate driver for inverter board
//! \param[in] handle  The hardware abstraction layer (HAL) handle
extern bool HAL_MTR_setGateDriver(HAL_MTR_Handle handle);

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // end of HAL_H definition

