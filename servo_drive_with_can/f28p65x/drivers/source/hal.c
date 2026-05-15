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

//! \file   hal.c
//! \brief  Contains the various functions related to the HAL object
//!


// **************************************************************************
// the includes

// drivers

// modules
#include "user.h"

// platforms
#include "hal.h"
#include "hal_obj.h"

// libraries
#include "device.h"

#include "communication.h"

#ifdef _FLASH
#pragma CODE_SECTION(Flash_initModule, ".TI.ramfunc");
#endif  // _FLASH

// Interrupt masks defines

#ifdef CMD_CAN_FD_ENABLE
#define NUM_OF_MSG                    (15U)
#define MCAN_STD_ID_FILT_START_ADDR   (0U)
#define MCAN_STD_ID_FILTER_NUM        (1U)
#define MCAN_EXT_ID_FILT_START_ADDR   (48U)
#define MCAN_EXT_ID_FILTER_NUM        (1U)
#define MCAN_TX_EVENT_START_ADDR      (100U)
#define MCAN_TX_EVENT_SIZE            (NUM_OF_MSG)
#define MCAN_TX_EVENT_WATERMARK       (NUM_OF_MSG/2)
#define MCAN_TX_BUFF_START_ADDR       (148U)
#define MCAN_TX_BUFF_SIZE             (NUM_OF_MSG)
#define MCAN_TX_FIFO_SIZE             (NUM_OF_MSG)
#define MCAN_FIFO_0_START_ADDR        (548U)
#define MCAN_FIFO_0_NUM               (NUM_OF_MSG)
#define MCAN_FIFO_0_WATERMARK         (NUM_OF_MSG)
#define MCAN_FIFO_1_START_ADDR        (748U)
#define MCAN_FIFO_1_NUM               (NUM_OF_MSG)
#define MCAN_FIFO_1_WATERMARK         (NUM_OF_MSG)
#define MCAN_RX_BUFF_START_ADDR       (948U)
#define MCAN_EXT_ID_AND_MASK          (0x1FFFFFFFU)
#define MCAN_TS_PRESCALAR             (0xB0)

#define MCAN_TX_BUFF_ELEM_SIZE       (MCAN_ELEM_SIZE_64Bytes)
#define MCAN_RX_BUFF_ELEM_SIZE       (MCAN_ELEM_SIZE_64Bytes)
#define MCAN_RX_FIFO0_ELEM_SIZE      (MCAN_ELEM_SIZE_64Bytes)
#define MCAN_RX_FIFO1_ELEM_SIZE      (MCAN_ELEM_SIZE_64Bytes)
#endif  // CMD_CAN_FD_ENABLE

// **************************************************************************
// the defines


// **************************************************************************
// the globals


// **************************************************************************
// the functions
//#ifdef _FLASH
void HAL_cal(HAL_Handle handle)
{
    SysCtl_deviceCal();

    return;
} // end of HAL_cal() function
//#endif

void HAL_disableGlobalInts(HAL_Handle handle)
{

  // disable global interrupts
  Interrupt_disableGlobal();

  return;
} // end of HAL_disableGlobalInts() function

void HAL_disableWdog(HAL_Handle halHandle)
{

  // disable watchdog
  SysCtl_disableWatchdog();

  return;
} // end of HAL_disableWdog() function

#ifdef CMD_CAN_ENABLE
void HAL_enableCANInts(HAL_Handle handle)
{
    HAL_Obj *obj = (HAL_Obj *)handle;

    // enable the CAN interrupts
    CAN_enableInterrupt(obj->canHandle, CAN_INT_IE0 | CAN_INT_ERROR |
                            CAN_INT_STATUS);

    // enable the PIE interrupts associated with the CAN interrupts
    Interrupt_enable(INT_CANA0);

    CAN_enableGlobalInterrupt(obj->canHandle, CAN_GLOBAL_INT_CANINT0);

    // enable the cpu interrupt for CAN interrupts
    Interrupt_enableInCPU(INTERRUPT_CPU_INT9);

    return;
} // end of HAL_enableCANInts() function
#endif  // CMD_CAN_ENABLE


#ifdef CMD_CAN_FD_ENABLE
void HAL_enableMCANInts(HAL_Handle handle )
{
    Interrupt_initModule();
    Interrupt_initVectorTable();

    Interrupt_register(MTRx_MCAN_INT_BASE, &CANFDIntrISR1);

    Interrupt_enable(MTRx_MCAN_INT_BASE);
    Interrupt_enableGlobal();
}

#endif
void HAL_enableADCInts(void)
{
    // enable the PIE interrupts associated with the ADC interrupts
    Interrupt_enable(MTR1_PIE_INT_NUM);

    // enable the ADC interrupts
    ADC_enableInterrupt(MTR1_ADC_INT_BASE, MTR1_ADC_INT_NUM);

    // enable the cpu interrupt for ADC interrupts
    Interrupt_enableInCPU(INTERRUPT_CPU_INT1);

    return;
} // end of HAL_enableADCInts() function

void HAL_enableDebugInt(HAL_Handle handle)
{

    // enable debug events
    ERTM;

    return;
} // end of HAL_enableDebugInt() function

#ifdef BOOSTXL_DRV8323RS
void HAL_enableDRV(HAL_Handle handle)
{
  HAL_Obj *obj = (HAL_Obj *)handle;


  DRV8323_enable(obj->drv8323Handle);

  return;
}  // end of HAL_enableDRV() function
#endif

void HAL_enableGlobalInts(HAL_Handle handle)
{

    // enable global interrupts
    Interrupt_enableGlobal();

    return;
} // end of HAL_enableGlobalInts() function


HAL_Handle HAL_init(void *pMemory,const size_t numBytes)
{
  HAL_Handle handle;
  HAL_Obj *obj;

  if(numBytes < sizeof(HAL_Obj))
    return((HAL_Handle)NULL);

  // assign the handle
  handle = (HAL_Handle)pMemory;

  // assign the object
  obj = (HAL_Obj *)handle;

  // disable watchdog
  SysCtl_disableWatchdog();

  // initialize the ADC handles
  obj->adcHandle[0] = ADCA_BASE;
  obj->adcHandle[1] = ADCB_BASE;
  obj->adcHandle[2] = ADCC_BASE;


  // initialize the ADC results
  obj->adcResult[0] = ADCARESULT_BASE;
  obj->adcResult[1] = ADCCRESULT_BASE;
  obj->adcResult[2] = ADCBRESULT_BASE;


  // initialize SCI handle
  obj->sciHandle = MTR1_SCI_BASE;        //!< the SCIA handle

#ifdef CMD_CAN_ENABLE
  // initialize CAN handle
  obj->canHandle = MTRx_CAN_BASE;        //!< the CANA handle
#endif

#ifdef CMD_CAN_FD_ENABLE
  // MCAN handle assigned to MCANA base offset
  obj->mcanHandle = MTRx_MCAN_DRIVER_BASE;
#endif

#ifdef SPI_ENABLE
  // initialize SPI handle
  obj->spiHandle[0] = MTR1_SPI_BASE;            //!< the SPIA handle
  obj->spiHandle[1] = SPIB_BASE;            //!< the SPIB handle
#endif  // SPI_ENABLE

#ifdef DATALOG_ENABLE
  // initialize DMA handle
  obj->dmaHandle = DMA_BASE;            //!< the DMA handle

  // initialize DMA channel handle
  obj->dmaChHandle[0] = DMA_CH1_BASE;   //!< the DMA Channel handle
  obj->dmaChHandle[1] = DMA_CH2_BASE;   //!< the DMA Channel handle
  obj->dmaChHandle[2] = DMA_CH3_BASE;   //!< the DMA Channel handle
  obj->dmaChHandle[3] = DMA_CH4_BASE;   //!< the DMA Channel handle
#endif  // DATALOG_ENABLE

  // initialize PWM handles for Motor 1
  obj->pwmHandle[0] = MTR1_PWM_U_BASE;
  obj->pwmHandle[1] = MTR1_PWM_V_BASE;
  obj->pwmHandle[2] = MTR1_PWM_W_BASE;


  // initialize CMPSS handle
  obj->cmpssHandle[0] = MTR1_CMPSS_U_BASE;    //!< the CMPSS handle
  obj->cmpssHandle[1] = MTR1_CMPSS_V_BASE;    //!< the CMPSS handle
  obj->cmpssHandle[2] = MTR1_CMPSS_W_BASE;    //!< the CMPSS handle


  // initialize timer handles
  obj->timerHandle[0] = CPUTIMER0_BASE;
  obj->timerHandle[1] = CPUTIMER1_BASE;
  obj->timerHandle[2] = CPUTIMER2_BASE;

#ifdef BOOSTXL_DRV8323RS
  obj->drv8323Handle = DRV8323_init(&obj->drv8323);
#endif

#ifdef EQEP_ENABLE
  // initialize QEP driver
  obj->qepHandle[0] = MTR1_QEP_BASE;           // EQEP1
  obj->qepHandle[1] = EQEP2_BASE;           // EQEP2
#endif  // EQEP_ENABLE

  return(handle);
} // end of HAL_init() function

