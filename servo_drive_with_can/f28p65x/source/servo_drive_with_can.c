//###########################################################################
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
//###########################################################################

//! \file   /solutions/universal_motorcontrol_lab/common/source/servo_drive_with_can.c
//!
//! \brief  This project is used to implement motor control with sensored
//!         Encoder based control along with CAN communications.
//!

// **************************************************************************

//
// solutions
//
#include <servo_main.h>

#pragma CODE_SECTION(mainISR, ".TI.ramfunc");

#if defined(CMD_CAN_ENABLE)
#pragma CODE_SECTION(canISR, ".TI.ramfunc");

#elif defined(CMD_CAN_FD_ENABLE)
#pragma CODE_SECTION(CANFDIntrISR1, ".TI.ramfunc");

#endif  // !CMD_CAN_ENABLE, !CMD_CAN_FD_ENABLE

//
// the globals
//
HAL_ADCData_t adcData = {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, 0.0f};

HAL_PWMData_t pwmData = {{0.0f, 0.0f, 0.0f}};

uint16_t counterLED = 0;  //!< Counter used to divide down the ISR rate for
                           //!< visually blinking an LED

uint16_t counterSpeed = 0;
uint16_t counterTrajSpeed = 0;

uint32_t offsetCalcCount = 0;     //!< Counter used to count the wait time
                                  //!< for offset calibration, unit: ISR cycles

uint32_t offsetCalcWaitTime = 50000;  //!< Wait time setting for current/voltage
                                      //!< offset calibration, unit: ISR cycles

float32_t angleDelta_rad;   //!< the rotor angle compensation value
float32_t angleGen_rad;     //!< the rotor angle from rampgen module
float32_t angleFoc_rad;     //!< the rotor angle for FOC modules
float32_t angleQEP_rad;     //!< the rotor angle from QEP modules

MATH_Vec2 Idq_ref_A;        //!< the reference current on d&q rotation axis
MATH_Vec2 Idq_offset_A;     //!< the offsetting current on d&q rotation axis
MATH_Vec2 Iab_in_A;         //!< the alpha&beta axis current are converter from
                            //!< 3-phase sampling input current of motor
MATH_Vec2 Idq_in_A;         //!< the d&q axis current are converter from
                            //!< 3-phase sampling input current of motor
MATH_Vec2 Vab_out_V;        //!< the output control voltage on alpha&beta axis
MATH_Vec2 Vdq_out_V;        //!< the output control voltage on d&q axis

USER_Params userParams;    //!< the user parameters for motor control
                            //!< and hardware board configuration
#pragma DATA_SECTION(userParams, "ctrl_data");

#ifdef CMD_CAN_FD_ENABLE
int32_t     error = 0;   // variable for keeping track of error
MCAN_TxBufElement    txMsg;
MCAN_RxBufElement rxMsg,rxMsg1[64];  // Rx Msg object
MCAN_RxFIFOStatus RxFS; // Rx FIFO status object
//int32_t loopCnt = 0U;
#endif // !CMD_CAN_FD_ENABLE

volatile MOTOR_Vars_t motorVars = MOTOR_VARS_INIT;
#pragma DATA_SECTION(motorVars, "ctrl_data");

CLARKE_Handle clarkeHandle_I;   //!< the handle for the current Clarke transform
CLARKE_Obj    clarke_I;         //!< the current Clarke transform object

HAL_Handle    halHandle;        //!< the handle for the hardware abstraction layer
HAL_Obj       hal;              //!< the hardware abstraction layer object

IPARK_Handle  iparkHandle;      //!< the handle for the inverse Park transform
IPARK_Obj     ipark;            //!< the inverse Park transform object

PARK_Handle   parkHandle;       //!< the handle for the Park object
PARK_Obj      park;             //!< the Park transform object

PI_Handle     piHandle_Id;      //!< the handle for the Id PI controller
PI_Obj        pi_Id;            //!< the Id PI controller object

PI_Handle     piHandle_Iq;      //!< the handle for the Iq PI controller
PI_Obj        pi_Iq;            //!< the Iq PI controller object

PI_Handle     piHandle_spd;     //!< the handle for the speed PI controller
PI_Obj        pi_spd;           //!< the speed PI controller object

SVGEN_Handle  svgenHandle;      //!< the handle for the space vector generator
SVGEN_Obj     svgen;            //!< the space vector generator object

TRAJ_Handle   trajHandle_spd; //!< the handle for the speed reference trajectory
TRAJ_Obj      traj_spd;       //!< the speed reference trajectory object

//!< the handles for the current offset calculation
FILTER_FO_Handle  filterHandle_I[USER_NUM_CURRENT_SENSORS];

//!< the current offset calculation
FILTER_FO_Obj     filter_I[USER_NUM_CURRENT_SENSORS];

//!< the handles for Angle Generate for open loop control
ANGLE_GEN_Handle angleGenHandle;
//!< the Angle Generate onject for open loop control
ANGLE_GEN_Obj    angleGen;

#if (DMC_BUILDLEVEL == DMC_LEVEL_1)
//!< the handles for Vs per Freq for open loop control
VS_FREQ_Handle VsFreqHandle;
//!< the Vs per Freq object for open loop control
VS_FREQ_Obj    VsFreq;
#endif  // DMC_BUILDLEVEL != DMC_LEVEL_2

#ifdef BOOSTXL_DRV8323RS
//
// Watch window interface to the 8323 SPI
//
DRV8323_SPIVars_t drvSPI8323Vars;
#pragma DATA_SECTION(drvSPI8323Vars, "ctrl_data");
#endif

#ifdef STEP_RESPONSE_EN
#include "graph.h"
#endif  // STEP_RESPONSE_EN

#ifdef STEP_RESPONSE_EN
extern GRAPH_Vars_t gGraphVars;
extern GRAPH_StepVars_t gStepVars;
#endif  // STEP_RESPONSE_EN

QEP_SENSOR_Handle qepHandle_sensor;
QEP_SENSOR_Obj qep_sensor;

#if defined(CMD_CAN_ENABLE)
CAN_COMM_Handle canCommHandle;
CAN_COMM_Obj    canComm;
#elif defined(CMD_CAN_FD_ENABLE)
CAN_FD_COMM_Handle canFdCommHandle;
CAN_FD_COMM_Obj    canFdComm;
#endif  // CMD_CAN_ENABLE | CMD_CAN_FD_ENABLE

