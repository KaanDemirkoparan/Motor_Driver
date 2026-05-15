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


//! \file   solutions/universal_motorcontrol_lab/f28p65x/drivers/hal_all.c
//! \brief  Contains the various functions related to the HAL object
//!
//

//
// the includes
//
#include "user.h"


//
// drivers
//

// modules

// platforms
#include "hal.h"
#include "hal_obj.h"

// libraries
#include "datalogIF.h"

#ifdef _FLASH
#pragma CODE_SECTION(Flash_initModule, ".TI.ramfunc");
#endif

// **************************************************************************
// the defines


// **************************************************************************
// the globals
HAL_Handle    halHandle;      //!< the handle for the hardware abstraction layer
HAL_Obj       hal;            //!< the hardware abstraction layer object
#pragma DATA_SECTION(halHandle, "hal_data");
#pragma DATA_SECTION(hal, "hal_data");

// **************************************************************************
// the functions

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

void HAL_enableCtrlInts(HAL_Handle handle)
{
    // enable the ADC interrupts for motor_1
    ADC_enableInterrupt(MTR1_ADC_INT_BASE, MTR1_ADC_INT_NUM);

    // enable the PIE interrupts associated with the ADC interrupts
    Interrupt_enable(MTR1_PIE_INT_NUM);    // motor_1

    return;
} // end of HAL_enableCtrlInts() function

// No HAL_enableADCIntsToTriggerCLA() in this device

void HAL_enableDebugInt(HAL_Handle handle)
{

    // enable debug events
    ERTM;

    return;
} // end of HAL_enableDebugInt() function

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
    {
        return((HAL_Handle)NULL);
    }

    // assign the handle
    handle = (HAL_Handle)pMemory;

    // assign the object
    obj = (HAL_Obj *)handle;

    // Two ADC modules in this device
    // initialize the ADC handles
    obj->adcHandle[0] = ADCA_BASE;
    obj->adcHandle[1] = ADCB_BASE;
    obj->adcHandle[2] = ADCC_BASE;

    // initialize the ADC results
    obj->adcResult[0] = ADCARESULT_BASE;
    obj->adcResult[1] = ADCBRESULT_BASE;
    obj->adcResult[2] = ADCCRESULT_BASE;

    // No DAC modules in this device
    // No CLA module in this device

    // initialize SCI handle
    obj->sciHandle = SCIA_BASE;             //!< the SCIA handle

    // initialize CAN handle
    obj->canHandle = CANA_BASE;             //!< the CANA handle

    // initialize DMA handle
    obj->dmaHandle = DMA_BASE;              //!< the DMA handle

#if defined(DATALOGF2_EN)
    // initialize DMA channel handle
    obj->dmaChHandle[0] = DMA_DATALOG1_BASE;     //!< the DMA Channel handle
    obj->dmaChHandle[1] = DMA_DATALOG2_BASE;     //!< the DMA Channel handle
#elif defined(DATALOGF4_EN) || defined(DATALOGI4_EN)
    obj->dmaChHandle[0] = DMA_DATALOG1_BASE;     //!< the DMA Channel handle
    obj->dmaChHandle[1] = DMA_DATALOG2_BASE;     //!< the DMA Channel handle
    obj->dmaChHandle[2] = DMA_DATALOG3_BASE;     //!< the DMA Channel handle
    obj->dmaChHandle[3] = DMA_DATALOG4_BASE;     //!< the DMA Channel handle
#endif  // DATALOGF2_EN | DATALOGF4_EN

    // initialize timer handles
    obj->timerHandle[0] = CPUTIMER0_BASE;
    obj->timerHandle[1] = CPUTIMER1_BASE;
    obj->timerHandle[2] = CPUTIMER2_BASE;

    return(handle);
} // end of HAL_init() function

HAL_MTR_Handle HAL_MTR1_init(void *pMemory, const size_t numBytes)
{
    HAL_MTR_Handle handle;
    HAL_MTR_Obj *obj;

    if(numBytes < sizeof(HAL_MTR_Obj))
    {
        return((HAL_MTR_Handle)NULL);
    }

    // assign the handle
    handle = (HAL_MTR_Handle)pMemory;

    // assign the object
    obj = (HAL_MTR_Obj *)handle;

    // initialize PWM handles for Motor 1
    obj->pwmHandle[0] = MTR1_PWM_U_BASE;        //!< the PWM handle
    obj->pwmHandle[1] = MTR1_PWM_V_BASE;        //!< the PWM handle
    obj->pwmHandle[2] = MTR1_PWM_W_BASE;        //!< the PWM handle

    // initialize CMPSS handle
    obj->cmpssHandle[0] = MTR1_CMPSS_U_BASE_H;  //!< the CMPSS handle
    obj->cmpssHandle[1] = MTR1_CMPSS_U_BASE_L;  //!< the CMPSS handle
    obj->cmpssHandle[2] = MTR1_CMPSS_V_BASE_H;  //!< the CMPSS handle
    obj->cmpssHandle[3] = MTR1_CMPSS_V_BASE_L;  //!< the CMPSS handle
    obj->cmpssHandle[4] = MTR1_CMPSS_W_BASE_H;  //!< the CMPSS handle
    obj->cmpssHandle[5] = MTR1_CMPSS_W_BASE_L;  //!< the CMPSS handle

#if defined(MOTOR1_HALL) && defined(CMD_CAP_EN)
#error HALL and CMD_CAP can't be enabled at the same time
#elif defined(MOTOR1_HALL)
    // initialize CAP handles for Motor 1
    obj->capHandle[0] = MTR1_CAP_U_BASE;        //!< the CAP handle
    obj->capHandle[1] = MTR1_CAP_V_BASE;        //!< the CAP handle
    obj->capHandle[2] = MTR1_CAP_W_BASE;        //!< the CAP handle
#elif defined(CMD_CAP_EN)
    obj->capHandle = MTR1_CAP_FREQ_BASE;        //!< the CAP handle
#endif // MOTOR1_HALL || CMD_CAP_EN

    // No PGA modules in this device

    // Assign gateEnableGPIO
#if defined(BSXL8323RS_REVA)
    // initialize drv8323 interface
    obj->drvicHandle = DRVIC_init(&obj->drvic);

    // initialize SPI handle
    obj->spiHandle = MTR1_SPI_BASE;             //!< the SPI handle

    obj->gateEnableGPIO = MTR1_GATE_EN_GPIO;
    obj->gateCalGPIO = MTR1_GATE_CAL_GPIO;
    // BSXL8323RS_REVA
#elif defined(BSXL8353RS_REVA)
    // initialize drv8353interface
    obj->drvicHandle = DRVIC_init(&obj->drvic);

    // initialize SPI handle
    obj->spiHandle = MTR1_SPI_BASE;             //!< the SPI handle

    obj->gateEnableGPIO = MTR1_GATE_EN_GPIO;
    // BSXL8353RS_REVA
#elif defined(BSXL3PHGAN_REVA)
    obj->gateEnableGPIO = MTR1_GATE_EN_GPIO;
    // BSXL3PHGAN_REVA
#endif  // Assign gateEnableGPIO

    // initialize QEP driver
    obj->qepHandle = MTR1_QEP_BASE;             // the QEP handle


    obj->motorNum = MTR_1;

    return(handle);
} // end of HAL_MTR1_init() function

void HAL_setParams(HAL_Handle handle)
{
    // disable global interrupts
    Interrupt_disableGlobal();

#ifdef _FLASH
    //
    // Copy time critical code and flash setup code to RAM. This includes the
    // following functions: InitFlash();
    //
    // The RamfuncsLoadStart, RamfuncsLoadSize, and RamfuncsRunStart symbols
    // are created by the linker. Refer to the device .cmd file.
    //
    memcpy(&runStart_ctrlfuncs, &loadStart_ctrlfuncs, (size_t)&loadSize_ctrlfuncs);
#endif  // _FLASH


    // setup the ADCs
    HAL_setupADCs(handle);


    // Sets up the CPU timer for time base
    HAL_setupTimeBaseTimer(handle, USER_TIME_BASE_FREQ_Hz);

    // Sets up the timers for CPU usage diagnostics
    HAL_setupCPUUsageTimer(handle);

#if defined(DATALOGF4_EN) || defined(DATALOGF2_EN)
    // setup the DMA
    HAL_setupDMA();
#endif  // DATALOGF4_EN || DATALOGF2_EN

    // setup the SCI
    HAL_setupSCIA(handle);

    // setup the i2c
    HAL_setupI2CA(handle);

    return;
} // end of HAL_setParams() function

void HAL_MTR_setParams(HAL_MTR_Handle handle, USER_Params *pUserParams)
{
    HAL_setNumCurrentSensors(handle, pUserParams->numCurrentSensors);
    HAL_setNumVoltageSensors(handle, pUserParams->numVoltageSensors);

    // setup the PWMs
    HAL_setupPWMs(handle);

    // setup the CMPSSs
    HAL_setupCMPSSs(handle);

#if defined(MOTOR1_ENC)
    // setup the EQEP
    HAL_setupQEP(handle);
#endif  // MOTOR1_ENC

    // setup faults
    HAL_setupMtrFaults(handle);

#if defined(MOTOR1_HALL) || defined(CMD_CAP_EN)
    // setup the CAPs
    HAL_setupCAPs(handle);
#endif  // MOTOR1_HALL || CMD_CAP_EN

#if defined(BSXL8323RS_REVA) || defined(BSXL8353RS_REVA)

    // setup the spi for drv8323/drv8353
    HAL_setupSPI(handle);
#endif  // BSXL8323RS_REVA || BSXL8353RS_REVA

    // disable the PWM
    HAL_disablePWM(handle);

    return;
} // end of HAL_MTR_setParams() function