void HAL_setParams(HAL_Handle handle)
{
  HAL_setNumCurrentSensors(handle,USER_NUM_CURRENT_SENSORS);
  HAL_setNumVoltageSensors(handle,USER_NUM_VOLTAGE_SENSORS);

  // Enable temperature sensor
  ASysCtl_enableTemperatureSensor();

  // initialize the interrupt controller
  Interrupt_initModule();

  // init vector table
  Interrupt_initVectorTable();

  //
  // Lock VREGCTL Register
  // The register VREGCTL is not supported in this device. It is locked to
  // prevent any writes to this register
  //
  ASysCtl_lockVREG();

  // setup the GPIOs
  HAL_setupGPIOs(handle);

  // setup the ADCs
  HAL_setupADCs(handle);        //

  // setup the CMPSSs
  HAL_setupCMPSSs(handle);      //

  // setup the PWMs
//  HAL_setupPWMs(handle,
//                (USER_SYSTEM_FREQ_MHz / 2.0f),
//                USER_PWM_PERIOD_usec,
//                USER_NUM_PWM_TICKS_PER_ISR_TICK);
  HAL_setupPWMs(handle,
                (USER_SYSTEM_FREQ_MHz),
                USER_PWM_PERIOD_usec,
                USER_NUM_PWM_TICKS_PER_ISR_TICK);

  // set the current scale factor
  HAL_setCurrentScaleFactor(handle,USER_CURRENT_SF);

  // set the voltage scale factor
  HAL_setVoltageScaleFactor(handle,USER_VOLTAGE_SF);

  // setup the timers
  HAL_setupTimers(handle, USER_SYSTEM_FREQ_MHz);

#ifdef BOOSTXL_DRV8323RS
  // setup the drv83xx interface
  HAL_setupGate(handle);
#endif

#if defined(SCI_ENABLE)
  // setup the sci
  HAL_setupSCIA(handle);
#endif // SCI_ENABLE

#ifdef CMD_CAN_ENABLE
  // setup the CANA
  HAL_setupCANA(handle);
#endif  // CMD_CAN_ENABLE

#ifdef CMD_CAN_FD_ENABLE
    // setup MCAN
  HAL_setupMCAN(handle);
#endif


#ifdef SPI_ENABLE
  // setup the SPI for DRV83xx
  HAL_setupSPIA(handle);
#endif

#ifdef EQEP_ENABLE
  // setup the eqep
  HAL_setupQEP(handle, HAL_QEP_QEP1);
#endif

  return;
} // end of HAL_setParams() function

void HAL_setupADCs(HAL_Handle handle)
{
    HAL_Obj *obj = (HAL_Obj *)handle;

#if defined(ADC_VREF_INT_EN)
    SysCtl_delay(100U);
    ADC_setVREF(obj->adcHandle[0], ADC_REFERENCE_INTERNAL, ADC_REFERENCE_3_3V);
    ADC_setVREF(obj->adcHandle[1], ADC_REFERENCE_INTERNAL, ADC_REFERENCE_3_3V);
    ADC_setVREF(obj->adcHandle[2], ADC_REFERENCE_INTERNAL, ADC_REFERENCE_3_3V);
    SysCtl_delay(100U);
#else // !ADC_VREF_INT_EN
    SysCtl_delay(100U);
    ADC_setVREF(obj->adcHandle[0], ADC_REFERENCE_EXTERNAL, ADC_REFERENCE_3_3V);
    ADC_setVREF(obj->adcHandle[1], ADC_REFERENCE_EXTERNAL, ADC_REFERENCE_3_3V);
    ADC_setVREF(obj->adcHandle[2], ADC_REFERENCE_EXTERNAL, ADC_REFERENCE_3_3V);
    SysCtl_delay(100U);
#endif // ADC_VREF_INT_EN

    // Set main clock scaling factor (50MHz max clock for the ADC module)
    ADC_setPrescaler(obj->adcHandle[0], ADC_CLK_DIV_4_0);
    ADC_setPrescaler(obj->adcHandle[1], ADC_CLK_DIV_4_0);
    ADC_setPrescaler(obj->adcHandle[2], ADC_CLK_DIV_4_0);

    // set the ADC interrupt pulse generation to end of conversion
    ADC_setInterruptPulseMode(obj->adcHandle[0], ADC_PULSE_END_OF_CONV);
    ADC_setInterruptPulseMode(obj->adcHandle[1], ADC_PULSE_END_OF_CONV);
    ADC_setInterruptPulseMode(obj->adcHandle[2], ADC_PULSE_END_OF_CONV);

    // enable the ADCs
    ADC_enableConverter(obj->adcHandle[0]);
    ADC_enableConverter(obj->adcHandle[1]);
    ADC_enableConverter(obj->adcHandle[2]);

    // set priority of SOCs
    ADC_setSOCPriority(obj->adcHandle[0], ADC_PRI_ALL_HIPRI);
    ADC_setSOCPriority(obj->adcHandle[1], ADC_PRI_ALL_HIPRI);
    ADC_setSOCPriority(obj->adcHandle[2], ADC_PRI_ALL_HIPRI);

    // delay to allow ADCs to power up
    SysCtl_delay(1000U);

    // configure the interrupt sources    //RA3
    ADC_setInterruptSource(MTR1_ADC_INT_BASE,
                           MTR1_ADC_INT_NUM, MTR1_ADC_INT_SOC);

    //Configure SOCs for ADC conversions:

    // TODO: Add PPB config here.

    // ISEN_A_M1
    ADC_setupSOC(MTR1_IU_ADC_BASE, MTR1_IU_ADC_SOC_NUM, MTR1_ADC_TRIGGER_SOC,
                 MTR1_IU_ADC_CH_NUM, MTR1_ADC_I_SAMPLEWINDOW);

    // ISEN_B_M1
    ADC_setupSOC(MTR1_IV_ADC_BASE, MTR1_IV_ADC_SOC_NUM, MTR1_ADC_TRIGGER_SOC,
                 MTR1_IV_ADC_CH_NUM, MTR1_ADC_I_SAMPLEWINDOW);

    // ISEN_C_M1
    ADC_setupSOC(MTR1_IW_ADC_BASE, MTR1_IW_ADC_SOC_NUM, MTR1_ADC_TRIGGER_SOC,
                 MTR1_IW_ADC_CH_NUM, MTR1_ADC_I_SAMPLEWINDOW);

    // VSEN_A_M1
    ADC_setupSOC(MTR1_VU_ADC_BASE, MTR1_VU_ADC_SOC_NUM, MTR1_ADC_TRIGGER_SOC,
                 MTR1_VU_ADC_CH_NUM, MTR1_ADC_V_SAMPLEWINDOW);

    // VSEN_B_M1
    ADC_setupSOC(MTR1_VV_ADC_BASE, MTR1_VV_ADC_SOC_NUM, MTR1_ADC_TRIGGER_SOC,
                 MTR1_VV_ADC_CH_NUM, MTR1_ADC_V_SAMPLEWINDOW);

    // VSEN_C_M1
    ADC_setupSOC(MTR1_VW_ADC_BASE, MTR1_VW_ADC_SOC_NUM, MTR1_ADC_TRIGGER_SOC,
                 MTR1_VW_ADC_CH_NUM, MTR1_ADC_V_SAMPLEWINDOW);

    // VSEN_DCBUS_M1-->Trig Interrupt
    ADC_setupSOC(MTR1_VDC_ADC_BASE, MTR1_VDC_ADC_SOC_NUM, MTR1_ADC_TRIGGER_SOC,
                 MTR1_VDC_ADC_CH_NUM, MTR1_ADC_V_SAMPLEWINDOW);


    return;
} // end of HAL_setupADCs() function

