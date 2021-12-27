#include <Arduino.h>
#define FASTLED_INTERNAL // supppresses notifications from FASTLED
#include <FastLED.h>

// define some basic setup terms for FASTLED
#define NUM_LEDS 36 // number of leds in the strip
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
CRGB nextStrip[NUM_LEDS] = {0};


volatile unsigned long currentTime = micros();
volatile unsigned long previousTime;
volatile unsigned long timeReadings[] = {0, 0, 0, 0};//number array containing the most recent four periods of 1/4 revolutions
volatile byte timeReadingIndex;
unsigned long timeReadingsSum;
unsigned long timeMarker = micros();
volatile unsigned long averagePeriod;
boolean completed = false;
int randomNumberR, randomNumberG, randomNumberB;
int randomNumber1, randomNumber2, randomNumber3, randomNumber1A, randomNumber2A, randomNumber3A;
boolean firstTime = false;
boolean nextColor = true;

/*****************
**Program Begins**
*****************/
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

  FastLED.setBrightness(25);
  
  delay(500);//just a small delay to get the micros() to a safely positive number. 
}//end of setup()


void loop() 
{
  timeMarker = millis();
  while(millis() < timeMarker + 10000)// random colors code
  {
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
  }
  FastLED.clear();
  
  while(millis() < timeMarker + 20000)// burst rings code
  {
    randomNumber1 = random(0, 256);
    randomNumber2 = random(0, 256);
    randomNumber3 = random(0, 256);
    randomNumber1A = random(0, 256);
    randomNumber2A = random(0, 256);
    randomNumber3A = random(0, 256);

    for(int i = NUM_LEDS - 1; i >= 0; i--)
    {
      leadStrip[i] = CRGB(randomNumber1, randomNumber2, randomNumber3);
  
      leadStrip[(i + 18) % 36] = CRGB(randomNumber1A, randomNumber2A, randomNumber3A);
      
      secondStrip[i] = CRGB(randomNumber1, randomNumber2, randomNumber3);
  
      secondStrip[(i + 18) % 36] = CRGB(randomNumber1A, randomNumber2A, randomNumber3A);
      
      thirdStrip[i] = CRGB(randomNumber1, randomNumber2, randomNumber3);
  
      thirdStrip[(i + 18) % 36] = CRGB(randomNumber1A, randomNumber2A, randomNumber3A);
      
      fourthStrip[i] = CRGB(randomNumber1, randomNumber2, randomNumber3);
  
      fourthStrip[(i + 18) % 36] = CRGB(randomNumber1A, randomNumber2A, randomNumber3A);
      
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
    }// end of for
  }// end of while
  FastLED.clear();
  
  // setup for the rainbow
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
  
  while(millis() < timeMarker + 30000)// rainbow code
  {
    //used to be passColorsDownTheLine(), but function calls slow down code execution, so I made it inline instead
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
  }
  FastLED.clear();
  //setup for spiral
  nextColor = true;
  randomNumber1 = random(0, 256);
  randomNumber2 = random(0, 256);
  randomNumber3 = random(0, 256);
  while(millis() < timeMarker + 45000)// spiral code
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
  }//end of while
  FastLED.clear();
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
