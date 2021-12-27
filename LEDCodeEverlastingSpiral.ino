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

boolean nextColor = true;
int randomNumber1, randomNumber2, randomNumber3, randomNumber1A, randomNumber2A, randomNumber3A;

/*****************
**Program Begins**
*****************/
void setup() 
{
  Serial.begin(115200);//set the serial transmission rate to 115200 bauds

  //generate a pseudo-random seed from analog pin 0, which is unused in this case
  randomSeed(analogRead(A0));
  //generate 3 initial random RGB values
  randomNumber1 = random(0, 256);
  randomNumber2 = random(0, 256);
  randomNumber3 = random(0, 256);
    
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
  for(int i = NUM_LEDS - 1; i >= 0; i--)//moves the LED's one position up every time it runs
  {
    if(nextColor == true)//if the smooth transition is completed, generate a new random color and 
    {
      randomNumber1A = random(0, 256);
      randomNumber2A = random(0, 256);
      randomNumber3A = random(0, 256);
      nextColor = false;
    }
    else if(i%4 == 0)//allows for a smooth transition to another color;
    {
      nextColor = true;//always assume that the transition is completed, until proven otherwise below
      if(randomNumber1 < randomNumber1A)
      {
        randomNumber1++;
        nextColor = false;
      }
      else if(randomNumber1 > randomNumber1A)
      {
        randomNumber1--;
        nextColor = false;
      }
      if(randomNumber2 < randomNumber2A)
      {
        randomNumber2++;
        nextColor = false;
      }
      else if(randomNumber2 > randomNumber2A)
      {
        randomNumber2--;
        nextColor = false;
      }
      if(randomNumber3 < randomNumber3A)
      {
        randomNumber3++;
        nextColor = false;
      }
      else if(randomNumber3 > randomNumber3A)
      {
        randomNumber3--;
        nextColor = false;
      }
    }
    for(int j = 0; j < NUM_LEDS; j += 12)//spaces out the LED's by a value of "j +=" every time it runs
    {
      leadStrip[(i + j) % 36] = CRGB(randomNumber1, randomNumber2, randomNumber3);
    }
    for(int j = 3; j < NUM_LEDS; j += 12)//different j values allow for a staggering effect, hence the spiral
    {
      secondStrip[(i + j) % 36] = CRGB(randomNumber1, randomNumber2, randomNumber3);
    }
    for(int j = 6; j < NUM_LEDS; j += 12)
    {
      thirdStrip[(i + j) % 36] = CRGB(randomNumber1, randomNumber2, randomNumber3);
    }
    for(int j = 9; j < NUM_LEDS; j += 12)
    {
      fourthStrip[(i + j) % 36] = CRGB(randomNumber1, randomNumber2, randomNumber3);
    }//end of j for loop[s]

    FastLED.show();
    delay(3);
    FastLED.clear();
  }//end of i for loop
}//end of loop() aka main()
