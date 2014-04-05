


// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#include "DHT.h"
#include "EchoRange.h"
#include <OneWire.h>

///Defines for DHT
#define DHTPIN 12     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
// DHT CONFIG
// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

#define echoPin 8 // Echo Pin
#define trigPin 9 // Trigger Pin

#define WaterTempPin 10

int led_pin = 13; // If PIN is 13 no resistor is needed (built in), any other io lines to the LED need a resistor in series with the 5v

OneWire  m_WaterTemp(10);
EchoRange m_EchoRange(echoPin, trigPin);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600); 
  dht.begin();
  m_EchoRange.begin();
  
  pinMode(led_pin,OUTPUT);
}

void loop() {
  
  digitalWrite(led_pin,HIGH);
  delay(1000);
  digitalWrite(led_pin,LOW);
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(t) || isnan(h)) {
    Serial.println("AIR_HUMID NA");        
    Serial.println("AIR_HUMID NA");
  } else {
    Serial.print("AIR_HUMID "); 
    Serial.println(h);
//    Serial.print(" %\t");
    Serial.print("AIR_TEMP_C "); 
    Serial.print(t);
    Serial.println("");
   
  }
  
  float rangeCm = m_EchoRange.readDistance();
  Serial.print("RANGE_CM ");
  Serial.print(rangeCm);
  Serial.println("");
  
  //WaterTemp
  ReadWaterTempC();
  
  //wait a bit.
  delay(5000); //5 seconds.
  
}//endloop

void ReadWaterTempC()
{
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float celsius, fahrenheit;
  
  if ( !m_WaterTemp.search(addr)) {
    //Serial.println("WATERTEMP_C NAsearch");
    m_WaterTemp.reset_search();
    delay(250);
    //return;
  }
  
  
  if (OneWire::crc8(addr, 7) != addr[7]) {
      //Serial.println("CRC is not valid!");
	  Serial.println("WATERTEMP_C NAcrc");
      return;
  }
  //Serial.println();
 
  // the first ROM byte indicates which chip
  switch (addr[0]) {
    case 0x10:
      //Serial.println("  Chip = DS18S20");  // or old DS1820
      type_s = 1;
      break;
    case 0x28:
      //Serial.println("  Chip = DS18B20");
      type_s = 0;
      break;
    case 0x22:
      //Serial.println("  Chip = DS1822");
      type_s = 0;
      break;
    default:
      //Serial.println("Device is not a DS18x20 family device.");
	  Serial.println("WATERTEMP_C NAdevfam");
      return;
  } 

  m_WaterTemp.reset();
  m_WaterTemp.select(addr);
  m_WaterTemp.write(0x44, 1);        // start conversion, with parasite power on at the end
  
  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.
  
  present = m_WaterTemp.reset();
  m_WaterTemp.select(addr);    
  m_WaterTemp.write(0xBE);         // Read Scratchpad

  //Serial.print("  Data = ");
  //Serial.print(present, HEX);
  //Serial.print(" ");
  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = m_WaterTemp.read();
    //Serial.print(data[i], HEX);
    //Serial.print(" ");
  }
  //Serial.print(" CRC=");
  //Serial.print(OneWire::crc8(data, 8), HEX);
  //Serial.println();

  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  }
  celsius = (float)raw / 16.0;
  //fahrenheit = celsius * 1.8 + 32.0;
  //Serial.print("  Temperature = ");
  Serial.print("WATERTEMP_C ");
  Serial.println(celsius);
}