// HAL_setupCMPSSs
void HAL_setupCMPSSs(HAL_Handle handle)
{
    HAL_Obj *obj = (HAL_Obj *)handle;

    uint16_t  cnt;
    uint16_t cmpsaDACH;
    uint16_t cmpsaDACL;

    // Refer to the Table 9-2 in Chapter 9 of TMS320F28004x
    // Technical Reference Manual (SPRUI33B), to configure the ePWM X-Bar
    cmpsaDACH = MTR1_CMPSS_DACH_VALUE;
    cmpsaDACL = MTR1_CMPSS_DACL_VALUE;

    ASysCtl_selectCMPHPMux(MTR1_IU_CMPHP_SEL, MTR1_IU_CMPHP_MUX);
#ifndef DRV8300DRGE_EVM
    ASysCtl_selectCMPLPMux(MTR1_IU_CMPLP_SEL, MTR1_IU_CMPLP_MUX);
#endif

    ASysCtl_selectCMPHPMux(MTR1_IV_CMPHP_SEL, MTR1_IV_CMPHP_MUX);
#ifndef BOOSTXL_DRV8323RS
    ASysCtl_selectCMPLPMux(MTR1_IV_CMPLP_SEL, MTR1_IV_CMPLP_MUX);
#endif

    ASysCtl_selectCMPHPMux(MTR1_IW_CMPHP_SEL, MTR1_IW_CMPHP_MUX);
#ifndef DRV8300DRGE_EVM
    ASysCtl_selectCMPLPMux(MTR1_IW_CMPLP_SEL, MTR1_IW_CMPLP_MUX);
#endif

    for(cnt=0;cnt<3;cnt++)
    {
        // Enable CMPSS and configure the negative input signal to come from the DAC
        CMPSS_enableModule(obj->cmpssHandle[cnt]);

        // NEG signal from DAC for COMP-H
        CMPSS_configHighComparator(obj->cmpssHandle[cnt], CMPSS_INSRC_DAC);

        // NEG signal from DAC for COMP-L
        CMPSS_configLowComparator(obj->cmpssHandle[cnt], CMPSS_INSRC_DAC);

        // Configure the output signals. Both CTRIPH and CTRIPOUTH will be fed by
        // the asynchronous comparator output.
        // Dig filter output ==> CTRIPH, Dig filter output ==> CTRIPOUTH
        CMPSS_configOutputsHigh(obj->cmpssHandle[cnt],
                                CMPSS_TRIP_FILTER |
                                CMPSS_TRIPOUT_FILTER);

        // Dig filter output ==> CTRIPL, Dig filter output ==> CTRIPOUTL
        CMPSS_configOutputsLow(obj->cmpssHandle[cnt],
                               CMPSS_TRIP_FILTER |
                               CMPSS_TRIPOUT_FILTER |
                               CMPSS_INV_INVERTED);

        // Configure digital filter. For this example, the maxiumum values will be
        // used for the clock prescale, sample window size, and threshold.
        CMPSS_configFilterHigh(obj->cmpssHandle[cnt], 32, 32, 30);
        CMPSS_initFilterHigh(obj->cmpssHandle[cnt]);

        // Initialize the filter logic and start filtering
        CMPSS_configFilterLow(obj->cmpssHandle[cnt], 32, 32, 30);
        CMPSS_initFilterLow(obj->cmpssHandle[cnt]);

        // Set up COMPHYSCTL register
        // COMP hysteresis set to 2x typical value
        CMPSS_setHysteresis(obj->cmpssHandle[cnt], 1);

        // Use VDDA as the reference for the DAC and set DAC value to midpoint for
        // arbitrary reference
        CMPSS_configDAC(obj->cmpssHandle[cnt],
                   CMPSS_DACREF_VDDA | CMPSS_DACVAL_SYSCLK | CMPSS_DACSRC_SHDW);

        // Set DAC-H to allowed MAX +ve current
        CMPSS_setDACValueHigh(obj->cmpssHandle[cnt], cmpsaDACH);

        // Set DAC-L to allowed MAX -ve current
        CMPSS_setDACValueLow(obj->cmpssHandle[cnt], cmpsaDACL);

        // Clear any high comparator digital filter output latch
        CMPSS_clearFilterLatchHigh(obj->cmpssHandle[cnt]);

        // Clear any low comparator digital filter output latch
        CMPSS_clearFilterLatchLow(obj->cmpssHandle[cnt]);
    }

    return;
} // end of HAL_setupCMPSSs() function

#ifdef BOOSTXL_DRV8323RS
void HAL_writeDRVData(HAL_Handle handle, DRV8323_SPIVars_t *drv8323Vars)
{
  HAL_Obj  *obj = (HAL_Obj *)handle;

  DRV8323_writeData(obj->drv8323Handle,drv8323Vars);

  return;
}  // end of HAL_writeDRVData() function


void HAL_readDRVData(HAL_Handle handle, DRV8323_SPIVars_t *drv8323Vars)
{
  HAL_Obj  *obj = (HAL_Obj *)handle;

  DRV8323_readData(obj->drv8323Handle,drv8323Vars);

  return;
}  // end of HAL_readDRVData() function

void HAL_setupDRVSPI(HAL_Handle handle, DRV8323_SPIVars_t *drv8323Vars)
{
  HAL_Obj  *obj = (HAL_Obj *)handle;

  DRV8323_setupSPI(obj->drv8323Handle, drv8323Vars);

  return;
}  // end of HAL_setupDRVSPI() function
#endif

void HAL_setupFaults(HAL_Handle handle)
{
    HAL_Obj *obj = (HAL_Obj *)handle;
    uint_least8_t cnt;

    // Configure TRIPx to OR the High and Low trips from comparators
    EALLOW;
    HWREG(XBARA_EPWM_CFG_REG_BASE + MTR1_XBAR_TRIP_ADDRL) = 0;
    HWREG(XBARA_EPWM_CFG_REG_BASE + MTR1_XBAR_TRIP_ADDRH) = 0;
    EDIS;

    // Configure TRIPx to be CTRIPxH and CTRIPxL using the ePWM X-BAR
    XBAR_setEPWMMuxConfig(MTR1_XBAR_TRIP, MTR1_IU_XBAR_EPWM_MUX_H);
#ifndef DRV8300DRGE_EVM
    XBAR_setEPWMMuxConfig(MTR1_XBAR_TRIP, MTR1_IU_XBAR_EPWM_MUX_L);
#endif

    // Configure TRIPx to be CTRIPxH and CTRIPxL using the ePWM X-BAR
    XBAR_setEPWMMuxConfig(MTR1_XBAR_TRIP, MTR1_IV_XBAR_EPWM_MUX_H);
#ifndef BOOSTXL_DRV8323RS
    XBAR_setEPWMMuxConfig(MTR1_XBAR_TRIP, MTR1_IV_XBAR_EPWM_MUX_L);
#endif

    // Configure TRIPx to be CTRIPxH and CTRIPxL using the ePWM X-BAR
    XBAR_setEPWMMuxConfig(MTR1_XBAR_TRIP, MTR1_IW_XBAR_EPWM_MUX_H);
#ifndef DRV8300DRGE_EVM
    XBAR_setEPWMMuxConfig(MTR1_XBAR_TRIP, MTR1_IW_XBAR_EPWM_MUX_L);
#endif

    // Disable all the mux first
    XBAR_disableEPWMMux(MTR1_XBAR_TRIP, 0xFFFF);

    // Enable Mux 0  OR Mux 4 to generate TRIP
    XBAR_enableEPWMMux(MTR1_XBAR_TRIP, MTR1_IU_XBAR_MUX |
                       MTR1_IV_XBAR_MUX | MTR1_IW_XBAR_MUX);

#ifdef BOOSTXL_DRV8323RS
    // configure the input x bar for TZ2 to GPIO, where Over Current is connected
    XBAR_setInputPin(INPUTXBAR_BASE, XBAR_INPUT1, HAL_PM_nFAULT_GPIO);
    XBAR_lockInput(INPUTXBAR_BASE, XBAR_INPUT1);
#endif // !BOOSTXL_DRV8323RS

    // Configure Trip Mechanism for the Motor control software
    // -Cycle by cycle trip on CPU halt
    // -One shot fault trip zone
    // These trips need to be repeated for EPWM1 ,2 & 3

    for(cnt=0;cnt<3;cnt++)
    {
        EPWM_enableTripZoneSignals(obj->pwmHandle[cnt],
                                   MTR1_TZ_OSHT1);

        EPWM_enableTripZoneSignals(obj->pwmHandle[cnt],
                            EPWM_TZ_SIGNAL_CBC6);

//        EPWM_enableTripZoneSignals(obj->pwmHandle[cnt],
//                            EPWM_TZ_SIGNAL_OSHT2);
//
//        EPWM_enableTripZoneSignals(obj->pwmHandle[cnt],
//                            EPWM_TZ_SIGNAL_OSHT3);

        //enable DC TRIP combinational input
        EPWM_enableDigitalCompareTripCombinationInput(obj->pwmHandle[cnt],
                                               MTR1_DCTRIPIN, EPWM_DC_TYPE_DCAH);

        EPWM_enableDigitalCompareTripCombinationInput(obj->pwmHandle[cnt],
                                               MTR1_DCTRIPIN, EPWM_DC_TYPE_DCBH);

        // Trigger event when DCAH is High
        EPWM_setTripZoneDigitalCompareEventCondition(obj->pwmHandle[cnt],
                                              EPWM_TZ_DC_OUTPUT_A1,
                                              EPWM_TZ_EVENT_DCXH_HIGH);

        // Trigger event when DCBH is High
        EPWM_setTripZoneDigitalCompareEventCondition(obj->pwmHandle[cnt],
                                              EPWM_TZ_DC_OUTPUT_B1,
                                              EPWM_TZ_EVENT_DCXL_HIGH);

        // Configure the DCA path to be un-filtered and asynchronous
        EPWM_setDigitalCompareEventSource(obj->pwmHandle[cnt],
                                   EPWM_DC_MODULE_A,
                                   EPWM_DC_EVENT_1,
                                   EPWM_DC_EVENT_SOURCE_FILT_SIGNAL);

        // Configure the DCB path to be un-filtered and asynchronous
        EPWM_setDigitalCompareEventSource(obj->pwmHandle[cnt],
                                   EPWM_DC_MODULE_B,
                                   EPWM_DC_EVENT_1,
                                   EPWM_DC_EVENT_SOURCE_FILT_SIGNAL);

        EPWM_setDigitalCompareEventSyncMode(obj->pwmHandle[cnt],
                                     EPWM_DC_MODULE_A,
                                     EPWM_DC_EVENT_1,
                                     EPWM_DC_EVENT_INPUT_NOT_SYNCED);

        EPWM_setDigitalCompareEventSyncMode(obj->pwmHandle[cnt],
                                     EPWM_DC_MODULE_B,
                                     EPWM_DC_EVENT_1,
                                     EPWM_DC_EVENT_INPUT_NOT_SYNCED);

        // Enable DCA as OST
        EPWM_enableTripZoneSignals(obj->pwmHandle[cnt], EPWM_TZ_SIGNAL_DCAEVT1);

        // Enable DCB as OST
        EPWM_enableTripZoneSignals(obj->pwmHandle[cnt], EPWM_TZ_SIGNAL_DCBEVT1);

        // What do we want the OST/CBC events to do?
        // TZA events can force EPWMxA
        // TZB events can force EPWMxB
        EPWM_setTripZoneAction(obj->pwmHandle[cnt],
                        EPWM_TZ_ACTION_EVENT_TZA,
                        EPWM_TZ_ACTION_LOW);

        EPWM_setTripZoneAction(obj->pwmHandle[cnt],
                        EPWM_TZ_ACTION_EVENT_TZB,
                        EPWM_TZ_ACTION_LOW);

        // Clear any high comparator digital filter output latch
        CMPSS_clearFilterLatchHigh(obj->cmpssHandle[cnt]);

        // Clear any low comparator digital filter output latch
        CMPSS_clearFilterLatchLow(obj->cmpssHandle[cnt]);

        // Clear any spurious fault
        EPWM_clearTripZoneFlag(obj->pwmHandle[cnt], HAL_TZ_INTERRUPT_ALL);
    }

    return;

} // end of HAL_setupFaults() function

