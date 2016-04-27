/*
 *  DEFINE ULTRASONIC SENSOR PINS HEJHEJ
 */

#define trigL 2
#define echoL 4
#define trigF 5
#define echoF 6
#define trigR 7
#define echoR 10

/*
 * DEFINE MOTOR PINS
 */

 #define motorL 12
 #define brakeL 9
 #define speedL 3

 #define motorR 13
 #define brakeR 8
 #define speedR 11

 /*
  * DEFINE FLAME SENSOR PINS
  */

 #define flameL A15
 #define flameML A14
 #define flameM A12
 #define flameMR A11
 int digitalPin = 52;
 int sirenPin = 22;

 // DEFINE SETTINGS
 int factor = 5; //NEEDS TO BE TESTED (so that the car turns as many angles as given in the turnLeft, turnRight function)
 int distantFire1 = 1000;
 int distantFire2 = 1000;
 int distantFire3 = 500;
 int distantFire4 = 950;
 int closeFire = 300;
 int foundFire = 20;

 // DEBUG SETTINGS
 int showFlameSensors = 1;
 int showDistSensors = 0;
 int debugPin = 53;
 


void setup() {
  
  Serial.begin (9600);
  
  // MOTOR SETUP! LEFT = A, RIGHT = B.
  pinMode(motorL, OUTPUT); //Initiates Motor Channel A pin
  pinMode(brakeL, OUTPUT); //Initiates Brake Channel A pin
  pinMode(motorR, OUTPUT); //Initiates Motor Channel B pin
  pinMode(brakeR, OUTPUT);  //Initiates Brake Channel B pin

  // ULTRA SONIC SENSOR SETUP
  pinMode(trigL, OUTPUT);
  pinMode(echoL, INPUT);
  pinMode(trigF, OUTPUT);
  pinMode(echoF, INPUT);
  pinMode(trigR, OUTPUT);
  pinMode(echoR, INPUT);

  // SIREN SETUP
  pinMode(sirenPin, OUTPUT); 
  digitalWrite(sirenPin, LOW);   // sets the Siren pin low as start value

  //DIGITAL FLAME SENSOR SETUP
  pinMode(digitalPin, INPUT);
  pinMode(debugPin, OUTPUT);
  digitalWrite(debugPin, LOW);


}

void loop() {
  
  int spd = speed(45);
  int dist = 20;
  while(checkDist('L') > dist && checkDist('F') > dist && checkDist('R') > dist){
   if(foundFlame()){
    Serial.print("FLAME! \n");
    extinguish();
   }
   forward(spd);
  }

  if(checkDist('L') < dist && checkDist('F') < dist && checkDist('R') < dist){
   backward(spd);
   delay(3000);
  }
  
   while(checkDist('L') <= dist){
      turnRightCont(spd);
   }
   
   while(checkDist('F') <= dist){
      if(checkDist('L') > checkDist('R')){
        turnLeftCont(spd);
    }else{
        turnRightCont(spd);
    }
  }
  
  while(checkDist('R') <= dist){
      turnLeftCont(spd);
  }

  


}

/************************************************************
 ******************** ULTRA SONIC METHODS
 ************************************************************/

long checkDist(char dir){
  // PINS
  int trigLpin = 2;
  int echoLpin = 4;
  int trigFpin = 5;
  int echoFpin = 6;
  int trigRpin = 7;
  int echoRpin = 10;
  
  int trigPin = trigFpin;
  int echoPin = echoFpin;
  String print = "F";
  switch(dir){
    case 'L':
        trigPin = trigLpin;
        echoPin = echoLpin;
        print = "L";
      break;

    case 'F':

      break;

    case 'R':
        trigPin = trigRpin;
        echoPin = echoRpin;
        print = "R";
      break;

    default:

      break;
  }
  long duration, distance;
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  if(showDistSensors == 1){
  String printNew = print + ": " + distance;
  Serial.print(printNew);
  Serial.print("\n");
  }
  return distance;
}

