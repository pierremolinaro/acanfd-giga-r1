// This demo runs on Arduino Giga R1
// The FDCAN1 module is configured in external loop back mode: it
// internally receives every CAN frame it sends, and emitted frames
// can be observed on TxCAN pin. No external hardware is required.

#ifndef ARDUINO_GIGA
  #error This sketch runs on an Arduino Giga R1 board
#endif

//-----------------------------------------------------------------
// IMPORTANT:
//   <ACANFD_GIGA_R1.h> should be included only once, generally from the .ino file
//   From an other file, include <ACANFD_GIGA_R1_from_cpp.h>
//   Before including <ACANFD_GIGA_R1.h>, you should define
//   Message RAM size for FDCAN1 and Message RAM size for FDCAN2.
//   Maximum required size is 2,560 (2,560 32-bit words).
//   A 0 size means the FDCAN module is not configured; its TxCAN and RxCAN pins
//   can be freely used for an other function.
//   The begin method checks if actual size is greater or equal to required size.
//   Hint: if you do not want to compute required size, print
//   fdcan1.messageRamRequiredMinimumSize () for getting it.
//-----------------------------------------------------------------

static const uint32_t FDCAN1_MESSAGE_RAM_WORD_SIZE = 2560 ;
static const uint32_t FDCAN2_MESSAGE_RAM_WORD_SIZE = 0 ; // FDCAN2 not used

#include <ACANFD_GIGA_R1.h>

//-----------------------------------------------------------------

static uint32_t gStandardSingleFilterMatchCount = 0 ;
static uint32_t gStandardDualFilterMatchCount = 0 ;
static uint32_t gStandardRangeFilterMatchCount = 0 ;
static uint32_t gStandardClassicFilterMatchCount = 0 ;
static uint32_t gExtendedSingleFilterMatchCount = 0 ;
static uint32_t gExtendedDualFilterMatchCount = 0 ;
static uint32_t gExtendedRangeFilterMatchCount = 0 ;
static uint32_t gExtendedClassicFilterMatchCount = 0 ;

//-----------------------------------------------------------------

static void callBackForStandardSingleFilter (const CANFDMessage & /* inMessage */) {
  gStandardSingleFilterMatchCount += 1 ;
}

//-----------------------------------------------------------------

static void callBackForStandardDualFilter (const CANFDMessage & /* inMessage */) {
  gStandardDualFilterMatchCount += 1 ;
}

//-----------------------------------------------------------------

static void callBackForStandardRangeFilter (const CANFDMessage & /* inMessage */) {
  gStandardRangeFilterMatchCount += 1 ;
}

//-----------------------------------------------------------------

static void callBackForStandardClassicFilter (const CANFDMessage & /* inMessage */) {
  gStandardClassicFilterMatchCount += 1 ;
}

//-----------------------------------------------------------------

static void callBackForExtendedSingleFilter (const CANFDMessage & /* inMessage */) {
  gExtendedSingleFilterMatchCount += 1 ;
}

//-----------------------------------------------------------------

static void callBackForExtendedDualFilter (const CANFDMessage & /* inMessage */) {
  gExtendedDualFilterMatchCount += 1 ;
}

//-----------------------------------------------------------------

static void callBackForExtendedRangeFilter (const CANFDMessage & /* inMessage */) {
  gExtendedRangeFilterMatchCount += 1 ;
}

//-----------------------------------------------------------------

static void callBackForExtendedClassicFilter (const CANFDMessage & /* inMessage */) {
  gExtendedClassicFilterMatchCount += 1 ;
}

//-----------------------------------------------------------------

