#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#define PIN        6
#define NUMPIXELS 64
#define LENGTH 8

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 50

void setup() {
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif

  pixels.begin();
}

void loop() {
  pixels.clear();

  fire();
}

int pixel(int i, int j){
  // return the value of the pixel, with given row # and column #
  return (8-i)*8 + (i+1)%2*(j-1) + (i%2)*(8-j);
}

void fire() {
  for(int i=LENGTH; i>0; i--){
    int columns[4] = {1,2,7,8};
    for(int j : columns){
      pixels.setPixelColor(pixel(i,j), pixels.Color(0, 0, 0));
    }
    pixels.show();
    delay(50);
  }
  for(int i=LENGTH; i>0; i--){
    int columns[4] = {1,2,7,8};
    for(int j : columns){
      pixels.setPixelColor(pixel(i,j), pixels.Color(0, 0, 0));
    }
    pixels.show();
    delay(50);
  }
}
