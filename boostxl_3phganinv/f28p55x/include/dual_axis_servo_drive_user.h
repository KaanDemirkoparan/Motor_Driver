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
// FILE:    dual_axis_servo_drive_user.h
//
// TITLE:   motor parameters definition
//
// Target Family: F2837x/F28004x/F28P55x
//
//-----------------------------------------------------------------------------


#ifndef DUAL_AXIS_SERVO_DRIVE_USER_H
#define DUAL_AXIS_SERVO_DRIVE_USER_H

//
// Include project specific include files.
//

//
// PWM, SAMPLING FREQUENCY and Current Loop Band width definitions for motor 1
// motor 2, can be set separately
//
#define DM_PWM_FREQUENCY        10   // in KHz

//
// Analog scaling with ADC
//
#define ADC_PU_SCALE_FACTOR         0.000244140625     // 1/2^12, 12bits ADC
#define ADC_PU_PPB_SCALE_FACTOR     0.000488281250     // 1/2^11, 12bits ADC

#if defined(LPVER_RELEASE)
//
// ADC and PWM Related defines for M1
// Current sensing
#define M1_IU_ADC_BASE         ADCA_BASE           //A2*/B6/C9, NC: Set up based board
#define M1_IV_ADC_BASE         ADCB_BASE           //B3*, NC: Set up based board
#define M1_IW_ADC_BASE         ADCC_BASE           //C1*/E11, NC: Set up based board
#define M1_VDC_ADC_BASE        ADCD_BASE           //A6/D14*/E14, NC: Set up based board

#define M1_IU_ADCRESULT_BASE   ADCARESULT_BASE     //A2, NC: Set up based board
#define M1_IV_ADCRESULT_BASE   ADCBRESULT_BASE     //B3, NC: Set up based board
#define M1_IW_ADCRESULT_BASE   ADCCRESULT_BASE     //C1, NC: Set up based board
#define M1_VDC_ADCRESULT_BASE  ADCDRESULT_BASE     //D14, NC: Set up based board

#define M1_IU_ADC_CH_NUM       ADC_CH_ADCIN2       //A2, NC: Set up based board
#define M1_IV_ADC_CH_NUM       ADC_CH_ADCIN3       //B3, NC: Set up based board
#define M1_IW_ADC_CH_NUM       ADC_CH_ADCIN1       //C1, NC: Set up based board
#define M1_VDC_ADC_CH_NUM      ADC_CH_ADCIN14      //D14, NC: Set up based board

#define M1_IU_ADC_SOC_NUM      ADC_SOC_NUMBER1     //A2, NC: Set up based board
#define M1_IV_ADC_SOC_NUM      ADC_SOC_NUMBER1     //B3, NC: Set up based board
#define M1_IW_ADC_SOC_NUM      ADC_SOC_NUMBER1     //C1, NC: Set up based board
#define M1_VDC_ADC_SOC_NUM     ADC_SOC_NUMBER1     //D14, NC: Set up based board

#define M1_IU_ADC_PPB_NUM      ADC_PPB_NUMBER1     // NC: Set up based board
#define M1_IV_ADC_PPB_NUM      ADC_PPB_NUMBER1     // NC: Set up based board
#define M1_IW_ADC_PPB_NUM      ADC_PPB_NUMBER1     // NC: Set up based board
#define M1_VDC_ADC_PPB_NUM     ADC_PPB_NUMBER1     // NC: Set up based board

//#define M1_U_CMPSS_BASE      CMPSS1_BASE         //A2/CMP1_P0: Set up based board
#define M1_U_CMPSS_BASE        CMPSS3_BASE         //Not used
#define M1_V_CMPSS_BASE        CMPSS3_BASE         //B3/CMP3_P3: Set up based board
#define M1_W_CMPSS_BASE        CMPSS4_BASE         //C1/CMP4_P2: Set up based board

#define M1_ADC_TRIGGER_SOC     ADC_TRIGGER_EPWM1_SOCA  // NC: Set up based board

#define M1_U_PWM_BASE          EPWM1_BASE          //NC: Set up based board
#define M1_V_PWM_BASE          EPWM2_BASE          //NC: Set up based board
#define M1_W_PWM_BASE          EPWM6_BASE          //NC: Set up based board

#define M1_INT_PWM             INT_EPWM1           //NC: Set up based board

#define M1_QEP_BASE            EQEP1_BASE          //NC: Set up based board

#define M1_SPI_BASE            SPIA_BASE           //NC: Set up based board

