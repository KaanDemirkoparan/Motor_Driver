//##############################################################################
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
//##############################################################################

//! \file   hal.h
//! \brief  Contains public interface to various functions related
//!         to the HAL object
//!

#ifndef _HAL_H_
#define _HAL_H_


// **************************************************************************
// the includes


// platforms
#include "hal_obj.h"

#include "svgen_current.h"


//!
//!
//! \defgroup HAL HAL
//!
//@{


#ifdef __cplusplus
extern "C" {
#endif


// **************************************************************************
// the defines

//! Trip Zones all interrupt
//!
#define HAL_TZ_INTERRUPT_ALL     ( EPWM_TZ_INTERRUPT_DCBEVT2 \
                                 + EPWM_TZ_INTERRUPT_DCBEVT1 \
                                 + EPWM_TZ_INTERRUPT_DCAEVT2 \
                                 + EPWM_TZ_INTERRUPT_DCAEVT1 \
                                 + EPWM_TZ_INTERRUPT_OST \
                                 + EPWM_TZ_INTERRUPT_CBC )

#define HAL_TZSEL_SIGNALS_ALL     ( EPWM_TZ_SIGNAL_CBC1 |                      \
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
                                    EPWM_TZ_SIGNAL_DCBEVT1 )

//! \brief Defines the comparator number for current prection
//!
#define HAL_NUM_CMPSS_CURRENT           3

#ifdef BOOSTXL_DRV8323RS
//! \brief Defines the gpio for the nFAULT of Power Module device
//!
#define HAL_PM_nFAULT_GPIO            72

//! \brief Defines the gpio for the SPI_CS of DRV device
//!
#define HAL_DRV_SPI_CS_GPIO           57

//! \brief Defines the gpio for the enable gate of DRV device
//!
#define HAL_DRV_EN_GATE_GPIO          42

#endif // !BOOSTXL_DRV8323RS

//! \brief Defines the function to turn LEDs off
//!
#define HAL_turnLEDOff            HAL_setGPIOLow

//! \brief Defines the function to turn LEDs on
//!
#define HAL_turnLEDOn             HAL_setGPIOHigh

//! \brief Defines the function to turn LEDs on
//!
#define HAL_toggleLED             HAL_toggleGPIO

//------------------------------------------------------------------------------
// ePWM
#define MTR1_PWM_U_BASE         EPWM5_BASE
#define MTR1_PWM_V_BASE         EPWM4_BASE
#define MTR1_PWM_W_BASE         EPWM6_BASE

// XBAR-EPWM
#define MTR1_XBAR_TRIP_ADDRL    XBAR_O_OUT1MUX0TO15CFG
#define MTR1_XBAR_TRIP_ADDRH    XBAR_O_OUT1MUX16TO31CFG

#define MTR1_XBAR_INPUT1        XBAR_INPUT1
#define MTR1_TZ_OSHT1           EPWM_TZ_SIGNAL_OSHT1

#define MTR1_XBAR_TRIP          XBAR_TRIP4
#define MTR1_DCTRIPIN           EPWM_DC_COMBINATIONAL_TRIPIN4

#ifdef BOOSTXL_DRV8323RS
//------------------------------------------------------------------------------
// ADC
#define MTR1_ADC_TRIGGER_SOC        ADC_TRIGGER_EPWM5_SOCA  // EPWM5_SOCA
#define MTR1_ADC_TRIGGER_COUNT      3 // PWM-CMPC Value for ADC trigger
#define MTR1_ADC_I_SAMPLEWINDOW     14
#define MTR1_ADC_V_SAMPLEWINDOW     20

// Phase Current:
#define MTR1_IU_ADC_BASE        ADCB_BASE               // ADC-B7
#define MTR1_IV_ADC_BASE        ADCA_BASE               // ADC-A7
#define MTR1_IW_ADC_BASE        ADCC_BASE               // ADC-C4

#define MTR1_IU_ADCRES_BASE     ADCBRESULT_BASE
#define MTR1_IV_ADCRES_BASE     ADCARESULT_BASE
#define MTR1_IW_ADCRES_BASE     ADCCRESULT_BASE

#define MTR1_IU_ADC_CH_NUM      ADC_CH_ADCIN7
#define MTR1_IV_ADC_CH_NUM      ADC_CH_ADCIN7
#define MTR1_IW_ADC_CH_NUM      ADC_CH_ADCIN4

#define MTR1_IU_ADC_SOC_NUM     ADC_SOC_NUMBER1         // SOC1-PPB1
#define MTR1_IV_ADC_SOC_NUM     ADC_SOC_NUMBER1         // SOC1-PPB1
#define MTR1_IW_ADC_SOC_NUM     ADC_SOC_NUMBER1         // SOC1-PPB1

#define MTR1_IU_ADC_PPB_NUM     ADC_PPB_NUMBER1
#define MTR1_IV_ADC_PPB_NUM     ADC_PPB_NUMBER1
#define MTR1_IW_ADC_PPB_NUM     ADC_PPB_NUMBER1

// Voltage, Phase and dc-bus
#define MTR1_VU_ADC_BASE        ADCC_BASE               // ADC-C14
#define MTR1_VV_ADC_BASE        ADCA_BASE               // ADC-A4
#define MTR1_VW_ADC_BASE        ADCB_BASE               // ADC-B4
#define MTR1_VDC_ADC_BASE       ADCB_BASE               // ADC-B0

#define MTR1_VU_ADCRES_BASE     ADCCRESULT_BASE
#define MTR1_VV_ADCRES_BASE     ADCARESULT_BASE
#define MTR1_VW_ADCRES_BASE     ADCBRESULT_BASE
#define MTR1_VDC_ADCRES_BASE    ADCBRESULT_BASE

#define MTR1_VU_ADC_CH_NUM      ADC_CH_ADCIN14
#define MTR1_VV_ADC_CH_NUM      ADC_CH_ADCIN4
#define MTR1_VW_ADC_CH_NUM      ADC_CH_ADCIN4
#define MTR1_VDC_ADC_CH_NUM     ADC_CH_ADCIN0

#define MTR1_VU_ADC_SOC_NUM     ADC_SOC_NUMBER2         // SOC2
#define MTR1_VV_ADC_SOC_NUM     ADC_SOC_NUMBER2         // SOC2
#define MTR1_VW_ADC_SOC_NUM     ADC_SOC_NUMBER2         // SOC2
#define MTR1_VDC_ADC_SOC_NUM    ADC_SOC_NUMBER3         // SOC3

//------------------------------------------------------------------------------
// CMPSS
#define MTR1_CMPSS_U_BASE       CMPSS7_BASE
#define MTR1_CMPSS_V_BASE       CMPSS9_BASE
#define MTR1_CMPSS_W_BASE       CMPSS5_BASE

