int topPot = A0;
int middlePot = A1;
int bottomPot = A2;
int topVal;
int middleVal;
int bottomVal;

void setup() {
  pinMode(topPot, INPUT);
  pinMode(middlePot, INPUT);
  pinMode(bottomPot, INPUT);
  Serial.begin(9600);
  topVal = 0;
  middleVal = 0;
  bottomVal = 0;
}

void loop() {

  if (analogRead(topPot) != 0) {
    topVal = analogRead(topPot);
  }
  if (analogRead(middlePot) != 0) {
    middleVal = analogRead(middlePot);
  }
  if (analogRead(bottomPot) != 0) {
    bottomVal = analogRead(bottomPot);
  }
  Serial.print("Top: ");
  Serial.print(topVal);
  Serial.print(" Middle: ");
  Serial.print(middleVal);
  Serial.print(" Bottom: ");
  Serial.println(bottomVal);


}