#define M1_IFB_U      ADC_readResult(M1_IU_ADCRESULT_BASE, M1_IU_ADC_SOC_NUM)
#define M1_IFB_V      ADC_readResult(M1_IV_ADCRESULT_BASE, M1_IV_ADC_SOC_NUM)
#define M1_IFB_W      ADC_readResult(M1_IW_ADCRESULT_BASE, M1_IW_ADC_SOC_NUM)

#define M1_VDC        ADC_readResult(M1_VDC_ADCRESULT_BASE, M1_VDC_ADC_SOC_NUM)

#define M1_IFB_U_PPB  ADC_readPPBResult(M1_IU_ADCRESULT_BASE, M1_IU_ADC_PPB_NUM)
#define M1_IFB_V_PPB  ADC_readPPBResult(M1_IV_ADCRESULT_BASE, M1_IV_ADC_PPB_NUM)
#define M1_IFB_W_PPB  ADC_readPPBResult(M1_IW_ADCRESULT_BASE, M1_IW_ADC_PPB_NUM)

#define M1_VDC_PPB    ADC_readPPBResult(M1_VDC_ADCRESULT_BASE, M1_VDC_ADC_PPB_NUM)

// Voltage sensing (Not used in this project)
#define M1_VU_ADC_BASE         ADCB_BASE           //B2*/C6/E12, NC: Set up based board
#define M1_VV_ADC_BASE         ADCA_BASE           //A11*/B10/C0, NC: Set up based board
#define M1_VW_ADC_BASE         ADCC_BASE           //A14/B14/C4*, NC: Set up based board

#define M1_VU_ADCRESULT_BASE   ADCBRESULT_BASE     //B2, NC: Set up based board
#define M1_VV_ADCRESULT_BASE   ADCARESULT_BASE     //A11, NC: Set up based board
#define M1_VW_ADCRESULT_BASE   ADCCRESULT_BASE     //C4, NC: Set up based board

#define M1_VU_ADC_CH_NUM       ADC_CH_ADCIN2       //B2, NC: Set up based board
#define M1_VV_ADC_CH_NUM       ADC_CH_ADCIN11      //A11, NC: Set up based board
#define M1_VW_ADC_CH_NUM       ADC_CH_ADCIN4       //C4, NC: Set up based board

#define M1_VU_ADC_SOC_NUM      ADC_SOC_NUMBER4     //B2, NC: Set up based board
#define M1_VV_ADC_SOC_NUM      ADC_SOC_NUMBER4     //A11, NC: Set up based board
#define M1_VW_ADC_SOC_NUM      ADC_SOC_NUMBER4     //C4, NC: Set up based board

#define M1_VFB_U      ADC_readResult(M1_VU_ADCRESULT_BASE, M1_VU_ADC_SOC_NUM)
#define M1_VFB_V      ADC_readResult(M1_VV_ADCRESULT_BASE, M1_VV_ADC_SOC_NUM)
#define M1_VFB_W      ADC_readResult(M1_VW_ADCRESULT_BASE, M1_VW_ADC_SOC_NUM)

//
// ADC and PWM Related defines for M2
// Current sensing
#define M2_IU_ADC_BASE         ADCC_BASE           //B4/C8*, NC: Set up based board
#define M2_IV_ADC_BASE         ADCD_BASE           //B5/D15*/E15, NC: Set up based board
#define M2_IW_ADC_BASE         ADCA_BASE           //A12*/C5, NC: Set up based board
#define M2_VDC_ADC_BASE        ADCB_BASE           //B0*/C11, NC: Set up based board

#define M2_IU_ADCRESULT_BASE   ADCCRESULT_BASE     //C8*, NC: Set up based board
#define M2_IV_ADCRESULT_BASE   ADCDRESULT_BASE     //D15*, NC: Set up based board
#define M2_IW_ADCRESULT_BASE   ADCARESULT_BASE     //A12*, NC: Set up based board
#define M2_VDC_ADCRESULT_BASE  ADCBRESULT_BASE     //B0*, NC: Set up based board

#define M2_IU_ADC_CH_NUM       ADC_CH_ADCIN8       //C8, NC: Set up based board
#define M2_IV_ADC_CH_NUM       ADC_CH_ADCIN15      //D15, NC: Set up based board
#define M2_IW_ADC_CH_NUM       ADC_CH_ADCIN12      //A12, NC: Set up based board
#define M2_VDC_ADC_CH_NUM      ADC_CH_ADCIN11      //B0, NC: Set up based board

