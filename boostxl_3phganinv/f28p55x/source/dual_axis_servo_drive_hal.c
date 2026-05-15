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

//------------------------------------------------------------------------------
//  Software:       Motor Control SDK
//
// FILE:    dual_axis_servo_drive_hal.c
//
// TITLE:   define initialize the handle functions of device
//
// Group:   C2000
//
// Target Family: F28P55x
//
//-----------------------------------------------------------------------------

//
// includes
//
#include "dual_axis_servo_drive_settings.h"
#include "dual_axis_servo_drive_hal.h"

#include "dual_axis_servo_drive.h"

#include "stdbool.h"
#include "stdint.h"

//
// SD Trip Level - scope for additional work
//
uint16_t hlt = 0x7FFF;
uint16_t llt = 0x0;

//
// These are defined by the linker file
//
extern uint32_t Cla1funcsLoadStart;
extern uint32_t Cla1funcsLoadSize;
extern uint32_t Cla1funcsRunStart;

extern uint32_t Cla1ConstRunStart;
extern uint32_t Cla1ConstLoadStart;
extern uint32_t Cla1ConstLoadSize;

//
// interrupt routines for CPU
//
extern __interrupt void motor1ControlISR(void);
extern __interrupt void motor2ControlISR(void);

//
// tasks 1-4 are owned by the FCL for motor 1
//
extern __interrupt void Cla1Task1(void);
extern __interrupt void Cla1Task2(void);
extern __interrupt void Cla1Task3(void);
extern __interrupt void Cla1Task4(void);

//
// tasks 5-8 are owned by the FCL for motor 2
//
extern __interrupt void Cla1Task5(void);
extern __interrupt void Cla1Task6(void);
extern __interrupt void Cla1Task7(void);
extern __interrupt void Cla1Task8(void);

//
// Enables interrupts
//
void HAL_enableInterrupts(HAL_MTR_Handle handle)
{
    HAL_MTR_Obj *obj = (HAL_MTR_Obj *)handle;

    // clear pending INT event
    EPWM_clearEventTriggerInterruptFlag(obj->pwmHandle[0]);

    if(handle == &halMtr[MTR_1])
    {
        // Enable PWM1INT in PIE group 3
        Interrupt_enable(M1_INT_PWM);
    }
    else if(handle == &halMtr[MTR_2])
    {
        Interrupt_enable(M2_INT_PWM);        // Enable PWM1INT in PIE group 3
    }

    // Enable group 3 interrupts - EPWM1 is here
    Interrupt_enableInCPU(INTERRUPT_CPU_INT3);

    return;
}

//
// initialize the handles
//
HAL_Handle HAL_init(void *pMemory, const size_t numBytes)
{
    HAL_Handle handle;
    HAL_Obj *obj;

    if(numBytes < sizeof(HAL_Obj))
    {
        return((HAL_Handle)NULL);
    }

    //
    // assign the handle
    //
    handle = (HAL_Handle)pMemory;

    //
    // assign the object
    //
    obj = (HAL_Obj *)handle;

    //
    // disable watchdog
    //
    SysCtl_disableWatchdog();

    //
    // initialize the DAC handles
    //
    obj->dacHandle[0] = DACA_BASE;

    //
    // initialize CLA handle
    //
    obj->claHandle = CLA1_BASE;

    //
    // initialize PGA handle
    //
    obj->pgaHandle[0] = PGA1_BASE;
    obj->pgaHandle[0] = PGA2_BASE;
    obj->pgaHandle[0] = PGA3_BASE;

    //
    // initialize SCI handle
    //
    obj->sciHandle[0] = SCIA_BASE;
    obj->sciHandle[1] = SCIB_BASE;

    //
    // initialize timer handles
    //
    obj->timerHandle[0] = CPUTIMER0_BASE;
    obj->timerHandle[1] = CPUTIMER1_BASE;
    obj->timerHandle[2] = CPUTIMER2_BASE;

    //
    // initialize ADC handles
    //
    obj->adcHandle[0] = ADCA_BASE;
    obj->adcHandle[1] = ADCB_BASE;
    obj->adcHandle[2] = ADCC_BASE;
    obj->adcHandle[3] = ADCD_BASE;
    obj->adcHandle[4] = ADCE_BASE;

    //
    // initialize the ADC results
    //
    obj->adcResult[0] = ADCARESULT_BASE;
    obj->adcResult[1] = ADCBRESULT_BASE;
    obj->adcResult[2] = ADCCRESULT_BASE;
    obj->adcResult[3] = ADCDRESULT_BASE;
    obj->adcResult[4] = ADCERESULT_BASE;

    return(handle);
} // end of HAL_init() function

//
// Initializes the hardware abstraction layer (HAL) object for motors
//
HAL_MTR_Handle HAL_MTR_init(void *pMemory, const size_t numBytes)
{
    HAL_MTR_Handle handle;
    HAL_MTR_Obj *obj;

    if(numBytes < sizeof(HAL_MTR_Obj))
    {
        return((HAL_MTR_Handle)NULL);
    }

    //
    // assign the handle
    //
    handle = (HAL_MTR_Handle)pMemory;

    //
    // assign the object
    //
    obj = (HAL_MTR_Obj *)handle;

    if(handle == &halMtr[MTR_1])
    {
        //
        // initialize SPI handle
        //
        obj->spiHandle = M1_SPI_BASE;

        //
        // initialize PWM handles for motor_1
        //
        obj->pwmHandle[0] = M1_U_PWM_BASE;
        obj->pwmHandle[1] = M1_V_PWM_BASE;
        obj->pwmHandle[2] = M1_W_PWM_BASE;

        //
        // initialize CMPSS handle
        //
        obj->cmpssHandle[0] = M1_U_CMPSS_BASE;
        obj->cmpssHandle[1] = M1_V_CMPSS_BASE;
        obj->cmpssHandle[2] = M1_W_CMPSS_BASE;

        // initialize QEP driver
        obj->qepHandle = M1_QEP_BASE;
    }
    else if(handle == &halMtr[MTR_2])
    {
        //
        // initialize SPI handle
        //
        obj->spiHandle = M2_SPI_BASE;

        //
        // initialize PWM handles for motor_2
        //
        obj->pwmHandle[0] = M2_U_PWM_BASE;
        obj->pwmHandle[1] = M2_V_PWM_BASE;
        obj->pwmHandle[2] = M2_W_PWM_BASE;

        //
        // initialize CMPSS handle for motor_2
        //
        obj->cmpssHandle[0] = M2_U_CMPSS_BASE;
        obj->cmpssHandle[1] = M2_V_CMPSS_BASE;
        obj->cmpssHandle[2] = M2_W_CMPSS_BASE;

        //
        // initialize QEP driver
        //
        obj->qepHandle = M2_QEP_BASE;
    }

     return(handle);
} // end of HAL_MTR_init() function

//
// sets the HAL parameters for motor
//
void HAL_setMotorParams(HAL_MTR_Handle handle)
{
    //
    // setup the PWMs
    //
    HAL_setupMotorPWMs(handle);

    //
    // setup the CMPSS
    //
    HAL_setupCMPSS(handle);

    //
    // setup the eqep
    //
    HAL_setupQEP(handle);

    return;
}


//
// sets the HAL parameters
//
void HAL_setParams(HAL_Handle handle)
{

    HAL_Obj *obj = (HAL_Obj *)handle;

    //
    // Make sure the LSPCLK divider is set to divide by 2
    //
    SysCtl_setLowSpeedClock(SYSCTL_LSPCLK_PRESCALE_2); // 75MHz for SFRA

    //
    // Disable pin locks and enable internal pullups.
    //
    Device_initGPIO();

    //
    // Initialize the PIE control registers to their default state.
    // The default state is all PIE interrupts disabled and flags
    // are cleared.
    //
    Interrupt_initModule();

    //
    // Sets up the CLA
    //
    HAL_setupCLA(handle);


    //
    // Clear all interrupts and initialize PIE vector table:
    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    // This will populate the entire table, even if the interrupt
    // is not used in this example.  This is useful for debug purposes.
    //
    Interrupt_initVectorTable();

    //
    // Timing sync for background loops
    //
    HAL_setupCpuTimer(obj->timerHandle[0], MICROSEC_50);    // A tasks
    HAL_setupCpuTimer(obj->timerHandle[1], MICROSEC_100);   // B tasks
    HAL_setupCpuTimer(obj->timerHandle[2], MICROSEC_150);   // C tasks

    //
    // Sets up the GPIO (General Purpose I/O) pins
    //
    HAL_setupGPIOs(handle);

#ifdef DACOUT_EN
    //
    // Sets up the DAC
    //
    HAL_setupDACs(handle);
#endif

    //
    // Sets up the ADC
    //
    HAL_setupADCs(handle);



    return;
}


