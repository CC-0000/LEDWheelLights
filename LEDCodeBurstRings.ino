#include <Arduino.h>
#define FASTLED_INTERNAL //supppresses notifications from FASTLED
#include <FastLED.h>

//define some basic setup terms for FASTLED
#define NUM_LEDS 36 //number of leds in the strip
#define LED_PIN_LEAD 4 // pin number for data transmission line 1
#define LED_PIN_SECOND 5 // pin number for data transmission line 2
#define LED_PIN_THIRD 6 // pin number for data transmission line 3
#define LED_PIN_FOURTH 7 // pin number for data transmission line 4


/****************
**Future Issues**
**1.) micros() can only run for approximately 71.6 minutes before it fills up and resets itself to 0. 
**    We will need a way to prevent negative time calculations from being createed
****************/

/*******************
**Global Variables**
*******************/
// initialize 4 arrays of GRB LED pixel objects, and turn them all off
CRGB leadStrip[NUM_LEDS] = {0};
CRGB secondStrip[NUM_LEDS] = {0};
CRGB thirdStrip[NUM_LEDS] = {0};
CRGB fourthStrip[NUM_LEDS] = {0};
CRGB nextStrip[NUM_LEDS] = {0};


volatile unsigned long currentTime = micros();
volatile unsigned long previousTime;
volatile unsigned long timeReadings[] = {0, 0, 0, 0};//number array containing the most recent four periods of 1/4 revolutions
volatile byte timeReadingIndex;
unsigned long timeReadingsSum;
unsigned long timeMarker = micros();
volatile unsigned long averagePeriod;
boolean completed = false;
int randomNumber1, randomNumber2, randomNumber3, randomNumber4, randomNumber5, randomNumber6;

void setup() 
{
  Serial.begin(115200);//set the serial transmission rate to 115200 bauds

  randomSeed(analogRead(A0));
  
  pinMode(LED_PIN_LEAD, OUTPUT);
  pinMode(LED_PIN_SECOND, OUTPUT);
  pinMode(LED_PIN_THIRD, OUTPUT);
  pinMode(LED_PIN_FOURTH, OUTPUT);

  //initialize the FastLED Library
  FastLED.addLeds<WS2812B, LED_PIN_LEAD, GRB>(leadStrip, NUM_LEDS);//add previsouly defined LED objects to the FastLED Library
  FastLED.addLeds<WS2812B, LED_PIN_SECOND, GRB>(secondStrip, NUM_LEDS);//add previsouly defined LED objects to the FastLED Library
  FastLED.addLeds<WS2812B, LED_PIN_THIRD, GRB>(thirdStrip, NUM_LEDS);//add previsouly defined LED objects to the FastLED Library
  FastLED.addLeds<WS2812B, LED_PIN_FOURTH, GRB>(fourthStrip, NUM_LEDS);//add previsouly defined LED objects to the FastLED Library

  FastLED.setBrightness(25);
  
  delay(5);//just a small delay to get the micros() to a safely positive number. 
}//end of setup()


void loop() 
{
    randomNumber1 = random(0, 256);
    randomNumber2 = random(0, 256);
    randomNumber3 = random(0, 256);
    randomNumber4 = random(0, 256);
    randomNumber5 = random(0, 256);
    randomNumber6 = random(0, 256);

    for(int i = NUM_LEDS - 1; i >= 0; i--)
    {
    leadStrip[i] = CRGB(randomNumber1, randomNumber2, randomNumber3);

    leadStrip[(i + 18) % 36] = CRGB(randomNumber4, randomNumber5, randomNumber6);
    
    secondStrip[i] = CRGB(randomNumber1, randomNumber2, randomNumber3);

    secondStrip[(i + 18) % 36] = CRGB(randomNumber4, randomNumber5, randomNumber6);
    
    thirdStrip[i] = CRGB(randomNumber1, randomNumber2, randomNumber3);

    thirdStrip[(i + 18) % 36] = CRGB(randomNumber4, randomNumber5, randomNumber6);
    
    fourthStrip[i] = CRGB(randomNumber1, randomNumber2, randomNumber3);

    fourthStrip[(i + 18) % 36] = CRGB(randomNumber4, randomNumber5, randomNumber6);
    
    FastLED.show();
    delay(24);
    leadStrip[i] = CRGB(0, 0, 0);
    leadStrip[(i + 18) % 36] = CRGB(0, 0, 0);
    secondStrip[i] = CRGB(0, 0, 0);
    secondStrip[(i + 18) % 36] = CRGB(0, 0, 0);
    thirdStrip[i] = CRGB(0, 0, 0);
    thirdStrip[(i + 18) % 36] = CRGB(0, 0, 0);
    fourthStrip[i] = CRGB(0, 0, 0);
    fourthStrip[(i + 18) % 36] = CRGB(0, 0, 0);
    }
}//end of loop() aka main()