#define MTR1_IU_CMPHP_SEL       ASYSCTL_CMPHPMUX_SELECT_7    // CMPSS7H-B7
#define MTR1_IU_CMPLP_SEL       ASYSCTL_CMPLPMUX_SELECT_7    // CMPSS7L-B7

#define MTR1_IV_CMPHP_SEL       ASYSCTL_CMPHPMUX_SELECT_9    // CMPSS9H-A7
//#define MTR1_IV_CMPLP_SEL       X    // X

#define MTR1_IW_CMPHP_SEL       ASYSCTL_CMPHPMUX_SELECT_5    // CMPSS5H-C4
#define MTR1_IW_CMPLP_SEL       ASYSCTL_CMPLPMUX_SELECT_5    // CMPSS5L-C4

#define MTR1_IU_CMPHP_MUX       2
#define MTR1_IU_CMPLP_MUX       2

#define MTR1_IV_CMPHP_MUX       2
//#define MTR1_IV_CMPLP_MUX       X

#define MTR1_IW_CMPHP_MUX       0
#define MTR1_IW_CMPLP_MUX       0

#define MTR1_CMPSS_DACH_VALUE   (2048 + 1024 + 512)
#define MTR1_CMPSS_DACL_VALUE   (2048 - 1024 - 512)

// XBAR-EPWM->Iu/Iv/Iw
#define MTR1_IU_XBAR_EPWM_MUX_H     XBAR_EPWM_MUX12_CMPSS7_CTRIPH
#define MTR1_IU_XBAR_EPWM_MUX_L     XBAR_EPWM_MUX13_CMPSS7_CTRIPL

#define MTR1_IV_XBAR_EPWM_MUX_H   XBAR_EPWM_MUX58_CMPSS9_CTRIPH
//#define MTR1_IV_XBAR_EPWM_MUX_L   X

#define MTR1_IW_XBAR_EPWM_MUX_H   XBAR_EPWM_MUX08_CMPSS5_CTRIPH
#define MTR1_IW_XBAR_EPWM_MUX_L   XBAR_EPWM_MUX09_CMPSS5_CTRIPL

#define MTR1_IU_XBAR_MUX        (XBAR_MUX12 | XBAR_MUX13)
#define MTR1_IV_XBAR_MUX        XBAR_MUX58
#define MTR1_IW_XBAR_MUX        (XBAR_MUX08 | XBAR_MUX09)

//! \brief Defines the PWM deadband falling edge delay count (system clocks)
// using 200MHz clock, (DB_CNT * 5ns)
#define MTR1_PWM_DBFED_CNT      20  // 100 ns

//! \brief Defines the PWM deadband rising edge delay count (system clocks)
#define MTR1_PWM_DBRED_CNT      20 // 100 ns

//------------------------------------------------------------------------------
// Interrupt
#define MTR1_PWM_INT_BASE       MTR1_PWM_U_BASE

#define MTR1_ADC_INT_BASE       MTR1_VDC_ADC_BASE       // ADC-B0   -SOC3
#define MTR1_ADC_INT_NUM        ADC_INT_NUMBER1         // ADCB_INT1-SOC3
#define MTR1_ADC_INT_SOC        MTR1_VDC_ADC_SOC_NUM    // ADCB_INT1-SOC3

#define MTR1_PIE_INT_NUM        INT_ADCB1               // ADCB_INT1-SOC3
#define MTR1_INT_ACK_GROUP      INTERRUPT_ACK_GROUP1    // ADCB_INT1-CPU_INT1

#elif defined(DRV8300DRGE_EVM)
//------------------------------------------------------------------------------
// ADC
#define MTR1_ADC_TRIGGER_SOC        ADC_TRIGGER_EPWM5_SOCA  // EPWM5_SOCA
#define MTR1_ADC_TRIGGER_COUNT      3 // PWM-CMPC Value for ADC trigger
#define MTR1_ADC_I_SAMPLEWINDOW     20
#define MTR1_ADC_V_SAMPLEWINDOW     28

// Phase Current:
#define MTR1_IU_ADC_BASE        ADCC_BASE               // ADC-C7
#define MTR1_IV_ADC_BASE        ADCB_BASE               // ADC-B7
#define MTR1_IW_ADC_BASE        ADCA_BASE               // ADC-A7

#define MTR1_IU_ADCRES_BASE     ADCCRESULT_BASE
#define MTR1_IV_ADCRES_BASE     ADCBRESULT_BASE
#define MTR1_IW_ADCRES_BASE     ADCARESULT_BASE

#define MTR1_IU_ADC_CH_NUM      ADC_CH_ADCIN7
#define MTR1_IV_ADC_CH_NUM      ADC_CH_ADCIN7
#define MTR1_IW_ADC_CH_NUM      ADC_CH_ADCIN7

#define MTR1_IU_ADC_SOC_NUM     ADC_SOC_NUMBER1         // SOC1-PPB1
#define MTR1_IV_ADC_SOC_NUM     ADC_SOC_NUMBER1         // SOC1-PPB1
#define MTR1_IW_ADC_SOC_NUM     ADC_SOC_NUMBER1         // SOC1-PPB1

#define MTR1_IU_ADC_PPB_NUM     ADC_PPB_NUMBER1
#define MTR1_IV_ADC_PPB_NUM     ADC_PPB_NUMBER1
#define MTR1_IW_ADC_PPB_NUM     ADC_PPB_NUMBER1

// Voltage, Phase and dc-bus
#define MTR1_VU_ADC_BASE        ADCC_BASE               // ADC-C14
#define MTR1_VV_ADC_BASE        ADCA_BASE               // ADC-A4
#define MTR1_VW_ADC_BASE        ADCB_BASE               // ADC-B4
#define MTR1_VDC_ADC_BASE       ADCC_BASE               // ADC-C4

#define MTR1_VU_ADCRES_BASE     ADCCRESULT_BASE
#define MTR1_VV_ADCRES_BASE     ADCARESULT_BASE
#define MTR1_VW_ADCRES_BASE     ADCBRESULT_BASE
#define MTR1_VDC_ADCRES_BASE    ADCCRESULT_BASE

#define MTR1_VU_ADC_CH_NUM      ADC_CH_ADCIN14
#define MTR1_VV_ADC_CH_NUM      ADC_CH_ADCIN4
#define MTR1_VW_ADC_CH_NUM      ADC_CH_ADCIN4
#define MTR1_VDC_ADC_CH_NUM     ADC_CH_ADCIN4

