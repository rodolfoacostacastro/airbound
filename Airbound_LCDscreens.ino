#include <LiquidCrystal_I2C.h>
#define RX_pin 5
#define LED_PIN A2

const byte ledPin = LED_PIN;

int pos = 0; // Index for postion of bit in a character
unsigned char CH = 0; // Character in the receiving string
unsigned int bits1 = 0; // Variable to count the length of frequency (in not any particular unit)
boolean capture = false; // HIGH if the received signal hods string else LOW

// set the LCD number of columns and rows
int lcdColumns = 20;
int lcdRows = 4;

int lcdColumns_2 = 20;
int lcdRows_2 = 4;

int lcdColumns_3 = 20;
int lcdRows_3 = 4;

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  
LiquidCrystal_I2C lcd2(0x26, lcdColumns_2, lcdRows_2);  
LiquidCrystal_I2C lcd3(0x25, lcdColumns_3, lcdRows_3);  


unsigned long previousMillis = 0;  
const long interval = 7000;  // interval at which to clear the screen (milliseconds)

 
void setup() 
{
   Serial.begin(9600); // Set the serial monitor to 115200 baud rate
   lcd.init();
   lcd2.init();
   lcd3.init();

  // turn on LCD backlight                      
  lcd.backlight();
//  lcd2.backlight();
//  lcd3.backlight();
  fixWelcomeText();

   pinMode(RX_pin, INPUT_PULLUP); // Configure RX_pin to PULLUP resistro (but it should be PULLDOWN XD STILL CONFUSED )
   pinMode(ledPin, OUTPUT);

}

void loop() 
{
    unsigned long currentMillis = millis();


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

           // lcd.setCursor(0, 1);
           // lcd.print((char)CH);
           //lcd.write((char)CH);
           lcd2.backlight();
           lcd3.backlight();
           lcd2.write((char)CH);
           lcd3.write((char)CH);


            pos = 0;
            capture = false;
         }
      }
      if(bits1 > 600)
      {
         capture = true;
         pos = 0;
       //  lcd.clear();

      }
   }


     if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // set the LED with the ledState of the variable:
//lcd.clear();
lcd2.noBacklight();
lcd3.noBacklight();
lcd2.clear();
lcd3.clear();

  }
    digitalWrite (ledPin, HIGH);

}


void fixWelcomeText() {
    
    lcd.setCursor(0, 0);
    lcd.print("welcome!! to tune in");
    lcd.setCursor(0, 1);
    lcd.print("place the stone in ");
    lcd.setCursor(0, 2);
    lcd.print ("your palm and point ");
    lcd.setCursor(0, 3);
    lcd.print("it towards the node");
}