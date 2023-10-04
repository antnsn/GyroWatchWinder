#include <Stepper.h>

#define STEPS 100

const int button1Pin = 2;
const int button2Pin = 3;
const int ledPin = 4;

int button1State = 0;
int button2State = 0;

Stepper motor1(STEPS, 9, 11, 10, 12); 
Stepper motor2(STEPS, 5, 7, 6, 8);
int counter = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Stepper motor test");
  pinMode(ledPin, OUTPUT);
  pinMode(button1Pin, INPUT);      
  pinMode(button2Pin, INPUT);
}

void loop() {
  blinkLed(100);
  button1State = digitalRead(button1Pin);
  button2State = digitalRead(button2Pin);
  Serial.println("Motor running");

  motor1.setSpeed(300);
  motor2.setSpeed(300);

  if (counter <= 30) {
    handleButtonActions();
  } else {
    pauseAndBlink();
  }
}

void handleButtonActions() {
  if (button1State == HIGH && button2State == LOW) {
    performMotorAction(motor1, "LOOP1");
  } else if (button1State == LOW && button2State == HIGH) {
    performMotorAction(motor2, "LOOP2");
  } else if (button1State == LOW && button2State == LOW) {
    performBothMotorsAction();
  } else if (button1State == HIGH && button2State == HIGH) {
    Serial.println("INTERFERENCE");
    counter++;
  }
}

void performMotorAction(Stepper &motor, const char* actionName) {
  Serial.println(actionName);
  
  int stepsToTake = -4096;
  motor.step(stepsToTake);
  delay(2000);

  stepsToTake = 4096;
  motor.step(stepsToTake);
  delay(2000);

  blinkLed(100);
  counter++;
}

void performBothMotorsAction() {
  Serial.println("LOOP1&2");
  
  int stepsToTake = -4096;
  motor1.step(stepsToTake);
  motor2.step(stepsToTake);

  stepsToTake = 4096;
  motor1.step(stepsToTake);
  motor2.step(stepsToTake);

  blinkLed(100);
  counter++;
}

void pauseAndBlink() {
  if (counter <= 180) {
    Serial.println("PAUSE");
    blinkLed(1000);
    counter++;
  } else {
    Serial.println("LONG PAUSE - 5 minutes");
    delay(300000);
    counter = 0;
  }
}

void blinkLed(int duration) {
  digitalWrite(ledPin, !digitalRead(ledPin));  // Toggle LED state
  delay(duration);
}
