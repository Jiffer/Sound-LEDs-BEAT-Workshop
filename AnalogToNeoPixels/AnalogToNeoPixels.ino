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

float decayingMicReading = 0;

// Setup function
// runs once at startup
void setup() {
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)

}

// main loop
// runs continuously:
void loop() {
  
  micToBrightness();
  // micToBrightnessEnvelopeFollow();
}

// Read Analog pin and map reading to a brightness value for all pixels
void micToBrightness(){
  // get value at analog pin
  int micValue = analogRead(A0);

  // map to new range
  int brightness = abs(map(micValue, micLow, micHigh, 0, 255));

  // constrain to range
  brightness = constrain(brightness, 0, 255);
  

  // set all pixels to the same brightness based on the current mic reading
  for(int i = 1; i < NUMPIXELS+1; i++){
    pixels.setPixelColor(i, pixels.Color(brightness, brightness, brightness));
  }
  // write them to the strip
    pixels.show();
}


// Read Analog pin and map reading to a brightness value for all pixels
void micToBrightnessEnvelopeFollow(){
  // get value at analog pin
  int micValue = analogRead(A0);

// Peak follower
// if new mic input value is greater than saved value than use the new input 
  if (micValue > decayingMicReading){
    decayingMicReading = micValue;
  }
  // if it is not greater then decay the value over time
  else{
    decayingMicReading *= .999;
  }

  // map to new range
  int brightness = abs(map(decayingMicReading, micLow, micHigh, 0, 255));

  // constrain to range
  brightness = constrain(brightness, 0, 255);

  // if (brightness < 50){
  //   brightness = 0;
  // }

  // set all pixels to the same brightness based on the current mic reading
  for(int i = 1; i < NUMPIXELS+1; i++){
    pixels.setPixelColor(i, pixels.Color(brightness, brightness, brightness));
  }
  // write them to the strip
    pixels.show();
}