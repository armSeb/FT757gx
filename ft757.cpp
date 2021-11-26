

#include <Arduino.h>
#include "ft757.h"

FT757::FT757() {}

// Public functions

void FT757::begin(long baud)
{
  rigCat.begin(baud, SWSERIAL_8N1, CAT_RX, CAT_TX, false);
}

bool FT757::setFreq(long freq)
{
  if(freq<500000 || freq > 30000000) return false;
  // Freq must be in 10Hz unit
  freq /=10;
  to_bcd_le(freq);
  buffer[4] = CAT_FSET;
  sendCmd();
  return true;
}


// Private functions

void FT757::to_bcd_le(unsigned long f)
{
  unsigned char a;

  // clear the nullpad
  flushBuffer();

  for (int i=0; i<=3; i++)
  {
    a = f % 10;
    f /= 10;
    a |= (f % 10) << 4;
    f /= 10;
    buffer[i] = a;
  }
}

void FT757::sendCmd()
{
  for (byte i=0; i<5; i++)
  {
    //Serial.println(buffer[i]); //Debug
    rigCat.write(buffer[i]);
  }
}

// empty the buffer
void FT757::flushBuffer()
{
  memset(buffer, 0, 5);
}