//
// Configure ADC
//
void HAL_setupADCs(HAL_Handle handle)
{
    HAL_Obj *obj = (HAL_Obj *)handle;

    uint16_t cnt;

    EALLOW;

    // Configure GPIO 11, 12, 13, 16, 17, 20, 21, 24, 28, and 33 as digital pins
    GPIO_setAnalogMode(11U, GPIO_ANALOG_DISABLED);
    GPIO_setAnalogMode(12U, GPIO_ANALOG_DISABLED);
    GPIO_setAnalogMode(13U, GPIO_ANALOG_DISABLED);
    GPIO_setAnalogMode(16U, GPIO_ANALOG_DISABLED);
    GPIO_setAnalogMode(17U, GPIO_ANALOG_DISABLED);
    GPIO_setAnalogMode(20U, GPIO_ANALOG_DISABLED);
    GPIO_setAnalogMode(21U, GPIO_ANALOG_DISABLED);
    GPIO_setAnalogMode(24U, GPIO_ANALOG_DISABLED);
    GPIO_setAnalogMode(28U, GPIO_ANALOG_DISABLED);
    GPIO_setAnalogMode(33U, GPIO_ANALOG_DISABLED);

    // PinMux for modules assigned to CPU1
    // Analog PinMux for A0, B15, C15, DACA_OUT
    GPIO_setPinConfig(GPIO_231_GPIO231);
    // AIO -> Analog mode selected
    GPIO_setAnalogMode(231, GPIO_ANALOG_ENABLED);

    // Analog PinMux for A4, B8, C14
    GPIO_setPinConfig(GPIO_225_GPIO225);
    // AIO -> Analog mode selected
    GPIO_setAnalogMode(225, GPIO_ANALOG_ENABLED);

    // Analog PinMux for A8, B0, C11
    GPIO_setPinConfig(GPIO_241_GPIO241);
    // AIO -> Analog mode selected
    GPIO_setAnalogMode(241, GPIO_ANALOG_ENABLED);

    // Analog PinMux for A9, GPIO227
    GPIO_setPinConfig(GPIO_227_GPIO227);
    // AGPIO -> Analog mode selected
    GPIO_setAnalogMode(227, GPIO_ANALOG_ENABLED);

    // Analog PinMux for A10, B1, C10, GPIO230
    GPIO_setPinConfig(GPIO_230_GPIO230);
    // AGPIO -> Analog mode selected
    GPIO_setAnalogMode(230, GPIO_ANALOG_ENABLED);

    // Analog PinMux for A2, B6, C9, PGA1_INP, GPIO224
    GPIO_setPinConfig(GPIO_224_GPIO224);
    // AGPIO -> Analog mode selected
    GPIO_setAnalogMode(224, GPIO_ANALOG_ENABLED);

    // Analog PinMux for A5, B12, C2, PGA2_INM
    GPIO_setPinConfig(GPIO_244_GPIO244);
    // AIO -> Analog mode selected
    GPIO_setAnalogMode(244, GPIO_ANALOG_ENABLED);

    // Analog PinMux for A6, D14, E14, GPIO228
    GPIO_setPinConfig(GPIO_228_GPIO228);
    // AGPIO -> Analog mode selected
    GPIO_setAnalogMode(228, GPIO_ANALOG_ENABLED);

    // Analog PinMux for A15, B9, C7, PGA1_INM
    GPIO_setPinConfig(GPIO_233_GPIO233);
    // AIO -> Analog mode selected
    GPIO_setAnalogMode(233, GPIO_ANALOG_ENABLED);

    // Analog PinMux for B2, C6, E12, GPIO226
    GPIO_setPinConfig(GPIO_226_GPIO226);
    // AGPIO -> Analog mode selected
    GPIO_setAnalogMode(226, GPIO_ANALOG_ENABLED);

    // Analog PinMux for A1, B7, D11, DACB_OUT
    GPIO_setPinConfig(GPIO_232_GPIO232);
    // AIO -> Analog mode selected
    GPIO_setAnalogMode(232, GPIO_ANALOG_ENABLED);

    // Analog PinMux for A26, D6, E6
    GPIO_setPinConfig(GPIO_209_GPIO209);
    // AIO -> Analog mode selected
    GPIO_setAnalogMode(209, GPIO_ANALOG_ENABLED);

    // Analog PinMux for B26, D7, E7
    GPIO_setPinConfig(GPIO_210_GPIO210);
    // AIO -> Analog mode selected
    GPIO_setAnalogMode(210, GPIO_ANALOG_ENABLED);

    // Analog PinMux for C25, D5, E5
    GPIO_setPinConfig(GPIO_208_GPIO208);
    // AIO -> Analog mode selected
    GPIO_setAnalogMode(208, GPIO_ANALOG_ENABLED);

    // Analog PinMux for D20, E20, VREFHI
    GPIO_setPinConfig(GPIO_234_GPIO234);
    // AIO -> Analog mode selected
    GPIO_setAnalogMode(234, GPIO_ANALOG_ENABLED);

    // Analog PinMux for B4, C8, GPIO236
    GPIO_setPinConfig(GPIO_236_GPIO236);
    // AGPIO -> Analog mode selected
    GPIO_setAnalogMode(236, GPIO_ANALOG_ENABLED);

    // Analog PinMux for B11, D16, E16, PGA3_INM
    GPIO_setPinConfig(GPIO_251_GPIO251);
    // AIO -> Analog mode selected
    GPIO_setAnalogMode(251, GPIO_ANALOG_ENABLED);

    // Analog PinMux for A12, C1, E11, PGA3_INP
    GPIO_setPinConfig(GPIO_238_GPIO238);
    // AIO -> Analog mode selected
    GPIO_setAnalogMode(238, GPIO_ANALOG_ENABLED);

    // Analog PinMux for A3, B3, PGA2_INP, C5, GPIO242
    GPIO_setPinConfig(GPIO_242_GPIO242);
    // AGPIO -> Analog mode selected
    GPIO_setAnalogMode(242, GPIO_ANALOG_ENABLED);

    // Analog PinMux for A11, B10, C0, PGA2_OUT
    GPIO_setPinConfig(GPIO_237_GPIO237);
    // AIO -> Analog mode selected
    GPIO_setAnalogMode(237, GPIO_ANALOG_ENABLED);

    // Analog PinMux for A14, B14, C4, PGA1_OUT
    GPIO_setPinConfig(GPIO_239_GPIO239);
    // AIO -> Analog mode selected
    GPIO_setAnalogMode(239, GPIO_ANALOG_ENABLED);

    // Analog PinMux for A27, D9, E9, GPIO212
    GPIO_setPinConfig(GPIO_212_GPIO212);
    // AGPIO -> Analog mode selected
    GPIO_setAnalogMode(212, GPIO_ANALOG_ENABLED);

    // Analog PinMux for A28, D19, E19, GPIO215
    GPIO_setPinConfig(GPIO_215_GPIO215);
    // AGPIO -> Analog mode selected
    GPIO_setAnalogMode(215, GPIO_ANALOG_ENABLED);

    // Analog PinMux for B27, D10, E10, GPIO213
    GPIO_setPinConfig(GPIO_213_GPIO213);
    // AGPIO -> Analog mode selected
    GPIO_setAnalogMode(213, GPIO_ANALOG_ENABLED);

    // Analog PinMux for C26, D8, E8, GPIO211
    GPIO_setPinConfig(GPIO_211_GPIO211);
    // AGPIO -> Analog mode selected
    GPIO_setAnalogMode(211, GPIO_ANALOG_ENABLED);

    // Analog PinMux for C27, D18, E18, GPIO214
    GPIO_setPinConfig(GPIO_214_GPIO214);
    // AGPIO -> Analog mode selected
    GPIO_setAnalogMode(214, GPIO_ANALOG_ENABLED);

    // Analog PinMux for A7, B30, C3, D12, E30
    GPIO_setPinConfig(GPIO_245_GPIO245);
    // AIO -> Analog mode selected
    GPIO_setAnalogMode(245, GPIO_ANALOG_ENABLED);

    // Analog PinMux for B5, D15, E15, PGA3_OUT
    GPIO_setPinConfig(GPIO_252_GPIO252);
    // AIO -> Analog mode selected
    GPIO_setAnalogMode(252, GPIO_ANALOG_ENABLED);

    // Analog PinMux for A13, B13, C13, D13, E13, VREFLO
    GPIO_setPinConfig(GPIO_235_GPIO235);
    // AIO -> Analog mode selected
    GPIO_setAnalogMode(235, GPIO_ANALOG_ENABLED);

    // Set the analog voltage reference selection to internal.
    ASysCtl_setAnalogReferenceInternal( ASYSCTL_ANAREF_INTREF_ADCA | ASYSCTL_ANAREF_INTREF_ADCB | ASYSCTL_ANAREF_INTREF_ADCC | ASYSCTL_ANAREF_INTREF_ADCD | ASYSCTL_ANAREF_INTREF_ADCE );

    // Set the internal analog voltage reference selection to 1.65V.
    ASysCtl_setAnalogReference1P65( ASYSCTL_ANAREF_ADCA | ASYSCTL_ANAREF_ADCB | ASYSCTL_ANAREF_ADCC | ASYSCTL_ANAREF_ADCD | ASYSCTL_ANAREF_ADCE );

    EDIS;

    EALLOW;

    // Enable internal voltage reference
    SysCtl_delay(1000U);
    ADC_setVREF(obj->adcHandle[0], ADC_REFERENCE_INTERNAL, ADC_REFERENCE_3_3V);
    ADC_setVREF(obj->adcHandle[1], ADC_REFERENCE_INTERNAL, ADC_REFERENCE_3_3V);
    ADC_setVREF(obj->adcHandle[2], ADC_REFERENCE_INTERNAL, ADC_REFERENCE_3_3V);
    ADC_setVREF(obj->adcHandle[3], ADC_REFERENCE_INTERNAL, ADC_REFERENCE_3_3V);
    ADC_setVREF(obj->adcHandle[4], ADC_REFERENCE_INTERNAL, ADC_REFERENCE_3_3V);
    SysCtl_delay(1000U);

    // setup ADC modules A, B, C, D, E
    for(cnt = 0; cnt < 5; cnt++)
    {

        // Set main clock scaling factor (50MHz max clock for the ADC module)
        ADC_setPrescaler(obj->adcHandle[cnt], ADC_CLK_DIV_2_0);

        // set the ADC interrupt pulse generation to end of conversion
        ADC_setInterruptPulseMode(obj->adcHandle[cnt], ADC_PULSE_END_OF_CONV);

        // enable the ADC
        ADC_enableConverter(obj->adcHandle[cnt]);

        // Disables SOC burst mode.
        ADC_disableBurstMode(obj->adcHandle[cnt]);

        // set priority of SOCs
        ADC_setSOCPriority(obj->adcHandle[cnt], ADC_PRI_ALL_HIPRI);
    }

    // delay to allow ADCs to power up
    SysCtl_delay(1000U);


    //-------------------------------------------------------------------------
    // For motor 1
    //-------------------------------------------------------------------------
    // Shunt Motor Currents (M1-Iu) @ B2->SOCB0
    // SOC0 will convert pin B2, sample window in SYSCLK cycles
    // trigger on ePWM6 SOCA/C
    ADC_setupSOC(M1_IU_ADC_BASE, M1_IU_ADC_SOC_NUM,
                 M1_ADC_TRIGGER_SOC, M1_IU_ADC_CH_NUM, 12);

    // Configure PPB to eliminate subtraction related calculation
    // PPB is associated with SOC0
    ADC_setupPPB(M1_IU_ADC_BASE, M1_IU_ADC_PPB_NUM, M1_IU_ADC_SOC_NUM);

    // Write zero to this for now till offset ISR is run
    ADC_setPPBCalibrationOffset(M1_IU_ADC_BASE, M1_IU_ADC_PPB_NUM, 0);

    // Shunt Motor Currents (M1-Iv) @ C0->SOCC0
    // SOC0 will convert pin C0, sample window in SYSCLK cycles
    // trigger on ePWM6 SOCA/C
    ADC_setupSOC(M1_IV_ADC_BASE, M1_IV_ADC_SOC_NUM,
                 M1_ADC_TRIGGER_SOC, M1_IV_ADC_CH_NUM, 12);

    // Configure PPB to eliminate subtraction related calculation
    // PPB is associated with SOC0
    ADC_setupPPB(M1_IV_ADC_BASE, M1_IV_ADC_PPB_NUM, M1_IV_ADC_SOC_NUM);

    // Write zero to this for now till offset ISR is run
    ADC_setPPBCalibrationOffset(M1_IV_ADC_BASE, M1_IV_ADC_PPB_NUM, 0);

    // Shunt Motor Currents (M1-Iw) @ A9->SOCA0
    // SOC0 will convert pin A9, sample window in SYSCLK cycles
    // trigger on ePWM6 SOCA/C
    ADC_setupSOC(M1_IW_ADC_BASE, M1_IW_ADC_SOC_NUM,
                 M1_ADC_TRIGGER_SOC, M1_IW_ADC_CH_NUM, 12);

    // Configure PPB to eliminate subtraction related calculation
    // PPB is associated with SOC0
    ADC_setupPPB(M1_IW_ADC_BASE, M1_IW_ADC_PPB_NUM, M1_IW_ADC_SOC_NUM);

    // Write zero to this for now till offset ISR is run
    ADC_setPPBCalibrationOffset(M1_IW_ADC_BASE, M1_IW_ADC_PPB_NUM, 0);

    // Phase Voltage (M1-Vfb-dc) @ A5->SOCA1
    // SOC1 will convert pin A5, sample window in SYSCLK cycles
    // trigger on ePWM1 SOCA/C
    ADC_setupSOC(M1_VDC_ADC_BASE, M1_VDC_ADC_SOC_NUM,
                 M1_ADC_TRIGGER_SOC, M1_VDC_ADC_CH_NUM, 12);

    // Configure PPB to eliminate subtraction related calculation
    // PPB is associated with SOC1
    ADC_setupPPB(M1_VDC_ADC_BASE, M1_VDC_ADC_PPB_NUM, M1_VDC_ADC_SOC_NUM);

    // Write zero to this for now till offset ISR is run
    ADC_setPPBCalibrationOffset(M1_VDC_ADC_BASE, M1_VDC_ADC_PPB_NUM, 0);

    //-------------------------------------------------------------------------
    // For motor 2
    //-------------------------------------------------------------------------
    // Shunt Motor Currents (M2-Iu) @ C3->SOCC2
    // SOC1 will convert pin C3, sample window in SYSCLK cycles
    // trigger on ePWM1 SOCA/C
    ADC_setupSOC(M2_IU_ADC_BASE, M2_IU_ADC_SOC_NUM,
                 M2_ADC_TRIGGER_SOC, M2_IU_ADC_CH_NUM, 12);

    // Configure PPB to eliminate subtraction related calculation
    // PPB is associated with SOC2
    ADC_setupPPB(M2_IU_ADC_BASE, M2_IU_ADC_PPB_NUM, M2_IU_ADC_SOC_NUM);

    // Write zero to this for now till offset ISR is run
    ADC_setPPBCalibrationOffset(M2_IU_ADC_BASE, M2_IU_ADC_PPB_NUM, 0);

    // Shunt Motor Currents (M2-Iv) @ C5->SOCC3
    // SOC2 will convert pin C5, sample window in SYSCLK cycles
    // trigger on ePWM1 SOCA/C
    ADC_setupSOC(M2_IV_ADC_BASE, M2_IV_ADC_SOC_NUM,
                 M2_ADC_TRIGGER_SOC, M2_IV_ADC_CH_NUM, 12);

    // Configure PPB to eliminate subtraction related calculation
    // PPB is associated with SOC3
    ADC_setupPPB(M2_IV_ADC_BASE, M2_IV_ADC_PPB_NUM, M2_IV_ADC_SOC_NUM);

    // Write zero to this for now till offset ISR is run
    ADC_setPPBCalibrationOffset(M2_IV_ADC_BASE, M2_IV_ADC_PPB_NUM, 0);

    // Shunt Motor Currents (M2-Iw) @ A3->SOCA2
    // SOC2 will convert pin A3, sample window in SYSCLK cycles
    // trigger on ePWM1 SOCA/C
    ADC_setupSOC(M2_IW_ADC_BASE, M2_IW_ADC_SOC_NUM,
                 M2_ADC_TRIGGER_SOC, M2_IW_ADC_CH_NUM, 12);

    // Configure PPB to eliminate subtraction related calculation
    // PPB is associated with SOC2
    ADC_setupPPB(M2_IW_ADC_BASE, M2_IW_ADC_PPB_NUM, M2_IW_ADC_SOC_NUM);

    // Write zero to this for now till offset ISR is run
    ADC_setPPBCalibrationOffset(M2_IW_ADC_BASE, M2_IW_ADC_PPB_NUM, 0);

    // Phase Voltage (M2-Vfb-dc) @ A6->SOCA3
    // SOC3 will convert pin A6, sample window in SYSCLK cycles
    // trigger on ePWM1 SOCA/C
    ADC_setupSOC(M2_VDC_ADC_BASE, M2_VDC_ADC_SOC_NUM,
                 M2_ADC_TRIGGER_SOC, M2_VDC_ADC_CH_NUM, 12);

    // Configure PPB to eliminate subtraction related calculation
    // PPB is associated with SOC3
    ADC_setupPPB(M2_VDC_ADC_BASE, M2_VDC_ADC_PPB_NUM, M2_VDC_ADC_SOC_NUM);

    // Write zero to this for now till offset ISR is run
    ADC_setPPBCalibrationOffset(M2_VDC_ADC_BASE, M2_VDC_ADC_PPB_NUM, 0);

    EDIS;

    return;
}