#define MTR1_VU_ADC_SOC_NUM     ADC_SOC_NUMBER2         // SOC2
#define MTR1_VV_ADC_SOC_NUM     ADC_SOC_NUMBER2         // SOC2
#define MTR1_VW_ADC_SOC_NUM     ADC_SOC_NUMBER2         // SOC2
#define MTR1_VDC_ADC_SOC_NUM    ADC_SOC_NUMBER3         // SOC3

//------------------------------------------------------------------------------
// CMPSS
#define MTR1_CMPSS_U_BASE       CMPSS11_BASE
#define MTR1_CMPSS_V_BASE       CMPSS7_BASE
#define MTR1_CMPSS_W_BASE       CMPSS9_BASE

#define MTR1_IU_CMPHP_SEL       ASYSCTL_CMPHPMUX_SELECT_11    // CMPSS11H-C7
#define MTR1_IU_CMPLP_SEL       ASYSCTL_CMPLPMUX_SELECT_11    // CMPSS11L-C7

#define MTR1_IV_CMPHP_SEL       ASYSCTL_CMPHPMUX_SELECT_7    // CMPSS7H-B7
#define MTR1_IV_CMPLP_SEL       ASYSCTL_CMPLPMUX_SELECT_7    // CMPSS7L-B7

#define MTR1_IW_CMPHP_SEL       ASYSCTL_CMPHPMUX_SELECT_9    // CMPSS9H-A7
//#define MTR1_IW_CMPLP_SEL       X    // X

#define MTR1_IU_CMPHP_MUX       1
#define MTR1_IU_CMPLP_MUX       1

#define MTR1_IV_CMPHP_MUX       2
#define MTR1_IV_CMPLP_MUX       2

#define MTR1_IW_CMPHP_MUX       2
//#define MTR1_IW_CMPLP_MUX       X

#define MTR1_CMPSS_DACH_VALUE   (2048 + 1024 + 512)
#define MTR1_CMPSS_DACL_VALUE   (2048 - 1024 - 512)

// XBAR-EPWM->Iu/Iv/Iw
#define MTR1_IU_XBAR_EPWM_MUX_H   XBAR_EPWM_MUX62_CMPSS11_CTRIPH
//#define MTR1_IU_XBAR_EPWM_MUX_L   XBAR_EPWM_MUX63_CMPSS11_CTRIPL

#define MTR1_IV_XBAR_EPWM_MUX_H     XBAR_EPWM_MUX12_CMPSS7_CTRIPH
#define MTR1_IV_XBAR_EPWM_MUX_L     XBAR_EPWM_MUX13_CMPSS7_CTRIPL

#define MTR1_IW_XBAR_EPWM_MUX_H   XBAR_EPWM_MUX58_CMPSS9_CTRIPH
//#define MTR1_IW_XBAR_EPWM_MUX_L   X

#define MTR1_IU_XBAR_MUX        (XBAR_MUX62)
#define MTR1_IV_XBAR_MUX        (XBAR_MUX12 | XBAR_MUX13)
#define MTR1_IW_XBAR_MUX        (XBAR_MUX58)

//! \brief Defines the PWM deadband falling edge delay count (system clocks)
// using 200MHz clock, (DB_CNT * 5ns)
#define MTR1_PWM_DBFED_CNT      20  // 100 ns

//! \brief Defines the PWM deadband rising edge delay count (system clocks)
#define MTR1_PWM_DBRED_CNT      20 // 100 ns

//------------------------------------------------------------------------------
// Interrupt
#define MTR1_PWM_INT_BASE       MTR1_PWM_U_BASE

#define MTR1_ADC_INT_BASE       MTR1_VDC_ADC_BASE       // ADC-C4   -SOC3
#define MTR1_ADC_INT_NUM        ADC_INT_NUMBER1         // ADCC_INT1-SOC3
#define MTR1_ADC_INT_SOC        MTR1_VDC_ADC_SOC_NUM    // ADCC_INT1-SOC3

#define MTR1_PIE_INT_NUM        INT_ADCC1               // ADCC_INT1-SOC3
#define MTR1_INT_ACK_GROUP      INTERRUPT_ACK_GROUP1    // ADCC_INT1-CPU_INT1
#endif // BOOSTXL_DRV8323RS, DRV8300DRGE_EVM

//------------------------------------------------------------------------------
// QEP
#define MTR1_QEP_BASE           EQEP1_BASE

//------------------------------------------------------------------------------
// Communication Peripherals

// CAN:
#define MTRx_CAN_BASE           CANA_BASE

// MCAN:
#define MTRx_MCAN_BASE          MCANA_BASE
#define MTRx_MCAN_DRIVER_BASE   MCANA_DRIVER_BASE
#define MTRx_MCAN_INT_BASE      INT_MCANA_1

// SCI:
#define MTR1_SCI_BASE           SCIA_BASE

// SPI:
#define MTR1_SPI_BASE           SPIA_BASE

// **************************************************************************
// the typedefs

//! \brief Enumeration for the LED numbers
//!
typedef enum
{
    HAL_GPIO_LED1 = 31,   //!< GPIO pin number for LaunchPad LED 4
    HAL_GPIO_LED2 = 31,   //!< GPIO pin number for LaunchPad LED 5
    HAL_GPIO_ISR  = 30,   //!< GPIO pin number for ISR Executing Time
    HAL_GPIO_BML  = 31    //!< GPIO pin number for Main Loop Executing Time
} HAL_LEDNumber_e;


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
    HAL_CPU_TIMER2 = 2,  //!< Select CPU Timer2
} HAL_CPUTimerNum_e;


// **************************************************************************
// the globals

extern __interrupt void mainISR(void);

#ifdef CMD_CAN_ENABLE
extern __interrupt void canISR(void);
#define CAN_PIE_INT_NUM        INT_CANA0
#endif



#ifdef CMD_CAN_FD_ENABLE
extern __interrupt void CANFDIntrISR0(void);
extern __interrupt void CANFDIntrISR1(void);
#endif


#ifdef CMD_CAN_FD_ENABLE
#define DATABYTES   64

#define MCAN_INTR_ERROR_BITS     (MCAN_IR_PED_MASK |        \
                                  MCAN_IR_PEA_MASK |        \
                                  MCAN_IR_EW_MASK  |        \
                                  MCAN_IR_EP_MASK  |        \
                                  MCAN_IR_ELO_MASK |        \
                                  MCAN_IR_BEU_MASK |        \
                                  MCAN_IR_BEC_MASK |        \
                                  MCAN_IR_TOO_MASK |        \
                                  MCAN_IR_MRAF_MASK)
#endif

// end of definition for site 1 (near emulator) on launchPad
// **************************************************************************
// the function prototypes