#if (DMC_BUILDLEVEL == DMC_LEVEL_4)
uint16_t counterCAN_TX = 0;
uint16_t numISRTicksPerCANTick = (uint16_t)(USER_PWM_FREQ_kHz/CAN_FEEDBACK_FREQ_kHz);
float32_t dummySpeed_Hz = 0;
#endif

#ifdef DEBUG_INT_DAC_EN // Internal DAC debug
#include "board.h"
float32_t dacval1, dacval2;
#endif

//
// the functions
//

void main(void)
 {
    //
    // Initialize device clock and peripherals
    //
    Device_init();

    //
    // initialize the user parameters
    //
    USER_setParams(&userParams);

    //
    // initialize the driver
    //
    halHandle = HAL_init(&hal, sizeof(hal));

    //
    // set the driver parameters
    //
    HAL_setParams(halHandle);

#ifdef DEBUG_INT_DAC_EN // Internal DAC debug
    Board_init();
#endif

    //
    // initialize the Clarke modules
    //
    clarkeHandle_I = CLARKE_init(&clarke_I, sizeof(clarke_I));

    //
    // set the Clarke parameters
    //
    setupClarke_I(clarkeHandle_I, userParams.numCurrentSensors);


    qepHandle_sensor  = QEP_SENSOR_init(&qep_sensor, sizeof(qep_sensor));
    QEP_SENSOR_setParams(qepHandle_sensor);

    //
    // initialize the inverse Park module
    //
    iparkHandle = IPARK_init(&ipark, sizeof(ipark));

    //
    // initialize the Park module
    //
    parkHandle = PARK_init(&park, sizeof(park));

    //
    // initialize the PI controllers
    //
    piHandle_Id  = PI_init(&pi_Id, sizeof(pi_Id));
    piHandle_Iq  = PI_init(&pi_Iq, sizeof(pi_Iq));
    piHandle_spd = PI_init(&pi_spd, sizeof(pi_spd));

    //
    // setup the controllers, speed, d/q-axis current pid regulator
    //
    setupControllers();

    //
    // initialize the space vector generator module
    //
    svgenHandle = SVGEN_init(&svgen, sizeof(svgen));

    //
    // initialize the speed reference trajectory
    //
    trajHandle_spd = TRAJ_init(&traj_spd, sizeof(traj_spd));

    //
    // configure the speed reference trajectory (Hz)
    //
    TRAJ_setTargetValue(trajHandle_spd, 0.0f);
    TRAJ_setIntValue(trajHandle_spd, 0.0f);
    TRAJ_setMinValue(trajHandle_spd, -USER_MOTOR_FREQ_MAX_HZ);
    TRAJ_setMaxValue(trajHandle_spd, USER_MOTOR_FREQ_MAX_HZ);
    TRAJ_setMaxDelta(trajHandle_spd, (USER_MAX_ACCEL_Hzps / USER_ISR_FREQ_Hz));

#if(DMC_BUILDLEVEL == DMC_LEVEL_1)
    // initialize the Vs per Freq module
    VsFreqHandle = VS_FREQ_init(&VsFreq, sizeof(VsFreq));

    VS_FREQ_setVsMagPu(VsFreqHandle, USER_MAX_VS_MAG_PU);

    VS_FREQ_setMaxFreq(VsFreqHandle, USER_MOTOR_FREQ_MAX_HZ);

    VS_FREQ_setProfile(VsFreqHandle,
                       USER_MOTOR_FREQ_LOW_HZ, USER_MOTOR_FREQ_HIGH_HZ,
                       USER_MOTOR_VOLT_MIN_V, USER_MOTOR_VOLT_MAX_V);
#endif // (DMC_BUILDLEVEL == DMC_LEVEL_2)

    //
    // initialize the angle generate module
    //
    angleGenHandle = ANGLE_GEN_init(&angleGen, sizeof(angleGen));
    ANGLE_GEN_setParams(angleGenHandle, userParams.ctrlPeriod_sec);

    //
    // initialize and configure offsets using first-order filter
    //
    {
        //
        // Sets the first-order filter denominator coefficients
        // a1, the filter coefficient value for z^(-1)
        // b0, the filter coefficient value for z^0
        // b1, the filter coefficient value for z^(-1)
        //
        uint16_t cnt = 0;

        float32_t b0 = userParams.offsetPole_rps / userParams.ctrlFreq_Hz;
        float32_t a1 = (b0 - 1.0f);
        float32_t b1 = 0.0f;

        //
        // For Current offset calibration filter
        //
        for(cnt = 0; cnt < USER_NUM_CURRENT_SENSORS; cnt++)
        {
            filterHandle_I[cnt] = FILTER_FO_init(&filter_I[cnt],
                                                   sizeof(filter_I[cnt]));

            FILTER_FO_setDenCoeffs(filterHandle_I[cnt], a1);
            FILTER_FO_setNumCoeffs(filterHandle_I[cnt], b0, b1);

            FILTER_FO_setInitialConditions(filterHandle_I[cnt],
                                        motorVars.offsets_I_A.value[cnt],
                                        motorVars.offsets_I_A.value[cnt]);
        }

        motorVars.flagEnableOffsetCalc = true;
        offsetCalcCount = 0;
    }

#if defined(CMD_CAN_ENABLE)
    // initialize the CAN communication module
    canCommHandle = CAN_COMM_init(&canComm, sizeof(canComm));
#elif defined(CMD_CAN_FD_ENABLE)
    // initialize the CAN_FD communication module
    canFdCommHandle = CAN_FD_COMM_init(&canFdComm, sizeof(canFdComm));

    //initialize TxMsg and RxMsg
    rxMsg.id = 0U;
    rxMsg.rtr = 0U;
    rxMsg.xtd = 0U;
    rxMsg.esi = 0U;
    rxMsg.rxts = 0U;
    rxMsg.dlc = 0U;
    rxMsg.brs = 0U;
    rxMsg.fdf = 0U;
    rxMsg.fidx = 0U;
    rxMsg.anmf = 0U;
    int32_t i;

    for(i=0; i < DATABYTES; i++)
    {
        rxMsg.data[i]  = 0;
    }


    txMsg.id       = ((uint32_t)(0xa5))<< 18U; // STDID[28:18]
    txMsg.rtr      = 0U;   // RTR = 0 (Data frame)
    txMsg.xtd      = 0U;   // XTD = 0 (11-bit standard identifier)
    txMsg.esi      = 0U;
    txMsg.dlc      = 15U;  // 64 bytes
    txMsg.brs      = 1U;   // Bit-rate switching enabled
    txMsg.fdf      = 1U;   // Frame transmitted in CAN FD format
    txMsg.efc      = 1U;   // Store TX events
    txMsg.mm       = 0xAAU;
    for(i = 0; i < DATABYTES; i++)
    {
        txMsg.data[i]  = 1;
    }
#endif  // !CMD_CAN_ENABLE | !CMD_CAN_FD_ENABLE

#if (DMC_BUILDLEVEL != DMC_LEVEL_2)
    motorVars.faultMask.all = FAULT_MASK_OC_OV;
#endif  // DMC_BUILDLEVEL != DMC_LEVEL_2

#if (DMC_BUILDLEVEL == DMC_LEVEL_2)
    motorVars.faultMask.all = FAULT_MASK_ALL_FLTS;

    //
    // setup OVM PWM
    //
//    HAL_setOvmParams(halHandle, &pwmData); // TODO: can remove this?
#endif  // DMC_BUILDLEVEL == DMC_LEVEL_2

    //
    // setup faults
    //
    HAL_setupFaults(halHandle);

    HAL_disablePWM(halHandle);

#ifdef BOOSTXL_DRV8323RS
    //
    // turn on the DRV83xx if present
    //
    HAL_enableDRV(halHandle);

    //
    // initialize the DRV83xx interface
    //
    HAL_setupDRVSPI(halHandle, &drvSPI8323Vars);

    drvSPI8323Vars.Ctrl_Reg_05.VDS_LVL = DRV8323_VDS_LEVEL_1P300_V;
    drvSPI8323Vars.Ctrl_Reg_05.DEAD_TIME = DRV8323_DEADTIME_100_NS;
    drvSPI8323Vars.Ctrl_Reg_03.IDRIVEN_HS = DRV8323_ISINK_HS_0P340_A;
    drvSPI8323Vars.Ctrl_Reg_03.IDRIVEP_HS = DRV8323_ISOUR_HS_0P190_A;
    drvSPI8323Vars.Ctrl_Reg_04.IDRIVEN_LS = DRV8323_ISINK_LS_0P340_A;
    drvSPI8323Vars.Ctrl_Reg_04.IDRIVEP_LS = DRV8323_ISOUR_LS_0P190_A;

    drvSPI8323Vars.writeCmd = 1;

#if (DMC_BUILDLEVEL == DMC_LEVEL_4)
    //
    // DRV83xx Read/Write
    //
   // drvSPI8323Vars.writeCmd = 1;
    HAL_writeDRVData(halHandle, &drvSPI8323Vars);

    //drvSPI8323Vars.readCmd = 1;
    HAL_readDRVData(halHandle, &drvSPI8323Vars);
#endif
#endif

    // Set some global variables
    motorVars.pwmISRCount = 0;          // clear the counter
    motorVars.IsRef_A = 1.0f;
    motorVars.speedRef_krpm = 0.600f;      //set reference speed to 100rpm here
    motorVars.speedRef_Hz = motorVars.speedRef_krpm *
            (1000.0f * USER_MOTOR_NUM_POLE_PAIRS) / 60.0f;  // set reference frequency to 14.0Hz, Electrical Rotation

    motorVars.flagSetupController = true;
    motorVars.flagClearFaults = true;

    motorVars.overCurrent_A = userParams.maxCurrent_A * 1.5f;

#if (DMC_BUILDLEVEL != DMC_LEVEL_1)
    motorVars.motorCtrlMode = MOTORCTRL_MODE_TORQUE;   //put into speed control

    // Manual Gain Overwriting
//    motorVars.Ki_Iq = 0.25f;
//    motorVars.Ki_Id = 0.25f;
//    motorVars.Kp_Iq = 0.35f;
//    motorVars.Kp_Id = 0.35f;
//    motorVars.Ki_spd = 0.004f;
//    motorVars.Kp_spd = 0.2f;
#endif  // (DMC_BUILDLEVEL == DMC_LEVEL_2)


#ifdef STEP_RESPONSE_EN
#if (DMC_BUILDLEVEL == DMC_LEVEL_2)
    gGraphVars.bufferTick = 1;

    gStepVars.IdRef_Default = 0.0f;
    gStepVars.IdRef_StepSize = -0.5f;

    gStepVars.spdRef_Default = 40.0f;
    gStepVars.spdRef_StepSize = 60.0f;

    motorVars.accelerationMax_Hzps = 10.0f;
    motorVars.accelerationStart_Hzps = 10.0f;
    motorVars.accelerationRun_Hzps = 200.0f;

    GRAPH_DataPointerInit(&(gStepVars),
                          &(motorVars.speed_Hz), &(Idq_in_A.value[0]),
                          &(Idq_in_A.value[1]), &(motorVars.speedRef_Hz),
                          &(Idq_ref_A.value[0]), &(Idq_ref_A.value[1]));
#else   // (DMC_BUILDLEVEL != DMC_LEVEL_2)
    gGraphVars.bufferTick = 5;

    // Verify the angle from QEP
    GRAPH_DataPointerInit(&(gStepVars),
                          &(angleGen_rad), &(angleFoc_rad),
                          &(adcData.I_A.value[1]), &(qep_sensor.Angle),
                          &(adcData.I_A.value[0]), &(adcData.I_A.value[2]));
#endif  // (DMC_BUILDLEVEL != DMC_LEVEL_2)
#endif  // STEP_RESPONSE_EN


#if (DMC_BUILDLEVEL == DMC_LEVEL_3)
    motorVars.flagEnableSys = true;
    motorVars.speedSetCAN_Hz = 30.0f;

#if defined(CMD_CAN_ENABLE)
    canComm.flagCmdEnable = true;       // false, test without CAN
    canComm.flagTxDone = true;          // false, test without CAN
#elif defined(CMD_CAN_FD_ENABLE)
    canFdComm.flagCmdEnable = true;     // false, test without CAN-FD
    canFdComm.flagTxDone = true;        // false, test without CAN-FD
#endif  // !CMD_CAN_ENABLE | !CMD_CAN_FD_ENABLE

    motorVars.accelerationMax_Hzps = 10.0f;
    motorVars.accelerationStart_Hzps = 10.0f;
    motorVars.accelerationRun_Hzps = 300.0f;

#elif (DMC_BUILDLEVEL == DMC_LEVEL_4)
//    motorVars.motorCtrlMode = MOTORCTRL_MODE_TORQUE;   // Torque control before encoder alignment
    motorVars.overCurrent_A =  USER_ADC_FULL_SCALE_CURRENT_A * 0.45f; // max. allowed overcurrent
    motorVars.flagEnableSys = true;
    motorVars.speedSetCAN_Hz = 0.0f; // Initial speed 0 Hz

    motorVars.accelerationMax_Hzps = 10.0f;
    motorVars.accelerationStart_Hzps = 10.0f;
    motorVars.accelerationRun_Hzps = 300.0f;

#if defined(CMD_CAN_ENABLE) ||  defined(CMD_CAN_FD_ENABLE)
    canComm.flagCmdEnable = true;       // false, test without CAN
    canComm.flagTxDone = true;          // false, test without CAN
#endif  // CMD_CAN_ENABLE | CMD_CAN_FD_ENABLE
#endif  // (DMC_BUILDLEVEL == DMC_LEVEL_4 or DMC_LEVEL_3)

    //
    // initialize the interrupt vector table
    //
    HAL_initIntVectorTable(halHandle);

    //
    // enable the ADC interrupts
    //
    HAL_enableADCInts();


#if defined(CMD_CAN_ENABLE)
    //
    // enable the CAN interrupts
    //
    HAL_enableCANInts(halHandle);
#elif defined(CMD_CAN_FD_ENABLE)
#if (DMC_BUILDLEVEL == DMC_LEVEL_3)
     //
     // enable the CANinterrupts
     //
     //HAL_enableMCANInts(halHandle);
#endif  // !DMC_LEVEL_3
#endif  // !CMD_CAN_ENABLE | !CMD_CAN_FD_ENABLE

    //
    // disable global interrupts
    //
    HAL_enableGlobalInts(halHandle);

    //
    // enable debug interrupts
    //
    HAL_enableDebugInt(halHandle);

    //
    // Waiting for enable system flag to be set
    //
    while(motorVars.flagEnableSys == false)
    {
#ifdef BOOSTXL_DRV8323RS
        //
        // DRV83xx Read/Write
        //
       // drvSPI8323Vars.writeCmd = 1;
        HAL_writeDRVData(halHandle, &drvSPI8323Vars);

        //drvSPI8323Vars.readCmd = 1;
        HAL_readDRVData(halHandle, &drvSPI8323Vars);
#endif
    }

    motorVars.flagClearFaults = true;

    //
    // loop while the enable system flag is true
    //
    while(motorVars.flagEnableSys == true)
    {
        //
        // 1ms time base
        //
        if(HAL_getTimerStatus(halHandle, HAL_CPU_TIMER1))
        {
            motorVars.timerCnt_1ms++;

            HAL_clearTimerFlag(halHandle, HAL_CPU_TIMER1);

#if (DMC_BUILDLEVEL == DMC_LEVEL_3) // DMC_BUILDLEVEL == DMC_LEVEL_3
#if defined(CMD_CAN_ENABLE)
            updateCANCmdFreq();
#elif defined(CMD_CAN_FD_ENABLE)
            updateCANFdCmdFreq(halHandle);
#endif  // !CMD_CAN_ENABLE | !CMD_CAN_FD_ENABLE
#endif // !DMC_LEVEL_3

#if (DMC_BUILDLEVEL == DMC_LEVEL_4) // DMC_BUILDLEVEL == DMC_LEVEL_4
            updateCANCmd_LEVEL4();
#endif
        }

        motorVars.mainLoopCount++;

        //
        // set the reference value for internal DACA and DACB // TODO: currently unused
        //
        HAL_setDACValue(halHandle, 0, motorVars.dacaVal);
        HAL_setDACValue(halHandle, 1, motorVars.dacbVal);

        //
        // set internal DAC value for on-chip comparator for current protection
        //
        calcMotorOverCurrentThreshold();

        {
            uint16_t  cmpssCnt;

            for(cmpssCnt = 0; cmpssCnt < HAL_NUM_CMPSS_CURRENT; cmpssCnt++)
            {
                HAL_setCMPSSDACValueHigh(halHandle,
                                         cmpssCnt, motorVars.dacValH);

                HAL_setCMPSSDACValueLow(halHandle,
                                        cmpssCnt, motorVars.dacValL);
            }
        }

        if(HAL_getPwmEnableStatus(halHandle) == true)
        {
            if(HAL_getTripFaults(halHandle) != 0)
            {
                //motorVars.faultNow.bit.moduleOverCurrent = 1; // Because of unknown problem, overcurrent is always triggered
            }
        }

        motorVars.faultUse.all =
                motorVars.faultNow.all & motorVars.faultMask.all;

        //
        // Had some faults to stop the motor
        //
        if(motorVars.faultUse.all != 0)
        {
            motorVars.flagRunIdentAndOnLine = false;
        }

        if(motorVars.flagClearFaults == true)
        {
            HAL_clearFaultStatus(halHandle);

            motorVars.faultNow.all &= FAULT_CLEAR_NVS;
            motorVars.flagClearFaults = false;
        }

        if((motorVars.flagRunIdentAndOnLine == true) &&
           (motorVars.flagEnableOffsetCalc == false))
        {
            if(HAL_getPwmEnableStatus(halHandle) == false)
            {
                //
                // enable the PWM
                //
                HAL_enablePWM(halHandle);
            }

            //
            // set the reference to the trajectory of speed
            //
            TRAJ_setTargetValue(trajHandle_spd, motorVars.speedRef_Hz);

            //
            // set the acceleration to the trajectory of speed
            //
            TRAJ_setMaxDelta(trajHandle_spd,
                           (motorVars.accelerationMax_Hzps / USER_ISR_FREQ_Hz));
        }
        else if(motorVars.flagEnableOffsetCalc == false)
        {
            //
            // disable the PWM
            //
            HAL_disablePWM(halHandle);

            //
            // clear integral outputs of the controllers
            //
            PI_setUi(piHandle_Id, 0.0f);
            PI_setUi(piHandle_Iq, 0.0f);
            PI_setUi(piHandle_spd, 0.0f);

            //
            // clear current references
            //
            Idq_ref_A.value[0] = 0.0f;
            Idq_ref_A.value[1] = 0.0f;

            //
            // clear current offsets
            //
            Idq_offset_A.value[0] = 0.0f;
            Idq_offset_A.value[1] = 0.0f;

            //
            // clear the trajectory of speed
            //
            TRAJ_setTargetValue(trajHandle_spd, 0.0f);
            TRAJ_setIntValue(trajHandle_spd, 0.0f);
        }

        // setup PI controllers
        if(motorVars.flagSetupController == true)
        {
            //
            // update the controller
            // set custom current and speed controllers gains
            //
            updateControllers();
        }

        //
        // update the global variables
        //
        updateGlobalVariables();

#ifdef BOOSTXL_DRV8323RS
        //
        // DRV8323 Read/Write
        //
        HAL_writeDRVData(halHandle, &drvSPI8323Vars);

        HAL_readDRVData(halHandle, &drvSPI8323Vars);
#endif

#ifdef STEP_RESPONSE_EN
        // Generate Step response
        GRAPH_generateStepResponse(&gGraphVars,&gStepVars);
#endif  //  STEP_RESPONSE_EN

    } // end of while() loop

    //
    // disable the PWM
    //
    HAL_disablePWM(halHandle);

} // end of main() function