//
// setup CLA
//
void HAL_setupCLA(HAL_Handle handle)
{

    HAL_Obj *obj = (HAL_Obj *)handle;

#ifdef _FLASH
    //
    // Copy CLA code from its load address (FLASH) to CLA program RAM
    //
    // Note: during debug the load and run addresses can be
    // the same as Code Composer Studio can load the CLA program
    // RAM directly.
    //
    // The ClafuncsLoadStart, ClafuncsLoadEnd, and ClafuncsRunStart
    // symbols are created by the linker.
    //
    memcpy((uint32_t *)&Cla1funcsRunStart, (uint32_t *)&Cla1funcsLoadStart,
            (uint32_t)&Cla1funcsLoadSize);

    memcpy((uint32_t *)&Cla1ConstRunStart, (uint32_t *)&Cla1ConstLoadStart,
            (uint32_t)&Cla1ConstLoadSize);
#endif //_FLASH

    // make sure QEP access is given to CLA as Secondary master
    SysCtl_setPeripheralAccessControl(SYSCTL_ACCESS_EQEP1,
                                      SYSCTL_ACCESS_CLA1,
                                      SYSCTL_ACCESS_FULL);

    SysCtl_setPeripheralAccessControl(SYSCTL_ACCESS_EQEP3,
                                      SYSCTL_ACCESS_CLA1,
                                      SYSCTL_ACCESS_FULL);


    // Initialize and wait for CLA1ToCPUMsgRAM
    MemCfg_initSections(MEMCFG_SECT_MSGCLA1TOCPU);
    while(MemCfg_getInitStatus(MEMCFG_SECT_MSGCLA1TOCPU) != 1);

    // Initialize and wait for CPUToCLA1MsgRAM

    MemCfg_initSections(MEMCFG_SECT_MSGCPUTOCLA1);
    while(MemCfg_getInitStatus(MEMCFG_SECT_MSGCPUTOCLA1) != 1);

    // Select LS5RAM to be the programming space for the CLA
    // First configure the CLA to be the master for LS5 and then
    // set the space to be a program block

    MemCfg_setLSRAMControllerSel(MEMCFG_SECT_LS4, MEMCFG_LSRAMCONTROLLER_CPU_CLA1);
    MemCfg_setCLAMemType(MEMCFG_SECT_LS4, MEMCFG_CLA_MEM_PROGRAM);

    MemCfg_setLSRAMControllerSel(MEMCFG_SECT_LS5, MEMCFG_LSRAMCONTROLLER_CPU_CLA1);
    MemCfg_setCLAMemType(MEMCFG_SECT_LS5, MEMCFG_CLA_MEM_PROGRAM);

    // Next configure LS2RAM and LS3RAM as data spaces for the CLA
    // First configure the CLA to be the master and then
    // set the spaces to be code blocks
    MemCfg_setLSRAMControllerSel(MEMCFG_SECT_LS2, MEMCFG_LSRAMCONTROLLER_CPU_CLA1);
    MemCfg_setCLAMemType(MEMCFG_SECT_LS2, MEMCFG_CLA_MEM_DATA);

    MemCfg_setLSRAMControllerSel(MEMCFG_SECT_LS3, MEMCFG_LSRAMCONTROLLER_CPU_CLA1);
    MemCfg_setCLAMemType(MEMCFG_SECT_LS3, MEMCFG_CLA_MEM_DATA);

    // Compute all CLA task vectors
    // On Type-1 CLAs the MVECT registers accept full 16-bit task addresses as
    // opposed to offsets used on older Type-0 CLAs
#pragma diag_suppress = 770
    CLA_mapTaskVector(obj->claHandle, CLA_MVECT_1, (uint16_t)(&Cla1Task1));
    CLA_mapTaskVector(obj->claHandle, CLA_MVECT_2, (uint16_t)(&Cla1Task2));
    CLA_mapTaskVector(obj->claHandle, CLA_MVECT_3, (uint16_t)(&Cla1Task3));
    CLA_mapTaskVector(obj->claHandle, CLA_MVECT_4, (uint16_t)(&Cla1Task4));
    CLA_mapTaskVector(obj->claHandle, CLA_MVECT_5, (uint16_t)(&Cla1Task5));
    CLA_mapTaskVector(obj->claHandle, CLA_MVECT_6, (uint16_t)(&Cla1Task6));
    CLA_mapTaskVector(obj->claHandle, CLA_MVECT_7, (uint16_t)(&Cla1Task7));
    CLA_mapTaskVector(obj->claHandle, CLA_MVECT_8, (uint16_t)(&Cla1Task8));
#pragma diag_suppress = 770

    // Enable the IACK instruction to start a task on CLA in software
    // for all  8 CLA tasks. Also, globally enable all 8 tasks (or a
    // subset of tasks) by writing to their respective bits in the
    // MIER register
    CLA_enableIACK(obj->claHandle);
    CLA_enableTasks(obj->claHandle, CLA_TASKFLAG_ALL);

    // Enable EPWM6 INT trigger for CLA TASK1
    CLA_setTriggerSource(CLA_TASK_1, CLA_TRIGGER_EPWM1INT);

    // Enable EPWM1 INT trigger for CLA TASK5
    CLA_setTriggerSource(CLA_TASK_5, CLA_TRIGGER_EPWM7INT);

    return;
}