//! \brief     Acknowledges an interrupt from the ADC so that another ADC interrupt can 
//!            happen again.
//! \param[in] handle     The hardware abstraction layer (HAL) handle
//! \param[in] adcIntNum  The interrupt number
static inline void HAL_ackADCInt(const ADC_IntNumber adcIntNum)
{
  // clear the ADC interrupt flag
  ADC_clearInterruptStatus(MTR1_ADC_INT_BASE, MTR1_ADC_INT_NUM);       // ADCB

  // Acknowledge interrupt from PIE group 1
  Interrupt_clearACKGroup(MTR1_INT_ACK_GROUP);

  return;
} // end of HAL_ackADCInt() function

#ifdef _FLASH
//! \brief      Executes calibration routines
//! \details    Values for offset and gain are programmed into OTP memory at
//!             the TI factory.  This calls and internal function that programs
//!             these offsets and gains into the ADC registers.
//! \param[in]  handle  The hardware abstraction layer (HAL) handle
extern void HAL_cal(HAL_Handle handle);
#endif

//! \brief      Disables global interrupts
//! \param[in]  handle  The hardware abstraction layer (HAL) handle
extern void HAL_disableGlobalInts(HAL_Handle handle);

//! \brief      Enables the ADC interrupts
//! \details    Enables the ADC interrupt in the PIE, and CPU.  Enables the 
//!             interrupt to be sent from the ADC peripheral.
//! \param[in]  void
extern void HAL_enableADCInts(void);

//! \brief      Enables the CAN interrupts
//! \details    Enables the CAN interrupt in the PIE, and CPU.
//! \param[in]  handle  The hardware abstraction layer (HAL) handle

extern void HAL_enableMCANInts(HAL_Handle handle);
//! \brief      Enables the CAN interrupts
//! \details    Enables the CAN interrupt in the PIE, and CPU.
//! \param[in]  handle  The hardware abstraction layer (HAL) handle

extern void HAL_enableCANInts(HAL_Handle handle);


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

#ifdef BOOSTXL_DRV8323RS
extern void HAL_enableDRV(HAL_Handle handle);

extern void HAL_writeDRVData(HAL_Handle handle, DRV8323_SPIVars_t *drv8323Vars);

extern void HAL_readDRVData(HAL_Handle handle, DRV8323_SPIVars_t *drv8323Vars);

extern void HAL_setupDRVSPI(HAL_Handle handle, DRV8323_SPIVars_t *drv8323Vars);
#endif

//! \brief     Enables global interrupts
//! \param[in] handle  The hardware abstraction layer (HAL) handle
extern void HAL_enableGlobalInts(HAL_Handle handle);

//! \brief     Gets the current scale factor
//! \param[in] handle  The hardware abstraction layer (HAL) handle
//! \return    The current scale factor
static inline float32_t HAL_getCurrentScaleFactor(HAL_Handle handle)
{
  HAL_Obj *obj = (HAL_Obj *)handle;

  return(obj->current_sf);
} // end of HAL_getCurrentScaleFactor() function

//! \brief     Gets the PWM duty cycle times
//! \param[in] handle       The hardware abstraction layer (HAL) handle
//! \param[in] pDutyCycles  A pointer to memory for the duty cycle durations
static inline void HAL_getDutyCycles(HAL_Handle handle,uint16_t *pDutyCycles)
{
    HAL_Obj *obj = (HAL_Obj *)handle;

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
static inline uint_least8_t HAL_getNumCurrentSensors(HAL_Handle handle)
{
  HAL_Obj *obj = (HAL_Obj *)handle;
  

  return(obj->numCurrentSensors);
} // end of HAL_getNumCurrentSensors() function


//! \brief     Gets the number of voltage sensors
//! \param[in] handle  The hardware abstraction layer (HAL) handle
//! \return    The number of voltage sensors
static inline uint_least8_t HAL_getNumVoltageSensors(HAL_Handle handle)
{
  HAL_Obj *obj = (HAL_Obj *)handle;
  

  return(obj->numVoltageSensors);
} // end of HAL_getNumVoltageSensors() function

//! \brief     Gets the pwm enable status
//! \param[in]  handle  The hardware abstraction layer (HAL) handle
//! \return    The pwm enable
static inline bool HAL_getPwmEnableStatus(HAL_Handle handle)
{
  HAL_Obj *obj = (HAL_Obj *)handle;

  return(obj->flagEnablePWM);
} // end of HAL_getPwmStatus() function

//! \brief     Gets the voltage scale factor
//! \param[in] handle  The hardware abstraction layer (HAL) handle
//! \return    The voltage scale factor
static inline float32_t HAL_getVoltageScaleFactor(HAL_Handle handle)
{
  HAL_Obj *obj = (HAL_Obj *)handle;

  return(obj->voltage_sf);
} // end of HAL_getVoltageScaleFactor() function


//! \brief     Acknowledges an interrupt that is set for estimator
//! \param[in] handle     The hardware abstraction layer (HAL) handle
static inline void HAL_ackEstInt(HAL_Handle handle)
{
    // Acknowledge this interrupt to receive more interrupts from group 1
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);

    return;
} // end of HAL_ackEstInt() function


//! \brief      Configures the fault protection logic
//! \details    Sets up the trip zone inputs so that when a comparator
//!             signal from outside the micro-controller trips a fault,
//!             the EPWM peripheral blocks will force the
//!             power switches into a high impedance state.
//! \param[in]  handle  The hardware abstraction layer (HAL) handle
extern void HAL_setupFaults(HAL_Handle handle);

//! \brief     Sets up the GATE object
//! \param[in] handle       The hardware abstraction layer (HAL) handle
extern void HAL_setupGate(HAL_Handle handle);

//! \brief      Initializes the hardware abstraction layer (HAL) object
//! \details    Initializes all handles to the microcontroller peripherals.
//!             Returns a handle to the HAL object.
//! \param[in]  pMemory   A pointer to the memory for the hardware abstraction layer object
//! \param[in]  numBytes  The number of bytes allocated for the hardware abstraction layer object, bytes
//! \return     The hardware abstraction layer (HAL) object handle
extern HAL_Handle HAL_init(void *pMemory,const size_t numBytes);

//! \brief      Initializes the interrupt vector table
//! \details    Points the ISR to the function mainISR.
//! \param[in]  handle  The hardware abstraction layer (HAL) handle
static inline void HAL_initIntVectorTable(HAL_Handle handle)
{
    Interrupt_register(MTR1_PIE_INT_NUM, &mainISR);

#ifdef CMD_CAN_ENABLE
    Interrupt_register(CAN_PIE_INT_NUM, &canISR);
#endif

    return;
} // end of HAL_initIntVectorTable() function