#ifdef BOOSTXL_DRV8323RS
void HAL_setupGate(HAL_Handle handle)
{
    HAL_Obj *obj = (HAL_Obj *)handle;

    DRV8323_setSPIHandle(obj->drv8323Handle, obj->spiHandle[0]);
    DRV8323_setGPIOCSNumber(obj->drv8323Handle, HAL_DRV_SPI_CS_GPIO);
    DRV8323_setGPIONumber(obj->drv8323Handle, HAL_DRV_EN_GATE_GPIO);

    return;
} // HAL_setupGate() function
#endif

void HAL_setupGPIOs(HAL_Handle handle)
{
    GPIO_unlockPortConfig(GPIO_PORT_A, 0xFFFFFFFF);

    // GPIO8->EPWM5A-DRV_UH*
    GPIO_setPinConfig(GPIO_8_EPWM5_A);
    GPIO_setDirectionMode(8, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(8, GPIO_PIN_TYPE_STD);

    //  GPIO9->EPWM5B->DRV_UL*
    GPIO_setPinConfig(GPIO_9_EPWM5_B);
    GPIO_setDirectionMode(9, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(9, GPIO_PIN_TYPE_STD);

    // GPIO6->EPWM4A->DRV_VH*
    GPIO_setPinConfig(GPIO_6_EPWM4_A);
    GPIO_setDirectionMode(6, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(6, GPIO_PIN_TYPE_STD);

    // GPIO7->EPWM4B->DRV_VL*
    GPIO_setPinConfig(GPIO_7_EPWM4_B);
    GPIO_setDirectionMode(7, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(7, GPIO_PIN_TYPE_STD);

    // GPIO10->EPWM6A->DRV_WH*
    GPIO_setPinConfig(GPIO_10_EPWM6_A);
    GPIO_setDirectionMode(10, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(10, GPIO_PIN_TYPE_STD);

    // GPIO11->EPWM6B->DRV_WL*
    GPIO_setPinConfig(GPIO_11_EPWM6_B);
    GPIO_setDirectionMode(11, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(11, GPIO_PIN_TYPE_STD);

#ifdef CMD_CAN_FD_ENABLE
    // GPIO4->MCAN_TX
    GPIO_setPinConfig(GPIO_4_MCANA_TX);
    GPIO_setDirectionMode(4, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(4, GPIO_PIN_TYPE_STD);

    // GPIO5->MCAN_RX
    GPIO_setPinConfig(GPIO_5_MCANA_RX);
    GPIO_setDirectionMode(5, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(5, GPIO_PIN_TYPE_STD);
#else
    // GPIO4->CANA_TX
    GPIO_setPinConfig(GPIO_4_CANA_TX);
    GPIO_setDirectionMode(4, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(4, GPIO_PIN_TYPE_STD);

    // GPIO5->CANA_RX
    GPIO_setPinConfig(GPIO_5_CANA_RX);
    GPIO_setDirectionMode(5, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(5, GPIO_PIN_TYPE_STD);
#endif

#ifdef BOOSTXL_DRV8323RS
    // GPIO16->SPIA_PICO->DRV_SDI
    GPIO_setPinConfig(GPIO_16_SPIA_PICO);
    GPIO_setDirectionMode(16, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(16, GPIO_PIN_TYPE_STD);

    // GPIO17->SPIA_POCI->DRV_SDO
    GPIO_setPinConfig(GPIO_17_SPIA_POCI);
    GPIO_setDirectionMode(17, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(17, GPIO_PIN_TYPE_PULLUP);

    // GPIO18->SPIA_CLK->DRV_SCLK
    GPIO_setPinConfig(GPIO_18_SPIA_CLK);
    GPIO_setDirectionMode(18, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(18, GPIO_PIN_TYPE_STD);

    // GPIO57->SPIA_PTE->DRV_CS
    // GPIO 57 connect to LP pin J2-12 with jumper
    GPIO_setPinConfig(GPIO_57_SPIA_PTE);
    GPIO_setDirectionMode(57, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(57, GPIO_PIN_TYPE_PULLUP);

    // GPIO133->M1_DRV_CAL (Low)
    GPIO_setPinConfig(GPIO_133_GPIO133);
    GPIO_writePin(133, 0);
    GPIO_setDirectionMode(133, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(133, GPIO_PIN_TYPE_STD);

    // GPIO72->nFAULT
    GPIO_setPinConfig(GPIO_72_GPIO72);
    GPIO_setDirectionMode(72, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(72, GPIO_PIN_TYPE_STD);

    // GPIO42->DRV-EN for J5/J6 connection
    GPIO_setPinConfig(GPIO_42_GPIO42);
    GPIO_writePin(42, 1);
    GPIO_setDirectionMode(42, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(42, GPIO_PIN_TYPE_PULLUP);

//    //GPIO104->nFAULT (Other option to avoid BOOT1 pin, requires jumper wiring)
//    GPIO_setPinConfig(GPIO_104_GPIO104);
//    GPIO_setDirectionMode(104, GPIO_DIR_MODE_IN);
//    GPIO_setPadConfig(104, GPIO_PIN_TYPE_STD);
#endif

    // GPIO23->EQEP1I
    GPIO_setPinConfig(GPIO_23_EQEP1_INDEX);
    GPIO_setDirectionMode(23, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(23, GPIO_PIN_TYPE_STD);

    // GPIO20->EQEP1A
    GPIO_setPinConfig(GPIO_20_EQEP1_A);
    GPIO_setDirectionMode(20, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(20, GPIO_PIN_TYPE_STD);

    //GPIO21->EQEP1B
    GPIO_setPinConfig(GPIO_21_EQEP1_B);
    GPIO_setDirectionMode(21, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(21, GPIO_PIN_TYPE_STD);

    // Analog PinMux for A0/DACA_OUT
    GPIO_setPinConfig(GPIO_227_GPIO227);
    // AIO -> Analog mode selected
    GPIO_setAnalogMode(227, GPIO_ANALOG_ENABLED);
    // Analog PinMux for A1
    GPIO_setPinConfig(GPIO_228_GPIO228);
    // AIO -> Analog mode selected
    GPIO_setAnalogMode(228, GPIO_ANALOG_ENABLED);
    // Analog PinMux for A10, GPIO213
    GPIO_setPinConfig(GPIO_213_GPIO213);
    // AGPIO -> Analog mode selected
    GPIO_setAnalogMode(213, GPIO_ANALOG_ENABLED);
    // Analog PinMux for A11, GPIO214
    GPIO_setPinConfig(GPIO_214_GPIO214);
    // AGPIO -> Analog mode selected
    GPIO_setAnalogMode(214, GPIO_ANALOG_ENABLED);
    // Analog PinMux for A14/B14/C14
    GPIO_setPinConfig(GPIO_225_GPIO225);
    // AIO -> Analog mode selected
    GPIO_setAnalogMode(225, GPIO_ANALOG_ENABLED);
    // Analog PinMux for A15/B15/C15
    GPIO_setPinConfig(GPIO_226_GPIO226);
    // AIO -> Analog mode selected
    GPIO_setAnalogMode(226, GPIO_ANALOG_ENABLED);
    // Analog PinMux for A2
    GPIO_setPinConfig(GPIO_229_GPIO229);
    // AIO -> Analog mode selected
    GPIO_setAnalogMode(229, GPIO_ANALOG_ENABLED);
    // Analog PinMux for A3
    GPIO_setPinConfig(GPIO_230_GPIO230);
    // AIO -> Analog mode selected
    GPIO_setAnalogMode(230, GPIO_ANALOG_ENABLED);
    // Analog PinMux for A4
    GPIO_setPinConfig(GPIO_231_GPIO231);
    // AIO -> Analog mode selected
    GPIO_setAnalogMode(231, GPIO_ANALOG_ENABLED);
    // Analog PinMux for A5
    GPIO_setPinConfig(GPIO_232_GPIO232);
    // AIO -> Analog mode selected
    GPIO_setAnalogMode(232, GPIO_ANALOG_ENABLED);
    // Analog PinMux for A6, GPIO209
    GPIO_setPinConfig(GPIO_209_GPIO209);
    // AGPIO -> Analog mode selected
    GPIO_setAnalogMode(209, GPIO_ANALOG_ENABLED);
    // Analog PinMux for A7, GPIO210
    GPIO_setPinConfig(GPIO_210_GPIO210);
    // AGPIO -> Analog mode selected
    GPIO_setAnalogMode(210, GPIO_ANALOG_ENABLED);
    // Analog PinMux for A8, GPIO211
    GPIO_setPinConfig(GPIO_211_GPIO211);
    // AGPIO -> Analog mode selected
    GPIO_setAnalogMode(211, GPIO_ANALOG_ENABLED);
    // Analog PinMux for A9, GPIO212
    GPIO_setPinConfig(GPIO_212_GPIO212);
    // AGPIO -> Analog mode selected
    GPIO_setAnalogMode(212, GPIO_ANALOG_ENABLED);
    // Analog PinMux for B0/VDAC
    GPIO_setPinConfig(GPIO_233_GPIO233);
    // AIO -> Analog mode selected
    GPIO_setAnalogMode(233, GPIO_ANALOG_ENABLED);
    // Analog PinMux for B1/DACC_OUT
    GPIO_setPinConfig(GPIO_234_GPIO234);
    // AIO -> Analog mode selected
    GPIO_setAnalogMode(234, GPIO_ANALOG_ENABLED);
    // Analog PinMux for B10, GPIO219
    GPIO_setPinConfig(GPIO_219_GPIO219);
    // AGPIO -> Analog mode selected
    GPIO_setAnalogMode(219, GPIO_ANALOG_ENABLED);
    // Analog PinMux for B11
    GPIO_setPinConfig(GPIO_240_GPIO240);
    // AIO -> Analog mode selected
    GPIO_setAnalogMode(240, GPIO_ANALOG_ENABLED);
    // Analog PinMux for B13
    GPIO_setPinConfig(GPIO_238_GPIO238);
    // AIO -> Analog mode selected
    GPIO_setAnalogMode(238, GPIO_ANALOG_ENABLED);
    // Analog PinMux for B2
    GPIO_setPinConfig(GPIO_235_GPIO235);
    // AIO -> Analog mode selected
    GPIO_setAnalogMode(235, GPIO_ANALOG_ENABLED);
    // Analog PinMux for B3
    GPIO_setPinConfig(GPIO_236_GPIO236);
    // AIO -> Analog mode selected
    GPIO_setAnalogMode(236, GPIO_ANALOG_ENABLED);
    // Analog PinMux for B4, GPIO215
    GPIO_setPinConfig(GPIO_215_GPIO215);
    // AGPIO -> Analog mode selected
    GPIO_setAnalogMode(215, GPIO_ANALOG_ENABLED);
    // Analog PinMux for B5, GPIO216
    GPIO_setPinConfig(GPIO_216_GPIO216);
    // AGPIO -> Analog mode selected
    GPIO_setAnalogMode(216, GPIO_ANALOG_ENABLED);
    // Analog PinMux for B6, GPIO207
    GPIO_setPinConfig(GPIO_207_GPIO207);
    // AGPIO -> Analog mode selected
    GPIO_setAnalogMode(207, GPIO_ANALOG_ENABLED);
    // Analog PinMux for B7, GPIO208
    GPIO_setPinConfig(GPIO_208_GPIO208);
    // AGPIO -> Analog mode selected
    GPIO_setAnalogMode(208, GPIO_ANALOG_ENABLED);
    // Analog PinMux for B8, GPIO217
    GPIO_setPinConfig(GPIO_217_GPIO217);
    // AGPIO -> Analog mode selected
    GPIO_setAnalogMode(217, GPIO_ANALOG_ENABLED);
    // Analog PinMux for B9, GPIO218
    GPIO_setPinConfig(GPIO_218_GPIO218);
    // AGPIO -> Analog mode selected
    GPIO_setAnalogMode(218, GPIO_ANALOG_ENABLED);
    // Analog PinMux for C0, GPIO199
    GPIO_setPinConfig(GPIO_199_GPIO199);
    // AGPIO -> Analog mode selected
    GPIO_setAnalogMode(199, GPIO_ANALOG_ENABLED);
    // Analog PinMux for C10
    GPIO_setPinConfig(GPIO_241_GPIO241);
    // AIO -> Analog mode selected
    GPIO_setAnalogMode(241, GPIO_ANALOG_ENABLED);
    // Analog PinMux for C11
    GPIO_setPinConfig(GPIO_242_GPIO242);
    // AIO -> Analog mode selected
    GPIO_setAnalogMode(242, GPIO_ANALOG_ENABLED);
    // Analog PinMux for C13
    GPIO_setPinConfig(GPIO_239_GPIO239);
    // AIO -> Analog mode selected
    GPIO_setAnalogMode(239, GPIO_ANALOG_ENABLED);
    // Analog PinMux for C2
    GPIO_setPinConfig(GPIO_237_GPIO237);
    // AIO -> Analog mode selected
    GPIO_setAnalogMode(237, GPIO_ANALOG_ENABLED);
    // Analog PinMux for C3, GPIO206
    GPIO_setPinConfig(GPIO_206_GPIO206);
    // AGPIO -> Analog mode selected
    GPIO_setAnalogMode(206, GPIO_ANALOG_ENABLED);
    // Analog PinMux for C4, GPIO205
    GPIO_setPinConfig(GPIO_205_GPIO205);
    // AGPIO -> Analog mode selected
    GPIO_setAnalogMode(205, GPIO_ANALOG_ENABLED);
    // Analog PinMux for C5, GPIO204
    GPIO_setPinConfig(GPIO_204_GPIO204);
    // AGPIO -> Analog mode selected
    GPIO_setAnalogMode(204, GPIO_ANALOG_ENABLED);
    // Analog PinMux for C6, GPIO203
    GPIO_setPinConfig(GPIO_203_GPIO203);
    // AGPIO -> Analog mode selected
    GPIO_setAnalogMode(203, GPIO_ANALOG_ENABLED);
    // Analog PinMux for C1, GPIO200
    GPIO_setPinConfig(GPIO_200_GPIO200);
    // AGPIO -> Analog mode selected
    GPIO_setAnalogMode(200, GPIO_ANALOG_ENABLED);
    // Analog PinMux for C7, GPIO198
    GPIO_setPinConfig(GPIO_198_GPIO198);
    // AGPIO -> Analog mode selected
    GPIO_setAnalogMode(198, GPIO_ANALOG_ENABLED);
    // Analog PinMux for C8, GPIO202
    GPIO_setPinConfig(GPIO_202_GPIO202);
    // AGPIO -> Analog mode selected
    GPIO_setAnalogMode(202, GPIO_ANALOG_ENABLED);
    // Analog PinMux for C9, GPIO201
    GPIO_setPinConfig(GPIO_201_GPIO201);
    // AGPIO -> Analog mode selected
    GPIO_setAnalogMode(201, GPIO_ANALOG_ENABLED);

    return;
}  // end of HAL_setupGPIOs() function


void HAL_setupPeripheralClks(HAL_Handle handle)
{
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_DMA);

    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TIMER0);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TIMER1);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TIMER2);

//    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_HRCAL);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);

    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EPWM1);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EPWM2);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EPWM3);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EPWM4);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EPWM5);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EPWM6);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EPWM7);

    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_ECAP1);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_ECAP2);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_ECAP3);

    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EQEP1);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EQEP2);

    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_SCIA);

    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_SPIA);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_SPIB);

    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_I2CA);

    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_CANA);