#define M2_IU_ADC_SOC_NUM      ADC_SOC_NUMBER2     //C8, NC: Set up based board
#define M2_IV_ADC_SOC_NUM      ADC_SOC_NUMBER2     //D15, NC: Set up based board
#define M2_IW_ADC_SOC_NUM      ADC_SOC_NUMBER2     //A12, NC: Set up based board
#define M2_VDC_ADC_SOC_NUM     ADC_SOC_NUMBER2     //B0, NC: Set up based board

#define M2_IU_ADC_PPB_NUM      ADC_PPB_NUMBER2     //NC: Set up based board
#define M2_IV_ADC_PPB_NUM      ADC_PPB_NUMBER2     //NC: Set up based board
#define M2_IW_ADC_PPB_NUM      ADC_PPB_NUMBER2     //NC: Set up based board
#define M2_VDC_ADC_PPB_NUM     ADC_PPB_NUMBER2     //NC: Set up based board

//#define M2_U_CMPSS_BASE      CMPSS4_BASE         // C8/CMP4_P0: Set up based board
#define M2_U_CMPSS_BASE        CMPSS1_BASE         // Not used
#define M2_V_CMPSS_BASE        CMPSS1_BASE         // D15/CMP1_P5: Set up based board
#define M2_W_CMPSS_BASE        CMPSS2_BASE         // A12/CMP2_P0: Set up based board

#define M2_ADC_TRIGGER_SOC     ADC_TRIGGER_EPWM7_SOCA  // NC: Set up based board

#define M2_U_PWM_BASE          EPWM7_BASE          // NC: Set up based board
#define M2_V_PWM_BASE          EPWM4_BASE          // NC: Set up based board
#define M2_W_PWM_BASE          EPWM5_BASE          // NC: Set up based board

#define M2_INT_PWM             INT_EPWM7           // NC: Set up based board

#define M2_QEP_BASE            EQEP3_BASE          // NC: Set up based board

#define M2_SPI_BASE            SPIB_BASE           // NC: Set up based board

#define M2_IFB_U        ADC_readResult(M2_IU_ADCRESULT_BASE, M2_IU_ADC_SOC_NUM)
#define M2_IFB_V        ADC_readResult(M2_IV_ADCRESULT_BASE, M2_IV_ADC_SOC_NUM)
#define M2_IFB_W        ADC_readResult(M2_IW_ADCRESULT_BASE, M2_IW_ADC_SOC_NUM)
#define M2_VDC          ADC_readResult(M2_VDC_ADCRESULT_BASE, M2_VDC_ADC_SOC_NUM)

#define M2_IFB_U_PPB    ADC_readPPBResult(M2_IU_ADCRESULT_BASE, M2_IU_ADC_PPB_NUM)
#define M2_IFB_V_PPB    ADC_readPPBResult(M2_IV_ADCRESULT_BASE, M2_IV_ADC_PPB_NUM)
#define M2_IFB_W_PPB    ADC_readPPBResult(M2_IW_ADCRESULT_BASE, M2_IW_ADC_PPB_NUM)
#define M2_VDC_PPB      ADC_readPPBResult(M2_VDC_ADCRESULT_BASE, M2_VDC_ADC_PPB_NUM)

// Voltage sensing (Not used)
#define M2_VU_ADC_BASE         ADCC_BASE           //A10/B1/C10*, NC: Set up based board
#define M2_VV_ADC_BASE         ADCA_BASE           //A5*, NC: Set up based board
#define M2_VW_ADC_BASE         ADCB_BASE           //A4/B8*, NC: Set up based board

#define M2_VU_ADCRESULT_BASE   ADCCRESULT_BASE     //C10*, NC: Set up based board
#define M2_VV_ADCRESULT_BASE   ADCARESULT_BASE     //A5*, NC: Set up based board
#define M2_VW_ADCRESULT_BASE   ADCBRESULT_BASE     //B8*, NC: Set up based board

#define M2_VU_ADC_CH_NUM       ADC_CH_ADCIN10      //C10, NC: Set up based board
#define M2_VV_ADC_CH_NUM       ADC_CH_ADCIN5       //A5, NC: Set up based board
#define M2_VW_ADC_CH_NUM       ADC_CH_ADCIN8       //B8, NC: Set up based board