//! \brief      Reads the ADC data
//! \details    Reads in the ADC result registers and scales the values
//!             according to the settings in user.h.  The structure gAdcData
//!             holds three phase voltages, three line currents, and one DC bus
//!             voltage.
//! \param[in]  handle    The hardware abstraction layer (HAL) handle
//! \param[in]  pADCData  A pointer to the ADC data buffer
static inline void HAL_readADCDataWithOffsets(HAL_Handle handle,HAL_ADCData_t *pADCData)
{
  float32_t value;

  float32_t current_sf = HAL_getCurrentScaleFactor(handle);
  float32_t voltage_sf = HAL_getVoltageScaleFactor(handle);

  // convert phase A current
  value = (float32_t)ADC_readResult(MTR1_IU_ADCRES_BASE, MTR1_IU_ADC_SOC_NUM);
  pADCData->I_A.value[0] = value * current_sf;

  // convert phase B current
  value = (float32_t)ADC_readResult(MTR1_IV_ADCRES_BASE, MTR1_IV_ADC_SOC_NUM);
  pADCData->I_A.value[1] = value * current_sf;

  // convert phase C current
  value = (float32_t)ADC_readResult(MTR1_IW_ADCRES_BASE, MTR1_IW_ADC_SOC_NUM);
  pADCData->I_A.value[2] = value * current_sf;

  // convert phase A voltage
  value = (float32_t)ADC_readResult(MTR1_VU_ADCRES_BASE, MTR1_VU_ADC_SOC_NUM);
  pADCData->V_V.value[0] = value * voltage_sf;

  // convert phase B voltage
  value = (float32_t)ADC_readResult(MTR1_VV_ADCRES_BASE, MTR1_VV_ADC_SOC_NUM);
  pADCData->V_V.value[1] = value * voltage_sf;

  // convert phase C voltage
  value = (float32_t)ADC_readResult(MTR1_VW_ADCRES_BASE, MTR1_VW_ADC_SOC_NUM);
  pADCData->V_V.value[2] = value * voltage_sf;

  // convert dcBus voltage
  value = (float32_t)ADC_readResult(MTR1_VDC_ADCRES_BASE, MTR1_VDC_ADC_SOC_NUM);
  pADCData->dcBus_V = value * voltage_sf;


  return;
} // end of HAL_readADCDataWithOffsets() function


//! \brief      Reads the ADC data
//! \details    Reads in the ADC result registers and scales the values
//!             according to the settings in user.h.  The structure gAdcData
//!             holds three phase voltages, three line currents, and one DC bus
//!             voltage.
//! \param[in]  handle    The hardware abstraction layer (HAL) handle
//! \param[in]  pADCData  A pointer to the ADC data buffer
static inline void HAL_readADCDataWithoutOffsets(HAL_Handle handle,HAL_ADCData_t *pADCData)
{
  float32_t value;

  float32_t current_sf = HAL_getCurrentScaleFactor(handle);
  float32_t voltage_sf = HAL_getVoltageScaleFactor(handle);

  // convert phase A current
  value = (float32_t)ADC_readResult(MTR1_IU_ADCRES_BASE, MTR1_IU_ADC_SOC_NUM);
  pADCData->I_A.value[0] = value * current_sf;

  // convert phase B current
  value = (float32_t)ADC_readResult(MTR1_IV_ADCRES_BASE, MTR1_IV_ADC_SOC_NUM);
  pADCData->I_A.value[1] = value * current_sf;

  // convert phase C current
  value = (float32_t)ADC_readResult(MTR1_IW_ADCRES_BASE, MTR1_IW_ADC_SOC_NUM);
  pADCData->I_A.value[2] = value * current_sf;

  // convert phase A voltage
  value = (float32_t)ADC_readResult(MTR1_VU_ADCRES_BASE, MTR1_VU_ADC_SOC_NUM);
  pADCData->V_V.value[0] = value * voltage_sf;

  // convert phase B voltage
  value = (float32_t)ADC_readResult(MTR1_VV_ADCRES_BASE, MTR1_VV_ADC_SOC_NUM);
  pADCData->V_V.value[1] = value * voltage_sf;

  // convert phase C voltage
  value = (float32_t)ADC_readResult(MTR1_VW_ADCRES_BASE, MTR1_VW_ADC_SOC_NUM);
  pADCData->V_V.value[2] = value * voltage_sf;

  // convert dcBus voltage
  value = (float32_t)ADC_readResult(MTR1_VDC_ADCRES_BASE, MTR1_VDC_ADC_SOC_NUM);
  pADCData->dcBus_V = value * voltage_sf;

//#if(DMC_BUILDLEVEL == DMC_LEVEL_4)
//  // read speed set
//  pADCData->throttle = (float32_t)ADC_readResult(obj->adcResult[1], ADC_SOC_NUMBER2);
//#endif

  return;
} // end of HAL_readADCDataWithOffsets() function

//! \brief     Reads the timer count
//! \param[in] handle       The hardware abstraction layer (HAL) handle
//! \param[in] timerNumber  The timer number, 0,1 or 2
//! \return    The timer count
static inline uint32_t HAL_readTimerCnt(HAL_Handle handle,const uint_least8_t timerNumber)
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
static inline uint32_t HAL_readGPIOData(HAL_Handle handle,const uint32_t gpioNumber)
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

//! \brief     Sets the number of voltage sensors
//! \param[in] handle             The hardware abstraction layer (HAL) handle
//! \param[in] numVoltageSensors  The number of voltage sensors
static inline void HAL_setNumVoltageSensors(HAL_Handle handle,const uint_least8_t numVoltageSensors)
{
  HAL_Obj *obj = (HAL_Obj *)handle;

  obj->numVoltageSensors = numVoltageSensors;

  return;
} // end of HAL_setNumVoltageSensors() function

//! \brief     Sets the number of current sensors
//! \param[in] handle             The hardware abstraction layer (HAL) handle
//! \param[in] numCurrentSensors  The number of current sensors
static inline void HAL_setNumCurrentSensors(HAL_Handle handle,const uint_least8_t numCurrentSensors)
{
  HAL_Obj *obj = (HAL_Obj *)handle;


  obj->numCurrentSensors = numCurrentSensors;

  return;
} // end of HAL_setNumCurrentSensors() function

//! \brief      Sets the hardware abstraction layer parameters
//! \details    Sets up the microcontroller peripherals.  Creates all of the scale
//!             factors for the ADC voltage and current conversions.  Sets the initial
//!             offset values for voltage and current measurements.
//! \param[in]  handle       The hardware abstraction layer (HAL) handle
extern void HAL_setParams(HAL_Handle handle);


