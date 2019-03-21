// #include <RFTransmitter.h>
#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile

RH_ASK driver(2000,11,2);//constructor, speed,rx,tx,ptt,pttInvert

//#define NODE_ID          1 //transmitter identification number (ID)
//#define OUTPUT_PIN       2 //output digital pin to transmitter
//RFTransmitter transmitter(OUTPUT_PIN, NODE_ID);

int inputPin= 0; // sensor analog input pin
int pumpPin= 1; // pump analog input pin

int raw= 0; //raw reading from input pin (0-1023 for 0-5V)
int rawpump= 0; //raw reading from input pin (0-1023 for 0-5V)
float Vin= 4.99; //input voltage (5V default)
float Vout= 0; //measured voltage on input pin (V)
float resistor= 1942; // resistor value of voltage divider in ohm
float resistance= 0; //calculated resistance

  char msg[10];
  char puuu[10] = "";
  char teee[10] = "";

void setup() {
 // Serial.begin(9600); //only needed for debugging
  driver.init();
        // Serial.println("init failed");
  }
void loop() {
  float temp = kty();
  float pump = pumpstat();
  
  dtostrf(temp, 2, 0, teee);
  dtostrf(pump, 2, 0, puuu); //   
  strcpy(msg, puuu);
  strcat(msg, ";");
  strcat(msg, teee);
  
  //Serial.println("Temperature: "); //debugging
  //Serial.println(msg); //debugging
  
    driver.send((uint8_t *)msg, strlen(msg));
    driver.waitPacketSent();

  delay(1000);
}

float kty() {
  //unsigned int port
   raw= analogRead(inputPin);
   Vout = raw * (Vin/1023.0);
   resistance= (resistor * (Vin-Vout))/ Vout;
   
   //Serial.println("Volts measured: "); //debugging
   //Serial.println(Vout); //debugging
   //Serial.println("Resistance: "); //debugging
   //Serial.println(resistance); //debugging

   // thermistor resistance-temp curve from kty81-210 data sheet, written as 4th order polynomial trend line
   return 6.89404e-13 * pow(resistance,4) - 3.84496e-9 * pow(resistance,3) - 5.61920e-6 * pow(resistance,2) + 0.111273 * resistance - 154.893;  
}
float pumpstat() {
   //unsigned int port
   rawpump = analogRead(pumpPin);
   return rawpump * (Vin/1023.0);
}
