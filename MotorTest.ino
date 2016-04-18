int factor = 5; //NEEDS TO BE TESTED (so that the car turns as many angles as given in the turnLeft, turnRight function

void setup() {
  
  //Setup Channel A
  pinMode(12, OUTPUT); //Initiates Motor Channel A pin
  pinMode(9, OUTPUT); //Initiates Brake Channel A pin

    //Setup Channel B
  pinMode(13, OUTPUT); //Initiates Motor Channel A pin
  pinMode(8, OUTPUT);  //Initiates Brake Channel A pin

  //LEFT ENGINE ON CHANNEL A, RIGHT ENGINE ON CHANNEL B


}

void loop() {

 forward(255);
 delay(2000);
 backward(255);
 delay(2000);
 fullBreak();
 delay(2000);
 turnLeft(200,100);
 turnRight(200,100);
 delay(2000);
  

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
          digitalWrite(9, HIGH);   //Disengage the Brake for Channel B
          break;
    case 'R':
          digitalWrite(8, HIGH);   //Disengage the Brake for Channel B
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
                digitalWrite(12, HIGH); //Establishes forward direction of Channel B
                digitalWrite(9, LOW);   //Disengage the Brake for Channel B
                analogWrite(3, speed);   //Spins the motor on Channel B at full speed
            }else{
                digitalWrite(12, LOW); //Establishes backward direction of Channel A
                digitalWrite(9, LOW);   //Disengage the Brake for Channel A
                analogWrite(3, speed);   //Spins the motor on Channel A at half speed
            }
            break;
    case 'R':
            if(direction==1){
               digitalWrite(13, HIGH); //Establishes forward direction of Channel B
               digitalWrite(8, LOW);   //Disengage the Brake for Channel B
               analogWrite(11, speed);   //Spins the motor on Channel B at full speed 
            }else{
              digitalWrite(13, LOW);  //Establishes backward direction of Channel B
              digitalWrite(8, LOW);   //Disengage the Brake for Channel B
              analogWrite(11, speed);    //Spins the motor on Channel B at half speed
            }
            break;
  }
}