#ifdef CMD_CAN_FD_ENABLE
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_MCANA);
#endif

    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_ADCA);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_ADCB);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_ADCC);

    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_CMPSS1);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_CMPSS2);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_CMPSS3);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_CMPSS4);

    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_FSITXA);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_FSIRXA);

    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_LINA);

    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_PMBUSA);

    return;
} // end of HAL_setupPeripheralClks() function

void HAL_setupPWMs(HAL_Handle handle,
                   const float32_t systemFreq_MHz,
                   const float32_t pwmPeriod_usec,
                   const uint_least16_t numPWMTicksPerISRTick)
{
    HAL_Obj       *obj = (HAL_Obj *)handle;

    uint16_t  cnt;

    uint16_t halfPeriod_cycles = (uint16_t)(systemFreq_MHz *
                                  pwmPeriod_usec / (float32_t)2.0);

    SysCtl_setEPWMClockDivider(SYSCTL_EPWMCLK_DIV_1);

    // disable the ePWM module time base clock sync signal
    // to synchronize all of the PWMs
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);

    // turns off the outputs of the EPWM peripherals which will put the power
    // switches into a high impedance state.
    EPWM_forceTripZoneEvent(obj->pwmHandle[0], EPWM_TZ_FORCE_EVENT_OST);
    EPWM_forceTripZoneEvent(obj->pwmHandle[1], EPWM_TZ_FORCE_EVENT_OST);
    EPWM_forceTripZoneEvent(obj->pwmHandle[2], EPWM_TZ_FORCE_EVENT_OST);