// p65x updated edit/test added required ADC configuration func
//
void HAL_setupAnalogPinmux_P65x(void)
{
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

    // Required to use B0 as an analog input:
    DAC_setReferenceVoltage(DACA_BASE, DAC_REF_ADC_VREFHI);
    DAC_setReferenceVoltage(DACC_BASE, DAC_REF_ADC_VREFHI);
    // TODO Test if actually required

    return;
}

void HAL_setupADCs(HAL_Handle handle)
{
    HAL_Obj *obj = (HAL_Obj *)handle;

    SysCtl_delay(100U);

#if defined(ADC_EXT_REF)
    ADC_setVREF(obj->adcHandle[0], ADC_REFERENCE_EXTERNAL, ADC_REFERENCE_VREFHI);
    ADC_setVREF(obj->adcHandle[1], ADC_REFERENCE_EXTERNAL, ADC_REFERENCE_VREFHI);
    ADC_setVREF(obj->adcHandle[2], ADC_REFERENCE_EXTERNAL, ADC_REFERENCE_VREFHI);
#else // !ADC_EXT_REF
    ADC_setVREF(obj->adcHandle[0], ADC_REFERENCE_INTERNAL, ADC_REFERENCE_3_3V);
    ADC_setVREF(obj->adcHandle[1], ADC_REFERENCE_INTERNAL, ADC_REFERENCE_3_3V);
    ADC_setVREF(obj->adcHandle[2], ADC_REFERENCE_INTERNAL, ADC_REFERENCE_3_3V);
#endif // ADC_EXT_REF check

    SysCtl_delay(100U);

    // Set main clock scaling factor (50MHz max clock for the ADC module)
    ADC_setPrescaler(obj->adcHandle[0], ADC_CLK_DIV_4_0);
    ADC_setPrescaler(obj->adcHandle[1], ADC_CLK_DIV_4_0);
    ADC_setPrescaler(obj->adcHandle[2], ADC_CLK_DIV_4_0);

    // set the ADC interrupt pulse generation to end of conversion
    ADC_setInterruptPulseMode(obj->adcHandle[0], ADC_PULSE_END_OF_CONV);
    ADC_setInterruptPulseMode(obj->adcHandle[1], ADC_PULSE_END_OF_CONV);
    ADC_setInterruptPulseMode(obj->adcHandle[2], ADC_PULSE_END_OF_CONV);

    // set priority of SOCs
    ADC_setSOCPriority(obj->adcHandle[0], ADC_PRI_ALL_HIPRI);
    ADC_setSOCPriority(obj->adcHandle[1], ADC_PRI_ALL_HIPRI);
    ADC_setSOCPriority(obj->adcHandle[2], ADC_PRI_ALL_HIPRI);

    // enable the ADCs
    ADC_enableConverter(obj->adcHandle[0]);
    ADC_enableConverter(obj->adcHandle[1]);
    ADC_enableConverter(obj->adcHandle[2]);

    // delay to allow ADCs to power up
    SysCtl_delay(1000U);

    HAL_setupAnalogPinmux_P65x();   // p65x added func for analog pinmux

    //-------------------------------------------------------------------------
    // configure the SOCs for M1
    // configure the interrupt sources
    // Interrupt for motor 1
    ADC_setInterruptSource(MTR1_ADC_INT_BASE,
                           MTR1_ADC_INT_NUM, MTR1_ADC_INT_SOC);
    // ISEN_A_M1
    ADC_setupSOC(MTR1_IU_ADC_BASE, MTR1_IU_ADC_SOC_NUM, MTR1_ADC_TRIGGER_SOC,
                 MTR1_IU_ADC_CH_NUM, MTR1_ADC_I_SAMPLEWINDOW);

    // Configure PPB to eliminate subtraction related calculation
    // PPB is associated with ADCA_SOC0
    ADC_setupPPB(MTR1_IU_ADC_BASE, MTR1_IU_ADC_PPB_NUM, MTR1_IU_ADC_SOC_NUM);

    // Write zero to this for now till offset calibration complete
    ADC_setPPBCalibrationOffset(MTR1_IU_ADC_BASE, MTR1_IU_ADC_PPB_NUM, 0);

    // ISEN_B_M1
    ADC_setupSOC(MTR1_IV_ADC_BASE, MTR1_IV_ADC_SOC_NUM, MTR1_ADC_TRIGGER_SOC,
                 MTR1_IV_ADC_CH_NUM, MTR1_ADC_I_SAMPLEWINDOW);

    // Configure PPB to eliminate subtraction related calculation
    // PPB is associated with ADCC_SOC0
    ADC_setupPPB(MTR1_IV_ADC_BASE, MTR1_IV_ADC_PPB_NUM, MTR1_IV_ADC_SOC_NUM);

    // Write zero to this for now till offset calibration complete
    ADC_setPPBCalibrationOffset(MTR1_IV_ADC_BASE, MTR1_IV_ADC_PPB_NUM, 0);

    // ISEN_C_M1
    ADC_setupSOC(MTR1_IW_ADC_BASE, MTR1_IW_ADC_SOC_NUM, MTR1_ADC_TRIGGER_SOC,
                 MTR1_IW_ADC_CH_NUM, MTR1_ADC_I_SAMPLEWINDOW);

    // Configure PPB to eliminate subtraction related calculation
    // PPB is associated with ADCA_SOC0
    ADC_setupPPB(MTR1_IW_ADC_BASE, MTR1_IW_ADC_PPB_NUM, MTR1_IW_ADC_SOC_NUM);

    // Write zero to this for now till offset calibration complete
    ADC_setPPBCalibrationOffset(MTR1_IW_ADC_BASE, MTR1_IW_ADC_PPB_NUM, 0);


#if defined(MOTOR1_FAST)
    // VSEN_A_M1
    ADC_setupSOC(MTR1_VU_ADC_BASE, MTR1_VU_ADC_SOC_NUM, MTR1_ADC_TRIGGER_SOC,
                 MTR1_VU_ADC_CH_NUM, MTR1_ADC_V_SAMPLEWINDOW);

    // VSEN_B_M1
    ADC_setupSOC(MTR1_VV_ADC_BASE, MTR1_VV_ADC_SOC_NUM, MTR1_ADC_TRIGGER_SOC,
                 MTR1_VV_ADC_CH_NUM, MTR1_ADC_V_SAMPLEWINDOW);

    // VSEN_C_M1
    ADC_setupSOC(MTR1_VW_ADC_BASE, MTR1_VW_ADC_SOC_NUM, MTR1_ADC_TRIGGER_SOC,
                 MTR1_VW_ADC_CH_NUM, MTR1_ADC_V_SAMPLEWINDOW);
#endif  // MOTOR1_FAST

    // VSEN_DCBUS_M1-->Trig Interrupt
    ADC_setupSOC(MTR1_VDC_ADC_BASE, MTR1_VDC_ADC_SOC_NUM, MTR1_ADC_TRIGGER_SOC,
                 MTR1_VDC_ADC_CH_NUM, MTR1_ADC_V_SAMPLEWINDOW);

#if defined(CMD_POT_EN)
    // POT_M1
    ADC_setupSOC(MTR1_POT_ADC_BASE, MTR1_POT_ADC_SOC_NUM, MTR1_ADC_TRIGGER_SOC,
                 MTR1_POT_ADC_CH_NUM, MTR1_ADC_V_SAMPLEWINDOW);
#endif  // CMD_POT_EN

    return;
} // end of HAL_setupADCs() function

#if defined(BUFDAC_MODE)
void HAL_setupDACs(HAL_Handle handle)
{
    HAL_Obj *obj = (HAL_Obj *)handle;

    // AIO -> A0/B15/C15/DACA_OUT Analog mode selected
    GPIO_setAnalogMode(231, GPIO_ANALOG_ENABLED);

    // AIO -> A1/B7/DACB_OUT Analog mode selected
    GPIO_setAnalogMode(232, GPIO_ANALOG_ENABLED);

    // AIO -> B3/VDAC, Analog mode selected
    GPIO_setAnalogMode(242, GPIO_ANALOG_ENABLED);

    // Set DAC reference voltage.
    DAC_setReferenceVoltage(obj->bufDACHandle[0], DAC_REF_ADC_VREFHI);
    DAC_setReferenceVoltage(obj->bufDACHandle[1], DAC_REF_ADC_VREFHI);

    // Set DAC load mode.
    DAC_setLoadMode(obj->bufDACHandle[0], DAC_LOAD_SYSCLK);
    DAC_setLoadMode(obj->bufDACHandle[1], DAC_LOAD_SYSCLK);

    // Set the DAC Gain Mode
    DAC_setGainMode(obj->bufDACHandle[0], DAC_GAIN_TWO);
    DAC_setGainMode(obj->bufDACHandle[1], DAC_GAIN_TWO);

    // Enable the DAC output
    DAC_enableOutput(obj->bufDACHandle[0]);
    DAC_enableOutput(obj->bufDACHandle[1]);

    // Set the DAC shadow output
    DAC_setShadowValue(obj->bufDACHandle[0], 0U);
    DAC_setShadowValue(obj->bufDACHandle[1], 0U);

    // Delay for buffered DAC to power up.
    DEVICE_DELAY_US(500);

    return;
} // end of HAL_setupDACs() function
#endif  // BUFDAC_MODE

