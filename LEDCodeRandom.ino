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
**2.) wait and Magnet_Detected have not been integrated yet as preliminary tests have shown that the UNO does not have enough processing power to render a full image
****************/

/*******************
**Global Variables**
*******************/
// initialize 4 arrays of GRB LED pixel objects, and turn them all off
CRGB leadStrip[NUM_LEDS] = {0};
CRGB secondStrip[NUM_LEDS] = {0};
CRGB thirdStrip[NUM_LEDS] = {0};
CRGB fourthStrip[NUM_LEDS] = {0};


volatile unsigned long currentTime = micros();
volatile unsigned long previousTime;
volatile unsigned long timeReadings[] = {0, 0, 0, 0};//number array containing the most recent four periods of 1/4 revolutions
volatile byte timeReadingIndex;
unsigned long timeReadingsSum;
unsigned long timeMarker = micros();
volatile unsigned long averagePeriod;


void setup() 
{
  Serial.begin(115200);//set the serial transmission rate to 115200 bauds

  randomSeed(analogRead(A0));
  
  pinMode(LED_PIN_LEAD, OUTPUT);
  pinMode(LED_PIN_SECOND, OUTPUT);
  pinMode(LED_PIN_THIRD, OUTPUT);
  pinMode(LED_PIN_FOURTH, OUTPUT);

  
  pinMode(2, INPUT_PULLUP);//attaches pin 2 to arduino's internal pullup resistor, hopefully this will nulify the need for a seperate 10k resistor 

  //initialize the interrupt pin
  attachInterrupt(0, Magnet_Detected, RISING);

  //initialize the FastLED Library
  FastLED.addLeds<WS2812B, LED_PIN_LEAD, GRB>(leadStrip, NUM_LEDS);//add previsouly defined LED objects to the FastLED Library
  FastLED.addLeds<WS2812B, LED_PIN_SECOND, GRB>(secondStrip, NUM_LEDS);//add previsouly defined LED objects to the FastLED Library
  FastLED.addLeds<WS2812B, LED_PIN_THIRD, GRB>(thirdStrip, NUM_LEDS);//add previsouly defined LED objects to the FastLED Library
  FastLED.addLeds<WS2812B, LED_PIN_FOURTH, GRB>(fourthStrip, NUM_LEDS);//add previsouly defined LED objects to the FastLED Library

  FastLED.setBrightness(18);
  
  delay(500);//just a small delay to get the micros() to a safely positive number. 
}//end of setup()


void loop() 
{
//  //create a pac man image from a single led strip
//  timeMarker = micros();
//  for(int i = 0; i < NUM_LEDS; i++)
//  {
//    leadStrip[i] = CRGB (255,255,0);
//  }
//  FastLED.show();//this shows all of the LEDs
//  wait((0.6666667)*averagePeriod);//wait() must always be preceded by a timeMarker which marks when you want to start counting or "waiting"
//  timeMarker = micros();
//  for(int i = 0; i < NUM_LEDS; i++)
//  {
//    leadStrip[i] = CRGB (0, 0, 0);
//  }
//  FastLED.show();//this shows all of the LEDs
//  wait((0.1666667)*averagePeriod);//wait() must always be preceded by a timeMarker which marks when you want to start counting or "waiting"
//  timeMarker = micros();
//  for(int i = 0; i < NUM_LEDS; i++)
//  {
//    leadStrip[i] = CRGB (255,255,0);
//  }
//  FastLED.show();//this shows all of the LEDs
//  wait((0.1666667)*averagePeriod);//wait() must always be preceded by a timeMarker which marks when you want to start counting or "waiting"
  for(int i = 0; i < NUM_LEDS; i++)
  {
    leadStrip[i] = CRGB (random(1, 256),random(1, 256),random(1, 256));
  }
  for(int i = 0; i < NUM_LEDS; i++)
  {
    secondStrip[i] = CRGB (random(1, 256),random(1, 256),random(1, 256));
  }
  for(int i = 0; i < NUM_LEDS; i++)
  {
    thirdStrip[i] = CRGB (random(1, 256),random(1, 256),random(1, 256));
  }
  for(int i = 0; i < NUM_LEDS; i++)
  {
    fourthStrip[i] = CRGB (random(1, 256),random(1, 256),random(1, 256));
  }
  FastLED.show();
  delay(10);
}//end of loop() aka main()


void Magnet_Detected()
{
  //calculate the most immediate period
  previousTime = currentTime;
  currentTime = micros();
  timeReadingIndex = timeReadingIndex%4;//keeps timeReading within the numbers 0-4
  timeReadings[timeReadingIndex] = (currentTime - previousTime);//stores the most current time period in the timeReading array

  //calculate the average period of a revolution based on the 4 readings inside of the timeReading array
  timeReadingsSum = 0;
  for(int i = 0; i < 4; i++)
  {
    timeReadingsSum += timeReadings[i];
  }
  averagePeriod = timeReadingsSum / 4;//store the calculated period
  
  //move the index over by 1
  timeReadingIndex++;
}//end of Magnet_Detected(), the interrupt function


void wait(int timeToWait)//timeToWait is measured in microseconds
{
  while(micros() < (timeMarker + timeToWait))
  {
    //do nothing aka wait
  }
}//end of wait(), a function that I made that's more accurate in a sense than delay.
