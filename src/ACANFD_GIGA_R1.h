//----------------------------------------------------------------------------------------

#pragma once

//----------------------------------------------------------------------------------------

#include <ACANFD_GIGA_R1_from_cpp.h>

//----------------------------------------------------------------------------------------
//  STM32H747XIH6 (DS12930 Rev 2, March 2023)
//    FDCAN1_RX : PA11, PB8, PD0, PH14, PI9
//    FDCAN1_TX : PA12, PB9, PD1, PH13
//    FDCAN2_RX : PB5, PB12
//    FDCAN2_TX : PB6, PB13
//----------------------------------------------------------------------------------------
//   FDCAN1
//----------------------------------------------------------------------------------------

const std::initializer_list <ACANFD_GIGA_R1::PinPort> FDCAN1_TX_PIN_ARRAY = {
  {PD_1,  9}, // Tx Pin: PD_1, AF9
  {PB_9,  9}, // Tx Pin: PB_9, AF9
  {PA_12, 9}, // Tx Pin: PA_12, AF9
  {PH_13, 9}  // Tx Pin: PH_13, AF9
} ;

//----------------------------------------------------------------------------------------

const std::initializer_list <ACANFD_GIGA_R1::PinPort> FDCAN1_RX_PIN_ARRAY = {
  {PD_0,  9}, // Rx Pin: PD_0, AF9
  {PB_8,  9}, // Rx Pin: PB_9, AF9
  {PA_11, 9}, // Rx Pin: PA_11, AF9
  {PH_14, 9}, // Tx Pin: PH_14, AF9
  {PI_9,  9}  // Tx Pin: PI_9, AF9
} ;

//----------------------------------------------------------------------------------------

void ACAN_FDCAN1_IT0_IRQHandler (void) ;

void ACAN_FDCAN1_IT1_IRQHandler (void) ;

const ACANFD_GIGA_R1::InterruptConfiguration fdcan1InterruptConfig = {
  uint32_t (ACAN_FDCAN1_IT0_IRQHandler),
  uint32_t (ACAN_FDCAN1_IT1_IRQHandler),
  0x8C,
  0x94,
  FDCAN1_IT0_IRQn, // Transmit interrupt
  FDCAN1_IT1_IRQn // Receive interrupt
} ;

//----------------------------------------------------------------------------------------

ACANFD_GIGA_R1 fdcan1 (
  FDCAN1, // CAN Peripheral base address
  0, // Message RAM start word offset
  FDCAN1_MESSAGE_RAM_WORD_SIZE, // Message RAM word size
  fdcan1InterruptConfig,
  FDCAN1_TX_PIN_ARRAY,
  FDCAN1_RX_PIN_ARRAY
) ;

//----------------------------------------------------------------------------------------

void ACAN_FDCAN1_IT0_IRQHandler (void) {
  fdcan1.isr0 () ;
}

//----------------------------------------------------------------------------------------

void ACAN_FDCAN1_IT1_IRQHandler (void) {
  fdcan1.isr1 () ;
}

//----------------------------------------------------------------------------------------
//   FDCAN2
//----------------------------------------------------------------------------------------

const std::initializer_list <ACANFD_GIGA_R1::PinPort> FDCAN2_TX_PIN_ARRAY = {
  {PB_6,  9}, // Tx Pin: PB_6, AF9
  {PB_13, 9}  // Tx Pin: PB_13, AF9
} ;

//----------------------------------------------------------------------------------------

const std::initializer_list <ACANFD_GIGA_R1::PinPort> FDCAN2_RX_PIN_ARRAY = {
  {PB_5,  9}, // Rx Pin: PB_5, AF9
  {PB_12, 9}  // Rx Pin: PB_12, AF9
} ;

//----------------------------------------------------------------------------------------

void ACAN_FDCAN2_IT0_IRQHandler (void) ;

void ACAN_FDCAN2_IT1_IRQHandler (void) ;

const ACANFD_GIGA_R1::InterruptConfiguration fdcan2InterruptConfig = {
  uint32_t (ACAN_FDCAN2_IT0_IRQHandler),
  uint32_t (ACAN_FDCAN2_IT1_IRQHandler),
  0x90,
  0x98,
  FDCAN2_IT0_IRQn, // Transmit interrupt
  FDCAN2_IT1_IRQn // Receive interrupt
} ;

//----------------------------------------------------------------------------------------

ACANFD_GIGA_R1 fdcan2 (
  FDCAN2, // CAN Peripheral base address
  FDCAN1_MESSAGE_RAM_WORD_SIZE, // Message RAM start word offset
  FDCAN2_MESSAGE_RAM_WORD_SIZE, // Message RAM word size
  fdcan2InterruptConfig,
  FDCAN2_TX_PIN_ARRAY,
  FDCAN2_RX_PIN_ARRAY
) ;

//----------------------------------------------------------------------------------------

void ACAN_FDCAN2_IT0_IRQHandler (void) {
  fdcan2.isr0 () ;
}

//----------------------------------------------------------------------------------------

void ACAN_FDCAN2_IT1_IRQHandler (void) {
  fdcan2.isr1 () ;
}

//----------------------------------------------------------------------------------------