#if defined(MOTOR1_HALL) && defined(CMD_CAP_EN)
#error HALL and CMD_CAP can't be enabled at the same time
#elif defined(MOTOR1_HALL)
void HAL_setupCAPs(HAL_MTR_Handle handle)
{
    HAL_MTR_Obj    *obj = (HAL_MTR_Obj *)handle;
    uint16_t  cnt;

    for(cnt = 0; cnt < 3; cnt++)
    {
        // Disable ,clear all capture flags and interrupts
        ECAP_disableInterrupt(obj->capHandle[cnt],
                              (ECAP_ISR_SOURCE_CAPTURE_EVENT_1  |
                               ECAP_ISR_SOURCE_CAPTURE_EVENT_2  |
                               ECAP_ISR_SOURCE_CAPTURE_EVENT_3  |
                               ECAP_ISR_SOURCE_CAPTURE_EVENT_4  |
                               ECAP_ISR_SOURCE_COUNTER_OVERFLOW |
                               ECAP_ISR_SOURCE_COUNTER_PERIOD   |
                               ECAP_ISR_SOURCE_COUNTER_COMPARE));

        ECAP_clearInterrupt(obj->capHandle[cnt],
                            (ECAP_ISR_SOURCE_CAPTURE_EVENT_1  |
                             ECAP_ISR_SOURCE_CAPTURE_EVENT_2  |
                             ECAP_ISR_SOURCE_CAPTURE_EVENT_3  |
                             ECAP_ISR_SOURCE_CAPTURE_EVENT_4  |
                             ECAP_ISR_SOURCE_COUNTER_OVERFLOW |
                             ECAP_ISR_SOURCE_COUNTER_PERIOD   |
                             ECAP_ISR_SOURCE_COUNTER_COMPARE));

        // Disable CAP1-CAP4 register loads
        ECAP_disableTimeStampCapture(obj->capHandle[cnt]);

        // Configure eCAP
        //    Enable capture mode.
        //    One shot mode, stop capture at event 3.
        //    Set polarity of the events to rising, falling, rising edge.
        //    Set capture in time difference mode.
        //    Select input from XBAR4/5/6.
        //    Enable eCAP module.
        //    Enable interrupt.
        ECAP_stopCounter(obj->capHandle[cnt]);
        ECAP_enableCaptureMode(obj->capHandle[cnt]);

        // Sets eCAP in Capture mode
        ECAP_setCaptureMode(obj->capHandle[cnt], ECAP_CONTINUOUS_CAPTURE_MODE, ECAP_EVENT_3);

        // Sets the Capture event prescaler.
        ECAP_setEventPrescaler(obj->capHandle[cnt], 0U);

        // Sets a phase shift value count.
        ECAP_setPhaseShiftCount(obj->capHandle[cnt], 0U);

        // Sets the Capture event polarity
        ECAP_setEventPolarity(obj->capHandle[cnt], ECAP_EVENT_1, ECAP_EVNT_FALLING_EDGE);
        ECAP_setEventPolarity(obj->capHandle[cnt], ECAP_EVENT_2, ECAP_EVNT_RISING_EDGE);
        ECAP_setEventPolarity(obj->capHandle[cnt], ECAP_EVENT_3, ECAP_EVNT_FALLING_EDGE);
        ECAP_setEventPolarity(obj->capHandle[cnt], ECAP_EVENT_4, ECAP_EVNT_RISING_EDGE);

        // Configure counter reset on events
        ECAP_enableCounterResetOnEvent(obj->capHandle[cnt], ECAP_EVENT_1);
        ECAP_enableCounterResetOnEvent(obj->capHandle[cnt], ECAP_EVENT_2);
        ECAP_enableCounterResetOnEvent(obj->capHandle[cnt], ECAP_EVENT_3);
        ECAP_enableCounterResetOnEvent(obj->capHandle[cnt], ECAP_EVENT_4);

        // Configures emulation mode.
        ECAP_setEmulationMode(obj->capHandle[cnt], ECAP_EMULATION_FREE_RUN);

        // Enable counter loading with phase shift value.
        ECAP_enableLoadCounter(obj->capHandle[cnt]);

        // Configures Sync out signal mode.
        ECAP_setSyncOutMode(obj->capHandle[cnt], ECAP_SYNC_OUT_SYNCI);

        // Set up the source for sync-in pulse
        ECAP_setSyncInPulseSource(obj->capHandle[cnt], ECAP_SYNC_IN_PULSE_SRC_DISABLE);

        // Starts Time stamp counter
        ECAP_startCounter(obj->capHandle[cnt]);

        // Enables time stamp capture
        ECAP_enableTimeStampCapture(obj->capHandle[cnt]);

        // Re-arms the eCAP module
        ECAP_reArm(obj->capHandle[cnt]);
    }

    XBAR_setInputPin(INPUTXBAR_BASE, MTR1_CAP_U_XBAR, MTR1_HALL_U_GPIO);
    XBAR_setInputPin(INPUTXBAR_BASE, MTR1_CAP_V_XBAR, MTR1_HALL_V_GPIO);
    XBAR_setInputPin(INPUTXBAR_BASE, MTR1_CAP_W_XBAR, MTR1_HALL_W_GPIO);

    ECAP_selectECAPInput(obj->capHandle[0], MTR1_CAP_U_INSEL);
    ECAP_selectECAPInput(obj->capHandle[1], MTR1_CAP_V_INSEL);
    ECAP_selectECAPInput(obj->capHandle[2], MTR1_CAP_W_INSEL);

    return;
}   // HAL_setupCAPs()

void HAL_resetCAPTimeStamp(HAL_MTR_Handle handle)
{
    HAL_MTR_Obj    *obj = (HAL_MTR_Obj *)handle;
    uint16_t  cnt;

    for(cnt = 0; cnt < 3; cnt++)
    {
        ECAP_setAPWMPeriod(obj->capHandle[cnt], 0);
        ECAP_setAPWMCompare(obj->capHandle[cnt], 0x01FFFFFF);
        ECAP_setAPWMShadowPeriod(obj->capHandle[cnt], 0x01FFFFFF);
        ECAP_setAPWMShadowCompare(obj->capHandle[cnt], 0x01FFFFFF);
    }

    return;
}   // HAL_resetCAPTimeStamp()
#elif defined(CMD_CAP_EN)
void HAL_setupCAPs(HAL_MTR_Handle handle)
{
    HAL_MTR_Obj    *obj = (HAL_MTR_Obj *)handle;

    // Disable ,clear all capture flags and interrupts
    ECAP_disableInterrupt(obj->capHandle,
                          (ECAP_ISR_SOURCE_CAPTURE_EVENT_1  |
                           ECAP_ISR_SOURCE_CAPTURE_EVENT_2  |
                           ECAP_ISR_SOURCE_CAPTURE_EVENT_3  |
                           ECAP_ISR_SOURCE_CAPTURE_EVENT_4  |
                           ECAP_ISR_SOURCE_COUNTER_OVERFLOW |
                           ECAP_ISR_SOURCE_COUNTER_PERIOD   |
                           ECAP_ISR_SOURCE_COUNTER_COMPARE));

    ECAP_clearInterrupt(obj->capHandle,
                        (ECAP_ISR_SOURCE_CAPTURE_EVENT_1  |
                         ECAP_ISR_SOURCE_CAPTURE_EVENT_2  |
                         ECAP_ISR_SOURCE_CAPTURE_EVENT_3  |
                         ECAP_ISR_SOURCE_CAPTURE_EVENT_4  |
                         ECAP_ISR_SOURCE_COUNTER_OVERFLOW |
                         ECAP_ISR_SOURCE_COUNTER_PERIOD   |
                         ECAP_ISR_SOURCE_COUNTER_COMPARE));

    // Disable CAP1-CAP4 register loads
    ECAP_disableTimeStampCapture(obj->capHandle);

    // Configure eCAP
    //    Enable capture mode.
    //    One shot mode, stop capture at event 3.
    //    Set polarity of the events to rising, falling, rising edge.
    //    Set capture in time difference mode.
    //    Select input from XBAR4/5/6.
    //    Enable eCAP module.
    //    Enable interrupt.
    ECAP_stopCounter(obj->capHandle);
    ECAP_enableCaptureMode(obj->capHandle);

    // Sets the Capture event prescaler.
    ECAP_setCaptureMode(obj->capHandle, ECAP_CONTINUOUS_CAPTURE_MODE, ECAP_EVENT_4);

    // Sets a phase shift value count.
    ECAP_setPhaseShiftCount(obj->capHandle, 0U);

    // Sets the Capture event polarity
    ECAP_setEventPolarity(obj->capHandle, ECAP_EVENT_1, ECAP_EVNT_FALLING_EDGE);
    ECAP_setEventPolarity(obj->capHandle, ECAP_EVENT_2, ECAP_EVNT_RISING_EDGE);
    ECAP_setEventPolarity(obj->capHandle, ECAP_EVENT_3, ECAP_EVNT_FALLING_EDGE);
    ECAP_setEventPolarity(obj->capHandle, ECAP_EVENT_4, ECAP_EVNT_RISING_EDGE);

    // Configure counter reset on events
    ECAP_enableCounterResetOnEvent(obj->capHandle, ECAP_EVENT_1);
    ECAP_enableCounterResetOnEvent(obj->capHandle, ECAP_EVENT_2);
    ECAP_enableCounterResetOnEvent(obj->capHandle, ECAP_EVENT_3);
    ECAP_enableCounterResetOnEvent(obj->capHandle, ECAP_EVENT_4);

    // Configures emulation mode.
    ECAP_setEmulationMode(obj->capHandle, ECAP_EMULATION_FREE_RUN);

    // Enable counter loading with phase shift value.
    ECAP_enableLoadCounter(obj->capHandle);

    // Configures Sync out signal mode.
    ECAP_setSyncOutMode(obj->capHandle, ECAP_SYNC_OUT_SYNCI);

    // Set up the source for sync-in pulse
    ECAP_setSyncInPulseSource(obj->capHandle, ECAP_SYNC_IN_PULSE_SRC_DISABLE);

    // Starts Time stamp counter
    ECAP_startCounter(obj->capHandle);

    // Enables time stamp capture
    ECAP_enableTimeStampCapture(obj->capHandle);

    // Re-arms the eCAP module
    ECAP_reArm(obj->capHandle);

    XBAR_setInputPin(INPUTXBAR_BASE, MTR1_CAP_FREQ_XBAR, MTR1_CAP_FREQ_GPIO);

    ECAP_selectECAPInput(obj->capHandle, MTR1_CAP_FREQ_INSEL);

    return;
}