//! \brief      Sets up the ADCs (Analog to Digital Converters)
//! \param[in]  handle  The hardware abstraction layer (HAL) handle
extern void HAL_setupADCs(HAL_Handle handle);


//! \brief     Sets up the CANA
//! \param[in] handle  The hardware abstraction layer (HAL) handle
extern void HAL_setupCANA(HAL_Handle handle);


//! \brief     Sets up the MCAN
//! \param[in] handle  The hardware abstraction layer (HAL) handle
extern void HAL_setupMCAN(HAL_Handle handle);


//! \brief     configure the CANA
//! \param[in] handle  The hardware abstraction layer (HAL) handle
extern void HAL_configMCAN(HAL_Handle halHandle);


//! \brief      runs ADC offset calibration
//! \param[in]  handle  The hardware abstraction layer (HAL) handle
extern void HAL_runADCZeroOffsetCalibration(uint32_t base);


//! \brief      Sets up the PGAs (Programmable Gain Amplifiers)
//! \param[in]  handle  The hardware abstraction layer (HAL) handle
extern void HAL_setupPGAs(HAL_Handle handle);


//! \brief      Sets up the CMPSSs (Comparator Subsystems)
//! \param[in]  handle  The hardware abstraction layer (HAL) handle
extern void HAL_setupCMPSSs(HAL_Handle handle);


//! \brief      Sets up the DACs (Buffered Digital-to-Analog Converter)
//! \param[in]  handle  The hardware abstraction layer (HAL) handle
extern void HAL_setupDACs(HAL_Handle handle);


//! \brief      Sets up the clocks
//! \details    Sets up the micro-controller's main oscillator
//! \param[in]  handle  The hardware abstraction layer (HAL) handle
extern void HAL_setupClks(HAL_Handle handle);


//! \brief     Sets up the faults
//! \param[in] handle  The hardware abstraction layer (HAL) handle
extern void HAL_setupFaults(HAL_Handle handle);


//! \brief     Sets up the GPIO (General Purpose I/O) pins
//! \param[in] handle  The hardware abstraction layer (HAL) handle
extern void HAL_setupGPIOs(HAL_Handle handle);


//! \brief     Sets up the FLASH.
//! \param[in] handle  The hardware abstraction layer (HAL) handle
extern void HAL_setupFlash(HAL_Handle handle);

//! \brief     Sets up the CLA
//! \param[in] handle  The hardware abstraction layer (HAL) handle
extern void HAL_setupCLA(HAL_Handle handle);

//! \brief     Sets up the peripheral clocks
//! \param[in] handle  The hardware abstraction layer (HAL) handle
extern void HAL_setupPeripheralClks(HAL_Handle handle);


//! \brief     Sets up the PIE (Peripheral Interrupt Expansion)
//! \param[in] handle  The hardware abstraction layer (HAL) handle
extern void HAL_setupPIE(HAL_Handle handle);

//! \brief     Sets up the QEP peripheral
//! \param[in] handle  The hardware abstraction layer (HAL) handle
extern void HAL_setupQEP(HAL_Handle handle,HAL_QEPSelect_e qep);

//! \brief     Sets up the SCIA
//! \param[in] handle  The hardware abstraction layer (HAL) handle
extern void HAL_setupSCIA(HAL_Handle handle);


//! \brief     Sets up the SPIA
//! \param[in] handle  The hardware abstraction layer (HAL) handle
extern void HAL_setupSPIA(HAL_Handle handle);


//! \brief     Sets up the SPIB
//! \param[in] handle  The hardware abstraction layer (HAL) handle
extern void HAL_setupSPIB(HAL_Handle handle);

//! \brief     Sets up the timers
//! \param[in] handle          The hardware abstraction layer (HAL) handle
//! \param[in] systemFreq_MHz  The system frequency, MHz
extern void HAL_setupTimers(HAL_Handle handle,const float32_t systemFreq_MHz);


//! \brief     Sets up the timers
//! \param[in] handle          The hardware abstraction layer (HAL) handle
//! \param[in] cpuTimerNumber  The CPU timer number
bool HAL_getTimerStatus(HAL_Handle halHandle, const uint16_t cpuTimerNumber);


//! \brief     Sets up the timers
//! \param[in] handle          The hardware abstraction layer (HAL) handle
//! \param[in] cpuTimerNumber  The CPU timer number
void HAL_clearTimerFlag(HAL_Handle halHandle, const uint16_t cpuTimerNumber);

#ifdef DATALOG_ENABLE
//! \brief     Sets up the DMA for datalog
//! \param[in] handle          The hardware abstraction layer (HAL) handle
//! \param[in] dmaChNumber     The DMC Channel Number
//! \param[in] dlogDestAddr    The Datalog buffer dest address
//! \param[in] dlogSrcAddr     The Datalog buffer src address
void HAL_setupDlogWithDMA(HAL_Handle handle, const uint16_t dmaChNumber,
                     const void *dlogDestAddr, const void *dlogSrcAddr);
#endif

#ifdef DATALOG_ENABLE
//! \brief     reset the DMA for datalog
static inline void HAL_resetDlogWithDMA(void)
{
    DMA_initController();
    return;
}
#endif

#ifdef DATALOG_ENABLE
//! \brief     Force trig the DMA channel for datalog
//! \param[in] handle          The hardware abstraction layer (HAL) handle
//! \param[in] dmaChNumber     The DMC Channel Number
static inline void HAL_trigDlogWithDMA(HAL_Handle handle, const uint16_t DMAChannel)
{
    HAL_Obj *obj = (HAL_Obj *)handle;

    DMA_startChannel(obj->dmaChHandle[DMAChannel]);
    DMA_forceTrigger(obj->dmaChHandle[DMAChannel]);

    return;
} // end of HAL_trigDlogWithDMA() function
#endif

//! \brief     Toggles the GPIO pin
//! \param[in] handle      The hardware abstraction layer (HAL) handle
//! \param[in] gpioNumber  The GPIO number
static inline void HAL_toggleGPIO(HAL_Handle handle,const uint32_t gpioNumber)
{

    // toggle GPIO
    GPIO_togglePin(gpioNumber);

    return;
} // end of HAL_toggleGPIO() function

//! \brief
//! \param[in]
//! \param[in]
void HAL_clearDataRAM(void *pMemory, uint16_t lengthMemory);


