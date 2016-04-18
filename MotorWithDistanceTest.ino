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

 // DEFINE SETTINGS
 int factor = 5; //NEEDS TO BE TESTED (so that the car turns as many angles as given in the turnLeft, turnRight function


void setup() {
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


}

void loop() {
  Serial.begin (9600);
  while(checkDist('L') > 10 && checkDist('F') > 10 && checkDist('R') > 10){
   // forward(255);
  }
  fullBreak();


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
  String printNew = print + ": " + distance;
  Serial.print(printNew);
  Serial.print("\n");
  delay(100);
  return distance;
}


/************************************************
 ***************** MOTOR METHODS
 ************************************************/

int speed(int speed){
  return (int) 2.55*speed;
}
void turnLeft(int angle, int speed){
  runMotor('R', speed,1);
  runMotor('L', speed,1);
  delay(factor*angle);
  fullBreak();
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
            if(direction==1){
               digitalWrite(motorR, HIGH); //Establishes forward direction of Channel B
               digitalWrite(brakeR, LOW);   //Disengage the Brake for Channel B
               analogWrite(speedR, speed);   //Spins the motor on Channel B at full speed 
            }else{
              digitalWrite(motorR, LOW);  //Establishes backward direction of Channel B
              digitalWrite(brakeR, LOW);   //Disengage the Brake for Channel B
              analogWrite(speedR, speed);    //Spins the motor on Channel B at half speed
            }
            break;
  }
}