//
// setup CMPSS
//
void HAL_setupCMPSS(HAL_MTR_Handle handle)
{
    HAL_MTR_Obj *obj = (HAL_MTR_Obj *)handle;

    uint16_t cnt;

    for(cnt = 0; cnt < 3; cnt++)
    {
        // Set up COMPCTL register
        // NEG signal from DAC for COMP-H
        CMPSS_configHighComparator(obj->cmpssHandle[cnt], CMPSS_INSRC_DAC);

        // NEG signal from DAC for COMP-L, COMP-L output is inverted
        CMPSS_configLowComparator(obj->cmpssHandle[cnt],
                                  (CMPSS_INSRC_DAC | CMPSS_INV_INVERTED)) ;

        // Dig filter output ==> CTRIPH, Dig filter output ==> CTRIPOUTH
        CMPSS_configOutputsHigh(obj->cmpssHandle[cnt],
                                (CMPSS_TRIP_FILTER | CMPSS_TRIPOUT_FILTER));

        // Dig filter output ==> CTRIPL, Dig filter output ==> CTRIPOUTL
        CMPSS_configOutputsLow(obj->cmpssHandle[cnt],
                               (CMPSS_TRIP_FILTER | CMPSS_TRIPOUT_FILTER));

        // Set up COMPHYSCTL register
        // COMP hysteresis set to 2x typical value
        CMPSS_setHysteresis(obj->cmpssHandle[cnt], 2);

        // set up COMPDACCTL register
        // VDDA is REF for CMPSS DACs, DAC updated on sysclock, Ramp bypassed
        CMPSS_configDAC(obj->cmpssHandle[cnt],
                (CMPSS_DACVAL_SYSCLK | CMPSS_DACSRC_SHDW));

        // Load DACs - High and Low
        // Set DAC-H to allowed MAX +ve current
        CMPSS_setDACValueHigh(obj->cmpssHandle[cnt], 2048);

        // Set DAC-L to allowed MAX -ve current
        CMPSS_setDACValueLow(obj->cmpssHandle[cnt], 2048);

        // digital filter settings - HIGH side
        // set time between samples, max : 1023, # of samples in window,
        // max : 31, recommended : thresh > sampWin/2
        // Init samples to filter input value
        CMPSS_configFilterHigh(obj->cmpssHandle[cnt], 20, 30, 18);
        CMPSS_initFilterHigh(obj->cmpssHandle[cnt]);

        // digital filter settings - LOW side
        // set time between samples, max : 1023, # of samples in window,
        // max : 31, recommended : thresh > sampWin/2
        // Init samples to filter input value
        CMPSS_configFilterLow(obj->cmpssHandle[cnt], 20, 30, 18);
        CMPSS_initFilterLow(obj->cmpssHandle[cnt]);

        // Clear the status register for latched comparator events
        CMPSS_clearFilterLatchHigh(obj->cmpssHandle[cnt]);
        CMPSS_clearFilterLatchLow(obj->cmpssHandle[cnt]);

        // Enable CMPSS
        CMPSS_enableModule(obj->cmpssHandle[cnt]);
    }

    //
    // Refer to TMS320F28P55x Technical Reference Manual (SPRUJ53)
    //
#if defined(LPVER_RELEASE)
    if(handle == &halMtr[MTR_1])
    {
        //B3*->CMP3_HP3, CMP3_LP3
        ASysCtl_selectCMPHPMux(ASYSCTL_CMPHPMUX_SELECT_3, 3);
        ASysCtl_selectCMPLPMux(ASYSCTL_CMPLPMUX_SELECT_3, 3);

        //C1*/E11->CMP4_HP2, CMP4_LP2
        ASysCtl_selectCMPHPMux(ASYSCTL_CMPHPMUX_SELECT_4, 2);
        ASysCtl_selectCMPLPMux(ASYSCTL_CMPLPMUX_SELECT_4, 2);

//        //A2*/B8/C9->CMP1_HP0
//        ASysCtl_selectCMPHPMux(ASYSCTL_CMPHPMUX_SELECT_1, 0);
//
//        //B3*->CMP3_HP3, CMP3_LP3
//        ASysCtl_selectCMPHPMux(ASYSCTL_CMPHPMUX_SELECT_3, 3);
//        ASysCtl_selectCMPLPMux(ASYSCTL_CMPLPMUX_SELECT_3, 3);
//
//        //C1*/E11->CMP4_LP2
//        ASysCtl_selectCMPLPMux(ASYSCTL_CMPLPMUX_SELECT_4, 2);
    }
    else if(handle == &halMtr[MTR_2])
    {
        //B5*/D15/E15->CMP1_HP5, CMP1_LP5
        ASysCtl_selectCMPHPMux(ASYSCTL_CMPHPMUX_SELECT_1, 5);
        ASysCtl_selectCMPLPMux(ASYSCTL_CMPLPMUX_SELECT_1, 5);

        //A12*/C5->CMP2_HP0, CMP2_LP0
        ASysCtl_selectCMPHPMux(ASYSCTL_CMPHPMUX_SELECT_2, 0);
//        ASysCtl_selectCMPLPMux(ASYSCTL_CMPLPMUX_SELECT_2, 0);

//        //B4/C8*->CMP4_HP0
//        ASysCtl_selectCMPHPMux(ASYSCTL_CMPHPMUX_SELECT_4, 0);
//
//        //B5*/D15/E15->CMP1_LP5
//        ASysCtl_selectCMPLPMux(ASYSCTL_CMPLPMUX_SELECT_1, 5);
//
//        //A12*/C5->CMP2_HP0, CMP2_LP0
//        ASysCtl_selectCMPHPMux(ASYSCTL_CMPHPMUX_SELECT_2, 0);
//        ASysCtl_selectCMPLPMux(ASYSCTL_CMPLPMUX_SELECT_2, 0);
    }

#elif defined(LPVER_TEST039)
    if(handle == &halMtr[MTR_1])
    {
        //B12*/C2->CMP3_HP1, CMP3_LP1
        ASysCtl_selectCMPHPMux(ASYSCTL_CMPHPMUX_SELECT_3, 1);
        ASysCtl_selectCMPLPMux(ASYSCTL_CMPLPMUX_SELECT_3, 1);

        //A7/B30/C3*/D12/E30->CMP4_HP0, CMP4_LP0
        ASysCtl_selectCMPHPMux(ASYSCTL_CMPHPMUX_SELECT_4, 0);
        ASysCtl_selectCMPLPMux(ASYSCTL_CMPLPMUX_SELECT_4, 0);
    }
    else if(handle == &halMtr[MTR_2])
    {
        //B5/D15*/E15->CMP1_HP5, CMP1_LP5
        ASysCtl_selectCMPHPMux(ASYSCTL_CMPHPMUX_SELECT_1, 5);
        ASysCtl_selectCMPLPMux(ASYSCTL_CMPLPMUX_SELECT_1, 5);

        //A12*/C5->CMP2_HP0, CMP2_LP0
        ASysCtl_selectCMPHPMux(ASYSCTL_CMPHPMUX_SELECT_2, 0);
        ASysCtl_selectCMPLPMux(ASYSCTL_CMPLPMUX_SELECT_2, 0);
    }

#else   // LPVER_
#error Select the right LP version
#endif  // LPVER_

    DEVICE_DELAY_US(500);

    return;
}

//
// Setup OCP limits and digital filter parameters of CMPSS
//
void HAL_setupCMPSS_DACValue(HAL_MTR_Handle handle,
                             uint16_t curHi, uint16_t curLo)
{
    HAL_MTR_Obj *obj = (HAL_MTR_Obj *)handle;

    uint16_t cnt;

    for(cnt = 0; cnt < 3; cnt++)
    {
        // comparator references
        // Set DAC-H to allowed MAX +ve current
        CMPSS_setDACValueHigh(obj->cmpssHandle[cnt], curHi);

        // Set DAC-L to allowed MAX -ve current
        CMPSS_setDACValueLow(obj->cmpssHandle[cnt], curLo);
    }

    return;
}

//
// Setup interrupts
//
void HAL_setupInterrupts(HAL_MTR_Handle handle)
{
    HAL_MTR_Obj *obj = (HAL_MTR_Obj *)handle;


    // Enable EPWM1 INT to generate MotorControlISR
    #if(SAMPLING_METHOD == SINGLE_SAMPLING)
    // Select INT @ ctr = 0
    EPWM_setInterruptSource(obj->pwmHandle[0],
                            EPWM_INT_TBCTR_ZERO);
    #elif(SAMPLING_METHOD == DOUBLE_SAMPLING)
    // Select INT @ ctr = 0 or ctr = prd
    EPWM_setInterruptSource(obj->pwmHandle[0],
                            EPWM_INT_TBCTR_ZERO_OR_PERIOD);
    #endif

    // Enable Interrupt Generation from the PWM module
    EPWM_enableInterrupt(obj->pwmHandle[0]);

    // This needs to be 1 for the INTFRC to work
    EPWM_setInterruptEventCount(obj->pwmHandle[0], 1);

    // Clear ePWM Interrupt flag
    EPWM_clearEventTriggerInterruptFlag(obj->pwmHandle[0]);

    if(handle == &halMtr[MTR_1])
    {
        Interrupt_register(M1_INT_PWM, &motor1ControlISR);

        ADC_setInterruptSource(M1_IW_ADC_BASE, ADC_INT_NUMBER1,
                               M1_IW_ADC_SOC_NUM);
        ADC_enableContinuousMode(M1_IW_ADC_BASE, ADC_INT_NUMBER1);
        ADC_enableInterrupt(M1_IW_ADC_BASE, ADC_INT_NUMBER1);
    }
    else if(handle == &halMtr[MTR_2])
    {
        Interrupt_register(M2_INT_PWM, &motor2ControlISR);

        // Enable AdcA-ADCINT1- to help verify EoC before result data read
        ADC_setInterruptSource(M2_IW_ADC_BASE, ADC_INT_NUMBER2,
                               M2_IW_ADC_SOC_NUM);
        ADC_enableContinuousMode(M2_IW_ADC_BASE, ADC_INT_NUMBER2);
        ADC_enableInterrupt(M2_IW_ADC_BASE, ADC_INT_NUMBER2);

    }

    return;
}

