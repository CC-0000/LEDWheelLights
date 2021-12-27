#include <Arduino.h>
#define FASTLED_INTERNAL //supppresses notifications from FASTLED
#include <FastLED.h>

//define some basic setup terms for FASTLED
#define NUM_LEDS 36 //number of leds in the strip
#define LED_PIN_LEAD 4 // pin number for data transmission line 1
#define LED_PIN_SECOND 5 // pin number for data transmission line 2
#define LED_PIN_THIRD 6 // pin number for data transmission line 3
#define LED_PIN_FOURTH 7 // pin number for data transmission line 4

/*******************
**Global Variables**
*******************/
// initialize 4 arrays of GRB LED pixel objects, and turn them all off
CRGB leadStrip[NUM_LEDS] = {0};
CRGB secondStrip[NUM_LEDS] = {0};
CRGB thirdStrip[NUM_LEDS] = {0};
CRGB fourthStrip[NUM_LEDS] = {0};
CRGB nextStrip[NUM_LEDS] = {0};

boolean completed = false;
int randomNumberR, randomNumberG, randomNumberB;
boolean runOnce = false;

/*****************
**Program Begins**
*****************/
void setup() 
{
  Serial.begin(115200);//set the serial transmission rate to 115200 bauds
  
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
  
  delay(50);//just a small delay to get the micros() to a safely positive number. 
}//end of setup()


void loop() 
{
  if(runOnce == false)
  {
    for(int i = 0; i < 6; i++)
    {
      leadStrip[i] = CRGB (255, 0, 0);
      secondStrip[i] = CRGB (255, 0, 0);
      thirdStrip[i] = CRGB (255, 0, 0);
      fourthStrip[i] = CRGB (255, 0, 0);
    }

    for(int i = 6; i < 12; i++)
    {
      leadStrip[i] = CRGB (255, 102, 0);
      secondStrip[i] = CRGB (255, 102, 0);
      thirdStrip[i] = CRGB (255, 102, 0);
      fourthStrip[i] = CRGB (255, 102, 0);
    }

    for(int i = 12; i < 18; i++)
    {
      leadStrip[i] = CRGB (255, 200, 0);
      secondStrip[i] = CRGB (255, 200, 0);
      thirdStrip[i] = CRGB (255, 200, 0);
      fourthStrip[i] = CRGB (255, 200, 0);
    }

    for(int i = 18; i < 24; i++)
    {
      leadStrip[i] = CRGB (128, 255, 0);
      secondStrip[i] = CRGB (128, 255, 0);
      thirdStrip[i] = CRGB (128, 255, 0);
      fourthStrip[i] = CRGB (128, 255, 0);
    }
    for(int i = 24; i < 30; i++)
    {
      leadStrip[i] = CRGB (51, 153, 255);
      secondStrip[i] = CRGB (51, 153, 255);
      thirdStrip[i] = CRGB (51, 153, 255);
      fourthStrip[i] = CRGB (51, 153, 255);
    }
    for(int i = 30; i < 36; i++)
    {
      leadStrip[i] = CRGB (255, 0, 255);
      secondStrip[i] = CRGB (255, 0, 255);
      thirdStrip[i] = CRGB (255, 0, 255);
      fourthStrip[i] = CRGB (255, 0, 255);
    }

    FastLED.show();
    runOnce = true;
  }
  passColorsDownTheLine();
}//end of loop() aka main()

void passColorsDownTheLine()//expects that all 4 strips are set to the same color
{
  //create another pseudo LED strip that represents what the previous strip needs to transition to
  //temporary variables to store the color of the tip of strip
  unsigned int temporaryR = leadStrip[0].r;
  unsigned int temporaryG = leadStrip[0].g;
  unsigned int temporaryB = leadStrip[0].b;
  //move all of the LED's down by 1, or out by 1 if looking at the strip on the bike
  for(int i = 0; i < NUM_LEDS; i++)
  {
    nextStrip[i] = leadStrip[i+1];
  }
  nextStrip[NUM_LEDS - 1] = CRGB (temporaryR, temporaryG, temporaryB);

  //always assume that the LED's have not been transitioned yet
  completed = false;
  //fade all of the old LEDs to the new pseudo strip
  while(completed == false)
  {
    completed = true;
    //fade the LED's one notch
    for(int i = 0; i < NUM_LEDS; i++)
    {
      //slightly change the r value
      if(leadStrip[i].r < nextStrip[i].r)
      {
        leadStrip[i].r = leadStrip[i].r + 1;
        secondStrip[i].r = secondStrip[i].r + 1;
        thirdStrip[i].r = thirdStrip[i].r + 1;
        fourthStrip[i].r = fourthStrip[i].r + 1;
        completed = false;
      }
      else if(leadStrip[i].r > nextStrip[i].r)
      {
        leadStrip[i].r = leadStrip[i].r - 1;
        secondStrip[i].r = secondStrip[i].r - 1;
        thirdStrip[i].r = thirdStrip[i].r - 1;
        fourthStrip[i].r = fourthStrip[i].r - 1;
        completed = false;
      }

      //slightly change the g value
      if(leadStrip[i].g < nextStrip[i].g)
      {
        leadStrip[i].g = leadStrip[i].g + 1;
        secondStrip[i].g = secondStrip[i].g + 1;
        thirdStrip[i].g = thirdStrip[i].g + 1;
        fourthStrip[i].g = fourthStrip[i].g + 1;
        completed = false;
      }
      else if(leadStrip[i].g > nextStrip[i].g)
      {
        leadStrip[i].g = leadStrip[i].g - 1;
        secondStrip[i].g = secondStrip[i].g - 1;
        thirdStrip[i].g = thirdStrip[i].g - 1;
        fourthStrip[i].g = fourthStrip[i].g - 1;
        completed = false;
      }

      //slightly change the b value
      if(leadStrip[i].b < nextStrip[i].b)
      {
        leadStrip[i].b = leadStrip[i].b + 1;
        secondStrip[i].b = secondStrip[i].b + 1;
        thirdStrip[i].b = thirdStrip[i].b + 1;
        fourthStrip[i].b = fourthStrip[i].b + 1;
        completed = false;
      }
      else if(leadStrip[i].b > nextStrip[i].b)
      {
        leadStrip[i].b = leadStrip[i].b - 1;
        secondStrip[i].b = secondStrip[i].b - 1;
        thirdStrip[i].b = thirdStrip[i].b - 1;
        fourthStrip[i].b = fourthStrip[i].b - 1;
        completed = false;
      }
    }//end of for loop
    FastLED.show();
  }//end of while loop
}//end of passColorsDownTheLine