__interrupt void mainISR(void)
{

    motorVars.pwmISRCount++;

    //
    // toggle status LED
    //
    counterLED++;

    if(counterLED > (uint32_t)(USER_ISR_FREQ_Hz / LED_BLINK_FREQ_Hz))
    {
        HAL_toggleLED(halHandle, HAL_GPIO_LED2);
        counterLED = 0;
    }

    //
    // acknowledge the ADC interrupt
    //
    HAL_ackADCInt(MTR1_ADC_INT_NUM);

    //
    // read the ADC data with offsets
    //
    HAL_readADCDataWithOffsets(halHandle, &adcData);


    //
    // calculate Vbus scale factor to scale offsets with Vbus
    //
    motorVars.Vbus_sf = adcData.dcBus_V;

    //
    // remove offsets
    //
    adcData.I_A.value[0] -= motorVars.offsets_I_A.value[0];
    adcData.I_A.value[1] -= motorVars.offsets_I_A.value[1];
    adcData.I_A.value[2] -= motorVars.offsets_I_A.value[2];
    adcData.V_V.value[0] -= motorVars.offsets_V_V.value[0] * motorVars.Vbus_sf;
    adcData.V_V.value[1] -= motorVars.offsets_V_V.value[1] * motorVars.Vbus_sf;
    adcData.V_V.value[2] -= motorVars.offsets_V_V.value[2] * motorVars.Vbus_sf;

    if(motorVars.flagEnableOffsetCalc == false)
    {
        float32_t outMax_V;
        MATH_Vec2 phasor;

        //
        // run Clarke transform on current
        //
        CLARKE_run(clarkeHandle_I, &adcData.I_A, &(Iab_in_A));


        ANGLE_GEN_run(angleGenHandle, motorVars.speedTraj_Hz);
        angleGen_rad = ANGLE_GEN_getAngle(angleGenHandle);

        qep_sensor.CaliAngle = angleGen_rad;
        QEP_SENSOR_run(qepHandle_sensor);
        motorVars.speed_Hz = qep_sensor.SpeedHz;

        counterTrajSpeed++;

        if(counterTrajSpeed >= userParams.numIsrTicksPerTrajTick)
        {
            //
            // clear counter
            //
            counterTrajSpeed = 0;

            //
            // run a trajectory for speed reference,
            // so the reference changes with a ramp instead of a step
            //
            TRAJ_run(trajHandle_spd);

            motorVars.speedTraj_Hz = TRAJ_getIntValue(trajHandle_spd);
        }

#if (DMC_BUILDLEVEL == DMC_LEVEL_1)
        angleFoc_rad = angleGen_rad;
#else
        angleFoc_rad = qep_sensor.Angle; //get angle from qep
#endif  // DMC_BUILDLEVEL != DMC_LEVEL_1

        //
        // compute the sin/cos phasor using fast RTS function, callable assembly
        //
        phasor.value[0] = cosf(angleFoc_rad);
        phasor.value[1] = sinf(angleFoc_rad);

        // set the phasor in the Park transform
        PARK_setPhasor(parkHandle,&phasor);

        // run the Park transform
        PARK_run(parkHandle,&(Iab_in_A), &(Idq_in_A));

        if(motorVars.motorCtrlMode == MOTORCTRL_MODE_SPEED)
        {
            //
            // run the speed controller
            //
            counterSpeed++;

            if(counterSpeed >= userParams.numCtrlTicksPerSpeedTick)
            {
                counterSpeed = 0;
                PI_run_series(piHandle_spd,
                          motorVars.speedTraj_Hz,
                          qep_sensor.SpeedHz,
                          0.0f,
                          (float32_t *)(&(Idq_ref_A.value[1])));
            }

        }

        else
        {
            Idq_ref_A.value[1] = motorVars.IsRef_A;

            if(qep_sensor.lsw == ENC_CALIBRATION_DONE)
            {
                motorVars.motorCtrlMode = MOTORCTRL_MODE_SPEED;
                motorVars.accelerationMax_Hzps = motorVars.accelerationRun_Hzps;
            }
            else
            {
                motorVars.accelerationMax_Hzps = motorVars.accelerationStart_Hzps;
            }
        }

        //
        // Maximum voltage output
        //
        userParams.maxVsMag_V = userParams.maxVsMag_pu * adcData.dcBus_V;
        PI_setMinMax(piHandle_Id,
                     (-userParams.maxVsMag_V), userParams.maxVsMag_V);

        //
        // run the Id controller
        //
        PI_run_series(piHandle_Id,
                      Idq_ref_A.value[0] + Idq_offset_A.value[0],
                      Idq_in_A.value[0],
                      0.0f,
                      &(Vdq_out_V.value[0]));

        //
        // calculate Iq controller limits, and run Iq controller using fast RTS
        // function, callable assembly
        //
        outMax_V = sqrtf((userParams.maxVsMag_V * userParams.maxVsMag_V) -
                        (Vdq_out_V.value[0] * Vdq_out_V.value[0]));

        PI_setMinMax(piHandle_Iq, -outMax_V, outMax_V);
        PI_run_series(piHandle_Iq,
                      Idq_ref_A.value[1] + Idq_offset_A.value[1],
                      Idq_in_A.value[1],
                      0.0f,
                      &(Vdq_out_V.value[1]));

#if(DMC_BUILDLEVEL == DMC_LEVEL_1)
        VS_FREQ_run(VsFreqHandle, motorVars.speedTraj_Hz);

        Vdq_out_V.value[0] = VS_FREQ_getVd_out(VsFreqHandle);
        Vdq_out_V.value[1] = VS_FREQ_getVq_out(VsFreqHandle);
#endif  // DMC_BUILDLEVEL == DMC_LEVEL_1

#if (DMC_BUILDLEVEL == DMC_LEVEL_4) // Control the CAN feedback sending
#if defined(CMD_CAN_ENABLE) ||  defined(CMD_CAN_FD_ENABLE)
        counterCAN_TX ++;
        if (counterCAN_TX >= numISRTicksPerCANTick)
        {
            counterCAN_TX = 0;
            canComm.flagTxDone = true;
        }
#endif  // CMD_CAN_ENABLE | CMD_CAN_FD_ENABLE
#endif // DMC_DUILDLEVEL == DMC_LEVEL_4

        //
        // set the phasor in the inverse Park transform
        //
        IPARK_setPhasor(iparkHandle, &phasor);

        //
        // run the inverse Park module
        //
        IPARK_run(iparkHandle, &Vdq_out_V, &Vab_out_V);

        //
        // setup the space vector generator (SVGEN) module
        //
        SVGEN_setup(svgenHandle, (1.0f / adcData.dcBus_V));

        //
        // run the space vector generator (SVGEN) module
        //
        SVGEN_run(svgenHandle, &Vab_out_V, &(pwmData.Vabc_pu));
    }
    else if(motorVars.flagEnableOffsetCalc == true)
    {
        runOffsetsCalculation();
    }

    if(HAL_getPwmEnableStatus(halHandle) == false)
    {
        //
        // clear PWM data
        //
        pwmData.Vabc_pu.value[0] = 0.0f;
        pwmData.Vabc_pu.value[1] = 0.0f;
        pwmData.Vabc_pu.value[2] = 0.0f;
    }

    //
    // write the PWM compare values
    //
    HAL_writePWMData(halHandle, &pwmData);

#ifdef STEP_RESPONSE_EN
    // Collect predefined data into arrays
    GRAPH_DATA(&gGraphVars, &gStepVars);
#endif  //  STEP_RESPONSE_EN

    motorVars.pwmISRCount++;

#ifdef DEBUG_INT_DAC_EN // Internal DAC debug
    // Scale values to plot on DAC
    dacval1 = (adcData.I_A.value[0] / USER_ADC_FULL_SCALE_CURRENT_A + 1) * 2048.0f;
    dacval2 = (angleFoc_rad * MATH_ONE_OVER_TWO_PI + 1) * 2048.0f;

    DAC_setShadowValue(myDAC0_BASE, dacval1);
    DAC_setShadowValue(myDAC1_BASE, dacval2);
#endif // !DEBUG_INT_DAC_EN

    return;
} // end of mainISR() function

