// LoopBackDemo

// This demo runs on Arduino Giga R1
// The FDCAN1 module is configured in external loop back mode: it
// internally receives every CAN frame it sends, and emitted frames
// can be observed on TxCAN pin. No external hardware is required.

#ifndef ARDUINO_GIGA
  #error This sketch runs on an Arduino Giga R1 board
#endif

//-----------------------------------------------------------------

//-----------------------------------------------------------------

void setup () {
  pinMode (LED_BUILTIN, OUTPUT) ;
  digitalWrite (LED_BUILTIN, HIGH) ;
  Serial.begin (9600) ;
  while (!Serial) {
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN)) ;
    delay (50) ;
  }
  Serial.print ("PCLK1 frequency: ") ;
  Serial.print (HAL_RCC_GetPCLK1Freq ()) ;
  Serial.println (" Hz") ;
  Serial.print ("PCLK2 frequency: ") ;
  Serial.print (HAL_RCC_GetPCLK2Freq ()) ;
  Serial.println (" Hz") ;
  Serial.print ("HCLK frequency: ") ;
  Serial.print (HAL_RCC_GetHCLKFreq ()) ;
  Serial.println (" Hz") ;
  Serial.print ("SysClock frequency: ") ;
  Serial.print (HAL_RCC_GetSysClockFreq ()) ;
  Serial.println (" Hz") ;
//--- Enable FDCAN clock
  if (!__HAL_RCC_FDCAN_IS_CLK_ENABLED ()) {
  //--- Enable CAN clock
    // __HAL_RCC_FDCAN_CLK_ENABLE () ;
    { SET_BIT (RCC->APB1HENR, RCC_APB1HENR_FDCANEN) ;
      uint32_t x __attribute__((unused)) = READ_BIT (RCC->APB1HENR, RCC_APB1HENR_FDCANEN) ;
    }
  //--- Reset CAN peripherals
    __HAL_RCC_FDCAN_FORCE_RESET () ;
    __HAL_RCC_FDCAN_RELEASE_RESET () ;
  //--- Select CAN clock
    LL_RCC_SetFDCANClockSource (LL_RCC_FDCAN_CLKSOURCE_PLL1Q) ;
  }
  Serial.print ("CAN Clock: ") ;
  Serial.print (LL_RCC_GetFDCANClockFreq (LL_RCC_FDCAN_CLKSOURCE_PLL1Q)) ;
  Serial.println (" Hz") ;
  Serial.println ("**** PLL1 ****") ;
  Serial.print ("  enabled: ") ;
  Serial.println (((RCC->CR & RCC_CR_PLL1ON_Msk) != 0) ? "yes" : "no") ;
  Serial.print ("  DIVM1: ") ;
  Serial.println ((RCC->PLLCKSELR & RCC_PLLCKSELR_DIVM1_Msk)  >> RCC_PLLCKSELR_DIVM1_Pos) ;
  Serial.print ("  DIVR1: ") ;
  Serial.println ((RCC->PLL1DIVR & RCC_PLL1DIVR_R1_Msk) >> RCC_PLL1DIVR_R1_Pos) ;
  Serial.print ("  DIVQ1: ") ;
  Serial.println ((RCC->PLL1DIVR & RCC_PLL1DIVR_Q1_Msk) >> RCC_PLL1DIVR_Q1_Pos) ;
  Serial.print ("  DIVP1: ") ;
  Serial.println ((RCC->PLL1DIVR & RCC_PLL1DIVR_P1_Msk) >> RCC_PLL1DIVR_P1_Pos) ;
  Serial.print ("  DIVN1: ") ;
  Serial.println ((RCC->PLL1DIVR & RCC_PLL1DIVR_N1_Msk) >> RCC_PLL1DIVR_N1_Pos) ;
  Serial.println ("**** PLL2 ****") ;
  Serial.print (" enabled: ") ;
  Serial.println (((RCC->CR & RCC_CR_PLL2ON_Msk) != 0) ? "yes" : "no") ;
}

//-----------------------------------------------------------------

static uint32_t gSendDate = 0 ;

//-----------------------------------------------------------------

void loop () {
  if (gSendDate < millis ()) {
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN)) ;
    gSendDate += 2000 ;
  }
}