void HAL_resetCAPTimeStamp(HAL_MTR_Handle handle)
{
    HAL_MTR_Obj    *obj = (HAL_MTR_Obj *)handle;

    ECAP_setAPWMPeriod(obj->capHandle, 0);
    ECAP_setAPWMCompare(obj->capHandle, 0x01FFFFFF);
    ECAP_setAPWMShadowPeriod(obj->capHandle, 0x01FFFFFF);
    ECAP_setAPWMShadowCompare(obj->capHandle, 0x01FFFFFF);

    return;
}
#endif  // MOTOR1_HALL || CMD_CAP_EN

// HAL_setupCMPSSs
void HAL_setupCMPSSs(HAL_MTR_Handle handle)
{
    HAL_MTR_Obj *obj = (HAL_MTR_Obj *)handle;

    uint16_t cmpsaDACH = MTR1_CMPSS_DACH_VALUE;
    uint16_t cmpsaDACL = MTR1_CMPSS_DACL_VALUE;

    ASysCtl_selectCMPHPMux(MTR1_IU_CMPHP_SEL, MTR1_IU_CMPHP_MUX);
    ASysCtl_selectCMPLPMux(MTR1_IU_CMPLP_SEL, MTR1_IU_CMPLP_MUX);

    ASysCtl_selectCMPHPMux(MTR1_IV_CMPHP_SEL, MTR1_IV_CMPHP_MUX);
    ASysCtl_selectCMPLPMux(MTR1_IV_CMPLP_SEL, MTR1_IV_CMPLP_MUX);

    ASysCtl_selectCMPHPMux(MTR1_IW_CMPHP_SEL, MTR1_IW_CMPHP_MUX);
    ASysCtl_selectCMPLPMux(MTR1_IW_CMPLP_SEL, MTR1_IW_CMPLP_MUX);

    uint16_t cmpss_index;

    //---------------- U, V, W Phase HIGH CMPSS -------------------------------//
    for (cmpss_index = 0; cmpss_index < HAL_NUM_CMPSS_CURRENT; (cmpss_index += 2))
    {
        // Enable CMPSS and configure the negative input signal to come from the DAC
        CMPSS_enableModule(obj->cmpssHandle[cmpss_index]);

        // NEG signal from DAC for COMP-H
        CMPSS_configHighComparator(obj->cmpssHandle[cmpss_index], CMPSS_INSRC_DAC);

        // Configure the output signals. Both CTRIPH and CTRIPOUTH will be fed by
        // the asynchronous comparator output.
        // Dig filter output ==> CTRIPH, Dig filter output ==> CTRIPOUTH
        CMPSS_configOutputsHigh(obj->cmpssHandle[cmpss_index],
                                CMPSS_TRIP_FILTER |
                                CMPSS_TRIPOUT_FILTER);

        // Configure digital filter. For this example, the maxiumum values will be
        // used for the clock prescale, sample window size, and threshold.
        CMPSS_configFilterHigh(obj->cmpssHandle[cmpss_index], 32, 32, 30);
        CMPSS_initFilterHigh(obj->cmpssHandle[cmpss_index]);

        // Set up COMPHYSCTL register
        // COMP hysteresis set to 2x typical value
        CMPSS_setHysteresis(obj->cmpssHandle[cmpss_index], 1);

        // Use VDDA as the reference for the DAC and set DAC value to midpoint for
        // arbitrary reference
        CMPSS_configDAC(obj->cmpssHandle[cmpss_index],
                   CMPSS_DACREF_VDDA | CMPSS_DACVAL_SYSCLK | CMPSS_DACSRC_SHDW);

        // Set DAC-H to allowed MAX +ve current
        CMPSS_setDACValueHigh(obj->cmpssHandle[cmpss_index], cmpsaDACH);

        // Clear any high comparator digital filter output latch
        CMPSS_clearFilterLatchHigh(obj->cmpssHandle[cmpss_index]);
    }

    //---------------- U, V, W Phase LOW CMPSS --------------------------------//
    for (cmpss_index = 1; cmpss_index < HAL_NUM_CMPSS_CURRENT; (cmpss_index += 2))
    {
        // Enable CMPSS and configure the negative input signal to come from the DAC
        CMPSS_enableModule(obj->cmpssHandle[cmpss_index]);

        // NEG signal from DAC for COMP-L
        CMPSS_configLowComparator(obj->cmpssHandle[cmpss_index], CMPSS_INSRC_DAC);

        // Dig filter output ==> CTRIPL, Dig filter output ==> CTRIPOUTL
        CMPSS_configOutputsLow(obj->cmpssHandle[cmpss_index],
                               CMPSS_TRIP_FILTER |
                               CMPSS_TRIPOUT_FILTER |
                               CMPSS_INV_INVERTED);

        // Initialize the filter logic and start filtering
        CMPSS_configFilterLow(obj->cmpssHandle[cmpss_index], 32, 32, 30);
        CMPSS_initFilterLow(obj->cmpssHandle[cmpss_index]);

        // Set up COMPHYSCTL register
        // COMP hysteresis set to 2x typical value
        CMPSS_setHysteresis(obj->cmpssHandle[cmpss_index], 1);

        // Use VDDA as the reference for the DAC and set DAC value to midpoint for
        // arbitrary reference
        CMPSS_configDAC(obj->cmpssHandle[cmpss_index],
                   CMPSS_DACREF_VDDA | CMPSS_DACVAL_SYSCLK | CMPSS_DACSRC_SHDW);

        // Set DAC-L to allowed MAX -ve current
        CMPSS_setDACValueLow(obj->cmpssHandle[cmpss_index], cmpsaDACL);

        // Clear any low comparator digital filter output latch
        CMPSS_clearFilterLatchLow(obj->cmpssHandle[cmpss_index]);
    }

    return;
} // end of HAL_setupCMPSSs() function

// HAL_setupGate & HAL_enableDRV
#if defined(BSXL8323RS_REVA) || defined(BSXL8353RS_REVA)
void HAL_enableDRV(HAL_MTR_Handle handle)
{
    HAL_MTR_Obj *obj = (HAL_MTR_Obj *)handle;

    DRVIC_enable(obj->drvicHandle);

    DEVICE_DELAY_US(1000.0f);      // delay 1000us

    return;
}  // end of HAL_enableDRV() function

void HAL_writeDRVData(HAL_MTR_Handle handle, DRVIC_VARS_t *drvicVars)
{
    HAL_MTR_Obj  *obj = (HAL_MTR_Obj *)handle;

    DRVIC_writeData(obj->drvicHandle, drvicVars);

    return;
}  // end of HAL_writeDRVData() function

void HAL_readDRVData(HAL_MTR_Handle handle, DRVIC_VARS_t *drvicVars)
{
    HAL_MTR_Obj  *obj = (HAL_MTR_Obj *)handle;

    DRVIC_readData(obj->drvicHandle, drvicVars);

    return;
}  // end of HAL_readDRVData() function


void HAL_setupDRVSPI(HAL_MTR_Handle handle, DRVIC_VARS_t *drvicVars)
{
    HAL_MTR_Obj  *obj = (HAL_MTR_Obj *)handle;

    DRVIC_setupSPI(obj->drvicHandle, drvicVars);

    return;
}  // end of HAL_setupDRVSPI() function

void HAL_setupGate(HAL_MTR_Handle handle)
{
    HAL_MTR_Obj *obj = (HAL_MTR_Obj *)handle;

    DRVIC_setSPIHandle(obj->drvicHandle, obj->spiHandle);

#if defined(DRV_CS_GPIO)
    DRVIC_setGPIOCSNumber(obj->drvicHandle, MTR1_DRV_SPI_CS_GPIO);
#endif  // DRV_CS_GPIO
    DRVIC_setGPIOENNumber(obj->drvicHandle, MTR1_GATE_EN_GPIO);

    return;
} // HAL_setupGate() function

