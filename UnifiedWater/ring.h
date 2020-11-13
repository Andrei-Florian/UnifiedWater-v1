// Universum | Universum Projects > WaterAid | ring

// Andrei Florian 30/MAY/2019

#include <Adafruit_NeoPixel.h>
#include <MKRGSM.h>

int pixels = 16;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixels, 5, NEO_GRB + NEO_KHZ800);

void colourLED(int wait)
{
  delay(wait);
  // generate a random colour
  int r = random(0, 100);
  int g = random(0, 100);
  int b = random(100, 255);
  uint32_t colour = strip.Color(r, g, b);
  
  for(int i = 0; i < pixels; i++)
  {
    strip.setPixelColor(i, colour); // set the colour of the pixel
    strip.show(); // send the command to the device
    delay(20);
  }

  delay(50);
  for(int i = 0; i <= 16; i++)
  {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
    strip.show();
    delay(20);
  }
}

void errorLED()
{
  uint32_t colour = strip.Color(255, 0, 0);

  for(int i = 0; i < 2; i++)
  {
    for(int i = 0; i < pixels; i++)
    {
      strip.setPixelColor(i, colour); // set the colour of the pixel
    }
  
    strip.show(); // send the command to the device

    delay(500);
    for(int i = 0; i <= 16; i++)
    {
      strip.setPixelColor(i, strip.Color(0, 0, 0));
      delay(20);
    }
    strip.show();
    delay(500);
  }
}

void startLEDRing()
{
  strip.begin();
  strip.show();

  delay(500);
  colourLED(50);
}
