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
 int pumpPin = 23;

 // DEFINE SETTINGS
 int factor = 5; //NEEDS TO BE TESTED (so that the car turns as many angles as given in the turnLeft, turnRight function)
 int distantFire1 = 900;
 int distantFire2 = 900;
 int distantFire3 = 22;
 int distantFire4 = 900;
 int closeFire = 18;
 int foundFire = 15;

 // DEBUG SETTINGS
 int showFlameSensors = 0;
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

  // PUMP SETUP
  pinMode(pumpPin, OUTPUT);
  digitalWrite(pumpPin, LOW);


}

void loop() {
  
  int spd = speed(40);
  int dist = 20;
  while(checkDist('L') > dist && checkDist('F') > dist && checkDist('R') > dist){
   int found = foundFlame();
   if(found){
    extinguish(found);
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

 int foundFlame(){
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
  boolean found1 = flame1 < distantFire1;
  boolean found2 = flame2 < distantFire2;
  boolean found3 = flame3 < distantFire3;
  boolean found4 = flame4 < distantFire4;
  boolean found5 = flame5 == 0;
  if(found1){
    Serial.println("\n");
    Serial.println(flame1);
    Serial.println(distantFire1);
    Serial.println(flame1 < distantFire1);
    Serial.println(found1);
    Serial.println("1");
    Serial.println("ETTAN");
    Serial.println("\n");
    return 1;
  }
  if(found2){
  Serial.println(flame2);
    Serial.println("2");
    return 2;
  }
  if(found3){
  Serial.println(flame3);
    Serial.println("3");
    return 3;
  }
  if(found4){
  Serial.println(flame4);
    Serial.println("4");
    return 4;
  }
  if(found5){
  Serial.println(flame5);
    Serial.println("5");
    return 5;
  }
  return 0;
  
 }

 void extinguish(int found){
    Serial.println(found);
    fullBreak();
    activateSiren();
    delay(2000);
    locateFire(found);
    deactivateSiren();
    
 }

 void activateSiren(){
  digitalWrite(sirenPin, HIGH); 
  digitalWrite(debugPin, HIGH);
 }

 void deactivateSiren(){
  digitalWrite(sirenPin, LOW); 
  digitalWrite(debugPin, LOW);
 }

 boolean locateFire(int found){
 // Serial.print("locateFire: ");
 // Serial.println(found);
  if(found < 3){
    //Serial.print("found < 3");
    while(flame(3) > distantFire3){
    //  Serial.println("left");
      turnLeftSlow(speed(40), 0.5);
    }
  }else if(found > 3){
   // Serial.print("found > 3");
    while(flame(3) > distantFire3){
    //  Serial.println("right");
      turnRightSlow(speed(40), 0.5);
    }
  }
  fullBreak();
  delay(1000);
  int i = 0;
  while(flame(3) > closeFire && i < 500){
   // Serial.println("forward");
    forwardSlow(speed(40), 0.6);
    delay(10);
    i++;
  }
  deactivateSiren();
  activatePump();
  delay(4000);
  
  
 }

 void activatePump(){
  digitalWrite(pumpPin, HIGH);
  delay(500);
  digitalWrite(pumpPin, LOW);
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

void turnLeftSlow(int spd, float delFactor){
  int factor = 300;
  turnLeftCont(spd);
  delay(factor - (int)(factor*delFactor));
  fullBreak();
  delay((int) (factor*delFactor));
}

void turnRightSlow(int spd, float delFactor){
  int factor = 300;
  turnRightCont(spd);
  delay(factor - (int)(factor*delFactor));
  fullBreak();
  delay((int) (factor*delFactor));
}

void forwardSlow(int spd, float delFactor){
  int factor = 300;
  forward(spd);
  delay(factor - (int)(factor*delFactor));
  fullBreak();
  delay((int) (factor*delFactor));
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
