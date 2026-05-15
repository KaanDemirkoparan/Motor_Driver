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

#ifndef _HAL_OBJ_H_
#define _HAL_OBJ_H_

//! \file  solutions/drv8312_c2_kit/f28004x/drivers/hal_obj.h
//! \brief Defines the structures for the HAL object 
//!

// drivers
#include "device.h"

// modules
#include "hal_data.h"

#include "user.h"

#if defined(BOOSTXL_DRV8323RS)
#include "drv8323.h"
#endif

// platforms


//!
//!
//! \defgroup HAL_OBJ HAL_OBJ
//!
//@{


#ifdef __cplusplus
extern "C" {
#endif

//! \brief      Defines the hardware abstraction layer (HAL) data
//! \details    The HAL object contains all handles to peripherals.  When accessing a
//!             peripheral on a processor, use a HAL function along with the HAL handle
//!             for that processor to access its peripherals.
//!

typedef struct _HAL_Obj_
{
  uint32_t      adcHandle[3];       //!< the ADC handles   /* change the array length from 2 to 3 for 28003x */

  uint32_t      adcResult[3];       //!< the ADC results    /* change the array length from 2 to 3 for 28003x */

  uint32_t      pwmHandle[3];       //<! the PWM handles

  uint32_t      timerHandle[3];     //<! the timer handles

  uint32_t      sciHandle;          //!< the SCI handle
  uint32_t      linHandle;          //!< the LIN handle

  uint32_t      canHandle;       //!< the CAN handle
  uint32_t      mcanHandle;      //!< the MCAN handle

  uint32_t      spiHandle[2];       //!< the SPI handle

  uint32_t      cmpssHandle[3];     //!< the CMPSS handle

#ifdef DATALOG_ENABLE
  uint32_t      dmaHandle;          //!< the DMA handle
  uint32_t      dmaChHandle[4];     //!< the DMA Channel handle
#endif  // DATALOG_ENABLE

  float_t       current_sf;         //!< the current scale factor, amps/cnt

  float_t       voltage_sf;         //!< the voltage scale factor, volts/cnt

  uint_least8_t numCurrentSensors;  //!< the number of current sensors
  uint_least8_t numVoltageSensors;  //!< the number of voltage sensors

#ifdef PWMDAC_ENABLE
  uint32_t      pwmDACHandle[4];    //<! the PWMDAC handles
#endif  // PWMDAC_ENABLE

  float_t       dcbus_voltage_sf;   //!< the voltage scale factor, volts/cnt

  bool          flagEnablePWM;     //<! the pwm enable flag

#ifdef BOOSTXL_DRV8323RS
  DRV8323_Handle drv8323Handle;   //!< the drv8320 interface handle
  DRV8323_Obj    drv8323;         //!< the drv8320 interface object
#endif

  uint32_t      qepHandle[2];      //!< the QEP handle

} HAL_Obj;

//! \brief      Defines the HAL handle
//! \details    The HAL handle is a pointer to a HAL object.  In all HAL functions
//!             the HAL handle is passed so that the function knows what peripherals
//!             are to be accessed.
//!
typedef struct _HAL_Obj_ *HAL_Handle;


//! \brief      Defines the HAL object
//!
extern HAL_Obj hal;


#ifdef __cplusplus
}
#endif // extern "C"

//@} // ingroup
#endif // end of _HAL_OBJ_H_ definition

