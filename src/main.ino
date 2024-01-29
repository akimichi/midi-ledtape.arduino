// #include <M5Atom.h>
#include <FastLED.h>

#define DATA_PIN        26  // LEDテープ信号端子
#define NUM_LEDS  15  // LEDの数

CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(115200);
  delay(1000);

  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(20);
}

void clear(){
  Serial.println("Clearing display");
  for(int num=0; num<NUM_LEDS; num++) {
    Serial.println(num);
    leds[num] = CRGB::Black;
    FastLED.show();
    delay(50);
  }
}
void loop() {
  // Initialize all device LEDs to off (black), one at a time
  clear();
  // Initialize device LEDs to on (red), one at a time
  // Serial.println("Showing LEDs");
  // for(int num=0; num<NUM_LEDS; num++) {
  //   Serial.println(num);
  //   leds[num] = CRGB::Red;
  //   FastLED.show(); 
  //   delay(50);
  // }
}