void runOffsetsCalculation(void)
{
    uint16_t cnt;

    if(motorVars.flagEnableSys == true)
    {
        //
        // enable the PWM
        //
        HAL_enablePWM(halHandle);

        //
        // set the 3-phase output PWMs to 50% duty cycle
        //
        pwmData.Vabc_pu.value[0] = 0.0f;
        pwmData.Vabc_pu.value[1] = 0.0f;
        pwmData.Vabc_pu.value[2] = 0.0f;

        for(cnt = 0; cnt < USER_NUM_CURRENT_SENSORS; cnt++)
        {
            //
            // reset current offsets used
            //
            motorVars.offsets_I_A.value[cnt] = 0.0f;

            //
            // run current offset estimation
            //
            FILTER_FO_run(filterHandle_I[cnt],
                          adcData.I_A.value[cnt]);
        }

        offsetCalcCount++;

        if(offsetCalcCount >= offsetCalcWaitTime)
        {
            for(cnt = 0; cnt < USER_NUM_CURRENT_SENSORS; cnt++)
            {
                //
                // get calculated current offsets from filter
                //
                motorVars.offsets_I_A.value[cnt] =
                        FILTER_FO_get_y1(filterHandle_I[cnt]);

                //
                // clear current filters
                //
                FILTER_FO_setInitialConditions(filterHandle_I[cnt],
                                        motorVars.offsets_I_A.value[cnt],
                                        motorVars.offsets_I_A.value[cnt]);
            }


            offsetCalcCount = 0;
            motorVars.flagEnableOffsetCalc = false;

            //
            // disable the PWM
            //
            HAL_disablePWM(halHandle);
        }
    }

    return;
} // end of runOffsetsCalculation() function

