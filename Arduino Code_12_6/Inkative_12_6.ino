#include <Servo.h>
#include <math.h> 
Servo servo1;

// 0 is for round control
// 1 is for rail control
#define DIR_PIN0 2
#define STEP_PIN0 3

#define DIR_PIN1 4
#define STEP_PIN1 5

float d2 = 0.0;
float d1 = 0.0;
int size = 0;
int deeper = 0;
float angle = 0;
int step_rail = 0;

int incomingByte = 0;   // for incoming serial data
int flag = -1;          // mark what kind of data is received
                        /* 
                           1: x;
                           2: y;
                           3: s;
                        */

void setup(){
  Serial.begin(9600);  
  pinMode(13, OUTPUT);
  pinMode(DIR_PIN0, OUTPUT); 
  pinMode(STEP_PIN0, OUTPUT);
  pinMode(DIR_PIN1, OUTPUT);
  pinMode(STEP_PIN1, OUTPUT); 
  servo1.attach(14); //analog pin 0
  
}

void loop(){
  // send data only when you receive data:
  if(Serial.available() > 0) {
     // read the incoming byte:
     incomingByte = Serial.read();
     
     if(incomingByte == 'x'){ 
       angle = random(-180,180)/* - angle*/;
       step_rail = random(-10,10);
       deeper = random(-45, 45);
     }
     else if(incomingByte == 'y') { 
       angle = random(-30,30)/* - angle*/;
       step_rail = random(-10,10);
       deeper = random(-45, 45);
     }
     else if(incomingByte == 's') { 
       angle = random(-30,30)/* - angle*/;
       step_rail = random(-10,10);
       deeper = random(-45, 45);
     }
     
       
     rotateDeg(angle, .05, DIR_PIN0, STEP_PIN0); 
     rotateDeg(step_rail, .05, DIR_PIN1, STEP_PIN1);
     servo1.write(deeper);

   //rotateDeg(5, .05, DIR_PIN0, STEP_PIN0); 
    // rotate(30, .05, DIR_PIN1, STEP_PIN1);
     //servo1.write(1*5);
  }
     
   delay(100); 
}

void rotateDeg(float deg, float speed, int motor_dir, int motor_step){ 
  //rotate a specific number of degrees (negitive for reverse movement)
  //speed is any number from .01 -> 1 with 1 being fastest - Slower is stronger
  int dir = (deg > 0)? HIGH:LOW;
  digitalWrite(motor_dir,dir); 

  int steps = abs(deg)*(1/0.225);
  float usDelay = (1/speed) * 70;

  for(int i=0; i < steps; i++){ 
    digitalWrite(motor_step, HIGH); 
    delayMicroseconds(usDelay); 

    digitalWrite(motor_step, LOW); 
    delayMicroseconds(usDelay); 
  } 
}
void rotate(int steps, float speed, int motor_dir, int motor_step){ 
  //rotate a specific number of microsteps (8 microsteps per step) - (negitive for reverse movement)
  //speed is any number from .01 -> 1 with 1 being fastest - Slower is stronger
  int dir = (steps > 0)? HIGH:LOW;
  steps = abs(steps);

  digitalWrite(motor_dir,dir); 

  float usDelay = (1/speed) * 70;

  for(int i=0; i < steps; i++){ 
    digitalWrite(motor_step, HIGH); 
    delayMicroseconds(usDelay); 

    digitalWrite(motor_step, LOW); 
    delayMicroseconds(usDelay); 
  } 
} 
