// AnalogToNeoPixels
// maps the analog pin A0 to turn NEO Pixels
//
// Requires Adafruit NeoPixel library
//

#include <Adafruit_NeoPixel.h>

#define PIXEL_PIN 3
#define NUMPIXELS 30

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS + 1, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

// Mic variables
int micLow = 300;
int micHigh = 600;

float smoothing = .9;
float micAvg;
float peakFollower = 0;
float peakDecay = .98;

// circular buffers
const int micHistoryLength = NUMPIXELS;
int peakHistory[micHistoryLength];
int peakHistoryPointer = 0;

// history update timers
unsigned long historyTimer = 0;
int historyInterval = 5;  // in ms

// Setup function
// runs once at startup
void setup() {
  Serial.begin(115200);
  pixels.begin();  // INITIALIZE NeoPixel strip object (REQUIRED)

  // wake up animation
  wakeUP();
}

// main loop
// runs continuously:
void loop() {

  // get value at analog pin
  int micValue = analogRead(A0);
  micProcessing(micValue);

  // VUMeter(micValue);
  // brightness(int(peakFollower));
  brightTrails();
}


// pass in mic value
// creates moving average
// peak follower
// circular buffer
void micProcessing(int micValue) {
  // moving average
  micAvg = smoothing * micAvg + (1 - smoothing) * micValue;

  // map to control range
  int mappedLevel = constrain(map(micValue, micLow, micHigh, 0, 500), 0, 255);
  // int mappedLevel = constrain(abs(map(micValue, micLow, micHigh, 0, 500)), 0, 255);

  // decay peakFollower value
  peakFollower *= peakDecay;

  // if new input value is greater than exponential decay, set it to higher level
  if (mappedLevel > peakFollower) {
    peakFollower = mappedLevel;
  }

  // check if it is time to store a sample
  if (millis() > historyTimer) {
    historyTimer = millis() + historyInterval;

    peakHistory[peakHistoryPointer++] = peakFollower;
    peakHistoryPointer %= micHistoryLength;
  }
}

void VUMeter(int level) {

  // map to new range
  int outputLevel = map(level, micLow, micHigh, 0, 50);

  // constrain to range
  outputLevel = constrain(outputLevel, 0, 30);

  // turn on outputLevel number of LEDs
  for (int i = 1; i < NUMPIXELS + 1; i++) {
    if (i < outputLevel + 1 && i < 10) {
      // yellow
      pixels.setPixelColor(i, pixels.Color(255, 100, 8));
    } else if (i < outputLevel + 1 && i < 25) {
      // green
      pixels.setPixelColor(i, pixels.Color(0, 200, 8));
    } else if (i < outputLevel + 1) {
      // red
      pixels.setPixelColor(i, pixels.Color(255, 0, 0));
    } else {
      // off
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    }
  }
  // write them to the strip
  pixels.show();
}

void brightness(int level) {
  level = constrain(level, 0, 255);

  int r = (int)(1.0 * level);
  int g = int(.4 * level);
  int b = int(.05 * level);
  // turn on outputLevel number of LEDs
  for (int i = 1; i < NUMPIXELS + 1; i++) {
    pixels.setPixelColor(i, pixels.Color(r, g, b));
  }
  // write them to the strip
  pixels.show();
}

void brightTrails() {
  for (int i = 0; i < NUMPIXELS; i++) {
    int tempPtr = (int)(NUMPIXELS - i + peakHistoryPointer) % micHistoryLength;
    int historyVal = peakHistory[tempPtr];

    // set color ratios
    int r = (int)(.5 * historyVal);
    int g = int(.4 * historyVal);
    int b = int(.15 * historyVal);
    // Serial.println(historyVal);
    pixels.setPixelColor(i, pixels.Color(r, g, b));
  }
  pixels.show();
}

// swipe color on then off
void wakeUP() {
  pixels.clear();
  pixels.show();
  for (int i = 0; i < NUMPIXELS; i++) {
    // set pixel to value R, G, B
    pixels.setPixelColor(i, pixels.Color(255, 100, 8));
    pixels.show();
    delay(5);
  }
  delay(50);
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    pixels.show();
    delay(5);
  }
  pixels.clear();
  pixels.show();
}