#if defined(CMD_CAN_ENABLE)

__interrupt void canISR(void)
{
    uint32_t status;

    //
    // Read the CAN interrupt status to find the cause of the interrupt
    //
    status = CAN_getInterruptCause(halHandle->canHandle);


    //
    // If the cause is a controller status interrupt, then get the status
    //
    if(status == CAN_INT_INT0ID_STATUS)
    {
        //
        // Read the controller status.  This will return a field of status
        // error bits that can indicate various errors.  Error processing
        // is not done in this example for simplicity.  Refer to the
        // API documentation for details about the error status bits.
        // The act of reading this status will clear the interrupt.
        //
        status = CAN_getStatus(halHandle->canHandle);

        // Check to see if an error occurred.
        if(((status  & ~(CAN_STATUS_TXOK | CAN_STATUS_RXOK)) != 7) &&
           ((status  & ~(CAN_STATUS_TXOK | CAN_STATUS_RXOK)) != 0))
        {
            // Set a flag to indicate some errors may have occurred.
            canComm.errorFlag = 1;
        }

    }
    // Check if the cause is the transmit message object 1
    else if(status == TX_MSG_OBJ_ID)
    {
        //
        // Getting to this point means that the TX interrupt occurred on
        // message object 1, and the message TX is complete.  Clear the
        // message object interrupt.
        //
        CAN_clearInterruptStatus(CANA_BASE, TX_MSG_OBJ_ID);

        // Increment a counter to keep track of how many messages have been
        // sent.  In a real application this could be used to set flags to
        // indicate when a message is sent.
        canComm.txMsgCount++;

        // Since the message was sent, clear any error flags.
        canComm.errorFlag = 0;
    }
    //
    // Check if the cause is the receive message object 2
    else if(status == RX_MSG_OBJ_ID)
    {
        //
        // Get the received message
        //
        CAN_readMessage(halHandle->canHandle, RX_MSG_OBJ_ID, canComm.rxMsgData);

        //
        // Getting to this point means that the RX interrupt occurred on
        // message object 2, and the message RX is complete.  Clear the
        // message object interrupt.
        //
        CAN_clearInterruptStatus(halHandle->canHandle, RX_MSG_OBJ_ID);

        canComm.rxMsgCount++;
        canComm.flagRxDone = true;

        // Since the message was received, clear any error flags.
        canComm. errorFlag = 0;
    }
    // If something unexpected caused the interrupt, this would handle it.
    else
    {
        //
        // Spurious interrupt handling can go here.
        //
    }

    //
    // Clear the global interrupt flag for the CAN interrupt line
    //
    CAN_clearGlobalInterruptStatus(halHandle->canHandle, CAN_GLOBAL_INT_CANINT0);

    //
    // Acknowledge this interrupt located in group 9
    //
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP9);
}