/*************************************************
 * ******* FLAME SENSING METHOD *****************
 ***********************************************/

 int flame(int direction){
  int sensorValue;
  int digitalVal;
  
  switch(direction){
    case 1: //LEFT
      sensorValue = analogRead(flameL);
      break;

    case 2: //MID-LEFT
      sensorValue = analogRead(flameML);
      break;

    case 3: //MID
      sensorValue = analogRead(flameM);
      
      break;

    case 4: //MID-RIGHT 
      sensorValue = analogRead(flameMR);

      break;

    case 5: //RIGHT
      digitalVal = digitalRead(digitalPin);
      return digitalVal;
      break;
  }
  return sensorValue;
 }

 boolean foundFlame(){
  int flame1 = flame(1);
  int flame2 = flame(2);
  int flame3 = flame(3);
  int flame4 = flame(4);
  int flame5 = flame(5);
  if(showFlameSensors == 1){
    Serial.print("[");
    Serial.print(flame1);
    Serial.print(", ");
    Serial.print(flame2);
    Serial.print(", ");
    Serial.print(flame3);
    Serial.print(", ");
    Serial.print(flame4);
    Serial.print(", ");
    Serial.print(flame5);
    Serial.print("] \n");
  }
  boolean found =  flame1 < distantFire1 || flame2 < distantFire2 || flame3 < distantFire3 || flame4 < distantFire4 || flame5 == 0;
  
  return found;
  
 }

 void extinguish(){
    fullBreak();
    activateSiren();
 }

 void activateSiren(){
  digitalWrite(sirenPin, HIGH); 
  digitalWrite(debugPin, HIGH);
  delay(2000);
  digitalWrite(sirenPin, LOW); 
  digitalWrite(debugPin, LOW);
 }

/************************************************
 ***************** MOTOR METHODS
 ************************************************/

int speed(int speed){
  return (int) 2.55*speed;
}

void turnLeftCont(int speed){
  runMotor('R', speed,1);
  runMotor('L', speed,1);
}
void turnLeft(int angle, int speed){
  runMotor('R', speed,1);
  runMotor('L', speed,1);
  delay(factor*angle);
  fullBreak();
}

void turnRightCont(int speed){
  runMotor('L', speed,-1);
  runMotor('R', speed,-1);
  
}

void turnRight(int angle, int speed){
  runMotor('L', speed,-1);
  runMotor('R', speed,-1);
  delay(factor*angle);
  fullBreak();
}

void fullBreak(){
  breakEnable('L');
  breakEnable('R');
}
void breakEnable(char channel){
  switch(channel){
    case 'L':
          digitalWrite(brakeL, HIGH);   //Disengage the Brake for Channel B
          break;
    case 'R':
          digitalWrite(brakeR, HIGH);   //Disengage the Brake for Channel B
          break;
  }
}

void forward(int speed){
  runMotor('L',speed,-1);
  runMotor('R', speed,1);
}

void backward(int speed){
  runMotor('L',speed,1);
  runMotor('R',speed,-1);
}

void runMotor(char channel,int speed, int direction){
  double motorFactor = 1.15;
  switch(channel){
    case 'L':
            if(direction==1){
                digitalWrite(motorL, HIGH); //Establishes forward direction of Channel B
                digitalWrite(brakeL, LOW);   //Disengage the Brake for Channel B
                analogWrite(speedL, speed);   //Spins the motor on Channel B at full speed
            }else{
                digitalWrite(motorL, LOW); //Establishes backward direction of Channel A
                digitalWrite(brakeL, LOW);   //Disengage the Brake for Channel A
                analogWrite(speedL, speed);   //Spins the motor on Channel A at half speed
            }
            break;
    case 'R':
            int spd = (int) (motorFactor*speed);
            if(direction==1){
               digitalWrite(motorR, HIGH); //Establishes forward direction of Channel B
               digitalWrite(brakeR, LOW);   //Disengage the Brake for Channel B
               analogWrite(speedR, spd);   //Spins the motor on Channel B at full speed 
            }else{
              digitalWrite(motorR, LOW);  //Establishes backward direction of Channel B
              digitalWrite(brakeR, LOW);   //Disengage the Brake for Channel B
              analogWrite(speedR, spd);    //Spins the motor on Channel B at half speed
            }
            break;
  }
}