void setup () {
  pinMode (LED_BUILTIN, OUTPUT) ;
  Serial.begin (9600) ;
  while (!Serial) {
    delay (50) ;
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN)) ;
  }
  Serial.println ("CAN1 CANFD loopback test") ;
  ACANFD_GIGA_R1_Settings settings (1000 * 1000, DataBitRateFactor::x8) ;

  Serial.print ("Bit Rate prescaler: ") ;
  Serial.println (settings.mBitRatePrescaler) ;
  Serial.print ("Arbitration Phase segment 1: ") ;
  Serial.println (settings.mArbitrationPhaseSegment1) ;
  Serial.print ("Arbitration Phase segment 2: ") ;
  Serial.println (settings.mArbitrationPhaseSegment2) ;
  Serial.print ("Arbitration SJW: ") ;
  Serial.println (settings.mArbitrationSJW) ;
  Serial.print ("Actual Arbitration Bit Rate: ") ;
  Serial.print (settings.actualArbitrationBitRate ()) ;
  Serial.println (" bit/s") ;
  Serial.print ("Arbitration Sample point: ") ;
  Serial.print (settings.arbitrationSamplePointFromBitStart ()) ;
  Serial.println ("%") ;
  Serial.print ("Exact Arbitration Bit Rate ? ") ;
  Serial.println (settings.exactArbitrationBitRate () ? "yes" : "no") ;
  Serial.print ("Data Phase segment 1: ") ;
  Serial.println (settings.mDataPhaseSegment1) ;
  Serial.print ("Data Phase segment 2: ") ;
  Serial.println (settings.mDataPhaseSegment2) ;
  Serial.print ("Data SJW: ") ;
  Serial.println (settings.mDataSJW) ;
  Serial.print ("Actual Data Bit Rate: ") ;
  Serial.print (settings.actualDataBitRate ()) ;
  Serial.println (" bit/s") ;
  Serial.print ("Data Sample point: ") ;
  Serial.print (settings.dataSamplePointFromBitStart ()) ;
  Serial.println ("%") ;
  Serial.print ("Exact Data Bit Rate ? ") ;
  Serial.println (settings.exactDataBitRate () ? "yes" : "no") ;

  settings.mModuleMode = ACANFD_GIGA_R1_Settings::EXTERNAL_LOOP_BACK ;


  ACANFD_GIGA_R1_StandardFilters standardFilters ;
//--- Add classic filter: identifier and mask (8 matching identifiers)
  standardFilters.addClassic (0x405, 0x7D5, ACANFD_GIGA_R1_FilterAction::FIFO0, callBackForStandardClassicFilter) ;
//--- Add range filter: low bound, high bound (36 matching identifiers)
  standardFilters.addRange (0x100, 0x123, ACANFD_GIGA_R1_FilterAction::FIFO1, callBackForStandardRangeFilter) ;
//--- Add dual filter: identifier1, identifier2 (2 matching identifiers)
  standardFilters.addDual (0x033, 0x44, ACANFD_GIGA_R1_FilterAction::FIFO0, callBackForStandardDualFilter) ;
//--- Add single filter: identifier (1 matching identifier)
  standardFilters.addSingle (0x055, ACANFD_GIGA_R1_FilterAction::FIFO0, callBackForStandardSingleFilter) ;

  ACANFD_GIGA_R1_ExtendedFilters extendedFilters ;
//--- Add single filter: identifier (1 matching identifier)
  extendedFilters.addSingle (0x5555, ACANFD_GIGA_R1_FilterAction::FIFO0, callBackForExtendedSingleFilter) ;
//--- Add dual filter: identifier1, identifier2 (2 matching identifiers)
  extendedFilters.addDual (0x3333, 0x4444, ACANFD_GIGA_R1_FilterAction::FIFO0, callBackForExtendedDualFilter) ;
//--- Add range filter: low bound, high bound (565 matching identifiers)
  extendedFilters.addRange (0x1000, 0x1234, ACANFD_GIGA_R1_FilterAction::FIFO1, callBackForExtendedRangeFilter) ;
//--- Add classic filter: identifier and mask (32 matching identifiers)
  extendedFilters.addClassic (0x6789, 0x1FFF67BD, ACANFD_GIGA_R1_FilterAction::FIFO0, callBackForExtendedClassicFilter) ;