#define M2_VU_ADC_SOC_NUM      ADC_SOC_NUMBER5     //C10, NC: Set up based board
#define M2_VV_ADC_SOC_NUM      ADC_SOC_NUMBER5     //A5, NC: Set up based board
#define M2_VW_ADC_SOC_NUM      ADC_SOC_NUMBER5     //B8, NC: Set up based board

#define M2_VFB_U      ADC_readResult(M2_VU_ADCRESULT_BASE, M2_VU_ADC_SOC_NUM)
#define M2_VFB_V      ADC_readResult(M2_VV_ADCRESULT_BASE, M2_VV_ADC_SOC_NUM)
#define M2_VFB_W      ADC_readResult(M2_VW_ADCRESULT_BASE, M2_VW_ADC_SOC_NUM)


#define  LPD_RED_LED1           47          // NC: Set up based board, Not used
#define  LPD_GREEN_LED2         51          // NC: Set up based board, Not used
#define  LPD_BLUE_LED2          LPD_GREEN_LED2

#define  LPD_LED1_WAIT_TIME     800
#define  LPD_LED2_WAIT_TIME     400

//
//
// define EN_GATE and SPI_CS pin for DRV device for Motor 1
//
#define M1_EN_GATE_GPIO         21          // NC: nEN_uc, Set up based board
#define M1_nFAULT_GPIO          22          // NC: OT, Set up based board
#define M1_XBAR_INPUT_GPIO      22          // NC: OT, Set up based board
#define M1_XBAR_INPUT_NUM       XBAR_INPUT1
#define M1_CLR_FAULT_GPIO       47          // Not used

//
// define EN_GATE and SPI_CS pin for DRV device for Motor 2
//
#define M2_EN_GATE_GPIO         20          // NC: nEN_uc, Set up based board
#define M2_nFAULT_GPIO          62          // NC: Set up based board
#define M2_XBAR_INPUT_GPIO      62          // NC: Set up based board
#define M2_XBAR_INPUT_NUM       XBAR_INPUT2
#define M2_CLR_FAULT_GPIO       51          // Not used

// LPVER_TEST039, only for development phase
#elif defined(LPVER_TEST039)
//
// ADC and PWM Related defines for M1
//
#define M1_IU_ADC_BASE         ADCA_BASE           //A11*/B10/C0, NC: Set up based board
#define M1_IV_ADC_BASE         ADCB_BASE           //B12*/C2, NC: Set up based board
#define M1_IW_ADC_BASE         ADCC_BASE           //A7/B30/C3*/D12/E30, NC: Set up based board
#define M1_VDC_ADC_BASE        ADCD_BASE           //A7/D14*/E14, NC: Set up based board

#define M1_IU_ADCRESULT_BASE   ADCARESULT_BASE     //A11, NC: Set up based board
#define M1_IV_ADCRESULT_BASE   ADCBRESULT_BASE     //B12, NC: Set up based board
#define M1_IW_ADCRESULT_BASE   ADCCRESULT_BASE     //C3, NC: Set up based board
#define M1_VDC_ADCRESULT_BASE  ADCDRESULT_BASE     //D14, NC: Set up based board

#define M1_IU_ADC_CH_NUM       ADC_CH_ADCIN11      //A11, NC: Set up based board
#define M1_IV_ADC_CH_NUM       ADC_CH_ADCIN12      //B12, NC: Set up based board
#define M1_IW_ADC_CH_NUM       ADC_CH_ADCIN3       //C3, NC: Set up based board
#define M1_VDC_ADC_CH_NUM      ADC_CH_ADCIN14      //D14, NC: Set up based board

#define M1_IU_ADC_SOC_NUM      ADC_SOC_NUMBER1     //A11, NC: Set up based board
#define M1_IV_ADC_SOC_NUM      ADC_SOC_NUMBER1     //B12, NC: Set up based board
#define M1_IW_ADC_SOC_NUM      ADC_SOC_NUMBER1     //C3, NC: Set up based board
#define M1_VDC_ADC_SOC_NUM     ADC_SOC_NUMBER1     //D14, NC: Set up based board

#define M1_IU_ADC_PPB_NUM      ADC_PPB_NUMBER1     // NC: Set up based board
#define M1_IV_ADC_PPB_NUM      ADC_PPB_NUMBER1     // NC: Set up based board
#define M1_IW_ADC_PPB_NUM      ADC_PPB_NUMBER1     // NC: Set up based board
#define M1_VDC_ADC_PPB_NUM     ADC_PPB_NUMBER1     // NC: Set up based board

