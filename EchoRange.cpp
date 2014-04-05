/* EchoRange library 

MIT license
written by Adafruit Industries
*/

#include "EchoRange.h"

EchoRange::EchoRange(uint8_t echoPin, uint8_t trigPin)
{
  m_echoPin = echoPin;
  m_trigPin = trigPin;

}

void EchoRange::begin(void) {
  // set up the pins!
  pinMode(m_trigPin, OUTPUT);
}

//boolean S == Scale.  True == Farenheit; False == Celcius
float EchoRange::readDistance(bool S) {
  float f;

  //digitalWrite(2, HIGH);
  // establish variables for duration of the ping,
  // and the distance result in inches and centimeters:
  float duration, inches, cm;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(m_trigPin, OUTPUT);// attach pin 3 to Trig
  digitalWrite(m_trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(m_trigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(m_trigPin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode (m_echoPin, INPUT);//attach pin 4 to Echo
  duration = pulseIn(m_echoPin, HIGH);

  // convert the time into a distance
  //inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);
 
  return cm;
}

long EchoRange::microsecondsToInches(long microseconds)
{
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74.0 / 2.0;
}

long EchoRange::microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29.0 / 2.0;
}


