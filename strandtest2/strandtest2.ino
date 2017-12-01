#include <Adafruit_NeoPixel.h>

#define PIN 6

Adafruit_NeoPixel strip = Adafruit_NeoPixel(108, PIN, NEO_GRB + NEO_KHZ800);

uint32_t bigEdges[] = {0, 6, 7, 13, 14, 20, 21, 27, 28, 34, 35, 41};
uint32_t middleEdges[] = {42, 47, 48, 53, 54, 59, 60, 65, 66, 71, 72, 77};
uint32_t smallEdges[] = {78, 82, 83, 87, 88, 92, 93, 97, 98, 102, 103, 107};

uint32_t segment1[] = {0,1,2,3,4,5,6,21,22,23,24,25,26,27,48,49,50,51,52,53,66,67,68,69,70,71,78,79,80,81,82,93,94,95,96,97};
uint32_t segment2[] = {7,8,9,10,11,12,13,28,29,30,31,32,33,34,72,73,74,75,76,77,54,55,56,57,58,59,98,99,100,101,102,83,84,85,86,87};
uint32_t segment3[] = {14,15,16,17,18,19,20,35,36,37,38,39,40,41,42,43,44,45,46,47,60,61,62,63,64,65,103,104,105,106,107,88,89,90,91,92};

void setup() {
  // put your setup code here, to run once:
  strip.begin();
  strip.show();
  Serial.begin(9600);
}

void loop() {
  //strip.setPixelColor(0,0);
  fillsegments();
  //strip.show();
  delay(1);
}
void fillCorners() {
  for (uint16_t i = 0; i < sizeof(bigEdges)/sizeof(uint32_t); i++) {
    Serial.println(i);
    strip.setPixelColor(bigEdges[i], strip.Color(0,150,0));
    strip.setPixelColor(middleEdges[i], strip.Color(150,0,0));
    strip.setPixelColor(smallEdges[i], strip.Color(0,0,150));
    strip.show();
    delay(1);
  }
}

void fillsegments() {
  for (uint16_t i = 0; i < sizeof(segment1)/sizeof(uint32_t); i++) {
    Serial.println(i);
    strip.setPixelColor(segment1[i], strip.Color(0,150,0));
    strip.setPixelColor(segment2[i], strip.Color(150,0,0));
    strip.setPixelColor(segment3[i], strip.Color(0,0,150));
    strip.show();
    delay(1);
  }
}

