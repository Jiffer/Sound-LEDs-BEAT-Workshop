// AnalogToNeoPixels
// maps the analog pin A0 to turn NEO Pixels
// 
// Requires Adafruit NeoPixel library
//

#include <Adafruit_NeoPixel.h>

#define PIXEL_PIN      3
#define NUMPIXELS      30

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS+1, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

// mic variables
int micLow = 300;
int micHigh = 600;

// Setup function
// runs once at startup
void setup() {
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)

  // wake up animation
  wakeUP(255, 100, 10);
}

// main loop
// runs continuously:
void loop() {
  
  // get value at analog pin
  int micValue = analogRead(A0);

  VUMeter(micValue);
}

void VUMeter(int level){
  // map to new range
  int outputLevel = abs(map(level, micLow, micHigh, 0, 50));

  // constrain to range
  outputLevel = constrain(outputLevel, 0, 30);

  // turn on outputLevel number of LEDs
  for(int i = 1; i < NUMPIXELS+1; i++){
    if(i < outputLevel+1 && i < 10){
      // yellow
      pixels.setPixelColor(i, pixels.Color(255, 100, 8));
    }else if(i < outputLevel+1 && i < 25){
      // green
      pixels.setPixelColor(i, pixels.Color(0, 200, 8));
    } else if(i < outputLevel+1){
      // red
      pixels.setPixelColor(i, pixels.Color(255, 0, 0));
    }
    else{
      // off
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    }
  }
  // write them to the strip
  pixels.show();
}


// swipe color on then off
void wakeUP(int r, int g, int b){
  pixels.clear();
  pixels.show();
  for(int i = 0; i < NUMPIXELS; i++){
      // set pixel to value R, G, B
      pixels.setPixelColor(i, pixels.Color(r, g, b));
      pixels.show();
      delay(5);
  }
  delay(100);
  for(int i = 0; i < NUMPIXELS; i++){
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
      pixels.show();
      delay(5);
  }
  pixels.clear();
  pixels.show();
}
