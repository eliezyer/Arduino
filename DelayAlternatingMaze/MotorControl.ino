#include <Stepper.h>

//Declaring variables for the algorithm
const int stepsPerRevolution = 50; //stepper motor revolution, 360 deg would be 200 steps
const int lD = 34; //signal to actuate in left door
const int mD = 35; //signal to actuate in middle door
const int rD = 36; //signal to actuate in right door
const int oD = 37; //signal to open the door
const int cD = 38; //signal to close the door
const int done = 39; //signal that the task was made
int intermStep = 5;
int intermSpeed = 15;
int fullSpeed = 60;
int lDoor = 0;
int mDoor = 0;
int rDoor = 0;
int oDoor = 0;
int cDoor = 0;
int slDoor = 0;
int smDoor = 0;
int srDoor = 0;
int soDoor = 0;
int scDoor = 0;
int shouldRead = 1;

//initialize stepper for each arm
Stepper MotorL(stepsPerRevolution,22,24,26,28); //Left arm stepper motor
Stepper MotorM(stepsPerRevolution,4,5,6,7); //Middle arm stepper motor
Stepper MotorR(stepsPerRevolution,8,9,10,12); //Right arm stepper motor

void setup(){
  //setting up input or output pins
  pinMode(lD,INPUT);
  pinMode(mD,INPUT);
  pinMode(rD,INPUT);
  pinMode(oD,INPUT);
  pinMode(cD,INPUT);
  pinMode(done,OUTPUT);
 
  Serial.begin(9600);
}

void loop(){
if(shouldRead){ // should read new entrance or execute a change? (1 = read new entrance, 0 = execute change)
   lDoor = digitalRead(lD);
   mDoor = digitalRead(mD);
   rDoor = digitalRead(rD);
   oDoor = digitalRead(oD);
   cDoor = digitalRead(cD);
   digitalWrite(done,LOW);
}
 //read specific digital ports and activate motors
 if(lDoor && oDoor ){ //open left door
        MotorControl(0,1);
        //read all the doors, had a changed? execute the change
        slDoor = digitalRead(lD);
        smDoor = digitalRead(mD);
        srDoor = digitalRead(rD);
        soDoor = digitalRead(oD);
        scDoor = digitalRead(cD);
        if (scDoor != cDoor){
          lDoor = slDoor;
          cDoor = scDoor;
          shouldRead = 0;
        
        }
        else {
          shouldRead = 1;
          lDoor = 0;
          mDoor = 0;
          oDoor = 0;
          rDoor = 0;
          cDoor = 0;
          digitalWrite(done,HIGH);
        
        }

  }
else if(lDoor && cDoor ){ //close
        MotorControl(0,0);
        //read all the doors, had a changed? execute the change
        slDoor = 0;
        smDoor = digitalRead(mD);
        srDoor = digitalRead(rD);
        soDoor = digitalRead(oD);
        scDoor = 0;
        if (smDoor != mDoor){
          mDoor = smDoor;
          oDoor = soDoor;
          lDoor = 0;
          shouldRead = 0;
        }
        else {
          shouldRead = 1;
          lDoor = 0;
          mDoor = 0;
          oDoor = 0;
          rDoor = 0;
          cDoor = 0;
          digitalWrite(done,HIGH);
        }
  }
 else if (mDoor && oDoor){ //open midlle door
        MotorControl(1,1);
        //read all the doors, had a changed? execute the change
        slDoor = digitalRead(lD);
        smDoor = digitalRead(mD);
        srDoor = digitalRead(rD);
        soDoor = digitalRead(oD);
        scDoor = digitalRead(cD);
        if (scDoor != cDoor){
          cDoor = scDoor;
          oDoor = 0;
          shouldRead = 0;
        }
        else {
          shouldRead = 1;
          lDoor = 0;
          mDoor = 0;
          oDoor = 0;
          rDoor = 0;
          cDoor = 0;        
          digitalWrite(done,HIGH);      
        
        }

   }
else if(mDoor && cDoor){ //close
        MotorControl(1,0);
        //read all the doors, had a changed? execute the change
        slDoor = digitalRead(lD);
        smDoor = 0;
        srDoor = digitalRead(rD);
        soDoor = digitalRead(oD);
        scDoor = 0;
        if ( slDoor != lDoor || srDoor != rDoor || soDoor != oDoor){
          lDoor = slDoor;
          mDoor = smDoor;
          rDoor = srDoor;
          oDoor = soDoor;
          shouldRead = 0;
        
        }
        else {
          shouldRead = 1;
          lDoor = 0;
          mDoor = 0;
          oDoor = 0;
          rDoor = 0;
          cDoor = 0;        
          digitalWrite(done,HIGH);                
        
        }
   }
else if (rDoor && oDoor ){ //open right door
        MotorControl(2,1);
             //read all the doors, had a changed? execute the change
        slDoor = digitalRead(lD);
        smDoor = digitalRead(mD);
        srDoor = digitalRead(rD);
        soDoor = digitalRead(oD);
        scDoor = digitalRead(cD);
        if (scDoor != cDoor){
          cDoor = scDoor;
          shouldRead = 0;
        
        }
        else {
          shouldRead = 1;
          lDoor = 0;
          mDoor = 0;
          oDoor = 0;
          rDoor = 0;
          cDoor = 0;        
          digitalWrite(done,HIGH);                
        
        }
   }
else if(rDoor && cDoor ){ //close
      MotorControl(2,0);
      //read expected doors, had a changed? execute the change
        slDoor = digitalRead(lD);
        smDoor = digitalRead(mD);
        srDoor = digitalRead(rD);
        soDoor = digitalRead(oD);
        scDoor = digitalRead(cD);
      if (smDoor != mDoor || soDoor != oDoor){
        mDoor = smDoor;
        oDoor = soDoor;
        shouldRead = 0;
      }
      else {
        shouldRead = 1;
          lDoor = 0;
          mDoor = 0;
          oDoor = 0;
          rDoor = 0;
          cDoor = 0;
          digitalWrite(done,HIGH);                
        
      }
   }
delay(250);
}