//    uint16_t       pwmDBRED = HAL_PWM_DBRED_CNT;
//    uint16_t       pwmDBFED = HAL_PWM_DBFED_CNT;

    for(cnt=0; cnt<3; cnt++)
    {
        // setup the Time-Base Control Register (TBCTL)
        EPWM_setTimeBaseCounterMode(obj->pwmHandle[cnt],
                                    EPWM_COUNTER_MODE_UP_DOWN);

        EPWM_disablePhaseShiftLoad(obj->pwmHandle[cnt]);

        EPWM_setPeriodLoadMode(obj->pwmHandle[cnt], EPWM_PERIOD_DIRECT_LOAD);

        EPWM_enableSyncOutPulseSource(obj->pwmHandle[cnt],
                                      EPWM_SYNC_OUT_PULSE_ON_SOFTWARE);

        EPWM_setClockPrescaler(obj->pwmHandle[cnt], EPWM_CLOCK_DIVIDER_1,
                                 EPWM_HSCLOCK_DIVIDER_1);

        EPWM_setCountModeAfterSync(obj->pwmHandle[cnt],
                                   EPWM_COUNT_MODE_UP_AFTER_SYNC);

        EPWM_setEmulationMode(obj->pwmHandle[cnt], EPWM_EMULATION_FREE_RUN);

        // setup the Timer-Based Phase Register (TBPHS)
        EPWM_setPhaseShift(obj->pwmHandle[cnt], 0);

        // setup the Time-Base Counter Register (TBCTR)
        EPWM_setTimeBaseCounter(obj->pwmHandle[cnt], 0);

        // setup the Time-Base Period Register (TBPRD)
        // set to zero initially
        EPWM_setTimeBasePeriod(obj->pwmHandle[cnt], 0);

        // setup the Counter-Compare Control Register (CMPCTL)
        EPWM_setCounterCompareShadowLoadMode(obj->pwmHandle[cnt],
                                             EPWM_COUNTER_COMPARE_A,
                                             EPWM_COMP_LOAD_ON_CNTR_ZERO);

        EPWM_setCounterCompareShadowLoadMode(obj->pwmHandle[cnt],
                                             EPWM_COUNTER_COMPARE_B,
                                             EPWM_COMP_LOAD_ON_CNTR_ZERO);

        EPWM_setCounterCompareShadowLoadMode(obj->pwmHandle[cnt],
                                             EPWM_COUNTER_COMPARE_C,
                                             EPWM_COMP_LOAD_ON_CNTR_ZERO);

        EPWM_setCounterCompareShadowLoadMode(obj->pwmHandle[cnt],
                                             EPWM_COUNTER_COMPARE_D,
                                             EPWM_COMP_LOAD_ON_CNTR_ZERO);

        // setup the Action-Qualifier Output A Register (AQCTLA)
        EPWM_setActionQualifierAction(obj->pwmHandle[cnt],
                                      EPWM_AQ_OUTPUT_A,
                                      EPWM_AQ_OUTPUT_HIGH,
                                      EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);

        EPWM_setActionQualifierAction(obj->pwmHandle[cnt],
                                      EPWM_AQ_OUTPUT_A,
                                      EPWM_AQ_OUTPUT_LOW,
                                      EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);

        // setup the Action-qualifier Continuous Software Force Register
        // (AQCSFRC)
        EPWM_setActionQualifierContSWForceAction(obj->pwmHandle[cnt],
                                                 EPWM_AQ_OUTPUT_B,
                                                 EPWM_AQ_SW_OUTPUT_HIGH);

        // setup the Dead-Band Generator Control Register (DBCTL)
        EPWM_setDeadBandDelayMode(obj->pwmHandle[cnt], EPWM_DB_RED, true);
        EPWM_setDeadBandDelayMode(obj->pwmHandle[cnt], EPWM_DB_FED, true);

        // select EPWMA as the input to the dead band generator
        EPWM_setRisingEdgeDeadBandDelayInput(obj->pwmHandle[cnt],
                                             EPWM_DB_INPUT_EPWMA);

        // configure the right polarity for active high complementary config.
        EPWM_setDeadBandDelayPolarity(obj->pwmHandle[cnt],
                                      EPWM_DB_RED,
                                      EPWM_DB_POLARITY_ACTIVE_HIGH);
        EPWM_setDeadBandDelayPolarity(obj->pwmHandle[cnt],
                                      EPWM_DB_FED,
                                      EPWM_DB_POLARITY_ACTIVE_LOW);

        // setup the Dead-Band Rising Edge Delay Register (DBRED)
        EPWM_setRisingEdgeDelayCount(obj->pwmHandle[cnt], MTR1_PWM_DBRED_CNT);

        // setup the Dead-Band Falling Edge Delay Register (DBFED)
        EPWM_setFallingEdgeDelayCount(obj->pwmHandle[cnt], MTR1_PWM_DBFED_CNT);

        // setup the PWM-Chopper Control Register (PCCTL)
        EPWM_disableChopper(obj->pwmHandle[cnt]);

        // setup the Trip Zone Select Register (TZSEL)
        EPWM_disableTripZoneSignals(obj->pwmHandle[cnt], HAL_TZSEL_SIGNALS_ALL);
    }

    // setup the Event Trigger Selection Register (ETSEL)
    EPWM_setInterruptSource(obj->pwmHandle[0], EPWM_INT_TBCTR_PERIOD);

    EPWM_disableInterrupt(obj->pwmHandle[0]);

    EPWM_setADCTriggerSource(obj->pwmHandle[0],
                             EPWM_SOC_A, EPWM_SOC_TBCTR_D_CMPC);

    EPWM_enableADCTrigger(obj->pwmHandle[0], EPWM_SOC_A);

    // setup the Event Trigger Prescale Register (ETPS)
    if(numPWMTicksPerISRTick > 15)
    {
        EPWM_setInterruptEventCount(obj->pwmHandle[0], 15);
        EPWM_setADCTriggerEventPrescale(obj->pwmHandle[0], EPWM_SOC_A, 15);
    }
    else if(numPWMTicksPerISRTick < 1)
    {
        EPWM_setInterruptEventCount(obj->pwmHandle[0], 1);
        EPWM_setADCTriggerEventPrescale(obj->pwmHandle[0], EPWM_SOC_A, 1);
    }
    else
    {
        EPWM_setInterruptEventCount(obj->pwmHandle[0], numPWMTicksPerISRTick);
        EPWM_setADCTriggerEventPrescale(obj->pwmHandle[0], EPWM_SOC_A,
                                        numPWMTicksPerISRTick);
    }

    // setup the Event Trigger Clear Register (ETCLR)
    EPWM_clearEventTriggerInterruptFlag(obj->pwmHandle[0]);
    EPWM_clearADCTriggerFlag(obj->pwmHandle[0], EPWM_SOC_A);

    // since the PWM is configured as an up/down counter, the period register is
    // set to one-half of the desired PWM period
    EPWM_setTimeBasePeriod(obj->pwmHandle[0], halfPeriod_cycles);
    EPWM_setTimeBasePeriod(obj->pwmHandle[1], halfPeriod_cycles);
    EPWM_setTimeBasePeriod(obj->pwmHandle[2], halfPeriod_cycles);

    // write the PWM data value  for ADC trigger
    EPWM_setCounterCompareValue(obj->pwmHandle[0], EPWM_COUNTER_COMPARE_C,
                                MTR1_ADC_TRIGGER_COUNT);

    // BSXL8323RH_REVB || BSXL8323RS_REVA || BSXL8353RS_REVA

    // enable the ePWM module time base clock sync signal
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);

    return;
}  // end of HAL_setupPWMs() function

