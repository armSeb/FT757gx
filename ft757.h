#ifndef CAT_h
#define CAT_h

/* Librairie CAT pour FT757gx
 *  Fonctionne sur Arduino et ESP8266
 *  
 *  Le Transceiver est branché sur la broche D6 par défaut.
 *  
 *  Inspired from : https://github.com/stdevPavelmc/ft817
 *  
 */

#include <Arduino.h>
#include <SoftwareSerial.h>

#define CAT_SPLIT   0x01
#define CAT_MR_VFO  0x02
#define CAT_V_M     0x03
#define CAT_D_LOCK  0x04
#define CAT_VFO_A_B 0x05
#define CAT_M_V     0x06
#define CAT_500_UP  0x07
#define CAT_500_DWN 0x08
#define CAT_CLAR    0x09
#define CAT_FSET    0x0a
#define CAT_EXCHG   0x0b

#define CAT_TX      D6
#define CAT_RX      D5

class FT757
{
  public:
    FT757();

    void begin(long baud);
    
    bool setFreq(long freq);

  private:
    void flushBuffer();
    void sendCmd();
    void to_bcd_le(unsigned long freq);

    SoftwareSerial rigCat;
    unsigned long freq;
    byte buffer[5];
    
};




#endif
