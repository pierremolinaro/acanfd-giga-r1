//----------------------------------------------------------------------------------------

#include <ACANFD_GIGA_R1_FIFO.h>

//----------------------------------------------------------------------------------------
// Default constructor
//----------------------------------------------------------------------------------------

ACANFD_GIGA_R1_FIFO::ACANFD_GIGA_R1_FIFO (void) :
mBuffer (nullptr),
mSize (0),
mReadIndex (0),
mCount (0),
mPeakCount (0) {
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------

ACANFD_GIGA_R1_FIFO:: ~ ACANFD_GIGA_R1_FIFO (void) {
  delete [] mBuffer ;
}

//----------------------------------------------------------------------------------------
// initWithSize
//----------------------------------------------------------------------------------------

void ACANFD_GIGA_R1_FIFO::initWithSize (const uint16_t inSize) {
  delete [] mBuffer ;
  mBuffer = new CANFDMessage [inSize] ;
  mSize = inSize ;
  mReadIndex = 0 ;
  mCount = 0 ;
  mPeakCount = 0 ;
}

//----------------------------------------------------------------------------------------
// append
//----------------------------------------------------------------------------------------

bool ACANFD_GIGA_R1_FIFO::append (const CANFDMessage & inMessage) {
  const bool ok = mCount < mSize ;
  if (ok) {
    uint16_t writeIndex = mReadIndex + mCount ;
    if (writeIndex >= mSize) {
      writeIndex -= mSize ;
    }
    mBuffer [writeIndex] = inMessage ;
    mCount += 1 ;
    if (mPeakCount < mCount) {
      mPeakCount = mCount ;
    }
  }else{
    mPeakCount = mSize + 1 ;
  }
  return ok ;
}

//----------------------------------------------------------------------------------------
// Remove
//----------------------------------------------------------------------------------------

bool ACANFD_GIGA_R1_FIFO::remove (CANFDMessage & outMessage) {
  const bool ok = mCount > 0 ;
  if (ok) {
    outMessage = mBuffer [mReadIndex] ;
    mCount -= 1 ;
    mReadIndex += 1 ;
    if (mReadIndex == mSize) {
      mReadIndex = 0 ;
    }
  }
  return ok ;
}

//----------------------------------------------------------------------------------------
// Free
//----------------------------------------------------------------------------------------

void ACANFD_GIGA_R1_FIFO::free (void) {
  delete [] mBuffer ; mBuffer = nullptr ;
  mSize = 0 ;
  mReadIndex = 0 ;
  mCount = 0 ;
  mPeakCount = 0 ;
}

//----------------------------------------------------------------------------------------