#ifdef EQEP_ENABLE
void HAL_setupQEP(HAL_Handle handle, HAL_QEPSelect_e qep)
{
  HAL_Obj   *obj = (HAL_Obj *)handle;

  //
  // Configure the decoder for quadrature count mode
  //
  EQEP_setDecoderConfig(obj->qepHandle[qep], (EQEP_CONFIG_2X_RESOLUTION |
                                     EQEP_CONFIG_QUADRATURE |
                                     EQEP_CONFIG_NO_SWAP));

  EQEP_setEmulationMode(obj->qepHandle[qep], EQEP_EMULATIONMODE_RUNFREE);

  //
  // Configure the position counter to be latched on a unit time out
  //

  EQEP_setLatchMode(obj->qepHandle[qep], (EQEP_LATCH_RISING_INDEX|EQEP_LATCH_UNIT_TIME_OUT));


  //
  // Configure the position counter to reset on an index event
  //

  EQEP_setPositionCounterConfig(obj->qepHandle[qep], EQEP_POSITION_RESET_IDX,
                                4*USER_MOTOR_NUM_ENC_SLOTS - 1);

   //
  // Enable the unit timer, setting the frequency to 100 Hz
  //
  EQEP_enableUnitTimer(obj->qepHandle[qep], (DEVICE_SYSCLK_FREQ / 100));

  // Disables the eQEP module position-compare unit
  EQEP_disableCompare(obj->qepHandle[qep]);

  //
  // Configure and enable the edge-capture unit. The capture clock divider is
  // SYSCLKOUT/64. The unit-position event divider is QCLK/32.
  //
  EQEP_setCaptureConfig(obj->qepHandle[qep], EQEP_CAPTURE_CLK_DIV_128,
                        EQEP_UNIT_POS_EVNT_DIV_32);

  EQEP_enableCapture(obj->qepHandle[qep]);

  // Enable UTO on QEP
  EQEP_enableInterrupt(obj->qepHandle[qep], EQEP_INT_UNIT_TIME_OUT);

  //
  // Enable the eQEP module
  //

  EQEP_enableModule(obj->qepHandle[qep]);

  return;
}
#endif

#if defined(SCI_ENABLE)
void HAL_setupSCIA(HAL_Handle halHandle)
{
  HAL_Obj *obj = (HAL_Obj *)halHandle;

  // Initialize SCIA and its FIFO.
  SCI_performSoftwareReset(obj->sciHandle);

  // Configure SCIA for echoback.
  SCI_setConfig(obj->sciHandle, DEVICE_LSPCLK_FREQ, 9600, (SCI_CONFIG_WLEN_8 |
                                                      SCI_CONFIG_STOP_ONE |
                                                      SCI_CONFIG_PAR_NONE));
  SCI_resetChannels(obj->sciHandle);

  SCI_resetRxFIFO(obj->sciHandle);

  SCI_resetTxFIFO(obj->sciHandle);

  SCI_clearInterruptStatus(obj->sciHandle, SCI_INT_TXFF | SCI_INT_RXFF);

  SCI_enableFIFO(obj->sciHandle);

  SCI_enableModule(obj->sciHandle);

  SCI_performSoftwareReset(obj->sciHandle);

}  // end of HAL_setupSCIA() function
#endif  // SCI_ENABLE

#ifdef CMD_CAN_FD_ENABLE
void HAL_setupMCAN(HAL_Handle halHandle)
{

    HAL_Obj *obj = (HAL_Obj *)halHandle;

    MCAN_InitParams            initParams;
    MCAN_MsgRAMConfigParams    msgRAMConfigParams;
    MCAN_BitTimingParams       bitTimes;
    MCAN_StdMsgIDFilterElement stdFiltelem;
//    MCAN_GlobalFiltConfig      filter_config_obj;
//    MCAN_ConfigParams          config_params;     // Reserve
//    MCAN_RxBufElement          rxMsg1;            // Reserve

//    filter_config_obj.anfs = 1;

//    config_params.filterConfig = filter_config_obj;   // Reserve

    //
    // Configure the divisor for the MCAN bit-clock
    //
    SysCtl_setMCANClk(SYSCTL_MCANA, SYSCTL_MCANCLK_DIV_5);

    // Initialize MCAN Init parameters.
    //
    //
    initParams.fdMode            = 0x1U; // FD operation enabled.
    initParams.brsEnable         = 0x1U; // Bit rate switching for
                                         // transmissions enabled.

    // Transmitter Delay Compensation parameters.
    //
    initParams.tdcConfig.tdcf    = 0xAU;
    initParams.tdcConfig.tdco    = 0x6U;

    //
    // Initialize Message RAM Sections Configuration Parameters for Rx
    //
    msgRAMConfigParams.flssa                = MCAN_STD_ID_FILT_START_ADDR;
    // Standard ID Filter List Start Address (0).
    msgRAMConfigParams.lss                  = MCAN_STD_ID_FILTER_NUM;
    // Standard ID Filter List Size (1).

    msgRAMConfigParams.rxFIFO1startAddr     = MCAN_FIFO_1_START_ADDR;
    // Rx FIFO1 Start Address (748U).
    msgRAMConfigParams.rxFIFO1size          = MCAN_FIFO_1_NUM;
    // Number of Rx FIFO elements (1).
    msgRAMConfigParams.rxFIFO1waterMark     = MCAN_FIFO_1_WATERMARK;
    // Level for Rx FIFO 1 watermark interrupt (1).
    msgRAMConfigParams.rxFIFO1OpMode        = 1U;
    // FIFO overwriting mode.
    msgRAMConfigParams.rxFIFO1ElemSize      = MCAN_ELEM_SIZE_64BYTES;
    // Rx FIFO1 Element Size. RBDS field of MCAN_RXESC Register

    //
    // Initialize Message RAM Sections Configuration Parameters
    //
    msgRAMConfigParams.txStartAddr          = MCAN_TX_BUFF_START_ADDR ;
    // Tx Buffers Start Address.
    msgRAMConfigParams.txBufNum             = MCAN_TX_BUFF_SIZE;
    // Number of Dedicated Transmit Buffers.
    msgRAMConfigParams.txBufMode            = 0U;
    msgRAMConfigParams.txBufElemSize        = MCAN_ELEM_SIZE_64BYTES;
    // Tx Buffer Element Size.

    //
    // Initialize Rx Buffer Filter element Configuration parameters.
    //
    stdFiltelem.sfid2              = 0x00U; // Standard ID Filter Mask.
    // A 0 in any bit-position indicates "don't care"
    stdFiltelem.sfid1              = 0xa5U; // Standard ID Filter
    stdFiltelem.sfec               = 0x6U; // Store into FIFO 1.
    stdFiltelem.sft                = 0x02U; // SFID1 = filter; SFID2 = mask

    //
    // Initialize bit timings.
    //
    bitTimes.nomRatePrescalar   = 0x3U; // Nominal Baud Rate Pre-scaler.
    bitTimes.nomTimeSeg1        = 0x9U; // Nominal Time segment before SP
    bitTimes.nomTimeSeg2        = 0x8U; // Nominal Time segment after SP
    bitTimes.nomSynchJumpWidth  = 0x8U; // Nominal SJW
    bitTimes.dataRatePrescalar  = 0x1U; // Data Baud Rate Pre-scaler.
    bitTimes.dataTimeSeg1       = 0x9U; // Data Time segment before SP
    bitTimes.dataTimeSeg2       = 0x8U; // Data Time segment after SP
    bitTimes.dataSynchJumpWidth = 0x8U; // Data SJW

    //
    // Wait for memory initialization to happen.
    //
    while(false == MCAN_isMemInitDone(obj->mcanHandle))
    {
    }

    //
    // Put MCAN in SW initialization mode.
    //
    MCAN_setOpMode(obj->mcanHandle, MCAN_OPERATION_MODE_SW_INIT);

    //
    // Wait till MCAN is not initialized.
    //
    while (MCAN_OPERATION_MODE_SW_INIT != MCAN_getOpMode(obj->mcanHandle))
    {}

    //
    // Initialize MCAN module.
    //
    MCAN_init(obj->mcanHandle, &initParams);

    //
    // Configure Bit timings.
    //
    MCAN_setBitTime(obj->mcanHandle, &bitTimes);

    //
    // Configure Message RAM Sections
    //
    MCAN_msgRAMConfig(obj->mcanHandle, &msgRAMConfigParams);

    //
    // Configure Standard ID filter element
    //
    MCAN_addStdMsgIDFilter(obj->mcanHandle, 0U, &stdFiltelem);


    /*
     * Uncommend the following line in order to bypass any type of Message Filter
     */
   // MCAN_config(obj->mcanHandle, &config_params);         // Reserve


    //
    // Take MCAN out of the SW initialization mode
    //
    MCAN_setOpMode(obj->mcanHandle, MCAN_OPERATION_MODE_NORMAL);

    while (MCAN_OPERATION_MODE_NORMAL != MCAN_getOpMode(obj->mcanHandle))
    {
    }

    //
    // Enable Interrupts.
    //
    MCAN_enableIntr(obj->mcanHandle, MCAN_INTR_MASK_ALL, 1U);

    //
    // Select Interrupt Line.
    //
    MCAN_selectIntrLine(obj->mcanHandle, MCAN_INTR_MASK_ALL, MCAN_INTR_LINE_NUM_1);

    //
    // Enable Interrupt Line.
    //
    MCAN_enableIntrLine(obj->mcanHandle, MCAN_INTR_LINE_NUM_1, 1U);

    return;
}


