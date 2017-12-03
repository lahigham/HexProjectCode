//necessary libraries
#include <Adafruit_NeoPixel.h>

//function declarations
void changeState(); //ISR
void softPotUpdate(); //updates & maps soft potentiometer readings to 8 bit
void soundUpdate(); //updates & maps sound sensor readings to 32 unsigned int
void colorUpdate(); //scales sound value to colors
void lightSensing(); //sets lights and color based on sensor input
void colorFlash(uint32_t color, byte hexSize); //flashes through strip with certain color
uint32_t Wheel(int WheelPos); //converts value 0 - 255 to color
void theaterChaseRainbow(uint8_t wait); //crawls through lights with rainbow pattern


//analog pins
#define topPot A0
#define middlePot A1
#define bottomPot A2
#define soundPin A3

//digital pins
#define interruptPin 3
#define stripPin 6

volatile int state;

//global potentiometer values
int topVal;
int middleVal;
int bottomVal;

//global sound sensor value
int amplitude;

//global color values
uint32_t color_seg1;
uint32_t color_seg2;
uint32_t color_seg3;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(108, stripPin, NEO_GRB + NEO_KHZ800);

//pixel index of the offset sides
uint32_t segment1[] = {0,1,2,3,4,5,6,21,22,23,24,25,26,27,48,49,50,51,52,53,66,67,68,69,70,71,78,79,80,81,82,93,94,95,96,97};
uint32_t segment2[] = {7,8,9,10,11,12,13,28,29,30,31,32,33,34,72,73,74,75,76,77,54,55,56,57,58,59,98,99,100,101,102,83,84,85,86,87};
uint32_t segment3[] = {14,15,16,17,18,19,20,35,36,37,38,39,40,41,42,43,44,45,46,47,60,61,62,63,64,65,103,104,105,106,107,88,89,90,91,92};

//pixel index for each hexagon
uint32_t bigHex[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41};
uint32_t middleHex[] = {42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77};
uint32_t smallHex[]= {78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107};

//colors
uint32_t off = strip.Color(0,0,0); //would be white if strip was an RGBW
uint32_t lightBlue = strip.Color(0,255,255);
uint32_t medBlue = strip.Color(0,205,255);
uint32_t royalBlue = strip.Color(0,0,255);
uint32_t lightPurple = strip.Color(239,201,255);
uint32_t medPurple = strip.Color(207,116,246);
uint32_t deepPurple = strip.Color(180,0,255);

void setup() {
  //serial monitor setup
  Serial.begin(9600);
    
  //potentiometer setup
  pinMode(topPot, INPUT);
  pinMode(middlePot, INPUT);
  pinMode(bottomPot, INPUT);
  topVal = 0;
  middleVal = 0;
  bottomVal = 0;

  //sound sensor setup
  pinMode(soundPin, INPUT);
  amplitude = 0;
  
  //button and ISR setup
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), changeState, RISING);

  //LED strip setup
  strip.begin();
  strip.setBrightness(190); //75% brightness
  strip.show();

  //color setup
  uint32_t color_seg1 = 0;
  uint32_t color_seg2 = 0;
  uint32_t color_seg3 = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  stateMachine();
}

void stateMachine(){
  switch(state){
    case 0: //Sensing mode
    Serial.println("State 0");
    softPotUpdate();
    colorUpdate();
    lightSensing();
    break;
    case 1: //Color fade through whole strand mode
    Serial.println("State 1");
    theaterChaseRainbow(60);
    break;
    case 2: //Flashy through each hex mode
    Serial.println("State 2");
    colorFlash(lightBlue, 3);
    delay(100);
    colorFlash(off, 3);
    delay(100);
    colorFlash(medBlue, 2);
    delay(100);
    colorFlash(off, 2);
    delay(100);
    colorFlash(royalBlue, 1);
    delay(100);
    colorFlash(off, 1);
    delay(100);
    colorFlash(lightPurple, 3);
    delay(100);
    colorFlash(off, 3);
    delay(100);
    colorFlash(medPurple, 2);
    delay(100);
    colorFlash(off, 2);
    delay(100);
    colorFlash(deepPurple, 1);
    delay(100);
    colorFlash(off, 1);
    break;
    case 3: //Clear lights
    Serial.println("State 3");
    strip.clear();
    break;
  }
}

//button interrupt service routine
void changeState(){
  if (state < 3){
    state = state + 1;
  }
  else{
    state = 0;
  }
}

//read soft potentiometers, scale to length of LED segments
void softPotUpdate(){
  if (analogRead(topPot) != 0) {
    topVal = analogRead(topPot);
    topVal = map(topVal, 0, 1023, 0, 35);
  }
  if (analogRead(middlePot) != 0) {
    middleVal = analogRead(middlePot);
    middleVal = map(middleVal, 0, 1023, 0, 35);
  }
  if (analogRead(bottomPot) != 0) {
    bottomVal = analogRead(bottomPot);
    bottomVal = map(bottomVal, 0, 1023, 0, 35);
  }
}

//read sound sensor, scale to 32-bit RGB color values
void soundUpdate(){
  if (analogRead(soundPin) <= 50){
      amplitude = analogRead(soundPin);
      amplitude = map(amplitude, 0, 28, 0, 255);
  }
}

//scales the value of the sound for each segment 
void colorUpdate(){
  soundUpdate();
  color_seg1 = Wheel(amplitude);
  color_seg2 = Wheel(amplitude/1.2);
  color_seg3 = Wheel(amplitude/3);
}

//change what lights are on based on soft pots, color based on sound
void lightSensing(){
  //segment 1 update
  for (uint16_t i = 0; i < topVal; i++){
    Serial.print(i);
    strip.setPixelColor(segment1[i], color_seg1);
  }
  //segment 2 update
  for (uint16_t i = 0; i < middleVal; i++){
    Serial.print(i);
    strip.setPixelColor(segment2[i], color_seg2);
  }  
  
  //segment 3 update
  for (uint16_t i = 0; i < bottomVal; i++){
    Serial.print(i);
    strip.setPixelColor(segment3[i], color_seg3);
  }
  strip.show();
  delay(100);
}

//flash through hexagons with specific colors
// 1 = bigHex
// 2 = middleHex
// 3 = smallHex
void colorFlash(uint32_t color, byte hexSize){
  if (hexSize == 1){
    for (uint16_t i = 0; i < sizeof(bigHex)/sizeof(uint32_t); i++) {
      strip.setPixelColor(bigHex[i], color);
      strip.show();
      delay(40);
    }
  }
  else if (hexSize == 2){
    for (uint16_t i = 0; i < sizeof(middleHex)/sizeof(uint32_t); i++) {
      strip.setPixelColor(middleHex[i], color);
      strip.show();
      delay(40);
    }
  }
  else{ //assuming it's 3
    for (uint16_t i = 0; i < sizeof(smallHex)/sizeof(uint32_t); i++) {
      strip.setPixelColor(smallHex[i], color);
      strip.show();
      delay(40);
    }
  }
}

//FOLLOWING FROM ADAFRUIT ARDUINO LIBRARY EXAMPLES

// Input a value 0 to 255 to get a color value.
// The colors are a transition r - g - b - back to r.
uint32_t Wheel(int WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