//! \brief     Reads PWM period register
//! \param[in] handle     The hardware abstraction layer (HAL) handle
//! \param[in] pwmNumber  The PWM number
//! \return    The PWM period value
static inline uint16_t HAL_readPWMPeriod(HAL_Handle handle,const uint16_t pwmNumber)
{
  HAL_Obj *obj = (HAL_Obj *)handle;

  // the period value to be returned
  uint16_t pwmPeriodValue;

  pwmPeriodValue = EPWM_getTimeBasePeriod(obj->pwmHandle[pwmNumber]);

  return(pwmPeriodValue);
} // end of HAL_readPWMPeriod() function


//! \brief     Writes PWM data to the PWM comparators for motor control
//! \param[in] handle    The hardware abstraction layer (HAL) handle
//! \param[in] pPWMData  The pointer to the PWM data
static inline void HAL_writePWMData(HAL_Handle handle, HAL_PWMData_t *pPWMData)
{
  HAL_Obj *obj = (HAL_Obj *)handle;
  uint_least8_t pwmCnt;

  for(pwmCnt=0;pwmCnt<3;pwmCnt++)
    {
      // compute the value
      float32_t period = (float32_t)(EPWM_getTimeBasePeriod(obj->pwmHandle[pwmCnt]));
      float32_t V_pu = -pPWMData->Vabc_pu.value[pwmCnt];      // Negative
      float32_t V_sat_pu = MATH_sat(V_pu,0.5,-0.5);           // -0.5~0.5
      float32_t V_sat_dc_pu = V_sat_pu + 0.5;                 // 0~1.0
      int16_t pwmValue  = (int16_t)(V_sat_dc_pu * period);  //

      // Save current CMP value for OVM
      pPWMData->cmpValue[pwmCnt] = pwmValue;

      // write the PWM data value
      EPWM_setCounterCompareValue(obj->pwmHandle[pwmCnt],
                                  EPWM_COUNTER_COMPARE_A,
                                  pwmValue);
    }

  // write the PWM data value  for ADC trigger
  EPWM_setCounterCompareValue(obj->pwmHandle[0],
                              EPWM_COUNTER_COMPARE_C,
                              1);
  return;
} // end of HAL_writePWMData() function


//! \brief      Enables the PWM devices
//! \details    Turns on the outputs of the EPWM peripheral which will allow
//!             the power switches to be controlled.
//! \param[in]  handle  The hardware abstraction layer (HAL) handle
static inline void HAL_enablePWM(HAL_Handle handle)
{
    HAL_Obj *obj = (HAL_Obj *)handle;

    // Clear comparator digital filter output latch
    CMPSS_clearFilterLatchHigh(obj->cmpssHandle[0]);
    CMPSS_clearFilterLatchLow(obj->cmpssHandle[0]);

    CMPSS_clearFilterLatchHigh(obj->cmpssHandle[1]);
    CMPSS_clearFilterLatchLow(obj->cmpssHandle[1]);

    CMPSS_clearFilterLatchHigh(obj->cmpssHandle[2]);
    CMPSS_clearFilterLatchLow(obj->cmpssHandle[2]);

    EPWM_clearTripZoneFlag(obj->pwmHandle[0], HAL_TZ_INTERRUPT_ALL);
    EPWM_clearTripZoneFlag(obj->pwmHandle[1], HAL_TZ_INTERRUPT_ALL);
    EPWM_clearTripZoneFlag(obj->pwmHandle[2], HAL_TZ_INTERRUPT_ALL);

    obj->flagEnablePWM = true;

    return;
} // end of HAL_enablePWM() function

//! \brief      Disables the PWM device
//! \details    Turns off the outputs of the EPWM peripherals which will put
//!             the power switches into a high impedance state.
//! \param[in]  handle  The hardware abstraction layer (HAL) handle
static inline void HAL_disablePWM(HAL_Handle handle)
{
  HAL_Obj *obj = (HAL_Obj *)handle;

  EPWM_forceTripZoneEvent(obj->pwmHandle[0], EPWM_TZ_FORCE_EVENT_OST);
  EPWM_forceTripZoneEvent(obj->pwmHandle[1], EPWM_TZ_FORCE_EVENT_OST);
  EPWM_forceTripZoneEvent(obj->pwmHandle[2], EPWM_TZ_FORCE_EVENT_OST);

  obj->flagEnablePWM = false;


  return;
} // end of HAL_disablePWM() function

//! \brief      clear fault status of motor control
//! \details
//! \param[in]  handle  The hardware abstraction layer (HAL) handle
static inline void HAL_clearFaultStatus(HAL_Handle handle)
{
    HAL_Obj *obj = (HAL_Obj *)handle;

    // Clear any comparator digital filter output latch
    CMPSS_clearFilterLatchHigh(obj->cmpssHandle[0]);
    CMPSS_clearFilterLatchLow(obj->cmpssHandle[0]);

    CMPSS_clearFilterLatchHigh(obj->cmpssHandle[1]);
    CMPSS_clearFilterLatchLow(obj->cmpssHandle[1]);

    CMPSS_clearFilterLatchHigh(obj->cmpssHandle[2]);
    CMPSS_clearFilterLatchLow(obj->cmpssHandle[2]);


    // Clear any Trip Zone flag
    EPWM_clearTripZoneFlag(obj->pwmHandle[0], HAL_TZ_INTERRUPT_ALL);
    EPWM_clearTripZoneFlag(obj->pwmHandle[1], HAL_TZ_INTERRUPT_ALL);
    EPWM_clearTripZoneFlag(obj->pwmHandle[2], HAL_TZ_INTERRUPT_ALL);

    return;
} // end of HAL_clearMtrFaultStatus() function


//! \brief      set the PWM mode to brake the motor
//! \details    Turns off the high side PWM, and turn on the low side PWM
//!
//! \param[in]  handle  The hardware abstraction layer (HAL) handle
static inline void HAL_setPWMBrake(HAL_Handle handle)
{
    HAL_Obj *obj = (HAL_Obj *)handle;

    EPWM_setActionQualifierContSWForceAction(obj->pwmHandle[0],
                                    EPWM_AQ_OUTPUT_A, EPWM_AQ_SW_OUTPUT_LOW);

    EPWM_setActionQualifierContSWForceAction(obj->pwmHandle[0],
                                    EPWM_AQ_OUTPUT_B, EPWM_AQ_SW_OUTPUT_HIGH);

    EPWM_setActionQualifierContSWForceAction(obj->pwmHandle[1],
                                    EPWM_AQ_OUTPUT_A, EPWM_AQ_SW_OUTPUT_LOW);

    EPWM_setActionQualifierContSWForceAction(obj->pwmHandle[1],
                                    EPWM_AQ_OUTPUT_B, EPWM_AQ_SW_OUTPUT_HIGH);

    EPWM_setActionQualifierContSWForceAction(obj->pwmHandle[2],
                                    EPWM_AQ_OUTPUT_A, EPWM_AQ_SW_OUTPUT_LOW);

    EPWM_setActionQualifierContSWForceAction(obj->pwmHandle[2],
                                    EPWM_AQ_OUTPUT_B, EPWM_AQ_SW_OUTPUT_HIGH);

    EPWM_clearTripZoneFlag(obj->pwmHandle[0], HAL_TZ_INTERRUPT_ALL);
    EPWM_clearTripZoneFlag(obj->pwmHandle[1], HAL_TZ_INTERRUPT_ALL);
    EPWM_clearTripZoneFlag(obj->pwmHandle[2], HAL_TZ_INTERRUPT_ALL);

    return;
} // end of HAL_disablePWM() function