//#define M1_U_CMPSS_BASE        CMPSS1_BASE         // A11/CMP1_P0: Set up based board
#define M1_U_CMPSS_BASE        CMPSS3_BASE         // Not used
#define M1_V_CMPSS_BASE        CMPSS3_BASE         // B12/CMP3_P1: Set up based board
#define M1_W_CMPSS_BASE        CMPSS4_BASE         // C3/CMP4_P1: Set up based board

#define M1_ADC_TRIGGER_SOC     ADC_TRIGGER_EPWM1_SOCA  // NC: Set up based board

#define M1_U_PWM_BASE          EPWM1_BASE          // NC: Set up based board
#define M1_V_PWM_BASE          EPWM2_BASE          // NC: Set up based board
#define M1_W_PWM_BASE          EPWM6_BASE          // NC: Set up based board

#define M1_INT_PWM             INT_EPWM1           // NC: Set up based board

#define M1_QEP_BASE            EQEP1_BASE          // NC: Set up based board

#define M1_SPI_BASE            SPIA_BASE           // NC: Set up based board

#define M1_IFB_U      ADC_readResult(M1_IU_ADCRESULT_BASE, M1_IU_ADC_SOC_NUM)
#define M1_IFB_V      ADC_readResult(M1_IV_ADCRESULT_BASE, M1_IV_ADC_SOC_NUM)
#define M1_IFB_W      ADC_readResult(M1_IW_ADCRESULT_BASE, M1_IW_ADC_SOC_NUM)

#define M1_VDC        ADC_readResult(M1_VDC_ADCRESULT_BASE, M1_VDC_ADC_SOC_NUM)

#define M1_IFB_U_PPB  ADC_readPPBResult(M1_IU_ADCRESULT_BASE, M1_IU_ADC_PPB_NUM)
#define M1_IFB_V_PPB  ADC_readPPBResult(M1_IV_ADCRESULT_BASE, M1_IV_ADC_PPB_NUM)
#define M1_IFB_W_PPB  ADC_readPPBResult(M1_IW_ADCRESULT_BASE, M1_IW_ADC_PPB_NUM)

#define M1_VDC_PPB    ADC_readPPBResult(M1_VDC_ADCRESULT_BASE, M1_VDC_ADC_PPB_NUM)

//
// ADC and PWM Related defines for M2
//
#define M2_IU_ADC_BASE         ADCC_BASE           //B4/C8*, NC: Set up based board
#define M2_IV_ADC_BASE         ADCD_BASE           //B5/D15*/E15, NC: Set up based board
#define M2_IW_ADC_BASE         ADCA_BASE           //A12*/C5, NC: Set up based board
#define M2_VDC_ADC_BASE        ADCB_BASE           //B11*/D16/E16, NC: Set up based board

#define M2_IU_ADCRESULT_BASE   ADCCRESULT_BASE     //C8*, NC: Set up based board
#define M2_IV_ADCRESULT_BASE   ADCDRESULT_BASE     //D15*, NC: Set up based board
#define M2_IW_ADCRESULT_BASE   ADCARESULT_BASE     //A12*, NC: Set up based board
#define M2_VDC_ADCRESULT_BASE  ADCBRESULT_BASE     //B11*, NC: Set up based board

#define M2_IU_ADC_CH_NUM       ADC_CH_ADCIN8       //C8, NC: Set up based board
#define M2_IV_ADC_CH_NUM       ADC_CH_ADCIN15      //D15, NC: Set up based board
#define M2_IW_ADC_CH_NUM       ADC_CH_ADCIN12      //A12, NC: Set up based board
#define M2_VDC_ADC_CH_NUM      ADC_CH_ADCIN11      //B11, NC: Set up based board

#define M2_IU_ADC_SOC_NUM      ADC_SOC_NUMBER2     //C8, NC: Set up based board
#define M2_IV_ADC_SOC_NUM      ADC_SOC_NUMBER2     //D15, NC: Set up based board
#define M2_IW_ADC_SOC_NUM      ADC_SOC_NUMBER2     //A12, NC: Set up based board
#define M2_VDC_ADC_SOC_NUM     ADC_SOC_NUMBER2     //B11, NC: Set up based board

#define M2_IU_ADC_PPB_NUM      ADC_PPB_NUMBER2     // NC: Set up based board
#define M2_IV_ADC_PPB_NUM      ADC_PPB_NUMBER2     // NC: Set up based board
#define M2_IW_ADC_PPB_NUM      ADC_PPB_NUMBER2     // NC: Set up based board
#define M2_VDC_ADC_PPB_NUM     ADC_PPB_NUMBER2     // NC: Set up based board

