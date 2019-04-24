#include "FastLED.h"

FASTLED_USING_NAMESPACE

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    3
#define LED_TYPE    WS2811
#define COLOR_ORDER RGB
#define NUM_LEDS    50
CRGB leds[NUM_LEDS];

#define BRIGHTNESS          10
#define FRAMES_PER_SECOND  120

String receiveVal = "";     
unsigned long delayStart = 0; // the time the delay started
bool delayRunning = false; // true if still waiting for delay to finish
unsigned long DELAY_TIME = 9500; // 9.5 sec
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
bool showRainbow = true;
bool showError = false;

void setup() 
{
  Serial.begin(4800); // For debugging
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  FastLED.setBrightness(BRIGHTNESS);

  delayStart = millis();   // start delay
  delayRunning = false; // not finished yet
}
  
void loop() 
{  
    if (showRainbow)
    {
      rainbowLights();
    }
    else if (showError)
    {
      FastLED.showColor(CRGB::Red);
    }
    else
    {
      FastLED.showColor(CRGB::Green);
    }

    if(Serial.available() > 0)  
    { 
      receiveVal = Serial.readString(); 

      if (receiveVal == "g" || receiveVal == "SUCCESS")
      {
        showRainbow = false;
        showError = false;

        delayStart = millis();
        delayRunning = true;
      }
      else if (receiveVal == "r" || receiveVal == "FAILURE")
      {
        showRainbow = false;
        showError = true;
        FastLED.showColor(CRGB::Red);
      }
    }

    if (delayRunning && ((millis() - delayStart) >= DELAY_TIME)) 
    {
      delayRunning = false;
      showRainbow = true;
    }
}

void rainbowLights()
{
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
  FastLED.show(); 
  FastLED.delay(1000/FRAMES_PER_SECOND); 
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
}