//
// setup CPU Timer
//
void HAL_setupCpuTimer(uint32_t base, uint32_t periodCount)
{
    CPUTimer_setPreScaler(CPUTIMER0_BASE, 0);  // divide by 1 (SYSCLKOUT)
    CPUTimer_setPeriod(base, periodCount);
    CPUTimer_stopTimer(base);               // Stop timer / reload / restart
    CPUTimer_setEmulationMode(base,
                              CPUTIMER_EMULATIONMODE_STOPAFTERNEXTDECREMENT);
    CPUTimer_reloadTimerCounter(base);      // Reload counter with period value
    CPUTimer_resumeTimer(base);

    return;
}

//
// setup DAC
//
void HAL_setupDACs(HAL_Handle handle)
{

    HAL_Obj *obj = (HAL_Obj *)handle;

    //
    // DAC-A  ---> Resolver carrier excitation
    // DAC-B  ---> General purpose display

    uint16_t cnt;

    for(cnt = 0; cnt < 1; cnt++)
    {
        // Set DAC voltage reference to VRefHi
        DAC_setReferenceVoltage(obj->dacHandle[cnt], DAC_REF_ADC_VREFHI);

        // Set the DAC gain to 2
        DAC_setGainMode(obj->dacHandle[cnt], DAC_GAIN_TWO);

        // enable value change only on sync signal
        DAC_setLoadMode(obj->dacHandle[cnt], DAC_LOAD_SYSCLK);

        //Enable DAC output
        DAC_enableOutput(obj->dacHandle[cnt]);

        // Set the DAC Shadow Output Value
        // Set the initial value to half of ADC range for 1.65V output
        DAC_setShadowValue(obj->dacHandle[cnt], 2048U);
    }

    DEVICE_DELAY_US(1000);

    return;
}

//
// setup PGA
//
void HAL_setupPGAs(HAL_Handle handle)
{

    HAL_Obj *obj = (HAL_Obj *)handle;

    uint16_t cnt;

    for(cnt = 0; cnt < 3; cnt++)
    {
        PGA_selectPMUXInput(obj->pgaHandle[cnt], PGA_PMUX_POS);
        PGA_selectNMUXInput(obj->pgaHandle[cnt], PGA_NMUX_RTAP);

        PGA_selectMMUXInput(obj->pgaHandle[cnt], PGA_MMUX_NON_INVERTING_GAIN_VSSA);
        PGA_setGain(obj->pgaHandle[cnt], PGA_GAIN_2);

        PGA_setFilterResistor(obj->pgaHandle[cnt], PGA_LOW_PASS_FILTER_DISABLED);
        PGA_disableExternalChop(obj->pgaHandle[cnt]);
        PGA_enable(obj->pgaHandle[cnt]);
    }

    DEVICE_DELAY_US(500);

    return;
}