//! \brief     Sets the DAC Shadow Output Value
//! \param[in] handle      The hardware abstraction layer (HAL) handle
//! \param[in] dacNumber   The DAC number
//! \param[in] dacValue    The DAC value
static inline void HAL_setDACValue(HAL_Handle handle,
                                   const uint16_t dacNumber,
                                   const uint16_t dacValue)
{
    return;
} // end of HAL_setDACValue() function


//! \brief     Sets the value of the internal DAC of the high comparator
//! \param[in] handle      The hardware abstraction layer (HAL) handle
//! \param[in] cmpssNumber The CMPSS number
//! \param[in] dacValue    The DAC value of the high comparator
static inline void HAL_setCMPSSDACValueHigh(HAL_Handle handle,
                                            const uint16_t cmpssNumber,
                                            const uint16_t dacValue)
{
    HAL_Obj *obj = (HAL_Obj *)handle;

    // set GPIO low
    CMPSS_setDACValueHigh(obj->cmpssHandle[cmpssNumber], dacValue);

    return;
} // end of HAL_setCMPSSDACValueHigh() function


//! \brief     Sets the value of the internal DAC of the low comparator
//! \param[in] handle      The hardware abstraction layer (HAL) handle
//! \param[in] cmpssNumber The CMPSS number
//! \param[in] dacValue    The DAC value of the low comparator
static inline void HAL_setCMPSSDACValueLow(HAL_Handle handle,
                                           const uint16_t cmpssNumber,
                                           const uint16_t dacValue)
{
    HAL_Obj *obj = (HAL_Obj *)handle;

    // set GPIO low
    CMPSS_setDACValueLow(obj->cmpssHandle[cmpssNumber], dacValue);

    return;
} // end of HAL_setCMPSSDACValueLow() function


//! \brief     Sets up the PWMs (Pulse Width Modulators)
//! \param[in] handle          The hardware abstraction layer (HAL) handle
//! \param[in] systemFreq_MHz  The system frequency, MHz
//! \param[in] pwmPeriod_usec  The PWM period, usec
//! \param[in] numPWMTicksPerISRTick  The number of PWM clock ticks per ISR clock tick
extern void HAL_setupPWMs(HAL_Handle handle,
                          const float32_t systemFreq_MHz,
                          const float32_t pwmPeriod_usec,
                          const uint16_t numPWMTicksPerISRTick);


//! \brief     Sets the current scale factor in the hal
//! \param[in] handle      The hardware abstraction layer (HAL) handle
//! \param[in] current_sf  The current scale factor
static inline void HAL_setCurrentScaleFactor(HAL_Handle handle,const float32_t current_sf)
{
  HAL_Obj *obj = (HAL_Obj *)handle;


  obj->current_sf = current_sf;

  return;
} // end of HAL_setCurrentScaleFactor() function

//! \brief     Sets the voltage scale factor in the hal
//! \param[in] handle      The hardware abstraction layer (HAL) handle
//! \param[in] voltage_sf  The voltage scale factor
static inline void HAL_setVoltageScaleFactor(HAL_Handle handle,const float32_t voltage_sf)
{
  HAL_Obj *obj = (HAL_Obj *)handle;

  obj->voltage_sf = voltage_sf;

  return;
} // end of HAL_setVoltageScaleFactor() function


static inline uint16_t HAL_getTripFaults(HAL_Handle handle)
{
    HAL_Obj *obj = (HAL_Obj *)handle;
    uint16_t tripFault = 0;


    tripFault = (EPWM_getTripZoneFlagStatus(obj->pwmHandle[0])
               & (EPWM_TZ_FLAG_OST | EPWM_TZ_FLAG_DCAEVT1 | EPWM_TZ_FLAG_DCAEVT2))
             |(EPWM_getTripZoneFlagStatus(obj->pwmHandle[1])
               & (EPWM_TZ_FLAG_OST | EPWM_TZ_FLAG_DCAEVT1 | EPWM_TZ_FLAG_DCAEVT2))
             | (EPWM_getTripZoneFlagStatus(obj->pwmHandle[2])
               & (EPWM_TZ_FLAG_OST | EPWM_TZ_FLAG_DCAEVT1 | EPWM_TZ_FLAG_DCAEVT2));


    return(tripFault);
}

//! \brief     Writes PWM data to the PWM comparator & Period for motor control
//! \param[in] handle    The hardware abstraction layer (HAL) handle
//! \param[in] pPWMData  The pointer to the PWM data
static inline void
HAL_writePWMAllData(HAL_Handle handle, HAL_PWMData_t *pPWMData)
{
    HAL_Obj *obj = (HAL_Obj *)handle;
    uint16_t pwmCnt;

    for(pwmCnt=0;pwmCnt<3;pwmCnt++)
    {
        // compute the value
        float32_t period = (float32_t)pPWMData->period;

        float32_t V_pu = -pPWMData->Vabc_pu.value[pwmCnt];
        float32_t V_sat_pu = MATH_sat(V_pu, 0.5, -0.5);
        float32_t V_sat_dc_pu = V_sat_pu + 0.5;
        int16_t pwmValue  = (int16_t)(V_sat_dc_pu * period);

        // Save current CMP value for OVM
        pPWMData->cmpValue[pwmCnt] = pwmValue;

        // Write the Time-Base Period Register (TBPRD)
        EPWM_setTimeBasePeriod(obj->pwmHandle[pwmCnt], pPWMData->period);

        // write the PWM data value
        EPWM_setCounterCompareValue(obj->pwmHandle[pwmCnt],
                                    EPWM_COUNTER_COMPARE_A,
                                    pwmValue);
    }

    // write the PWM data value  for ADC trigger
    EPWM_setCounterCompareValue(obj->pwmHandle[0],
                                EPWM_COUNTER_COMPARE_C,
                                1);

    return;
} // end of HAL_writePWMData() function

#ifdef __cplusplus
}
#endif // extern "C"

//@}  // ingroup


#endif // end of _HAL_H_ definition