void MotorControl(int motor, int opcl){ //Function to control motors opening and close
  if (motor == 0){ // left motor
    if (opcl == 1){ // open door
        MotorL.setSpeed(intermSpeed);
        MotorL.step(intermStep);
        MotorL.setSpeed(fullSpeed);
        MotorL.step((stepsPerRevolution-(2*intermStep)));
        MotorL.setSpeed(intermSpeed);
        MotorL.step(intermStep);
    }
    else if(opcl == 0){ // close door
        MotorL.setSpeed(intermSpeed);
        MotorL.step(-intermStep);
        MotorL.setSpeed(fullSpeed);
        MotorL.step(-(stepsPerRevolution-(2*intermStep)));
        MotorL.setSpeed(intermSpeed);
        MotorL.step(-intermStep);
    }
  }
  else if (motor == 1){ //middle motor
    if (opcl == 1){ // open door
        MotorM.setSpeed(intermSpeed);
        MotorM.step(-intermStep);
        MotorM.setSpeed(fullSpeed);
        MotorM.step(-(stepsPerRevolution-(2*intermStep)));
        MotorM.setSpeed(intermSpeed);
        MotorM.step(-intermStep);
    }
    else if(opcl == 0){ // close door
        MotorM.setSpeed(intermSpeed);
        MotorM.step(intermStep);
        MotorM.setSpeed(fullSpeed);
        MotorM.step((stepsPerRevolution-(2*intermStep)));
        MotorM.setSpeed(intermSpeed);
        MotorM.step(intermStep);
    }
  }
  else if(motor == 2){ //right motor
    if (opcl == 1){ // open door
        MotorR.setSpeed(intermSpeed);
        MotorR.step(-intermStep);
        MotorR.setSpeed(fullSpeed);
        MotorR.step(-(stepsPerRevolution-(2*intermStep)));
        MotorR.setSpeed(intermSpeed);
        MotorR.step(-intermStep);
    }
    else if(opcl == 0){ // close door
        MotorR.setSpeed(intermSpeed);
        MotorR.step(intermStep);
        MotorR.setSpeed(fullSpeed);
        MotorR.step((stepsPerRevolution-(2*intermStep)));
        MotorR.setSpeed(intermSpeed);
        MotorR.step(intermStep);
    }
  }
}
