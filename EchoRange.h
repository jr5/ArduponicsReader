#ifndef EchoRange_H
#define EchoRange_H
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

/* EchoRange library 

MIT license
written by Adafruit Industries
*/

// how many timing transitions we need to keep track of. 2 * number bits + extra
#define MAXTIMINGS 85

#define EchoRange11 11
#define EchoRange22 22
#define EchoRange21 21
#define AM2301 21

class EchoRange {
 private:
  //uint8_t data[6];
  //uint8_t _pin, _type, _count;
  //boolean read(void);
  //unsigned long _lastreadtime;
  //boolean firstreading;
  uint8_t m_echoPin;
  uint8_t m_trigPin;
  
  

 public:
  EchoRange(uint8_t pEcho,uint8_t pTrig);
  void begin(void);
  float readDistance(bool S=false);
  //float convertCtoF(float);
  //float readHumidity(void);
  long microsecondsToInches(long microseconds);
  long microsecondsToCentimeters(long microseconds);
};
#endif