//#define M2_U_CMPSS_BASE        CMPSS4_BASE         // C8/CMP4_P0: Set up based board
#define M2_U_CMPSS_BASE        CMPSS1_BASE         // Not used
#define M2_V_CMPSS_BASE        CMPSS1_BASE         // D15/CMP1_P5: Set up based board
//#define M2_W_CMPSS_BASE        CMPSS1_BASE         // Not used
#define M2_W_CMPSS_BASE        CMPSS2_BASE         // A12/CMP2_P0: Set up based board

#define M2_ADC_TRIGGER_SOC     ADC_TRIGGER_EPWM7_SOCA  // NC: Set up based board

#define M2_U_PWM_BASE          EPWM7_BASE          // NC: Set up based board
#define M2_V_PWM_BASE          EPWM4_BASE          // NC: Set up based board
#define M2_W_PWM_BASE          EPWM5_BASE          // NC: Set up based board

#define M2_INT_PWM             INT_EPWM7           // NC: Set up based board

#define M2_QEP_BASE            EQEP2_BASE          // NC: Set up based board

#define M2_SPI_BASE            SPIB_BASE           // NC: Set up based board

#define M2_IFB_U        ADC_readResult(M2_IU_ADCRESULT_BASE, M2_IU_ADC_SOC_NUM)
#define M2_IFB_V        ADC_readResult(M2_IV_ADCRESULT_BASE, M2_IV_ADC_SOC_NUM)
#define M2_IFB_W        ADC_readResult(M2_IW_ADCRESULT_BASE, M2_IW_ADC_SOC_NUM)
#define M2_VDC          ADC_readResult(M2_VDC_ADCRESULT_BASE, M2_VDC_ADC_SOC_NUM)

#define M2_IFB_U_PPB    ADC_readPPBResult(M2_IU_ADCRESULT_BASE, M2_IU_ADC_PPB_NUM)
#define M2_IFB_V_PPB    ADC_readPPBResult(M2_IV_ADCRESULT_BASE, M2_IV_ADC_PPB_NUM)
#define M2_IFB_W_PPB    ADC_readPPBResult(M2_IW_ADCRESULT_BASE, M2_IW_ADC_PPB_NUM)
#define M2_VDC_PPB      ADC_readPPBResult(M2_VDC_ADCRESULT_BASE, M2_VDC_ADC_PPB_NUM)


#define  LPD_RED_LED1           23          // NC: Set up based board, Not used
#define  LPD_GREEN_LED2         22          // NC: Set up based board, Not used
#define  LPD_BLUE_LED2          LPD_GREEN_LED2

#define  LPD_LED1_WAIT_TIME     800
#define  LPD_LED2_WAIT_TIME     400

//
// define EN_GATE and SPI_CS pin for DRV device for Motor 1
//
#define M1_EN_GATE_GPIO         37          // NC: nEN_uc, Set up based board
#define M1_nFAULT_GPIO          33          // NC: OT, Set up based board
#define M1_XBAR_INPUT_GPIO      33          // NC: OT, Set up based board
#define M1_XBAR_INPUT_NUM       XBAR_INPUT1
#define M1_CLR_FAULT_GPIO       22          // Not used

//
// define EN_GATE and SPI_CS pin for DRV device for Motor 2
//
#define M2_EN_GATE_GPIO         20          // NC: nEN_uc, Set up based board
#define M2_nFAULT_GPIO          21          // NC: Set up based board
#define M2_XBAR_INPUT_GPIO      21          // NC: Set up based board
#define M2_XBAR_INPUT_NUM       XBAR_INPUT2
#define M2_CLR_FAULT_GPIO       23          // Not used

#else   // LPVER_
#error Select the right LP version
#endif  // LPVER_
//
// Motor_1 Parameters
//

//
// PWM, SAMPLING FREQUENCY and Current Loop Band width definitions
//
#define M1_PWM_FREQUENCY           DM_PWM_FREQUENCY   // in KHz

#if(SAMPLING_METHOD == SINGLE_SAMPLING)
#define M1_ISR_FREQUENCY           (M1_PWM_FREQUENCY)

#elif(SAMPLING_METHOD == DOUBLE_SAMPLING)
#define M1_ISR_FREQUENCY           (2*M1_PWM_FREQUENCY)