void HAL_setupSPI(HAL_MTR_Handle handle)
{
    HAL_MTR_Obj   *obj = (HAL_MTR_Obj *)handle;

    // Must put SPI into reset before configuring it
    SPI_disableModule(obj->spiHandle);

    // SPI configuration. Use a 500kHz SPICLK and 16-bit word size, 30MHz LSPCLK
    SPI_setConfig(obj->spiHandle, DEVICE_LSPCLK_FREQ, SPI_PROT_POL0PHA0,
                  SPI_MODE_CONTROLLER, 400000, 16);

    SPI_disableLoopback(obj->spiHandle);

    SPI_setEmulationMode(obj->spiHandle, SPI_EMULATION_FREE_RUN);

    SPI_enableFIFO(obj->spiHandle);
    SPI_setFIFOInterruptLevel(obj->spiHandle, SPI_FIFO_TX0, SPI_FIFO_RX0);
    SPI_setTxFifoTransmitDelay(obj->spiHandle, 0x10);

    SPI_clearInterruptStatus(obj->spiHandle, SPI_INT_TXFF);

    // Configuration complete. Enable the module.
    SPI_enableModule(obj->spiHandle);

    return;
}  // end of HAL_setupSPI() function

// BSXL8323RS_REVA || BSXL8353RS_REVA

#elif defined(BSXL3PHGAN_REVA)
void HAL_enableDRV(HAL_MTR_Handle handle)
{
    HAL_MTR_Obj *obj = (HAL_MTR_Obj *)handle;

    // Set EN_GATE (nEN_uC) to low for enabling the DRV
    GPIO_writePin(obj->gateEnableGPIO, 0);

    return;
} // HAL_setupGate() function
// BSXL3PHGAN_REVA
#endif  // HAL_setupGate & HAL_enableDRV

void HAL_setupMtrFaults(HAL_MTR_Handle handle)
{
    HAL_MTR_Obj *obj = (HAL_MTR_Obj *)handle;
    uint16_t cnt;

    // Configure TRIP 7 to OR the High and Low trips from both
    // comparator 5, 3 & 1, clear everything first
    EALLOW;
    HWREG(XBARA_EPWM_CFG_REG_BASE + MTR1_XBAR_TRIP_ADDRL1) = 0;
    HWREG(XBARA_EPWM_CFG_REG_BASE + MTR1_XBAR_TRIP_ADDRL2) = 0;
    HWREG(XBARA_EPWM_CFG_REG_BASE + MTR1_XBAR_TRIP_ADDRH1) = 0;
    HWREG(XBARA_EPWM_CFG_REG_BASE + MTR1_XBAR_TRIP_ADDRH2) = 0;
    EDIS;

    // Configure TRIP7 to be CTRIPxH and CTRIPxL using the ePWM X-BAR
    XBAR_setEPWMMuxConfig(MTR1_XBAR_TRIP, MTR1_IU_XBAR_EPWM_MUX_H);
    XBAR_setEPWMMuxConfig(MTR1_XBAR_TRIP, MTR1_IU_XBAR_EPWM_MUX_L);

    XBAR_setEPWMMuxConfig(MTR1_XBAR_TRIP, MTR1_IV_XBAR_EPWM_MUX_H);
    XBAR_setEPWMMuxConfig(MTR1_XBAR_TRIP, MTR1_IV_XBAR_EPWM_MUX_L);

    XBAR_setEPWMMuxConfig(MTR1_XBAR_TRIP, MTR1_IW_XBAR_EPWM_MUX_H);
    XBAR_setEPWMMuxConfig(MTR1_XBAR_TRIP, MTR1_IW_XBAR_EPWM_MUX_L);

    // Disable all the mux first
    XBAR_disableEPWMMux(MTR1_XBAR_TRIP, 0xFFFF);

    // Enable logical OR of Muxes 7, 9, 12, 58 to generate TRIP
    // p65x updated, add extra OR for IV H/L
    XBAR_enableEPWMMux(MTR1_XBAR_TRIP, MTR1_IU_XBAR_MUX_H | MTR1_IU_XBAR_MUX_L |
                                       MTR1_IV_XBAR_MUX_H | MTR1_IV_XBAR_MUX_L |
                                       MTR1_IW_XBAR_MUX_H | MTR1_IW_XBAR_MUX_L);

#if !defined(DRV8300DRGE_EVM)
    // configure the input x bar for TZ2 to GPIO, where Over Current is connected
    XBAR_setInputPin(INPUTXBAR_BASE, MTR1_XBAR_INPUT1, MTR1_PM_nFAULT_GPIO);
    XBAR_lockInput(INPUTXBAR_BASE, MTR1_XBAR_INPUT1);
#endif

    for(cnt=0;cnt<3;cnt++)
    {
        EPWM_enableTripZoneSignals(obj->pwmHandle[cnt], MTR1_TZ_OSHT1);
    }

    // Configure Trip Mechanism for the Motor control software
    // -Cycle by cycle trip on CPU halt
    // -One shot fault trip zone
    // These trips need to be repeated for EPWM1 ,2 & 3

    for(cnt=0; cnt<3; cnt++)
    {
        EPWM_enableTripZoneSignals(obj->pwmHandle[cnt],
                                   EPWM_TZ_SIGNAL_CBC6);

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
    }

    for(cnt=0; cnt<HAL_NUM_CMPSS_CURRENT; cnt++)
    {
        // Clear any high comparator digital filter output latch
        CMPSS_clearFilterLatchHigh(obj->cmpssHandle[cnt]);

        // Clear any low comparator digital filter output latch
        CMPSS_clearFilterLatchLow(obj->cmpssHandle[cnt]);
    }

    // Clear any spurious fault
    EPWM_clearTripZoneFlag(obj->pwmHandle[0], HAL_TZFLAG_INTERRUPT_ALL);
    EPWM_clearTripZoneFlag(obj->pwmHandle[1], HAL_TZFLAG_INTERRUPT_ALL);
    EPWM_clearTripZoneFlag(obj->pwmHandle[2], HAL_TZFLAG_INTERRUPT_ALL);

    return;
} // end of HAL_setupMtrFaults() function