void HAL_configMCAN(HAL_Handle halHandle)
{
    // Reserve this function for further update

    return;
}
#endif  // CMD_CAN_ENABLE

#ifdef CMD_CAN_ENABLE
void HAL_setupCANA(HAL_Handle halHandle)
{
    HAL_Obj *obj = (HAL_Obj *)halHandle;

    // Initialize the CAN controller
    CAN_initModule(obj->canHandle);

    // Set up the CAN bus bit rate to 200kHz
    // Refer to the Driver Library User Guide for information on how to set
    // tighter timing control. Additionally, consult the device data sheet
    // for more information about the CAN module clocking.
    CAN_setBitRate(obj->canHandle, DEVICE_SYSCLK_FREQ, 500000, 16);

    // Initialize the transmit message object used for sending CAN messages.
    // Message Object Parameters:
    //      Message Object ID Number: 1
    //      Message Identifier: 0x1
    //      Message Frame: Standard
    //      Message Type: Transmit
    //      Message ID Mask: 0x0
    //      Message Object Flags: Transmit Interrupt
    //      Message Data Length: 8 Bytes
    CAN_setupMessageObject(obj->canHandle, TX_MSG_OBJ_ID, 0x1, CAN_MSG_FRAME_STD,
                           CAN_MSG_OBJ_TYPE_TX, 0, CAN_MSG_OBJ_TX_INT_ENABLE,
                           MSG_DATA_LENGTH);

    // Initialize the receive message object used for receiving CAN messages.
    // Message Object Parameters:
    //      Message Object ID Number: 2
    //      Message Identifier: 0x1
    //      Message Frame: Standard
    //      Message Type: Receive
    //      Message ID Mask: 0x0
    //      Message Object Flags: Receive Interrupt
    //      Message Data Length: 8 Bytes
    CAN_setupMessageObject(obj->canHandle, RX_MSG_OBJ_ID, 0x1, CAN_MSG_FRAME_STD,
                           CAN_MSG_OBJ_TYPE_RX, 0, CAN_MSG_OBJ_RX_INT_ENABLE,
                           MSG_DATA_LENGTH);

    // Start CAN module operations
    CAN_startModule(obj->canHandle);

    return;
}
#endif  // CMD_CAN_ENABLE

#ifdef SPI_ENABLE
void HAL_setupSPIA(HAL_Handle handle)
{
  HAL_Obj   *obj = (HAL_Obj *)handle;

  // Must put SPI into reset before configuring it
  SPI_disableModule(obj->spiHandle[0]);

  // SPI configuration. Use a 1MHz SPICLK and 16-bit word size. 500kbps
  SPI_setConfig(obj->spiHandle[0], DEVICE_LSPCLK_FREQ, SPI_PROT_POL0PHA0,
                SPI_MODE_CONTROLLER, 400000, 16);

  SPI_disableLoopback(obj->spiHandle[0]);

  SPI_setEmulationMode(obj->spiHandle[0], SPI_EMULATION_FREE_RUN);

  SPI_enableFIFO(obj->spiHandle[0]);
  HWREGH((obj->spiHandle[0])+SPI_O_FFCT) = 0x0018;

  SPI_clearInterruptStatus(obj->spiHandle[0], SPI_INT_TXFF);

  // Configuration complete. Enable the module.
  SPI_enableModule(obj->spiHandle[0]);

  return;
}  // end of HAL_setupSPIA() function

void HAL_setupSPIB(HAL_Handle handle)
{
  HAL_Obj   *obj = (HAL_Obj *)handle;

  // Must put SPI into reset before configuring it
  SPI_disableModule(obj->spiHandle[1]);

  // SPI configuration. Use a 1MHz SPICLK and 16-bit word size.
  SPI_setConfig(obj->spiHandle[1], DEVICE_LSPCLK_FREQ, SPI_PROT_POL0PHA0,
                SPI_MODE_CONTROLLER, 400000, 16);

  SPI_disableLoopback(obj->spiHandle[1]);

  SPI_setEmulationMode(obj->spiHandle[1], SPI_EMULATION_FREE_RUN);

  SPI_enableFIFO(obj->spiHandle[1]);
  HWREGH((obj->spiHandle[1])+SPI_O_FFCT) = 0x0018;

  SPI_clearInterruptStatus(obj->spiHandle[1], SPI_INT_TXFF);

  // Configuration complete. Enable the module.
  SPI_enableModule(obj->spiHandle[1]);

  return;
}  // end of HAL_setupSPIB() function
#endif

void HAL_setupTimers(HAL_Handle handle, const float32_t systemFreq_MHz)
{
  HAL_Obj  *obj = (HAL_Obj *)handle;

  //1ms
  uint32_t timerPeriod_1ms = (uint32_t)(systemFreq_MHz *
                              (float32_t)1000.0) - 1;

  // use timer 0 for CPU usage diagnostics
  CPUTimer_setPreScaler(obj->timerHandle[0], 0);
  CPUTimer_setEmulationMode(obj->timerHandle[0],
                            CPUTIMER_EMULATIONMODE_RUNFREE);
  CPUTimer_setPeriod(obj->timerHandle[0], timerPeriod_1ms);

  //10ms
  uint32_t timerPeriod_10ms = (uint32_t)(systemFreq_MHz *
                              (float32_t)10000.0) - 1;

  // use timer 1 for CPU usage diagnostics
  CPUTimer_setPreScaler(obj->timerHandle[1], 0);
  CPUTimer_setEmulationMode(obj->timerHandle[1],
                            CPUTIMER_EMULATIONMODE_RUNFREE);
  CPUTimer_setPeriod(obj->timerHandle[1], timerPeriod_10ms);

  // use timer 2 for CPU usage diagnostics
  CPUTimer_setPreScaler(obj->timerHandle[2], 0);
  CPUTimer_setEmulationMode(obj->timerHandle[2],
                            CPUTIMER_EMULATIONMODE_RUNFREE);
  CPUTimer_setPeriod(obj->timerHandle[2], 0xFFFFFFFF);

  return;
}  // end of HAL_setupTimers() function


//------------------------------------------------------------------
bool HAL_getTimerStatus(HAL_Handle halHandle, const uint16_t cpuTimerNumber)
{
   HAL_Obj   *obj = (HAL_Obj *)halHandle;
   bool cpuTimerStatus;

   cpuTimerStatus = CPUTimer_getTimerOverflowStatus(obj->timerHandle[cpuTimerNumber]);
   return cpuTimerStatus;
}   // end of HAL_getTimerStatus() function


void HAL_clearTimerFlag(HAL_Handle halHandle, const uint16_t cpuTimerNumber)
{
   HAL_Obj   *obj = (HAL_Obj *)halHandle;

   CPUTimer_clearOverflowFlag(obj->timerHandle[cpuTimerNumber]);
}   // end of HAL_clearTimerFlag() function

#ifdef DATALOG_ENABLE
void HAL_setupDlogWithDMA(HAL_Handle handle, const uint16_t DMAChannel, const void *dlogDestAddr, const void *dlogSrcAddr)
{
    HAL_Obj *obj = (HAL_Obj *)handle;

    const void *destAddr;
    const void *srcAddr;
    destAddr = (const void *)dlogDestAddr;
    srcAddr  = (const void *)dlogSrcAddr;

    //
    // configure DMA Channel
    //
    DMA_configAddresses(obj->dmaChHandle[DMAChannel], destAddr, srcAddr);
    DMA_configBurst(obj->dmaChHandle[DMAChannel],DLOG_BURST,2,2);
    DMA_configTransfer(obj->dmaChHandle[DMAChannel],DLOG_TRANSFER,1,1);
    DMA_configMode(obj->dmaChHandle[DMAChannel],DMA_TRIGGER_SOFTWARE,
                   DMA_CFG_ONESHOT_ENABLE+DMA_CFG_CONTINUOUS_ENABLE+DMA_CFG_SIZE_32BIT);
    DMA_setInterruptMode(obj->dmaChHandle[DMAChannel],DMA_INT_AT_END);
    DMA_enableTrigger(obj->dmaChHandle[DMAChannel]);
    DMA_disableInterrupt(obj->dmaChHandle[DMAChannel]);

    return;
}    //end of HAL_initDlogDMA() function
#endif  //  DATALOG_ENABLE

#ifdef _FLASH
void HAL_clearDataRAM(void *pMemory, uint16_t lengthMemory)
{
    uint16_t *pMemoryStart;
    uint16_t LoopCount, LoopLength;

    pMemoryStart = pMemory;
    LoopLength = lengthMemory;

    for(LoopCount=0; LoopCount<LoopLength; LoopCount++)
        *(pMemoryStart+LoopCount) = 0x0000;
}   //end of HAL_clearDataRAM() function
#endif

// end of file
