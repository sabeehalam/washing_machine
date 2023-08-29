#include <JC_Button.h>

#define LEFT_ROTATE_PIN 5
#define RIGHT_ROTATE_PIN 2
#define START_PUSH_BUTTON 4
#define STOP_PUSH_BUTTON 3
#define FIRST_CHECK_PIN 6
#define SECOND_CHECK_PIN 7
#define WASHING_DELAY 10000
#define WASHING_HOLD_DELAY 15000
#define COUNTER_REVS 6

// pin assignments and button def
const byte START_BUTTON_PIN(START_PUSH_BUTTON);
const byte STOP_BUTTON_PIN(STOP_PUSH_BUTTON);
Button StartBtn(START_BUTTON_PIN, true);       // define the button
Button StopBtn(STOP_BUTTON_PIN, true);

static uint32_t time_start = 0;
static uint8_t right_rotate_pin = 0;
static uint8_t left_rotate_pin = 0;
static uint8_t dir = 0;
static uint8_t counter = 0;
static uint8_t complete = 0;
static uint8_t start = 0;
static uint8_t time_taken = 0;
  
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  StartBtn.begin();       // define the button
  StopBtn.begin();       // define the button
  pinMode(LEFT_ROTATE_PIN, OUTPUT);
  pinMode(RIGHT_ROTATE_PIN, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  StartBtn.read();
  StopBtn.read();

  if (StartBtn.wasReleased()){
    start = 1;
    complete = 0;
  }
  if(StopBtn.wasReleased()){
    start = 0;
    complete = 1;
    time_taken = 0;
  }

  if(complete != 1 && start == 1){

    // Serial.print("Direction: ");
    // Serial.println(dir);
    // Serial.print("Start: ");
    // Serial.println(start);

    if(dir == 0){ //Check the direction selected
      if(time_taken != 1){ // Check whether new time has been declared. Runs if time hasn't been declared
        Serial.println("Right pin rotate on");
        digitalWrite(RIGHT_ROTATE_PIN, HIGH); // Turn on right rotation pin
        time_start = millis(); //Note the time at start to measure the timer
        Serial.print("Time Start");
        Serial.println(time_start);
        time_taken = 1;
        right_rotate_pin = 1;
      }

      if(millis()-time_start > WASHING_DELAY && millis()- time_start < WASHING_HOLD_DELAY && right_rotate_pin == 1){
        digitalWrite(RIGHT_ROTATE_PIN, LOW);
        right_rotate_pin = 0;
        Serial.println("Right pin rotate off");
      }

      if(millis()-time_start > WASHING_HOLD_DELAY){
        Serial.println("time reset right");
        time_taken = 0;
        dir = 1;
        counter++;
      }
    }

    if(dir == 1){
      if(time_taken != 1){
        digitalWrite(LEFT_ROTATE_PIN, HIGH);
        time_start = millis();
        time_taken = 1;
        left_rotate_pin = 1;
        Serial.println("Left pin rotate on");
      }

      if(millis()-time_start > WASHING_DELAY && millis()-time_start < WASHING_HOLD_DELAY && left_rotate_pin == 1){
        digitalWrite(LEFT_ROTATE_PIN, LOW);
        left_rotate_pin = 0;
        Serial.println("Left pin rotate off");
      }

      if(millis()-time_start > WASHING_HOLD_DELAY){
        time_taken = 0;
        dir = 0;
        counter++;
        Serial.println("time reset left");
      } 
    }
  }
  // Stop the  washing process if called upon by stop button or completion
  if(start == 0){
    digitalWrite(LEFT_ROTATE_PIN, LOW);
    digitalWrite(RIGHT_ROTATE_PIN, LOW);
    right_rotate_pin = 0;
    left_rotate_pin = 0;
  }
  // Check counter value to stop the washing process
  if(counter >COUNTER_REVS){
    complete = 1;
    start = 0;
    counter = 0;
    Serial.print("Washing Completed");
  }
}