//
// Sets up the GPIO (General Purpose I/O) pins
//
void HAL_setupGPIOs(HAL_Handle handle)
{
    EALLOW;
#if defined(LPVER_RELEASE)
    // Motor 1 related GPIOs
    // EPWM1A->UH for J1_J3&J2_J4 Connection
    GPIO_setControllerCore(0, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_0_EPWM1_A);
    GPIO_setDirectionMode(0, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(0, GPIO_PIN_TYPE_STD);

    // EPWM1B->UL for J1_J3&J2_J4 Connection
    GPIO_setControllerCore(1, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_1_EPWM1_B);
    GPIO_setDirectionMode(1, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(1, GPIO_PIN_TYPE_STD);

    // EPWM2A->VH for J1_J3&J2_J4 Connection
    GPIO_setControllerCore(2, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_2_EPWM2_A);
    GPIO_setDirectionMode(2, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(2, GPIO_PIN_TYPE_STD);

    // EPWM2B->VL for J1_J3&J2_J4 Connection
    GPIO_setControllerCore(3, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_3_EPWM2_B);
    GPIO_setDirectionMode(3, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(3, GPIO_PIN_TYPE_STD);

    // EPWM6A->WH for J1_J3&J2_J4 Connection
    GPIO_setControllerCore(10, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_10_EPWM6_A);
    GPIO_setDirectionMode(10, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(10, GPIO_PIN_TYPE_STD);

    // EPWM6B->WL for J1_J3&J2_J4 Connection
    GPIO_setControllerCore(11, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_11_EPWM6_B);
    GPIO_setDirectionMode(11, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(11, GPIO_PIN_TYPE_STD);

    // GPIO40->EQEP1A for J12
    GPIO_setControllerCore(40, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_40_EQEP1_A);
    GPIO_setDirectionMode(40, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(40, GPIO_PIN_TYPE_STD);

    // GPIO41->EQEP1B for J12
    GPIO_setControllerCore(41, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_41_EQEP1_B);
    GPIO_setDirectionMode(41, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(41, GPIO_PIN_TYPE_STD);

    // GPIO59->EQEP1I on J12
    GPIO_setControllerCore(59, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_59_EQEP1_INDEX);
    GPIO_setDirectionMode(59, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(59, GPIO_PIN_TYPE_PULLUP);

    // GPIO21->LPD_LED2_GGREEN->nEN_uc on Site_1
    GPIO_setControllerCore(21, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_21_GPIO21);
    GPIO_writePin(21, 0);
    GPIO_setDirectionMode(21, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(21, GPIO_PIN_TYPE_STD);

    // GPIO22->GPIO22->OT on Site_1
    GPIO_setControllerCore(22, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_22_GPIO22);
    GPIO_setDirectionMode(22, GPIO_DIR_MODE_IN);
//    GPIO_setPadConfig(22, GPIO_PIN_TYPE_STD);
    GPIO_setPadConfig(22, GPIO_PIN_TYPE_INVERT);

    // Motor 2 related GPIOs
    // GPIO12->EPWM7A->UH for J5_J7&J6_J8 Connection
    GPIO_setControllerCore(12, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_12_EPWM7_A);
    GPIO_setDirectionMode(12, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(12, GPIO_PIN_TYPE_STD);

    // GPIO13->EPWM7B->UL for J5_J7&J6_J8 Connection
    GPIO_setControllerCore(13, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_13_EPWM7_B);
    GPIO_setDirectionMode(13, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(13, GPIO_PIN_TYPE_STD);

    // GPIO6->EPWM4A->VH for J5_J7&J6_J8 Connection
    GPIO_setControllerCore(6, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_6_EPWM4_A);
    GPIO_setDirectionMode(6, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(6, GPIO_PIN_TYPE_STD);

    // GPIO7->EPWM4B->VL for J5_J7&J6_J8 Connection
    GPIO_setControllerCore(7, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_7_EPWM4_B);
    GPIO_setDirectionMode(7, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(7, GPIO_PIN_TYPE_STD);

    // GPIO37->EPWM5A->WH for J5_J7&J6_J8 Connection
    GPIO_setControllerCore(37, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_37_EPWM5_A);
    GPIO_setDirectionMode(37, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(37, GPIO_PIN_TYPE_STD);

    // GPIO35->EPWM5B->WL for J5_J7&J6_J8 Connection
    GPIO_setControllerCore(35, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_35_EPWM5_B);
    GPIO_setDirectionMode(35, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(35, GPIO_PIN_TYPE_STD);

    // GPIO25->EQEP3A on J13
    GPIO_setControllerCore(25, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_25_EQEP3_A);
    GPIO_setDirectionMode(25, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(25, GPIO_PIN_TYPE_PULLUP);

    // GPIO26->EQEP3B on J13
    GPIO_setControllerCore(26, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_26_EQEP3_B);
    GPIO_setDirectionMode(26, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(26, GPIO_PIN_TYPE_PULLUP);

    // GPIO30->EQEP3I on J13
    GPIO_setControllerCore(30, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_30_EQEP3_INDEX);
    GPIO_setDirectionMode(30, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(30, GPIO_PIN_TYPE_STD);

    // GPIO20->LPD_LED1_RED->nEN_uc on Site_2
    GPIO_setControllerCore(20, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_20_GPIO20);
    GPIO_writePin(20, 0);
    GPIO_setDirectionMode(20, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(20, GPIO_PIN_TYPE_STD);

    // GPIO62->GPIO62->OT on Site_2
    GPIO_setControllerCore(62, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_62_GPIO62);
    GPIO_setDirectionMode(62, GPIO_DIR_MODE_IN);
//    GPIO_setPadConfig(62, GPIO_PIN_TYPE_STD);
    GPIO_setPadConfig(62, GPIO_PIN_TYPE_INVERT);

    // Interface
    // GPIO28->SCIRXDA
    GPIO_setControllerCore(28, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_28_SCIA_RX);
    GPIO_setDirectionMode(28, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(28, GPIO_PIN_TYPE_STD);

    // GPIO29->SCITXDA
    GPIO_setControllerCore(29, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_29_SCIA_TX);
    GPIO_setDirectionMode(29, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(29, GPIO_PIN_TYPE_STD);

    // GPIO15->SCIRXDB
    GPIO_setControllerCore(15, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_15_SCIB_RX);
    GPIO_setDirectionMode(15, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(15, GPIO_PIN_TYPE_STD);

    // GPIO56->SCITXDB
    GPIO_setControllerCore(56, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_56_SCIB_TX);
    GPIO_setDirectionMode(56, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(56, GPIO_PIN_TYPE_STD);

    // CAN/CAN_FD TX for J14
    GPIO_setControllerCore(4, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_4_MCANA_TX);
    GPIO_setDirectionMode(4, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(4, GPIO_PIN_TYPE_STD);

    // CAN/CAN_FD RX for J14
    GPIO_setControllerCore(5, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_5_MCANA_RX);
    GPIO_setDirectionMode(5, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(5, GPIO_PIN_TYPE_STD);

    // GPIO34->TEST1_GPIO
    GPIO_setControllerCore(34, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_34_GPIO34);
    GPIO_writePin(34, 0);
    GPIO_setDirectionMode(34, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(34, GPIO_PIN_TYPE_STD);

    // GPIO51->TEST2_GPIO
    GPIO_setControllerCore(51, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_51_GPIO51);
    GPIO_writePin(51, 0);
    GPIO_setDirectionMode(51, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(51, GPIO_PIN_TYPE_STD);

    // GPIO47->TEST3_GPIO (LED_R)
    GPIO_setControllerCore(47, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_47_GPIO47);
    GPIO_writePin(23, 0);
    GPIO_setDirectionMode(47, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(47, GPIO_PIN_TYPE_STD);

    // GPIO48->TEST4_GPIO (LED_G)
    GPIO_setControllerCore(48, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_48_GPIO48);
    GPIO_writePin(48, 0);
    GPIO_setDirectionMode(48, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(48, GPIO_PIN_TYPE_STD);

    // GPIO24->TEST5_GPIO
    GPIO_setControllerCore(24, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_24_GPIO24);
    GPIO_writePin(24, 0);
    GPIO_setDirectionMode(24, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(24, GPIO_PIN_TYPE_STD);

// LPVER_TEST039, only for development phase
#elif defined(LPVER_TEST039)
    // Motor 1 related GPIOs
    // EPWM1A->UH for J1_J3&J2_J4 Connection
    GPIO_setControllerCore(0, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_0_EPWM1_A);
    GPIO_setDirectionMode(0, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(0, GPIO_PIN_TYPE_STD);

    // EPWM1B->UL for J1_J3&J2_J4 Connection
    GPIO_setControllerCore(1, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_1_EPWM1_B);
    GPIO_setDirectionMode(1, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(1, GPIO_PIN_TYPE_STD);

    // EPWM2A->VH for J1_J3&J2_J4 Connection
    GPIO_setControllerCore(2, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_2_EPWM2_A);
    GPIO_setDirectionMode(2, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(2, GPIO_PIN_TYPE_STD);

    // EPWM2B->VL for J1_J3&J2_J4 Connection
    GPIO_setControllerCore(3, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_3_EPWM2_B);
    GPIO_setDirectionMode(3, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(3, GPIO_PIN_TYPE_STD);

    // EPWM6A->WH for J1_J3&J2_J4 Connection
    GPIO_setControllerCore(10, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_10_EPWM6_A);
    GPIO_setDirectionMode(10, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(10, GPIO_PIN_TYPE_STD);

    // EPWM6B->WL for J1_J3&J2_J4 Connection
    GPIO_setControllerCore(11, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_11_EPWM6_B);
    GPIO_setDirectionMode(11, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(11, GPIO_PIN_TYPE_STD);

    // GPIO40->EQEP1A for J12
    GPIO_setControllerCore(40, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_40_EQEP1_A);
    GPIO_setDirectionMode(40, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(40, GPIO_PIN_TYPE_STD);

    // GPIO41->EQEP1B for J12
    GPIO_setControllerCore(41, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_41_EQEP1_B);
    GPIO_setDirectionMode(41, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(41, GPIO_PIN_TYPE_STD);

    // GPIO59->EQEP1I on J12
    GPIO_setControllerCore(59, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_59_EQEP1_INDEX);
    GPIO_setDirectionMode(59, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(59, GPIO_PIN_TYPE_PULLUP);

    // GPIO37->nEN_uc on Site_1
    GPIO_setControllerCore(37, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_37_GPIO37);
    GPIO_writePin(37, 0);
    GPIO_setDirectionMode(37, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(37, GPIO_PIN_TYPE_STD);

    // GPIO33->GPIO33->OT on Site_1
    GPIO_setControllerCore(33, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_33_GPIO33);
    GPIO_setDirectionMode(33, GPIO_DIR_MODE_IN);
//    GPIO_setPadConfig(33, GPIO_PIN_TYPE_STD);
    GPIO_setPadConfig(33, GPIO_PIN_TYPE_INVERT);

    // Motor 2 related GPIOs
    // GPIO12->EPWM7A->UH for J5_J7&J6_J8 Connection
    GPIO_setControllerCore(12, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_12_EPWM7_A);
    GPIO_setDirectionMode(12, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(12, GPIO_PIN_TYPE_STD);

    // GPIO13->EPWM7B->UL for J5_J7&J6_J8 Connection
    GPIO_setControllerCore(13, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_13_EPWM7_B);
    GPIO_setDirectionMode(13, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(13, GPIO_PIN_TYPE_STD);

    // EPWM4A->VH for J5_J7&J6_J8 Connection
    GPIO_setControllerCore(6, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_6_EPWM4_A);
    GPIO_setDirectionMode(6, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(6, GPIO_PIN_TYPE_STD);

    // EPWM4B->VL for J5_J7&J6_J8 Connection
    GPIO_setControllerCore(7, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_7_EPWM4_B);
    GPIO_setDirectionMode(7, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(7, GPIO_PIN_TYPE_STD);

    // GPIO8->EPWM5A->WH for J5_J7&J6_J8 Connection jumper to GPIO16
    GPIO_setControllerCore(8, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_8_EPWM5_A);
    GPIO_setDirectionMode(8, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(8, GPIO_PIN_TYPE_STD);

    // GPIO16->Input, Using a jumper wire connect to GPIO8
    GPIO_setControllerCore(16, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_16_GPIO16);
    GPIO_setDirectionMode(16, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(16, GPIO_PIN_TYPE_STD);

    // GPIO35->EPWM5B->WL for J5_J7&J6_J8 Connection
    GPIO_setControllerCore(35, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_35_EPWM5_B);
    GPIO_setDirectionMode(35, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(35, GPIO_PIN_TYPE_STD);

    // GPIO14->EQEP2A on J13
    GPIO_setControllerCore(14, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_14_EQEP2_A);
    GPIO_setDirectionMode(14, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(14, GPIO_PIN_TYPE_PULLUP);

    // GPIO55->EQEP2B on J13
    GPIO_setControllerCore(55, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_55_EQEP2_B);
    GPIO_setDirectionMode(55, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(55, GPIO_PIN_TYPE_PULLUP);

    // GPIO57->EQEP2I on J13
    GPIO_setControllerCore(57, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_57_EQEP2_INDEX);
    GPIO_setDirectionMode(57, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(57, GPIO_PIN_TYPE_PULLUP);

    // GPIO20->nEN_uc on Site_2
    GPIO_setControllerCore(20, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_20_GPIO20);
    GPIO_writePin(20, 0);
    GPIO_setDirectionMode(20, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(20, GPIO_PIN_TYPE_STD);

    // GPIO21->GPIO21->OT on Site_2
    GPIO_setControllerCore(21, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_21_GPIO21);
    GPIO_setDirectionMode(21, GPIO_DIR_MODE_IN);
//    GPIO_setPadConfig(21, GPIO_PIN_TYPE_STD);
    GPIO_setPadConfig(21, GPIO_PIN_TYPE_INVERT);

    // GPIO28->SCIRXDA
    GPIO_setControllerCore(28, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_28_SCIA_RX);
    GPIO_setDirectionMode(28, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(28, GPIO_PIN_TYPE_STD);

    // GPIO29->SCITXDA
    GPIO_setControllerCore(29, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_29_SCIA_TX);
    GPIO_setDirectionMode(29, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(29, GPIO_PIN_TYPE_STD);

    // GPIO15->SCIRXDB
    GPIO_setControllerCore(15, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_15_SCIB_RX);
    GPIO_setDirectionMode(15, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(15, GPIO_PIN_TYPE_STD);

    // GPIO56->SCITXDB
    GPIO_setControllerCore(56, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_56_SCIB_TX);
    GPIO_setDirectionMode(56, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(56, GPIO_PIN_TYPE_STD);

    // CAN/CAN_FD TX for J14
    GPIO_setControllerCore(4, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_4_MCANA_TX);
    GPIO_setDirectionMode(4, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(4, GPIO_PIN_TYPE_STD);

    // CAN/CAN_FD RX for J14
    GPIO_setControllerCore(5, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_5_MCANA_RX);
    GPIO_setDirectionMode(5, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(5, GPIO_PIN_TYPE_STD);

    // GPIO24->TEST1_GPIO
    GPIO_setControllerCore(24, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_24_GPIO24);
    GPIO_writePin(24, 0);
    GPIO_setDirectionMode(24, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(24, GPIO_PIN_TYPE_STD);

    // GPIO51->TEST2_GPIO
    GPIO_setControllerCore(51, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_51_GPIO51);
    GPIO_writePin(51, 0);
    GPIO_setDirectionMode(51, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(51, GPIO_PIN_TYPE_STD);

    // GPIO34->TEST3_GPIO
    GPIO_setControllerCore(34, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_34_GPIO34);
    GPIO_writePin(34, 0);
    GPIO_setDirectionMode(34, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(34, GPIO_PIN_TYPE_STD);

    // GPIO22->TEST4_GPIO (LED_R)
    GPIO_setControllerCore(22, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_22_GPIO22);
    GPIO_writePin(22, 0);
    GPIO_setDirectionMode(22, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(22, GPIO_PIN_TYPE_STD);

    // GPIO23->TEST5_GPIO (LED_G)
    GPIO_setControllerCore(23, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_23_GPIO23);
    GPIO_writePin(23, 0);
    GPIO_setDirectionMode(23, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(23, GPIO_PIN_TYPE_STD);

#else   // LPVER_
#error Select the right LP version
#endif  // LPVER_

    EDIS;

    return;
}



// Sets up the PWMs (Pulse Width Modulators) for motor
void HAL_setupMotorPWMs(HAL_MTR_Handle handle)
{
    HAL_MTR_Obj *obj = (HAL_MTR_Obj *)handle;

    uint16_t  cnt;
    uint16_t  halfPeriod = 0;

    // *****************************************
    // Inverter PWM configuration - PWM 1, 2, 3
    // *****************************************
    for(cnt = 0; cnt < 3; cnt++)
    {
        // Time Base SubModule Registers
        // set Immediate load
        EPWM_setPeriodLoadMode(obj->pwmHandle[cnt], EPWM_PERIOD_DIRECT_LOAD);
        EPWM_setTimeBasePeriod(obj->pwmHandle[cnt], 0);
        EPWM_setPhaseShift(obj->pwmHandle[cnt], 0);
        EPWM_setTimeBaseCounter(obj->pwmHandle[cnt], 0);
        EPWM_setTimeBaseCounterMode(obj->pwmHandle[cnt],
                                    EPWM_COUNTER_MODE_UP_DOWN);

        EPWM_setClockPrescaler(obj->pwmHandle[cnt], EPWM_CLOCK_DIVIDER_1,
                               EPWM_HSCLOCK_DIVIDER_1);

        // Counter Compare Submodule Registers
        // set duty 0% initially
        EPWM_setCounterCompareValue(obj->pwmHandle[cnt],
                                    EPWM_COUNTER_COMPARE_A, 0);
        EPWM_setCounterCompareShadowLoadMode(obj->pwmHandle[cnt],
                                             EPWM_COUNTER_COMPARE_A,
                                             EPWM_COMP_LOAD_ON_CNTR_ZERO);

        // Action Qualifier SubModule Registers
        EPWM_setActionQualifierActionComplete(obj->pwmHandle[cnt],
                                              EPWM_AQ_OUTPUT_A,
                (EPWM_ActionQualifierEventAction)(EPWM_AQ_OUTPUT_LOW_UP_CMPA |
                                               EPWM_AQ_OUTPUT_HIGH_DOWN_CMPA));

        // Active high complementary PWMs - Set up the deadband
        EPWM_setRisingEdgeDeadBandDelayInput(obj->pwmHandle[cnt],
                                             EPWM_DB_INPUT_EPWMA);
        EPWM_setFallingEdgeDeadBandDelayInput(obj->pwmHandle[cnt],
                                              EPWM_DB_INPUT_EPWMA);

        EPWM_setDeadBandDelayMode(obj->pwmHandle[cnt], EPWM_DB_RED, true);
        EPWM_setDeadBandDelayMode(obj->pwmHandle[cnt], EPWM_DB_FED, true);
        EPWM_setDeadBandDelayPolarity(obj->pwmHandle[cnt], EPWM_DB_RED,
                                      EPWM_DB_POLARITY_ACTIVE_HIGH);
        EPWM_setDeadBandDelayPolarity(obj->pwmHandle[cnt],
                                      EPWM_DB_FED, EPWM_DB_POLARITY_ACTIVE_LOW);

        EPWM_setRisingEdgeDelayCount(obj->pwmHandle[cnt], EPWM_DB_DELAY_RISE);
        EPWM_setFallingEdgeDelayCount(obj->pwmHandle[cnt], EPWM_DB_DELAY_FALL);

        EPWM_enablePhaseShiftLoad(obj->pwmHandle[cnt]);

        EPWM_setCountModeAfterSync(obj->pwmHandle[cnt],
                                   EPWM_COUNT_MODE_UP_AFTER_SYNC);
        // configure sync
        EPWM_enableSyncOutPulseSource(obj->pwmHandle[cnt],
                                      EPWM_SYNC_OUT_PULSE_ON_CNTR_ZERO);
    }

    SysCtl_setSyncOutputConfig(SYSCTL_SYNC_OUT_SRC_EPWM1SYNCOUT);

    //EPWM1->EWPM4
//    SysCtl_setSyncInputConfig(SYSCTL_SYNC_IN_EPWM4,
//                              SYSCTL_SYNC_IN_SRC_EPWM1SYNCOUT);

    if(handle == &halMtr[MTR_1])
    {
        EPWM_disablePhaseShiftLoad(obj->pwmHandle[0]);  //EPWM1

        // sync "down-stream", EPWM1
        EPWM_enableSyncOutPulseSource(obj->pwmHandle[0],
                                      EPWM_SYNC_OUT_PULSE_ON_CNTR_ZERO);
        EPWM_setSyncInPulseSource(obj->pwmHandle[1], EPWM_SYNC_IN_PULSE_SRC_SYNCOUT_EPWM1);
        EPWM_setSyncInPulseSource(obj->pwmHandle[2], EPWM_SYNC_IN_PULSE_SRC_SYNCOUT_EPWM1);

        halfPeriod = M1_INV_PWM_TICKS / 2;     // 100MHz EPWMCLK

        EPWM_setPhaseShift(obj->pwmHandle[0], 0);    //EPWM1
        EPWM_setPhaseShift(obj->pwmHandle[1], 2);    //EPWM2
        EPWM_setPhaseShift(obj->pwmHandle[2], 2);    //EPWM6

        EPWM_setTimeBasePeriod(obj->pwmHandle[0], halfPeriod);
        EPWM_setTimeBasePeriod(obj->pwmHandle[1], halfPeriod);
        EPWM_setTimeBasePeriod(obj->pwmHandle[2], halfPeriod);
    }
    else if(handle == &halMtr[MTR_2])
    {
        halfPeriod = M2_INV_PWM_TICKS / 2;     // 100MHz EPWMCLK

        EPWM_setSyncInPulseSource(obj->pwmHandle[0], EPWM_SYNC_IN_PULSE_SRC_SYNCOUT_EPWM1);
        EPWM_setSyncInPulseSource(obj->pwmHandle[1], EPWM_SYNC_IN_PULSE_SRC_SYNCOUT_EPWM1);
        EPWM_setSyncInPulseSource(obj->pwmHandle[2], EPWM_SYNC_IN_PULSE_SRC_SYNCOUT_EPWM1);

        // sync "down-stream"
        EPWM_setPhaseShift(obj->pwmHandle[0], (halfPeriod>>1) + 2);           //EPWM7
        EPWM_setPhaseShift(obj->pwmHandle[1], (halfPeriod>>1) + 2);           //EPWM4
        EPWM_setPhaseShift(obj->pwmHandle[2], (halfPeriod>>1) + 2);           //EPWM5

        EPWM_setTimeBasePeriod(obj->pwmHandle[0], halfPeriod);
        EPWM_setTimeBasePeriod(obj->pwmHandle[1], halfPeriod);
        EPWM_setTimeBasePeriod(obj->pwmHandle[2], halfPeriod);
    }

    // Setting up link from EPWM to ADC
    // EPWM1/EPWM4 - Inverter currents at sampling frequency
    //               (@ PRD or @ (PRD&ZRO) )
#if(SAMPLING_METHOD == SINGLE_SAMPLING)
    // Select SOC from counter at ctr = prd
    EPWM_setADCTriggerSource(obj->pwmHandle[0],
                             EPWM_SOC_A, EPWM_SOC_TBCTR_ZERO);
#elif(SAMPLING_METHOD == DOUBLE_SAMPLING)
    // Select SOC from counter at ctr = 0 or ctr = prd
    EPWM_setADCTriggerSource(obj->pwmHandle[0], EPWM_SOC_A,
                             EPWM_SOC_TBCTR_ZERO_OR_PERIOD);
#endif

    // Generate pulse on 1st event
    EPWM_setADCTriggerEventPrescale(obj->pwmHandle[0], EPWM_SOC_A, 1);

    // Enable SOC on A group
    EPWM_enableADCTrigger(obj->pwmHandle[0], EPWM_SOC_A);

    return;
}

//
// Configure Motor Fault Protection Against Over Current
//
void HAL_setupMotorFaultProtection(HAL_MTR_Handle handle,
                                   const float32_t currentLimit)
{
    HAL_MTR_Obj *obj = (HAL_MTR_Obj *)handle;

    uint16_t  cnt;

    EPWM_DigitalCompareTripInput tripInSet = EPWM_DC_TRIP_TRIPIN4;

    // High and Low Compare event trips
    uint16_t curHi = 0;
    uint16_t curLo = 0;

    if(handle == &halMtr[MTR_1])
    {
        tripInSet = EPWM_DC_TRIP_TRIPIN4;

        curHi = 2048 + M1_CURRENT_SCALE(currentLimit);
        curLo = 2048 - M1_CURRENT_SCALE(currentLimit);

        //Select GPIO as INPUTXBAR1
        XBAR_setInputPin(INPUTXBAR_BASE, M1_XBAR_INPUT_NUM, M1_XBAR_INPUT_GPIO);

        // Configure TRIP 4 to OR the High and Low trips from both
        // comparator 1 & 3, clear everything first
        EALLOW;
        HWREG(XBAR_EPWM_CFG_REG_BASE + XBAR_O_TRIP4MUX0TO15CFG) = 0;
        HWREG(XBAR_EPWM_CFG_REG_BASE + XBAR_O_TRIP4MUX16TO31CFG) = 0;
        EDIS;

#if defined(LPVER_RELEASE)
        // Enable Muxes for input of CMPSS3H and CMPSS3L, mux for MUX04
        XBAR_setEPWMMuxConfig(XBAR_TRIP4, XBAR_EPWM_MUX04_CMPSS3_CTRIPH_OR_L);

        // Enable Muxes for input of CMPSS4H and CMPSSS4L, mux for MUX06
        XBAR_setEPWMMuxConfig(XBAR_TRIP4, XBAR_EPWM_MUX06_CMPSS4_CTRIPH_OR_L);

        //inputxbar2 trip
        XBAR_setEPWMMuxConfig(XBAR_TRIP4, XBAR_EPWM_MUX01_INPUTXBAR1_INPUT1);

        // Disable all the muxes first
        XBAR_disableEPWMMux(XBAR_TRIP4, 0xFFFF);

        // Enable MUX00, MUX04, MUX07 OR MUX01 to generate TRIP4
        XBAR_enableEPWMMux(XBAR_TRIP4, XBAR_MUX04 | XBAR_MUX06 | XBAR_MUX01);

//        // Enable Muxes for input of CMPSS1H, mux for MUX00
//        XBAR_setEPWMMuxConfig(XBAR_TRIP4, XBAR_EPWM_MUX00_CMPSS1_CTRIPH);
//
//        // Enable Muxes for input of CMPSS3H and CMPSS3L, mux for MUX04
//        XBAR_setEPWMMuxConfig(XBAR_TRIP4, XBAR_EPWM_MUX04_CMPSS3_CTRIPH_OR_L);
//
//        // Enable Muxes for input of CMPSSS4L, mux for MUX07
//        XBAR_setEPWMMuxConfig(XBAR_TRIP4, XBAR_EPWM_MUX07_CMPSS4_CTRIPL);
//
//        //inputxbar2 trip
//        XBAR_setEPWMMuxConfig(XBAR_TRIP4, XBAR_EPWM_MUX01_INPUTXBAR1_INPUT1);
//
//        // Disable all the muxes first
//        XBAR_disableEPWMMux(XBAR_TRIP4, 0xFFFF);
//
//        // Enable MUX00, MUX04, MUX07 OR MUX01 to generate TRIP4
//        XBAR_enableEPWMMux(XBAR_TRIP4, XBAR_MUX00 | XBAR_MUX04 | XBAR_MUX07 | XBAR_MUX01);

// LPVER_TEST039, only for development phase
#elif defined(LPVER_TEST039)
        // Enable Muxes for ored input of CMPSS1H and CMPSS1L, mux for Mux0x
        //cmpss3 - tripH or tripL
        XBAR_setEPWMMuxConfig(XBAR_TRIP4, XBAR_EPWM_MUX04_CMPSS3_CTRIPH_OR_L);

        //cmpss4 - tripL
        XBAR_setEPWMMuxConfig(XBAR_TRIP4, XBAR_EPWM_MUX06_CMPSS4_CTRIPH_OR_L);

        //inputxbar2 trip
        XBAR_setEPWMMuxConfig(XBAR_TRIP4, XBAR_EPWM_MUX01_INPUTXBAR1_INPUT1);

        // Disable all the muxes first
        XBAR_disableEPWMMux(XBAR_TRIP4, 0xFFFF);

        // Enable Mux 0  OR Mux 4 to generate TRIP4
        XBAR_enableEPWMMux(XBAR_TRIP4, XBAR_MUX04 | XBAR_MUX06 | XBAR_MUX01);
#else   // LPVER_
#error Select the right LP version
#endif  // LPVER_
    }
    else if(handle == &halMtr[MTR_2])
    {
        tripInSet = EPWM_DC_TRIP_TRIPIN5;

        curHi = 2048 + M2_CURRENT_SCALE(currentLimit);
        curLo = 2048 - M2_CURRENT_SCALE(currentLimit);

        //Select GPIO as INPUTXBAR1
        XBAR_setInputPin(INPUTXBAR_BASE, M2_XBAR_INPUT_NUM, M2_XBAR_INPUT_GPIO);

        // Configure TRIP 5 to OR the High and Low trips from both
        // comparator 5, 5, and 2, clear everything first
        EALLOW;
        HWREG(XBAR_EPWM_CFG_REG_BASE + XBAR_O_TRIP5MUX0TO15CFG) = 0;
        HWREG(XBAR_EPWM_CFG_REG_BASE + XBAR_O_TRIP5MUX16TO31CFG) = 0;
        EDIS;

#if defined(LPVER_RELEASE)
        // Enable Muxes for input of CMPSS1L, mux for MUX01
        XBAR_setEPWMMuxConfig(XBAR_TRIP5, XBAR_EPWM_MUX01_CMPSS1_CTRIPL);

        // Enable Muxes for input of CMPSS2H and 2L, mux for MUX02
        XBAR_setEPWMMuxConfig(XBAR_TRIP5, XBAR_EPWM_MUX02_CMPSS2_CTRIPH);

        //inputxbar2 trip
        XBAR_setEPWMMuxConfig(XBAR_TRIP5, XBAR_EPWM_MUX03_INPUTXBAR1_INPUT2);

        // Disable all the muxes first
        XBAR_disableEPWMMux(XBAR_TRIP5, 0xFFFF);

        // Enable MUX01, MUX02 OR MUX03 to generate TRIP5
        XBAR_enableEPWMMux(XBAR_TRIP5, XBAR_MUX01 | XBAR_MUX02 | XBAR_MUX03);

//        // Enable Muxes for input of CMPSS4H mux for MUX06
//        XBAR_setEPWMMuxConfig(XBAR_TRIP5, XBAR_EPWM_MUX06_CMPSS4_CTRIPH);
//
//        // Enable Muxes for input of CMPSS1L, mux for MUX01
//        XBAR_setEPWMMuxConfig(XBAR_TRIP5, XBAR_EPWM_MUX01_CMPSS1_CTRIPL);
//
//        // Enable Muxes for input of CMPSS2H and CMPSS2L, mux for MUX02
//        XBAR_setEPWMMuxConfig(XBAR_TRIP5, XBAR_EPWM_MUX02_CMPSS2_CTRIPH);
//
//        //inputxbar2 trip
//        XBAR_setEPWMMuxConfig(XBAR_TRIP5, XBAR_EPWM_MUX03_INPUTXBAR1_INPUT2);
//
//        // Disable all the muxes first
//        XBAR_disableEPWMMux(XBAR_TRIP5, 0xFFFF);
//
//        // Enable MUX06, MUX01, MUX02 OR MUX03 to generate TRIP5
//        XBAR_enableEPWMMux(XBAR_TRIP5, XBAR_MUX06 | XBAR_MUX01 | XBAR_MUX02 | XBAR_MUX03);
// LPVER_TEST039, only for development phase
#elif defined(LPVER_TEST039)
        // Enable Muxes for ored input of CMPSS1H and CMPSS1L, mux for Mux0x
        //cmpss1 - tripL
        XBAR_setEPWMMuxConfig(XBAR_TRIP5, XBAR_EPWM_MUX00_CMPSS1_CTRIPH_OR_L);

        //cmpss2 - tripH or tripL
        XBAR_setEPWMMuxConfig(XBAR_TRIP5, XBAR_EPWM_MUX02_CMPSS2_CTRIPH);

        //inputxbar2 trip
        XBAR_setEPWMMuxConfig(XBAR_TRIP5, XBAR_EPWM_MUX03_INPUTXBAR1_INPUT2);

        // Disable all the muxes first
        XBAR_disableEPWMMux(XBAR_TRIP5, 0xFFFF);

        // Enable Mux 0  OR Mux 4 to generate TRIP5
        XBAR_enableEPWMMux(XBAR_TRIP5, XBAR_MUX00 | XBAR_MUX02 | XBAR_MUX03);
#else   // LPVER_
#error Select the right LP version
#endif  // LPVER_
    }


    //
    // Configure TRIP for motor inverter phases
    //
    for(cnt = 0; cnt < 3; cnt++)
    {
        // comparator references
        // Set DAC-H to allowed MAX +ve current
        CMPSS_setDACValueHigh(obj->cmpssHandle[cnt], curHi);

        // Set DAC-L to allowed MAX -ve current
        CMPSS_setDACValueLow(obj->cmpssHandle[cnt], curLo);

        //Trip 4 is the input to the DCAHCOMPSEL
        EPWM_selectDigitalCompareTripInput(obj->pwmHandle[cnt],
                                           tripInSet,
                                           EPWM_DC_TYPE_DCAH);

        EPWM_setTripZoneDigitalCompareEventCondition(obj->pwmHandle[cnt],
                                                     EPWM_TZ_DC_OUTPUT_A1,
                                                     EPWM_TZ_EVENT_DCXH_HIGH);

        EPWM_setDigitalCompareEventSource(obj->pwmHandle[cnt],
                                          EPWM_DC_MODULE_A,
                                          EPWM_DC_EVENT_1,
                                          EPWM_DC_EVENT_SOURCE_ORIG_SIGNAL);

        EPWM_setDigitalCompareEventSyncMode(obj->pwmHandle[cnt],
                                            EPWM_DC_MODULE_A,
                                            EPWM_DC_EVENT_1,
                                            EPWM_DC_EVENT_INPUT_NOT_SYNCED);

        EPWM_enableTripZoneSignals(obj->pwmHandle[cnt], EPWM_TZ_SIGNAL_DCAEVT1);

        // Emulator Stop
        EPWM_enableTripZoneSignals(obj->pwmHandle[cnt], EPWM_TZ_SIGNAL_CBC6);

        // What do we want the OST/CBC events to do?
        // TZA events can force EPWMxA
        // TZB events can force EPWMxB

        // EPWMxA will go low
        // EPWMxB will go low
        EPWM_setTripZoneAction(obj->pwmHandle[cnt],
                               EPWM_TZ_ACTION_EVENT_TZA,
                               EPWM_TZ_ACTION_LOW);

        EPWM_setTripZoneAction(obj->pwmHandle[cnt],
                               EPWM_TZ_ACTION_EVENT_TZB,
                               EPWM_TZ_ACTION_LOW);
    }

    // clear EPWM trip flags
    DEVICE_DELAY_US(1L);

    for(cnt = 0; cnt < 3; cnt++)
    {
        // clear any spurious  OST & DCAEVT1 flags
        EPWM_clearTripZoneFlag(obj->pwmHandle[cnt], (EPWM_TZ_FLAG_OST |
                                                     EPWM_TZ_FLAG_DCAEVT1 |
                                                     EPWM_TZ_FLAG_CBC ));

        // clear any spurious  HLATCH - (not in TRIP gen path)
        CMPSS_clearFilterLatchHigh(obj->cmpssHandle[cnt]);

        // clear any spurious  LLATCH - (not in TRIP gen path)
        CMPSS_clearFilterLatchLow(obj->cmpssHandle[cnt]);
    }

    DEVICE_DELAY_US(1L);

    return;
}

void HAL_setupQEP(HAL_MTR_Handle handle)
{
    HAL_MTR_Obj *obj = (HAL_MTR_Obj *)handle;

    // Configure the decoder for quadrature count mode, counting both
    // rising and falling edges (that is, 2x resolution)
    EQEP_setDecoderConfig(obj->qepHandle, (EQEP_CONFIG_2X_RESOLUTION |
                                           EQEP_CONFIG_QUADRATURE |
                                           EQEP_CONFIG_NO_SWAP) );

    EQEP_setEmulationMode(obj->qepHandle, EQEP_EMULATIONMODE_RUNFREE);

    // Configure the position counter to be latched on a unit time out
    // and latch on rising edge of index pulse
    EQEP_setLatchMode(obj->qepHandle, (EQEP_LATCH_RISING_INDEX |
                                       EQEP_LATCH_UNIT_TIME_OUT) );

    // Configure the position counter to reset on a maximum position
    if(handle == &halMtr[MTR_1])
    {
        EQEP_setPositionCounterConfig(obj->qepHandle,
                                      EQEP_POSITION_RESET_MAX_POS,
                                      ((4 * M1_ENCODER_LINES) - 1) );

        // Enable the unit timer, setting the frequency to 10KHz
        EQEP_enableUnitTimer(obj->qepHandle, M1_QEP_UNIT_TIMER_TICKS - 1);
    }
    else if(handle == &halMtr[MTR_2])
    {
        EQEP_setPositionCounterConfig(obj->qepHandle,
                                      EQEP_POSITION_RESET_MAX_POS,
                                      ((4 * M2_ENCODER_LINES) - 1) );

        // Enable the unit timer, setting the frequency to 10KHz
        EQEP_enableUnitTimer(obj->qepHandle, M2_QEP_UNIT_TIMER_TICKS - 1);
    }

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

// end of the file
