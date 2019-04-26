#include "FastLED.h"

#define DATA_PIN    3
#define LED_TYPE    WS2811
#define COLOR_ORDER RGB
#define NUM_LEDS    50
#define DELAY_TIME  9500 // 9.5 sec
#define BRIGHTNESS  10
#define FRAMES_PER_SECOND  120

enum colourMode {
  rainbow,
  red,
  green
};

CRGB leds[NUM_LEDS];   
char receiveVal[8];
uint16_t delayStart = 0; // the time the delay started
bool delayRunning = false; // true if still waiting for delay to finish
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
colourMode mode = rainbow;

void setup() 
{
  Serial.begin(4800); // For debugging
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
}
  
void loop() 
{  
  switch(mode) {
    case red:
      FastLED.showColor(CRGB::Red);
      break;
    case green:
      FastLED.showColor(CRGB::Green);
      break;
    default:
      rainbowLights();
  }

  if(Serial.available() > 0)  
  { 
    Serial.readString().toCharArray(receiveVal,8); 

    if (strcmp(receiveVal,"SUCCESS")==0)
    {
      mode = green;

      delayStart = millis();
      delayRunning = true;
    }
    else if (strcmp(receiveVal,"FAILURE")==0)
    {
      mode = red;
    }
  }

  if (delayRunning && ((millis() - delayStart) >= DELAY_TIME)) 
  {
    delayRunning = false;
    mode = rainbow;
  }
}

void rainbowLights()
{
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
  FastLED.show(); 
  FastLED.delay(1000/FRAMES_PER_SECOND); 
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
}