void updateCANCmdFreq(void)
{
    uint16_t canData = 0;

    if(canComm.flagRxDone == true)
    {
        canComm.flagCmdRxRun = (bool)(canComm.rxMsgData[0]);
        canComm.flagStateRxRun = (bool)(canComm.rxMsgData[1]);

        canComm.speedRef_Hz = ((float32_t)((canComm.rxMsgData[2]<<8) +
                canComm.rxMsgData[3])) * canComm.speedConv_sf;

        canComm.speedFdb_Hz = ((float32_t)((canComm.rxMsgData[4]<<8) +
                canComm.rxMsgData[5])) * canComm.speedConv_sf;

        canComm.flagTxDone = true;
        canComm.flagRxDone = false;
    }

    if((canComm.flagTxDone == true) && (canComm.waitTimeCnt == 0))
    {
        canComm.txMsgData[0] = (uint16_t)(motorVars.flagRunCmdCAN);
        canComm.txMsgData[1] = (uint16_t)(motorVars.flagRunIdentAndOnLine);

        canData = (uint16_t)(motorVars.speedSetCAN_Hz * canComm.speedInv_sf);
        canComm.txMsgData[2] = (canData>>8) & 0x00FF;
        canComm.txMsgData[3] = canData & 0x00FF;


        canData = (uint16_t)(motorVars.speedFdbAbs_Hz * canComm.speedInv_sf);
        canComm.txMsgData[4] = (canData>>8) & 0x00FF;
        canComm.txMsgData[5] = canData & 0x00FF;

        CAN_sendMessage(CANA_BASE, TX_MSG_OBJ_ID, MSG_DATA_LENGTH,
                        (uint16_t *)(&canComm.txMsgData[0]));

        canComm.waitTimeCnt = canComm.waitTimeDelay;
        canComm.flagTxDone = false;
    }

    if(canComm.waitTimeCnt > 0)
    {
        canComm.waitTimeCnt--;
    }

    if(canComm.flagCmdEnable == true)
    {
        motorVars.flagRunIdentAndOnLine = canComm.flagCmdRxRun;
        motorVars.speedRef_Hz = canComm.speedRef_Hz;
    }

    return;
}

