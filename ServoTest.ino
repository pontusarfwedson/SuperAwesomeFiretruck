#include <Servo.h> 

Servo myservo;
int speed=0;
int dirr=1; //1 is counterclock, -1 is clockwise

void setup() 
{ 
  Serial.begin(9600);
  myservo.attach(9);
  int currSpeed=0;
  if(dirr==1){
    currSpeed = 95-speed;
    myservo.write(currSpeed);
  }else{
    currSpeed = 95+speed;
    myservo.write(currSpeed);
  }
  Serial.print(currSpeed);
} 

void loop() {
  
  } 
