
//how to inflate and deinflate a softRobot with one pump & one valve
// we use for the pump pwm-fade-with-millis.ino for the pump
// (the example based in Fading LED with analogWrite and millis() aka blinking a led wothout delay)
// See baldengineer.com/fading-led-analogwrite-millis-example.html for more information
// Created by Rodolfo Acosa Castro

#define PUMP_PIN 3
#define VENTIL_PIN 2 
#define PULSE_PIN A2
#define LED_PIN 7
#define INPUT_PIN 9

//#define ANOLOG_PIN A0 
#define THRESHOLD 1 // when the time duration of a pulse in PULSE_PIN is more than 2 microseconds

const byte pwmPump = PUMP_PIN;
const byte ventilPin = VENTIL_PIN;
const byte pulsePin = PULSE_PIN;
const byte ledPin = LED_PIN;
const byte inputPin = INPUT_PIN;

//const int analogInPin = ANOLOG_PIN;  
int sensorValue = 0;        // value read from the lightSensor

int ventilState = LOW;             // ventilState used to set the ventil-solenoid
unsigned long previousMillis = 0;        // will store last time ventilState was updated
const long interval = 3000;           // interval at which 
unsigned long duration;

// define directions for PUMP fade
#define UP 0
#define DOWN 1

// constants for min and max PWM
const int minPWM = 0;
const int maxPWM = 255;

// State Variable for Fade Direction
byte fadeDirection = UP;

// Global Fade Value
// but be bigger than byte and signed, for rollover
int fadeValue = 0;

// How smooth to fade?
byte fadeIncrement = 5;

// millis() timing Variable, just for fading
unsigned long previousFadeMillis;

// How fast to increment?
int fadeInterval = 80;

int count = 0;


void setup() {
   // put pwmLED into known state (off)
   Serial.begin(9600);
   pinMode(ventilPin, OUTPUT);
   pinMode(pulsePin, INPUT_PULLUP);
   pinMode(inputPin, INPUT_PULLDOWN);
   pinMode(ledPin, OUTPUT);
   analogWrite(pwmPump, fadeValue);

   digitalWrite(ventilPin, LOW);
}

void loop() {

   unsigned long currentMillis = millis();
   unsigned long currentMillis2 = millis(); 
 
  duration = pulseIn(pulsePin, HIGH, 5000);

  if (duration >= THRESHOLD){

   // doTheFade(currentMillis);
   //  analogWrite(pwmPump, 200);
     pumpAnimation();
    //int monitorTime = currentMillis2 - previousMillis ;
    //Serial.println(monitorTime);

 if (currentMillis2 - previousMillis >= interval) {
    // save the last time you turned on the pump
    previousMillis = currentMillis2;
    digitalWrite(ventilPin, HIGH);
    Serial.println("ventil ON!!");
    delay (3000);
    } 
      

    }else{
          
          analogWrite(pwmPump, LOW);      
          digitalWrite(ventilPin, LOW);
          //Serial.println("ventil OFF");

      }

  int buttonState = digitalRead(inputPin);
   //Serial.println (buttonState);

   if (buttonState == 1){
      pumpAnimation ();

    } else {
      digitalWrite (ledPin, HIGH);
   }

}


void doTheFade(unsigned long thisMillis) {
   // is it time to update yet?
   // if not, nothing happens
   if (thisMillis - previousFadeMillis >= fadeInterval) {
      // yup, it's time!
      if (fadeDirection == UP) {
         fadeValue = fadeValue + fadeIncrement;
         if (fadeValue >= maxPWM) {
            // At max, limit and change direction
            fadeValue = maxPWM;
            fadeDirection = DOWN;
         }
      } else {
         //if we aren't going up, we're going down
         fadeValue = fadeValue - fadeIncrement;
         if (fadeValue <= minPWM) {
            // At min, limit and change direction
            fadeValue = minPWM;
            fadeDirection = UP;
         }
      }
      // Only need to update when it changes
      analogWrite(pwmPump, fadeValue);      
      // reset millis for the next iteration (fade timer only)
      previousFadeMillis = thisMillis;
   }
}


void pumpAnimation() {
  
      digitalWrite(ventilPin, HIGH);
      delay (1000);
      digitalWrite(ventilPin, LOW);
      delay (50);
      analogWrite(pwmPump, 255);
      delay (2300);
      analogWrite(pwmPump, LOW);
      delay (50);
      digitalWrite(ventilPin, HIGH);
      delay (200);
      digitalWrite(ventilPin, LOW);
      delay (50);
      analogWrite(pwmPump, 255);
      delay (3000);
      analogWrite(pwmPump, LOW);
      delay (50);
      
      digitalWrite(ventilPin, HIGH);
      delay (2000);
      digitalWrite(ventilPin, LOW);
      delay (50);
      analogWrite(pwmPump, 200);
      delay (2000);
      analogWrite(pwmPump, LOW);
      delay (50);
      digitalWrite(ventilPin, HIGH);
      delay (1000);
      digitalWrite(ventilPin, LOW);
      delay (50);
      analogWrite(pwmPump, 255);
      delay (1000);
      analogWrite(pwmPump, LOW);
      delay (50);
      
      digitalWrite(ventilPin, HIGH);
      delay (20000);
      digitalWrite(ventilPin, LOW);
      delay (50);
}