void updateCANCmd_LEVEL4(void)
{
    int16_t canTXBuffer = 0;

    if(canComm.flagRxDone == true)
    {
        // RX data frame
        // Motor run state, rxMsgData[0] & rxMsgData[1]
        canComm.flagCmdRxRun = (bool)(canComm.rxMsgData[0]);
        canComm.flagStateRxRun = (bool)(canComm.rxMsgData[1]);

        // Set speed, rxMsgData[2] & rxMsgData[3]
        canComm.speedRef_Hz = (float32_t)((int16_t)((canComm.rxMsgData[2]<<8) +
                canComm.rxMsgData[3])) * canComm.speedConv_sf;

        // Feedback speed, rxMsgData[4] & rxMsgData[5]
        canComm.speedFdb_Hz = (float32_t)((int16_t)((canComm.rxMsgData[4]<<8) +
        canComm.rxMsgData[5])) * canComm.speedConv_sf;

        canComm.flagRxDone = false;
    }

    // Update CAN TX data
    if(canComm.flagTxDone == true)
    {
        // Motor run state, rxMsgData[0] & rxMsgData[1]
        canComm.txMsgData[0] = (uint16_t)(motorVars.flagRunCmdCAN);
        canComm.txMsgData[1] = (uint16_t)(motorVars.flagRunIdentAndOnLine);

        // Set speed, rxMsgData[2] & rxMsgData[3]
        canTXBuffer = (int16_t)(motorVars.speedSetCAN_Hz * canComm.speedInv_sf);
        canComm.txMsgData[2] = (canTXBuffer>>8) & 0x00FF;
        canComm.txMsgData[3] = canTXBuffer & 0x00FF;
        // Feedback speed, rxMsgData[4] & rxMsgData[5]
        canTXBuffer = (int16_t)(motorVars.speedFdbAbs_Hz * canComm.speedInv_sf);
        //dummySpeed_Hz = dummySpeed_Hz + 0.1;
        //canTXBuffer = (int16_t)(dummySpeed_Hz*10);
        canComm.txMsgData[4] = (canTXBuffer>>8) & 0x00FF;
        canComm.txMsgData[5] = canTXBuffer & 0x00FF;

        // Send TX data (txMsgData[8]) via CAN
        CAN_sendMessage(CANA_BASE, TX_MSG_OBJ_ID, MSG_DATA_LENGTH,
                        (uint16_t *)(&canComm.txMsgData[0]));

        canComm.flagTxDone = false;
    }

    if(canComm.flagCmdEnable == true)
    {
        motorVars.flagRunIdentAndOnLine = canComm.flagCmdRxRun;
        motorVars.speedRef_Hz = canComm.speedRef_Hz;
    }

    return;
}
#elif defined(CMD_CAN_FD_ENABLE)

