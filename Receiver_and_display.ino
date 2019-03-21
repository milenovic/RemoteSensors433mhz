//#include <PinChangeInterruptHandler.h>
//#include <RFReceiver.h>

#include <RH_ASK.h>
#include <SPI.h>
RH_ASK driver(2000,2,11);//constructor, speed,rx,tx,ptt,pttInvert

#define LATCH 8
#define CLK 12
#define DATA 11

     int pump;
     int rT;
     char a[5];
     char b[5];

//for timer
unsigned long startTime = 0;
//led pin
int ledPin = 13;
//digit codes 0123456789.*nothing*-
byte digitOne[13] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x80, 0x00, 0x40};
byte digitTwo[13] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x80, 0x00, 0x40};

//string to hold te message
char inString[10];
int first = 0;
int second = 0;

void setup() {
  //Serial.begin(9600); //debugging
  pinMode(ledPin, OUTPUT);
  startTime = millis();
  pinMode(LATCH, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(DATA, OUTPUT);
  
  driver.init();
         //Serial.println("init failed");
  
  //receiver.begin();
}

void loop() {
  uint8_t buf[12];
  uint8_t buflen = sizeof(buf);
  //new
  unsigned long loopTime = millis() - startTime; //Calculate the time since last time message was received (timer resterted)
  //char msg[MAX_PACKAGE_SIZE];
  //byte senderId = 0;
  //byte packageId = 0;
  //byte len = 0;

  if (loopTime <= 6000) //Check if the time is less than 6s, and if yes, do the temp display
  {
    
    //if (receiver.ready()) {
    if (driver.recv(buf, &buflen)) {
      startTime = millis();
      int i;
    for (i = 0; i < buflen; i++)
    {
      inString[i] = char(buf[i]); // Fill Sensor1CharMsg Char array with corresponding chars from buffer.
    }
    inString[buflen] = '\0'; // Null terminate the char array

      //Serial.print("Raw: "); //debug
      //Serial.println(inString); //debug

    strcpy(a, strtok(inString , ";"));
    strcpy(b, strtok(NULL, ";"));
    pump = atoi(a);
    rT = atoi(b);
      
      //Serial.print("pump: "); //debug
      //Serial.println(pump); //debug
      //Serial.print("rT: "); //debug
      //Serial.println(rT); //debug
      if (rT > 99) { //more than 99 C, so blink 99!
        digitalWrite(LATCH, LOW);
        shiftOut(DATA, CLK, MSBFIRST, ~digitTwo[10]); // digitTwo
        shiftOut(DATA, CLK, MSBFIRST, ~digitOne[10]); // digitOne
        digitalWrite(LATCH, HIGH);
        delay(300);
        digitalWrite(LATCH, LOW);
        shiftOut(DATA, CLK, MSBFIRST, ~digitTwo[9]); // digitTwo
        shiftOut(DATA, CLK, MSBFIRST, ~digitOne[9]); // digitOne
        digitalWrite(LATCH, HIGH);
        delay(500);
        digitalWrite(LATCH, LOW);
        shiftOut(DATA, CLK, MSBFIRST, ~digitTwo[10]); // digitTwo
        shiftOut(DATA, CLK, MSBFIRST, ~digitOne[10]); // digitOne
        digitalWrite(LATCH, HIGH);
        delay(300);
        digitalWrite(LATCH, LOW);
        shiftOut(DATA, CLK, MSBFIRST, ~digitTwo[9]); // digitTwo
        shiftOut(DATA, CLK, MSBFIRST, ~digitOne[9]); // digitOne
        digitalWrite(LATCH, HIGH);
        delay(500);
      }
      else if (rT < 0) { //less than 0, so write 0
        digitalWrite(LATCH, LOW);
        shiftOut(DATA, CLK, MSBFIRST, ~digitTwo[0]); // digitTwo
        shiftOut(DATA, CLK, MSBFIRST, ~digitOne[11]); // digitOne
        digitalWrite(LATCH, HIGH);
      }
      else { //all good, write output
        first = (rT / 10) % 10;
        second = rT % 10;

        digitalWrite(LATCH, LOW);
        shiftOut(DATA, CLK, MSBFIRST, ~digitTwo[second]); // digitTwo
        shiftOut(DATA, CLK, MSBFIRST, ~digitOne[first]); // digitOne
        digitalWrite(LATCH, HIGH);
      } //temp range IFs end here
      if (pump > 2) {
        digitalWrite(ledPin, HIGH);
      }
      else {
        digitalWrite(ledPin, LOW);
      }
   //   inString = ""; //BROKEN
    }//if ready ends here
  } //time less than 6s
  else {
    //package not received in 6s, so display --
    digitalWrite(LATCH, LOW);
    shiftOut(DATA, CLK, MSBFIRST, ~digitTwo[12]); // digitTwo
    shiftOut(DATA, CLK, MSBFIRST, ~digitOne[12]); // digitOne
    digitalWrite(LATCH, HIGH);
    startTime = millis();
  } //time more than 6s ends here
} //loop ends here