#endif

//
// Keep PWM Period same between single sampling and double sampling
//
#define M1_INV_PWM_TICKS         ((SYSTEM_FREQUENCY / M1_PWM_FREQUENCY) * 1000)
#define M1_INV_PWM_DB            (200.0)
#define M1_QEP_UNIT_TIMER_TICKS  (SYSTEM_FREQUENCY/(2*M1_PWM_FREQUENCY) * 1000)

#define M1_INV_PWM_TBPRD         (M1_INV_PWM_TICKS / 2)
#define M1_INV_PWM_HALF_TBPRD    (M1_INV_PWM_TBPRD / 2)
#define M1_SAMPLING_FREQ         (M1_ISR_FREQUENCY * 1000)
#define M1_CUR_LOOP_BANDWIDTH    (2.0F * PI * M1_SAMPLING_FREQ / 100)

#define M1_TPWM_CARRIER          (1000.0 / M1_PWM_FREQUENCY)    //in uSec

//
// FCL Computation time predetermined from library
//
#define M1_FCL_COMPUTATION_TIME  (1.00)  //in uS

//
// set the motor parameters to the one available
//
#define M1_ENCODER_LINES         1000 // Encoder lines for Tekic

//
// Define the electrical motor parameters
//
#define M1_RS      0.381334811     // Stator resistance (ohm)
#define M1_RR      NULL            // Rotor resistance (ohm)
#define M1_LS      0.000169791776  // Stator inductance (H)
#define M1_LD      M1_LS           // Stator d-axis inductance (H)
#define M1_LQ      M1_LS           // Stator q-axis inductance (H)
#define M1_LR      NULL            // Rotor inductance (H)
#define M1_LM      NULL            // Magnetizing inductance (H)
#define M1_KB      0.039           // BEMF Constant (V/Hz)
#define M1_POLES   8               // Number of poles

//
// NOTE:-
// Base voltage and base current information from TIDA-00909 doc is
// based off of an ADC that works at 3.3V reference.
// The base current = 16.5A (for a spread of 3.3V - 1.65V = 1.65V)
// The base voltage  = 81.5 / sqrt(3)=
// Define the base quantites
//
#define M1_BASE_VOLTAGE     47.05 // Base peak phase voltage (volt), Vdc/sqrt(3)
#define M1_BASE_CURRENT     16.5  // Base peak phase current (amp),
                                  // the maximum measurable peak current
#define M1_BASE_TORQUE      NULL  // Base torque (N.m)
#define M1_BASE_FLUX        NULL  // Base flux linkage (volt.sec/rad)
#define M1_BASE_FREQ        250   // Base electrical frequency (Hz)
#define M1_MAXIMUM_CURRENT  5.0   // Motor maximum torque current (amp)

#define M1_MAXIMUM_VOLTAGE  36.0    // DC bus maximum voltage (V)
#define M1_MINIMUM_VOLTAGE  5.0     // DC bus minimum voltage (V)

#define M1_VDCBUS_MAX       50.0    // maximum dc bus voltage for motor
#define M1_VDCBUS_MIN       10.0    // minimum dc bus voltage for motor

//
// Current sensors scaling
// 1.0pu current ==> 9.95A -> 2048 counts ==> 8A -> 1647
//
#define M1_CURRENT_SCALE(A)   (2048 * A / M1_BASE_CURRENT)

//
// Analog scaling with ADC
//
#define M1_ADC_PU_SCALE_FACTOR          0.000244140625     // 1/2^12
#define M1_ADC_PU_PPB_SCALE_FACTOR      0.000488281250     // 1/2^11

//
// Current Scale
//
#define M1_MAXIMUM_SCALE_CURRENT        33.0
#define M1_CURRENT_SF                   (M1_MAXIMUM_SCALE_CURRENT / 4096.0)
#define M1_CURRENT_INV_SF               (4096.0 / M1_MAXIMUM_SCALE_CURRENT)

//
// Voltage Scale
//
#define M1_MAXIMUM_SCALE_VOLATGE        81.5
#define M1_VOLTAGE_SF                   (M1_MAXIMUM_SCALE_VOLATGE / 4096.0)
#define M1_VOLTAGE_INV_SF               (4096.0 / M1_MAXIMUM_SCALE_VOLATGE)

//
// Motor_2 Parameters
//

//
// PWM, SAMPLING FREQUENCY and Current Loop Band width definitions
//
#define M2_PWM_FREQUENCY           DM_PWM_FREQUENCY   // in KHz

