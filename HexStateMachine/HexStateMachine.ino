volatile int state;
#define interruptPin 3


void setup() {
  // put your setup code here, to run once:
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), changeState, RISING);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  stateMachine();
}

void stateMachine(){
  switch(state){
    case 0:
    Serial.println("State 0");
    break;
    case 1:
    Serial.println("State 1");
    break;
    case 2:
    Serial.println("State 2");
    break;
  }
}


void changeState(){
  Serial.println('ISR***********************');
  if (state < 2){
    state = state + 1;
  }
  else{
    state = 0;
  }
}