//--- Reject extended frames that do not match any filter
  settings.mNonMatchingExtendedFrameReception = ACANFD_GIGA_R1_FilterAction::REJECT ;

// Therefore FIFO0 receives 1 + 2 + 32 = 35 frames, FIFO1 receives 565 frames.

//--- Allocate FIFO 1
  settings.mHardwareRxFIFO1Size = 10 ; // By default, 0
  settings.mDriverReceiveFIFO1Size = 10 ; // By default, 0

  const uint32_t errorCode = fdcan1.beginFD (settings, standardFilters, extendedFilters) ;

  Serial.print ("Message RAM required minimum size: ") ;
  Serial.print (fdcan1.messageRamRequiredMinimumSize ()) ;
  Serial.println (" words") ;

  if (0 == errorCode) {
    Serial.println ("can configuration ok") ;
  }else{
    Serial.print ("Error can configuration: 0x") ;
    Serial.println (errorCode, HEX) ;
  }
}

//-----------------------------------------------------------------

static const uint32_t PERIOD = 1000 ;
static uint32_t gBlinkDate = PERIOD ;
static uint32_t gSentIdentifier = 0 ;
static bool gOk = true ;
static bool gSendExtended = false ;

//-----------------------------------------------------------------

static void printCount (const uint32_t inActualCount, const uint32_t inExpectedCount) {
  Serial.print (", ") ;
  if (inActualCount == inExpectedCount) {
    Serial.print ("ok") ;
  }else{
    Serial.print (inActualCount) ;
    Serial.print ("/") ;
    Serial.print (inExpectedCount) ;
  }
}

//-----------------------------------------------------------------

void loop () {
//--- Send standard frame ?
  if (!gSendExtended && gOk && (gSentIdentifier <= 0x7FF) && fdcan1.sendBufferNotFullForIndex (0)) {
    CANFDMessage frame ;
    frame.id = gSentIdentifier ;
    gSentIdentifier += 1 ;
    const uint32_t sendStatus = fdcan1.tryToSendReturnStatusFD (frame) ;
    if (sendStatus != 0) {
      gOk = false ;
      Serial.print ("Sent error 0x") ;
      Serial.println (sendStatus) ;
    }
  }
//--- All standard frame have been sent ?
  if (!gSendExtended && gOk && (gSentIdentifier > 0x7FF)) {
    gSendExtended = true ;
    gSentIdentifier = 0 ;
  }
//--- Send extended frame ?
  if (gSendExtended && gOk && (gSentIdentifier <= 0x1FFFFFFF) && fdcan1.sendBufferNotFullForIndex (0)) {
    CANFDMessage frame ;
    frame.id = gSentIdentifier ;
    frame.ext = true ;
    gSentIdentifier += 1 ;
    const uint32_t sendStatus = fdcan1.tryToSendReturnStatusFD (frame) ;
    if (sendStatus != 0) {
      gOk = false ;
      Serial.print ("Sent error 0x") ;
      Serial.println (sendStatus) ;
    }
  }
//--- Receive frame
  fdcan1.dispatchReceivedMessage () ;
//--- Blink led and display
  if (gBlinkDate <= millis ()) {
    gBlinkDate += PERIOD ;
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN)) ;
    Serial.print ("Sent: ") ;
    Serial.print (gSentIdentifier) ;
    printCount (gStandardSingleFilterMatchCount, 1) ;
    printCount (gStandardDualFilterMatchCount, 2) ;
    printCount (gStandardRangeFilterMatchCount, 36) ;
    printCount (gStandardClassicFilterMatchCount, 8) ;
    printCount (gExtendedSingleFilterMatchCount, 1) ;
    printCount (gExtendedDualFilterMatchCount, 2) ;
    printCount (gExtendedRangeFilterMatchCount, 565) ;
    printCount (gExtendedClassicFilterMatchCount, 32) ;
    Serial.println () ;
  }
}

//-----------------------------------------------------------------
