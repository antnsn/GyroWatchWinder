#include <Stepper.h>
#include <Timeout.h>

#define STEPS 100

const int button1Pin = 2;
const int button2Pin = 3;
const int ledPin = 4;

int button1State = 0;
int button2State = 0;

// Define the two stepper motors
Stepper motor1(STEPS, 9, 11, 10, 12);
Stepper motor2(STEPS, 5, 7, 6, 8);

// Create a timer instance
Timeout runTimer;
Timeout pauseTimer;

// Define possible states for the state machine
enum State {
  RUN,
  PAUSE
};

bool initialRunDone = false;

// Initialize the state to RUN
State currentState = RUN;

void setup() {
  Serial.begin(9600);
  Serial.println("Stepper motor test");

  // Initialize pins
  pinMode(ledPin, OUTPUT);
  pinMode(button1Pin, INPUT);
  pinMode(button2Pin, INPUT);
  
  // Start the motor actions immediately upon setup
  motorActions();
}

void loop() {
  blinkLed(500);
  button1State = digitalRead(button1Pin);
  button2State = digitalRead(button2Pin);

  if (currentState == RUN) {
    handleButtonActions();
    if (!initialRunDone) {
      motorActions();
      runTimer.start(600000); // Start the timer for 10 minutes once motors are run
      initialRunDone = true;
    }
    if (runTimer.time_over()) {
      transitionToPause();
    }
  } else if (currentState == PAUSE) {
    if (pauseTimer.time_over()) {
      transitionToRun();
    }
  }
}


int transitionTimerID;  // Store the ID for our transition timer

// Function to transition from RUN to PAUSE
void transitionToPause() {
  currentState = PAUSE;
  Serial.println("Pausing for 50 minutes");
  runTimer.expire(); // Ensure the run timer is expired
  pauseTimer.start(3000000); // Set the pause timer for 50 minutes
}

void transitionToRun() {
  currentState = RUN;
  Serial.println("Running for 10 minutes");
  motorActions();
  pauseTimer.expire(); // Ensure the pause timer is expired
  runTimer.start(600000); // Set the run timer for 10 minutes
}


// Function to initiate motor actions
void motorActions() {
  Serial.println("Motor running");
  motor1.setSpeed(300);
  motor2.setSpeed(300);
}

// Function to handle various button states and their corresponding actions
void handleButtonActions() {
  if (button1State == HIGH && button2State == LOW) {
    performMotorAction(motor1, "LOOP1");
  } else if (button1State == LOW && button2State == HIGH) {
    performMotorAction(motor2, "LOOP2");
  } else if (button1State == LOW && button2State == LOW) {
    performBothMotorsAction();
  } else if (button1State == HIGH && button2State == HIGH) {
    Serial.println("INTERFERENCE");  // Indicate potential button interference
  }
}

// Function to perform motor actions for a single motor
void performMotorAction(Stepper &motor, const char* actionName) {
  Serial.println(actionName);
  int stepsToTake = -4096;
  motor.step(stepsToTake);
  delay(500);
  stepsToTake = 4096;
  motor.step(stepsToTake);
  blinkLed(500);
}

// Function to perform motor actions for both motors simultaneously
void performBothMotorsAction() {
  Serial.println("LOOP1&2");
  int stepsToTake = -4096;
  motor1.step(stepsToTake);
  motor2.step(stepsToTake);
  delay(500);
  stepsToTake = 4096;
  motor1.step(stepsToTake);
  motor2.step(stepsToTake);
  blinkLed(500);
}

// Function to toggle the LED state (blink)
void blinkLed(int duration) {
  digitalWrite(ledPin, !digitalRead(ledPin));  // Toggle LED state
  delay(duration);
}
