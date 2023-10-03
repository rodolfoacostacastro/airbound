#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"

#define FPSerial Serial1
#define RX_pin 5
#define LED_PIN 7

DFRobotDFPlayerMini myDFPlayer;

const byte ledPin = LED_PIN;

int pos = 0; // Index for postion of bit in a character
unsigned char CH = 0; // Character in the receiving string
unsigned int bits1 = 0; // Variable to count the length of frequency (in not any particular unit)
boolean capture = false; // HIGH if the received signal hods string else LOW


unsigned long previousMillis = 0;  
const long interval = 17000;  // interval at which to clear the screen (milliseconds)

bool flagMp3 = false;

bool isPlaying = false;
bool isNotPlaying = false;
bool lightOn = false;
bool timeRunning = false;

void setup() 
{
   
   FPSerial.begin(9600); // connection to the MP3 module
   Serial.begin(9600); // Set the serial monitor to 115200 baud rate
   pinMode(ledPin, OUTPUT);

   pinMode(RX_pin, INPUT_PULLUP); // Configure RX_pin to PULLUP resistro (change to the original code example!! Rodolfo)
  
  if (!myDFPlayer.begin(FPSerial, /*isACK = */true, /*doReset = */true)) {  //Use serial to communicate with mp3.
    while(true){
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }  

  myDFPlayer.volume(15);  //Set volume value. From 0 to 30 // 15 is better if you are using an extra amp
//  myDFPlayer.play(1);  //Play the first mp3

}

void loop() 
{
    unsigned long currentMillis = millis();
  //  static unsigned long timer = millis();
  


  if (flagMp3) {
    if (isPlaying == false) {
      myDFPlayer.start();
      isPlaying = true;
      isNotPlaying = false;
    }
  }


  if (!flagMp3) {
    if (isNotPlaying == false) {
      myDFPlayer.pause(); 
      isPlaying = false;
      isNotPlaying = true;
    }
  }

//Code to read ultrasound pulses from an u_transmiter
//recieving a pulse will turn on a flag to start the Mp3-module playing


   if(!digitalRead(RX_pin)) // When RX_pin is HIGH execute
   {
      bits1 = 0;
      unsigned long deltaT = millis(); // Returns the number of milliseconds passed since the Arduino board began running the current program
      // Wait for 10 millisecond and record the no. of time for which the RX_pin goes high during that period
      while(millis()-deltaT <= 10) if(!digitalRead(RX_pin)) bits1 ++;
      //Serial.print ("Time RX goes high: ");
     // Serial.println(bits1); 
      if(capture)
      {

         boolean b = 0; // temp variable to store bit info of the signal
         if(bits1 > 290 && bits1 < 600) b = 0;
         if(bits1 > 20 && bits1 < 290) b = 1;
         if(b) bitSet(CH,7-pos); else bitClear(CH,7-pos); // set and reset the bits in the character from lSV
         //Serial.print(b);
         pos++;
         if(pos == 8) // After writing character finished print it
         {
            Serial.print((char)CH);

            pos = 0;
            capture = false;
            flagMp3 = true;

         }
      }
      if(bits1 > 600)
      {
         capture = true;
         pos = 0;
      }
   }


     if (currentMillis - previousMillis >= interval) {
   previousMillis = currentMillis;


  flagMp3 = false;

  }

  digitalWrite (ledPin, HIGH); //decoration Led 


}