void HAL_setupGPIOs(HAL_Handle handle)
{
    // GPIO8->EPWM5A->M1_UH*
    GPIO_setPinConfig(GPIO_8_EPWM5_A);
    GPIO_setDirectionMode(8, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(8, GPIO_PIN_TYPE_STD);

    // GPIO9->EPWM5B->M1_UH*
    GPIO_setPinConfig(GPIO_9_EPWM5_B);
    GPIO_setDirectionMode(9, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(9, GPIO_PIN_TYPE_STD);

    // GPIO6->EPWM4A->M1_VH*
    GPIO_setPinConfig(GPIO_6_EPWM4_A);
    GPIO_setDirectionMode(6, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(6, GPIO_PIN_TYPE_STD);

    // GPIO7->EPWM2B->M1_VL*
    GPIO_setPinConfig(GPIO_7_EPWM4_B);
    GPIO_setDirectionMode(7, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(7, GPIO_PIN_TYPE_STD);

    // GPIO10->EPWM6A->M1_WH*
    GPIO_setPinConfig(GPIO_10_EPWM6_A);
    GPIO_setDirectionMode(10, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(10, GPIO_PIN_TYPE_STD);

    // GPIO11->EPWM6B->M1_WL*
    GPIO_setPinConfig(GPIO_11_EPWM6_B);
    GPIO_setDirectionMode(11, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(11, GPIO_PIN_TYPE_STD);

#if defined(MOTOR1_ENC)
    // GPIO20->EQEP1_A
    GPIO_setPinConfig(GPIO_20_EQEP1_A);
    GPIO_setDirectionMode(20, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(20, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(20, GPIO_QUAL_3SAMPLE);
    GPIO_setQualificationPeriod(20, 2);

    // GPIO21->EQEP1_B
    GPIO_setPinConfig(GPIO_21_EQEP1_B);
    GPIO_setDirectionMode(21, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(21, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(21, GPIO_QUAL_3SAMPLE);
    GPIO_setQualificationPeriod(21, 2);

    // GPIO23->EQEP1_I
    GPIO_setPinConfig(GPIO_23_EQEP1_INDEX);
    GPIO_setDirectionMode(23, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(23, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(23, GPIO_QUAL_3SAMPLE);
    GPIO_setQualificationPeriod(23, 2);

#else   // !MOTOR1_ENC
    // GPIO20->Reserve
    GPIO_setPinConfig(GPIO_20_GPIO20);
    GPIO_setDirectionMode(20, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(20, GPIO_PIN_TYPE_STD);

    // GPIO21->Reserve
    GPIO_setPinConfig(GPIO_21_GPIO21);
    GPIO_setDirectionMode(21, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(21, GPIO_PIN_TYPE_STD);

    // GPIO23->Reserve
    GPIO_setPinConfig(GPIO_23_GPIO23);
    GPIO_setDirectionMode(23, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(23, GPIO_PIN_TYPE_STD);
#endif   // !MOTOR1_ENC

#if defined(MOTOR1_HALL)
    // GPIO24->ECAP1->HALL_U
    GPIO_setPinConfig(GPIO_24_GPIO24);
    GPIO_setDirectionMode(24, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(24, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(24, GPIO_QUAL_3SAMPLE);
    GPIO_setQualificationPeriod(24, 2);

    // GPIO79->ECAP2->HALL_V
    GPIO_setPinConfig(GPIO_79_GPIO79);
    GPIO_setDirectionMode(79, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(79, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(79, GPIO_QUAL_3SAMPLE);
    GPIO_setQualificationPeriod(79, 2);

    // GPIO103->ECAP3->HALL_W
    GPIO_setPinConfig(GPIO_103_GPIO103);
    GPIO_setDirectionMode(103, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(103, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(103, GPIO_QUAL_3SAMPLE);
    GPIO_setQualificationPeriod(103, 2);
#else // !MOTOR1_HALL
    // GPIO24->Reserve
    GPIO_setPinConfig(GPIO_24_GPIO24);
    GPIO_setDirectionMode(24, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(24, GPIO_PIN_TYPE_STD);

    // GPIO79->Reserve
    GPIO_setPinConfig(GPIO_79_GPIO79);
    GPIO_setDirectionMode(79, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(79, GPIO_PIN_TYPE_STD);

    // GPIO103->Reserve
    GPIO_setPinConfig(GPIO_103_GPIO103);
    GPIO_setDirectionMode(103, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(103, GPIO_PIN_TYPE_STD);
#endif   // !MOTOR1_HALL

#if defined(DAC128S_ENABLE) && defined(DAC128S_SPID)
    // GPIO94->SPID_PTE->DAC128S_SYNC
    GPIO_setPinConfig(GPIO_94_SPID_PTE);
    GPIO_setDirectionMode(94, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(94, GPIO_PIN_TYPE_STD);

    // GPIO93->SPID_CLK->DAC128S_SCLK
    GPIO_setPinConfig(GPIO_93_SPID_CLK);
    GPIO_setDirectionMode(93, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(93, GPIO_PIN_TYPE_STD);

    // GPIO91->SPID_PICO->DAC128S_SDI
    GPIO_setPinConfig(GPIO_91_SPID_PICO);
    GPIO_setDirectionMode(91, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(91, GPIO_PIN_TYPE_STD);

    // GPIO92->SPID_POCI->DAC128S_SDO
    GPIO_setPinConfig(GPIO_92_SPID_POCI);
    GPIO_setDirectionMode(92, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(92, GPIO_PIN_TYPE_STD);
#else
    // GPIO94->Reserve
    GPIO_setPinConfig(GPIO_94_GPIO94);
    GPIO_setDirectionMode(94, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(94, GPIO_PIN_TYPE_STD);

    // GPIO93->Reserve GPIO
    GPIO_setPinConfig(GPIO_93_GPIO93);
    GPIO_setDirectionMode(93, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(93, GPIO_PIN_TYPE_STD);

    // GPIO91->Reserve
    GPIO_setPinConfig(GPIO_91_GPIO91);
    GPIO_setDirectionMode(91, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(91, GPIO_PIN_TYPE_STD);

    // GPIO92->Reserve
    GPIO_setPinConfig(GPIO_92_GPIO92);
    GPIO_setDirectionMode(92, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(92, GPIO_PIN_TYPE_STD);
#endif  // DAC128S_ENABLE && DAC128S_SPID

#if defined(CMD_CAN_EN)
    // GPIO4->CAN_TX
    GPIO_setPinConfig(GPIO_4_CANA_TX);
    GPIO_setDirectionMode(4, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(4, GPIO_PIN_TYPE_STD);

    // GPIO5->CAN_RX
    GPIO_setPinConfig(GPIO_5_CANA_RX);
    GPIO_setDirectionMode(5, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(5, GPIO_PIN_TYPE_STD);
#else   // !CMD_CAN_EN
    // GPIO4->Reserve
    GPIO_setPinConfig(GPIO_4_GPIO4);
    GPIO_setDirectionMode(4, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(4, GPIO_PIN_TYPE_STD);

    // GPIO5->Reserve
    GPIO_setPinConfig(GPIO_5_GPIO5);
    GPIO_setDirectionMode(5, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(5, GPIO_PIN_TYPE_STD);
#endif  // !CMD_CAN_EN

#if defined(SFRA_ENABLE)
    // GPIO55->SCIB_RX for SFRA
    GPIO_setPinConfig(GPIO_55_SCIB_RX);
    GPIO_setDirectionMode(55, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(55, GPIO_PIN_TYPE_STD);

    // GPIO38->SCIB_TX for SFRA
    GPIO_setPinConfig(GPIO_38_SCIB_TX);
    GPIO_setDirectionMode(38, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(38, GPIO_PIN_TYPE_STD);
#else  // !SFRA_ENABLE
    // GPIO55->Reserve
    GPIO_setPinConfig(GPIO_55_GPIO55);
    GPIO_setDirectionMode(55, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(55, GPIO_PIN_TYPE_STD);

    // GPIO38->Reserve
    GPIO_setPinConfig(GPIO_38_GPIO38);
    GPIO_setDirectionMode(38, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(38, GPIO_PIN_TYPE_STD);
#endif // !SFRA_ENABLE

#if defined(BSXL8323RS_REVA) || defined(BSXL8353RS_REVA)

#if defined(DRV_CS_GPIO)
    // GPIO23->M1_DRV_nSCS(GPIO Mode)
    GPIO_setPinConfig(GPIO_23_GPIO23);
    GPIO_writePin(23, 1);
    GPIO_setDirectionMode(23, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(23, GPIO_PIN_TYPE_STD);
#endif

    // GPIO72->M1_DRV_nFAULT*
    GPIO_setPinConfig(GPIO_72_GPIO72);
    GPIO_setDirectionMode(72, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(72, GPIO_PIN_TYPE_PULLUP);

    // GPIO42->M1_DRV_ENABLE*
    GPIO_setPinConfig(GPIO_42_GPIO42);
    GPIO_writePin(42, 1);
    GPIO_setDirectionMode(42, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(42, GPIO_PIN_TYPE_PULLUP);

    // GPIO14->M1_DRV_MODE->6-PWM-Mode (->GND)
    GPIO_setPinConfig(GPIO_14_GPIO14);
    GPIO_writePin(14, 0);
    GPIO_setDirectionMode(14, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(14, GPIO_PIN_TYPE_STD);

    // GPIO15->M1_LED
    GPIO_setPinConfig(GPIO_15_GPIO15);
    GPIO_writePin(15, 0);
    GPIO_setDirectionMode(15, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(15, GPIO_PIN_TYPE_STD);

    // GPIO17->SPIA_POCI->M1_DRV_SDO
    GPIO_setPinConfig(GPIO_17_SPIA_POCI);
    GPIO_setDirectionMode(17, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(17, GPIO_PIN_TYPE_STD);

    // GPIO16->SPIA_PICO->M1_DRV_SDI
    GPIO_setPinConfig(GPIO_16_SPIA_PICO);
    GPIO_setDirectionMode(16, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(16, GPIO_PIN_TYPE_STD);

    // GPIO57->M1_DRV_SCS->Use a Jumper connect to J2-12
    GPIO_setPinConfig(GPIO_57_SPIA_PTE);
    GPIO_setDirectionMode(57, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(57, GPIO_PIN_TYPE_PULLUP);

    // GPIO18->SPIA_CLK->M1_DRV_SCLK
    GPIO_setPinConfig(GPIO_18_SPIA_CLK);
    GPIO_setDirectionMode(18, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(18, GPIO_PIN_TYPE_STD);

    // end of BSXL8323RS_REVA || BSXL8353RS_REVA
//-----------------------------------------------------------------------------
#elif defined(BSXL3PHGAN_REVA)
    // GPIO12-> nEN_uC (site 1)
    GPIO_setPinConfig(GPIO_12_GPIO12);
    GPIO_writePin(12, 1);
    GPIO_setDirectionMode(12, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(12, GPIO_PIN_TYPE_PULLUP);

    // GPIO14->OT (Site 1)
    GPIO_setPinConfig(GPIO_14_GPIO14);
    GPIO_setDirectionMode(14, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(14, GPIO_PIN_TYPE_PULLUP);
    GPIO_setQualificationMode(14, GPIO_QUAL_3SAMPLE);
    GPIO_setQualificationPeriod(14, 2);

//------------------------------------------------------------------------------
#elif defined(DRV8300DRGE_EVM)
    // Fill as needed...
#else
#error The GPIOs is not configured for motor control board
#endif

    return;
}  // end of HAL_setupGPIOs() function

void HAL_setupPWMs(HAL_MTR_Handle handle)
{
    HAL_MTR_Obj    *obj = (HAL_MTR_Obj *)handle;
    uint16_t  cnt;

    uint16_t pwmPeriodCycles = (uint16_t)(USER_M1_PWM_TBPRD_NUM);
    uint16_t numPWMTicksPerISRTick = USER_M1_NUM_PWM_TICKS_PER_ISR_TICK;

    // disable the ePWM module time base clock sync signal
    // to synchronize all of the PWMs
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);

    // Set EPWM CLK divider to /1
    SysCtl_setEPWMClockDivider(SYSCTL_EPWMCLK_DIV_1);

    // turns off the outputs of the EPWM peripherals which will put the power
    // switches into a high impedance state.
    EPWM_forceTripZoneEvent(obj->pwmHandle[0], EPWM_TZ_FORCE_EVENT_OST);
    EPWM_forceTripZoneEvent(obj->pwmHandle[1], EPWM_TZ_FORCE_EVENT_OST);
    EPWM_forceTripZoneEvent(obj->pwmHandle[2], EPWM_TZ_FORCE_EVENT_OST);

#if defined(BSXL8323RS_REVA) || defined(BSXL8353RS_REVA) || \
    defined(BSXL3PHGAN_REVA) || defined(DRV8300DRGE_EVM)

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
                                      EPWM_AQ_OUTPUT_HIGH,
                                      EPWM_AQ_OUTPUT_ON_TIMEBASE_PERIOD);

        EPWM_setActionQualifierAction(obj->pwmHandle[cnt],
                                      EPWM_AQ_OUTPUT_A,
                                      EPWM_AQ_OUTPUT_LOW,
                                      EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);

        EPWM_setActionQualifierAction(obj->pwmHandle[cnt],
                                      EPWM_AQ_OUTPUT_A,
                                      EPWM_AQ_OUTPUT_LOW,
                                      EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);

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

    // BSXL8323RS_REVA || BSXL8353RS_REVA || \
    // BSXL3PHGAN_REVA
#else
#error The PWM is not configured for motor_1 control
#endif  // boards

    // setup the Event Trigger Selection Register (ETSEL)
    EPWM_setInterruptSource(obj->pwmHandle[0], EPWM_INT_TBCTR_ZERO);

    EPWM_enableInterrupt(obj->pwmHandle[0]);

    EPWM_setADCTriggerSource(obj->pwmHandle[0],
                             EPWM_SOC_A, EPWM_SOC_TBCTR_D_CMPC);

    EPWM_enableADCTrigger(obj->pwmHandle[0], EPWM_SOC_A);

    // setup the Event Trigger Prescale Register (ETPS)
    if(numPWMTicksPerISRTick > 15)
    {
        numPWMTicksPerISRTick = 15;
    }
    else if(numPWMTicksPerISRTick < 1)
    {
        numPWMTicksPerISRTick = 1;
    }

    EPWM_setInterruptEventCount(obj->pwmHandle[0], numPWMTicksPerISRTick);

    EPWM_setADCTriggerEventPrescale(obj->pwmHandle[0], EPWM_SOC_A,
                                    numPWMTicksPerISRTick);
    EPWM_setADCTriggerEventPrescale(obj->pwmHandle[0], EPWM_SOC_B,
                                    numPWMTicksPerISRTick);

    // setup the Event Trigger Clear Register (ETCLR)
    EPWM_clearEventTriggerInterruptFlag(obj->pwmHandle[0]);
    EPWM_clearADCTriggerFlag(obj->pwmHandle[0], EPWM_SOC_A);
    EPWM_clearADCTriggerFlag(obj->pwmHandle[0], EPWM_SOC_B);

    // since the PWM is configured as an up/down counter, the period register is
    // set to one-half of the desired PWM period
    EPWM_setTimeBasePeriod(obj->pwmHandle[0], pwmPeriodCycles);
    EPWM_setTimeBasePeriod(obj->pwmHandle[1], pwmPeriodCycles);
    EPWM_setTimeBasePeriod(obj->pwmHandle[2], pwmPeriodCycles);

    // write the PWM data value  for ADC trigger
    EPWM_setCounterCompareValue(obj->pwmHandle[0], EPWM_COUNTER_COMPARE_C, 10);

    // sync "down-stream"
    EPWM_enableSyncOutPulseSource(obj->pwmHandle[0], EPWM_SYNC_OUT_PULSE_ON_CNTR_ZERO);

    return;
}  // end of HAL_setupPWMs() function

#if defined(MOTOR1_ENC)
void HAL_setupQEP(HAL_MTR_Handle handle)
{
    HAL_MTR_Obj   *obj = (HAL_MTR_Obj *)handle;

    // Configure the decoder for quadrature count mode, counting both
    // rising and falling edges (that is, 2x resolution), QDECCTL
    EQEP_setDecoderConfig(obj->qepHandle, (EQEP_CONFIG_2X_RESOLUTION |
                                           EQEP_CONFIG_QUADRATURE |
                                           EQEP_CONFIG_NO_SWAP) );

    EQEP_setEmulationMode(obj->qepHandle, EQEP_EMULATIONMODE_RUNFREE);

    // Configure the position counter to be latched on a unit time out
    // and latch on rising edge of index pulse
    EQEP_setLatchMode(obj->qepHandle, (EQEP_LATCH_RISING_INDEX |
                                       EQEP_LATCH_UNIT_TIME_OUT) );

    EQEP_setPositionCounterConfig(obj->qepHandle, EQEP_POSITION_RESET_MAX_POS,
                                  (uint32_t)(USER_MOTOR1_ENC_POS_MAX));

#if defined(ENC_UVW)
    EQEP_setInitialPosition(obj->qepHandle, USER_MOTOR1_ENC_POS_OFFSET);

    EQEP_setPositionInitMode(obj->qepHandle, EQEP_INIT_RISING_INDEX);
#endif

    // Enable the unit timer, setting the frequency to 10KHz
    // QUPRD, QEPCTL
    EQEP_enableUnitTimer(obj->qepHandle, (USER_M1_QEP_UNIT_TIMER_TICKS - 1));

    // Disables the eQEP module position-compare unit
    EQEP_disableCompare(obj->qepHandle);

    // Configure and enable the edge-capture unit. The capture clock divider is
    // SYSCLKOUT/128. The unit-position event divider is QCLK/32.
    EQEP_setCaptureConfig(obj->qepHandle, EQEP_CAPTURE_CLK_DIV_128,
                                          EQEP_UNIT_POS_EVNT_DIV_32);

    // Enable QEP edge-capture unit
    EQEP_enableCapture(obj->qepHandle);

    // Enable UTO on QEP
    EQEP_enableInterrupt(obj->qepHandle, EQEP_INT_UNIT_TIME_OUT);

    // Enable the eQEP module
    EQEP_enableModule(obj->qepHandle);

    return;
}
#endif  // MOTOR1_ENC

void HAL_setupSCIA(HAL_Handle halHandle)
{
    HAL_Obj *obj = (HAL_Obj *)halHandle;

    // Initialize SCIA and its FIFO.
    SCI_performSoftwareReset(obj->sciHandle);

    // Configure SCIA for echoback.
    SCI_setConfig(obj->sciHandle, DEVICE_LSPCLK_FREQ, 9600,
                        ( SCI_CONFIG_WLEN_8 |
                          SCI_CONFIG_STOP_ONE |
                          SCI_CONFIG_PAR_NONE ) );

    SCI_resetChannels(obj->sciHandle);

    SCI_resetRxFIFO(obj->sciHandle);

    SCI_resetTxFIFO(obj->sciHandle);

    SCI_clearInterruptStatus(obj->sciHandle, SCI_INT_TXFF | SCI_INT_RXFF);

    SCI_enableFIFO(obj->sciHandle);

    SCI_enableModule(obj->sciHandle);

    SCI_performSoftwareReset(obj->sciHandle);

    return;
}  // end of DRV_setupSci() function

void HAL_setupI2CA(HAL_Handle halHandle)
{
    HAL_Obj *obj = (HAL_Obj *)halHandle;

    // Must put I2C into reset before configuring it
    I2C_disableModule(obj->i2cHandle);

    // I2C configuration. Use a 400kHz I2CCLK with a 50% duty cycle.
    I2C_initController(obj->i2cHandle, DEVICE_SYSCLK_FREQ, 400000, I2C_DUTYCYCLE_50);
    I2C_setConfig(obj->i2cHandle, I2C_CONTROLLER_SEND_MODE);
    I2C_setTargetAddress(obj->i2cHandle, I2C_TARGET_ADDRESS);
    I2C_disableLoopback(obj->i2cHandle);
    I2C_setBitCount(obj->i2cHandle, I2C_BITCOUNT_8);
    I2C_setDataCount(obj->i2cHandle, 2);
    I2C_setAddressMode(obj->i2cHandle, I2C_ADDR_MODE_7BITS);

    // Enable stop condition and register-access-ready interrupts
    I2C_enableInterrupt(obj->i2cHandle, I2C_INT_ADDR_TARGET |
                                        I2C_INT_ARB_LOST |
                                        I2C_INT_NO_ACK |
                                        I2C_INT_STOP_CONDITION);

    // FIFO configuration
    I2C_enableFIFO(obj->i2cHandle);
    I2C_setFIFOInterruptLevel(obj->i2cHandle, I2C_FIFO_TXEMPTY, I2C_FIFO_RX2);

//    I2C_clearInterruptStatus(obj->i2cHandle, I2C_INT_RXFF | I2C_INT_TXFF);
    I2C_clearInterruptStatus(obj->i2cHandle, I2C_INT_ARB_LOST | I2C_INT_NO_ACK);

    // Configuration complete. Enable the module.
    I2C_setEmulationMode(obj->i2cHandle, I2C_EMULATION_FREE_RUN);
    I2C_enableModule(obj->i2cHandle);

    return;
}  // end of HAL_setupI2CA() function

void HAL_setupTimeBaseTimer(HAL_Handle handle, const float32_t timeBaseFreq_Hz)
{
    HAL_Obj  *obj = (HAL_Obj *)handle;

    uint32_t timerPeriod = (uint32_t)((USER_SYSTEM_FREQ_MHz * 1000.0f *1000.0f) /
                                      timeBaseFreq_Hz);

    // use timer 0 for CPU usage diagnostics
    CPUTimer_setPreScaler(obj->timerHandle[0], 0);

    CPUTimer_setEmulationMode(obj->timerHandle[0],
                              CPUTIMER_EMULATIONMODE_RUNFREE);

    CPUTimer_setPeriod(obj->timerHandle[0], timerPeriod);

    CPUTimer_startTimer(obj->timerHandle[0]);

    return;
}  // end of HAL_setupTimeBaseTimer() function

void HAL_setupADCTriggerTimer(HAL_Handle handle, const float32_t adcTriggerFreq_Hz)
{
    HAL_Obj  *obj = (HAL_Obj *)handle;

    uint32_t timerPeriod = (uint32_t)((USER_SYSTEM_FREQ_MHz * 1000.0f *1000.0f) /
                                      adcTriggerFreq_Hz);

    // use timer 1 for ADC trigger
    CPUTimer_setPreScaler(obj->timerHandle[1], 0);

    CPUTimer_setEmulationMode(obj->timerHandle[1],
                              CPUTIMER_EMULATIONMODE_RUNFREE);

    CPUTimer_setPeriod(obj->timerHandle[1], timerPeriod);

    CPUTimer_enableInterrupt(obj->timerHandle[1]);

    CPUTimer_startTimer(obj->timerHandle[1]);

    return;
}  // end of HAL_setupADCTriggerTimer() function

void HAL_setupCPUUsageTimer(HAL_Handle handle)
{
    HAL_Obj  *obj = (HAL_Obj *)handle;

    // use timer 2 for CPU usage diagnostics
    CPUTimer_setPreScaler(obj->timerHandle[2], 0);

    CPUTimer_setEmulationMode(obj->timerHandle[2],
                              CPUTIMER_EMULATIONMODE_RUNFREE);

    CPUTimer_setPeriod(obj->timerHandle[2], 0xFFFFFFFF);

    CPUTimer_startTimer(obj->timerHandle[2]);

    return;
}  // end of HAL_setupCPUUsageTimer() function

#if defined(DATALOGF4_EN) || defined(DATALOGF2_EN)
void HAL_setupDMA(void)
{
    // Initializes the DMA controller to a known state
    DMA_initController();

    // Sets DMA emulation mode, Continue DMA operation
    DMA_setEmulationMode(DMA_EMULATION_FREE_RUN);

    return;
}    //end of HAL_setupDMA() function

void HAL_setupDMAforDLOG(HAL_Handle handle, const uint16_t dmaChNum,
                          const void *destAddr, const void *srcAddr)
{
    HAL_Obj *obj = (HAL_Obj *)handle;
    DMA_configAddresses(obj->dmaChHandle[dmaChNum], destAddr, srcAddr);

    // configure DMA Channel
    DMA_configBurst(obj->dmaChHandle[dmaChNum], DLOG_BURST_SIZE, 2, 2);
    DMA_configTransfer(obj->dmaChHandle[dmaChNum], DLOG_TRANSFER_SIZE, 1, 1);
    DMA_configWrap(obj->dmaChHandle[dmaChNum], 0xFFFF, 0, 0xFFFF, 0);

    DMA_configMode(obj->dmaChHandle[dmaChNum], DMA_TRIGGER_SOFTWARE,
                   DMA_CFG_ONESHOT_ENABLE | DMA_CFG_CONTINUOUS_ENABLE |
                   DMA_CFG_SIZE_32BIT);

    DMA_setInterruptMode(obj->dmaChHandle[dmaChNum], DMA_INT_AT_END);
    DMA_enableTrigger(obj->dmaChHandle[dmaChNum]);
    DMA_disableInterrupt(obj->dmaChHandle[dmaChNum]);

    return;
}    //end of HAL_setupDMAforDLOG() function
#endif  // DATALOGF4_EN || DATALOGF2_EN

void HAL_clearDataRAM(void *pMemory, uint16_t lengthMemory)
{
    uint16_t *pMemoryStart;
    uint16_t loopCount, loopLength;

    pMemoryStart = pMemory;
    loopLength = lengthMemory;

    for(loopCount = 0; loopCount < loopLength; loopCount++)
    {
        *(pMemoryStart + loopCount) = 0x0000;
    }
}   //end of HAL_clearDataRAM() function

void HAL_setMtrCMPSSDACValue(HAL_MTR_Handle handle,
                             const uint16_t dacValH, const uint16_t dacValL)
{
    HAL_MTR_Obj *obj = (HAL_MTR_Obj *)handle;

    CMPSS_setDACValueHigh(obj->cmpssHandle[0], dacValH);
    CMPSS_setDACValueLow(obj->cmpssHandle[1], dacValL);

    CMPSS_setDACValueHigh(obj->cmpssHandle[2], dacValH);
    CMPSS_setDACValueLow(obj->cmpssHandle[3], dacValL);

    CMPSS_setDACValueHigh(obj->cmpssHandle[4], dacValH);
    CMPSS_setDACValueLow(obj->cmpssHandle[5], dacValL);

    return;
}   // end of HAL_setMtrCMPSSDACValue() function

void HAL_setTriggerPrams(HAL_PWMData_t *pPWMData, const float32_t systemFreq_MHz,
                   const float32_t deadband_us, const float32_t noiseWindow_us,
                   const float32_t adcSample_us)
{
    uint16_t deadband =  (uint16_t)(deadband_us * systemFreq_MHz);
    uint16_t noiseWindow =  (uint16_t)(noiseWindow_us * systemFreq_MHz);
    uint16_t adcSample =  (uint16_t)(adcSample_us * systemFreq_MHz);

    pPWMData->deadband = deadband;
    pPWMData->noiseWindow = noiseWindow;
    pPWMData->adcSample = adcSample;

    pPWMData->minCMPValue = deadband + noiseWindow + adcSample;

    return;
}   // end of HAL_setTriggerPrams() function

bool HAL_MTR_setGateDriver(HAL_MTR_Handle handle)
{
    bool driverStatus = false;

    SysCtl_delay(5000U);

    // Setup Gate Enable
#if defined(BSXL8323RS_REVA)
    // enable DRV8323RS
    HAL_setupGate(handle);
    SysCtl_delay(1000U);

    // turn on the DRV8323RS
    HAL_enableDRV(handle);
    SysCtl_delay(1000U);

    // initialize the DRV8323RS interface
    HAL_setupDRVSPI(handle, &drvicVars_M1);

    SysCtl_delay(1000U);

    drvicVars_M1.ctrlReg02.bit.OTW_REP = true;
    drvicVars_M1.ctrlReg02.bit.PWM_MODE = DRV8323_PWMMODE_6;

    drvicVars_M1.ctrlReg05.bit.VDS_LVL = DRV8323_VDS_LEVEL_1P700_V;
    drvicVars_M1.ctrlReg05.bit.OCP_MODE = DRV8323_AUTOMATIC_RETRY;
    drvicVars_M1.ctrlReg05.bit.DEAD_TIME = DRV8323_DEADTIME_100_NS;

    drvicVars_M1.ctrlReg06.bit.CSA_GAIN = DRV8323_Gain_10VpV;         // ADC_FULL_SCALE_CURRENT = 47.14285714A
//    drvicVars_M1.ctrlReg06.bit.CSA_GAIN = DRV8323_Gain_20VpV;       // ADC_FULL_SCALE_CURRENT = 23.57142857A
    drvicVars_M1.ctrlReg06.bit.LS_REF = false;
    drvicVars_M1.ctrlReg06.bit.VREF_DIV = true;
    drvicVars_M1.ctrlReg06.bit.CSA_FET = false;

    drvicVars_M1.writeCmd = 1;
    HAL_writeDRVData(handle, &drvicVars_M1);
    SysCtl_delay(1000U);

    drvicVars_M1.writeCmd = 1;
    HAL_writeDRVData(handle, &drvicVars_M1);
    SysCtl_delay(1000U);

    // BSXL8323RS_REVA
#elif defined(BSXL8353RS_REVA)
    // enable DRV8353RS
    HAL_setupGate(handle);
    SysCtl_delay(1000U);

    // turn on the DRV8353RS
    HAL_enableDRV(handle);
    SysCtl_delay(1000U);

    // initialize the DRV8353RS interface
    HAL_setupDRVSPI(handle, &drvicVars_M1);
    SysCtl_delay(1000U);

    drvicVars_M1.ctrlReg03.bit.IDRIVEP_HS = DRV8353_ISOUR_HS_0P820_A;
    drvicVars_M1.ctrlReg03.bit.IDRIVEN_HS = DRV8353_ISINK_HS_1P640_A;

    drvicVars_M1.ctrlReg04.bit.IDRIVEP_LS = DRV8353_ISOUR_LS_0P820_A;
    drvicVars_M1.ctrlReg04.bit.IDRIVEN_LS = DRV8353_ISINK_LS_1P640_A;

    drvicVars_M1.ctrlReg05.bit.VDS_LVL = DRV8353_VDS_LEVEL_1P500_V;
    drvicVars_M1.ctrlReg05.bit.OCP_MODE = DRV8353_LATCHED_SHUTDOWN;
    drvicVars_M1.ctrlReg05.bit.DEAD_TIME = DRV8353_DEADTIME_100_NS;

    drvicVars_M1.ctrlReg06.bit.CSA_GAIN = DRV8353_Gain_10VpV;
    drvicVars_M1.ctrlReg06.bit.LS_REF = false;
    drvicVars_M1.ctrlReg06.bit.VREF_DIV = true;
    drvicVars_M1.ctrlReg06.bit.CSA_FET = false;

    // write DRV8353RS control registers
    drvicVars_M1.writeCmd = 1;
    HAL_writeDRVData(handle, &drvicVars_M1);
    SysCtl_delay(1000U);

    // write DRV8353RS control registers again
    drvicVars_M1.writeCmd = 1;
    HAL_writeDRVData(handle, &drvicVars_M1);
    SysCtl_delay(1000U);

    // BSXL8353RS_REVA
#elif defined(BSXL3PHGAN_REVA)
    // turn on the 3PhGaN if present
    HAL_enableDRV(handle);
    SysCtl_delay(1000U);

    // BSXL3PHGAN_REVA
#endif  // Setup Gate Enable

    return(driverStatus);
}


// end of file