__interrupt void CANFDIntrISR1(void)
{
    uint32_t status;

   status =  MCAN_getIntrStatus(halHandle->mcanHandle);
    //error = status;
    canFdComm.errorFlag = 22;
    error = status;




    //
    // If the cause is a controller status interrupt, then get the status
    //

    if (status & MCAN_INTR_ERROR_BITS != 0  )
    {
        error = status+1;
        // Set a flag to indicate some errors may have occurred.
        canFdComm.errorFlag = 1;

        MCAN_clearIntrStatus(halHandle->mcanHandle, status);
    }



    // Check if the cause is the transmit message object 1
    else if(status == MCAN_IR_TC_MASK) // confirm if this is the correct way???
    {



        // Increment a counter to keep track of how many messages have been
        // sent.  In a real application this could be used to set flags to
        // indicate when a message is sent.
        canFdComm.txMsgCount++;

        // Since the message was sent, clear any error flags.
        canFdComm.errorFlag = 0;

        MCAN_clearIntrStatus(halHandle->mcanHandle, status);
    }
    //
    // Check if the cause is the receive message object 2
    else if(status == MCAN_IR_RF1N_MASK)   // confirm if this is the correct way ???
    {
        //
        // Get the received message
        //

        MCAN_clearIntrStatus(halHandle->mcanHandle, status);

        canFdComm.rxMsgCount++;
        canFdComm.flagRxDone = true;

        // Since the message was received, clear any error flags.
        canFdComm.errorFlag = 0;
    }
    // If something unexpected caused the interrupt, this would handle it.
    else
    {

        canFdComm.errorFlag = 2;
        canFdComm.rxMsgCount++;
        canFdComm.flagRxDone = true;

        //
        // Spurious interrupt handling can go here.
        //

        MCAN_clearIntrStatus(halHandle->mcanHandle, status);
    }


    //
      //   Acknowledge this interrupt located in group 9
      //
      Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP9);

}


void updateCANFdCmdFreq(HAL_Handle handle)
{
    HAL_Obj  *obj = (HAL_Obj *)handle;

    uint16_t canFdData = 0;
    RxFS.num = 1;
    uint16_t iterator;
//    uint32_t tx_status = 0;
   // static uint16_t loopCounter;
    canFdComm.flagRxDone = true;
    if(canFdComm.flagRxDone == true)
    {

        MCAN_getRxFIFOStatus(obj->mcanHandle, &RxFS);
        if(RxFS.fillLvl)
        {


        MCAN_readMsgRam(obj->mcanHandle, MCAN_MEM_TYPE_FIFO, 0U,
                                        MCAN_RX_FIFO_NUM_1, &rxMsg);

        for (iterator=0; iterator< 6; iterator++)
        {
            canFdComm.rxMsgData[iterator] = rxMsg.data[iterator];
        }
        canFdComm.flagCmdRxRun = (bool)(canFdComm.rxMsgData[0]);
        canFdComm.flagStateRxRun = (bool)(canFdComm.rxMsgData[1]);

        canFdComm.speedRef_Hz = ((float32_t)((canFdComm.rxMsgData[2]<<8) +
        canFdComm.rxMsgData[3])) * canFdComm.speedConv_sf;

        canFdComm.speedFdb_Hz = ((float32_t)((canFdComm.rxMsgData[4]<<8) +
        canFdComm.rxMsgData[5])) * canFdComm.speedConv_sf;

       // rxMsg1[++loopCounter] = rxMsg;

        RxFS.num = MCAN_RX_FIFO_NUM_1;
        MCAN_writeRxFIFOAck(obj->mcanHandle, MCAN_RX_FIFO_NUM_1,
                            RxFS.getIdx);
        canFdComm.rxMsgCount++;
        }

        canFdComm.flagTxDone = true;
        //canFdComm.flagRxDone = false;
    }

    if((canFdComm.flagTxDone == true) && (canFdComm.waitTimeCnt == 0))
    {
        canFdComm.txMsgData[0] = (uint16_t)(motorVars.flagRunCmdCAN);  // Do we need a different flag for CANFD ?? // Update MotorVars ??
        canFdComm.txMsgData[1] = (uint16_t)(motorVars.flagRunIdentAndOnLine);

        canFdData = (uint16_t)(motorVars.speedSetCAN_Hz * canFdComm.speedInv_sf);
        canFdComm.txMsgData[2] = (canFdData>>8) & 0x00FF;
        canFdComm.txMsgData[3] = canFdData & 0x00FF;


        canFdData = (uint16_t)(motorVars.speedFdbAbs_Hz * canFdComm.speedInv_sf);
        canFdComm.txMsgData[4] = (canFdData>>8) & 0x00FF;
        canFdComm.txMsgData[5] = canFdData & 0x00FF;

        for (iterator=0; iterator< 6; iterator++)
        {
                     txMsg.data[iterator] = canFdComm.txMsgData[iterator];
        }

        MCAN_writeMsgRam(obj->mcanHandle, MCAN_MEM_TYPE_BUF, 0,
                             &txMsg);

        HWREG(obj->mcanHandle + MCAN_TXBAR) = 0x00000001;
        /*MCAN_txBufAddReq(obj->mcanHandle,0);*/

        /*while(!tx_status)
        {
            tx_status =MCAN_getTxBufTransmissionStatus(obj->mcanHandle);
        }*/
        while(HWREG(obj->mcanHandle + MCAN_TXBTO) == 0x00000000)
            {
            }
        canFdComm.txMsgCount++;

        canFdComm.waitTimeCnt = canFdComm.waitTimeDelay;
        //canFdComm.flagTxDone = false;
    }

    if(canFdComm.waitTimeCnt > 0)
    {
        canFdComm.waitTimeCnt--;
    }

    if(canFdComm.flagCmdEnable == true)
    {
        motorVars.flagRunIdentAndOnLine = canFdComm.flagCmdRxRun;
        motorVars.speedRef_Hz = canFdComm.speedRef_Hz;
    }

    return;
}
#endif  // !CMD_CAN_ENABLE, !CMD_CAN_FD_ENABLE

//
// End of File
//