#if(SAMPLING_METHOD == SINGLE_SAMPLING)
#define M2_ISR_FREQUENCY           (M2_PWM_FREQUENCY)

#elif(SAMPLING_METHOD == DOUBLE_SAMPLING)
#define M2_ISR_FREQUENCY           (2 * M2_PWM_FREQUENCY)

#endif

//
// Keep PWM Period same between single sampling and double sampling
//
#define M2_INV_PWM_TICKS         ((SYSTEM_FREQUENCY / M2_PWM_FREQUENCY) * 1000)
#define M2_INV_PWM_DB            (200.0)
#define M2_QEP_UNIT_TIMER_TICKS  ((SYSTEM_FREQUENCY)/(2*M2_PWM_FREQUENCY)*1000)

#define M2_INV_PWM_TBPRD         (M2_INV_PWM_TICKS / 2)
#define M2_INV_PWM_HALF_TBPRD    (M2_INV_PWM_TBPRD / 2)
#define M2_SAMPLING_FREQ         (M2_ISR_FREQUENCY * 1000)
#define M2_CUR_LOOP_BANDWIDTH    (2.0F * PI * M2_SAMPLING_FREQ / 100)

#define M2_TPWM_CARRIER          (1000.0 / M2_PWM_FREQUENCY)    //in uSec

//
// FCL Computation time predetermined from library
// tests on F28004x
//
#define M2_FCL_COMPUTATION_TIME    (1.00)  //in uS

//
// set the motor parameters to the one available
//
#define M2_ENCODER_LINES            1000        // Encoder lines

//
// Define the electrical motor parameters
//
#define M2_RS       0.381334811     // Stator resistance (ohm)
#define M2_RR       NULL            // Rotor resistance (ohm)
#define M2_LS       0.000169791776  // Stator inductance (H)
#define M2_LD       M1_LS           // Stator d-axis inductance (H)
#define M2_LQ       M1_LS           // Stator q-axis inductance (H)
#define M2_LR       NULL            // Rotor inductance (H)
#define M2_LM       NULL            // Magnetizing inductance (H)
#define M2_KB       0.8             // BEMF Constant (V/Hz)
#define M2_POLES    8               // Number of poles

//
// Define the base quantites
//
#define M2_BASE_VOLTAGE     42.78 // Base peak phase voltage (volt), Vdc/sqrt(3)
#define M2_BASE_CURRENT     16.5  // Base peak phase current (amp),
                                  // the maximum measurable peak current
#define M2_BASE_TORQUE      NULL  // Base torque (N.m)
#define M2_BASE_FLUX        NULL  // Base flux linkage (volt.sec/rad)
#define M2_BASE_FREQ        250   // Base electrical frequency (Hz)
#define M2_MAXIMUM_CURRENT  5.0   // Motor maximum torque current (amp)

#define M2_MAXIMUM_VOLTAGE  36.0    // DC bus maximum voltage (V)
#define M2_MINIMUM_VOLTAGE  5.0     // DC bus minimum voltage (V)

#define M2_VDCBUS_MAX       50.0    // maximum dc bus voltage for motor
#define M2_VDCBUS_MIN       10.0    // minimum dc bus voltage for motor

//
// Current sensors scaling
// 1.0pu current ==> 9.95A -> 2048 counts ==> 8A -> 1647
//
#define M2_CURRENT_SCALE(A)             (2048 * A / M2_BASE_CURRENT)

//
// Analog scaling with ADC
//
#define M2_ADC_PU_SCALE_FACTOR          0.000244140625     // 1/2^12
#define M2_ADC_PU_PPB_SCALE_FACTOR      0.000488281250     // 1/2^11

//
// Current Scale
//
#define M2_MAXIMUM_SCALE_CURRENT        33.0
#define M2_CURRENT_SF                  (M2_MAXIMUM_SCALE_CURRENT / 4096.0)
#define M2_CURRENT_INV_SF              (4096.0 / M2_MAXIMUM_SCALE_CURRENT)

//
// Voltage Scale
//
#define M2_MAXIMUM_SCALE_VOLATGE        81.50
#define M2_VOLTAGE_SF                   (M2_MAXIMUM_SCALE_VOLATGE / 4096.0)
#define M2_VOLTAGE_INV_SF               (4096.0 / M2_MAXIMUM_SCALE_VOLATGE)

#endif  // end of DUAL_AXIS_SERVO_DRIVE_USER_H definition

