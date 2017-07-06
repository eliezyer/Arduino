//Declaring variables for the algorithm
const int SwitchL = 10; //left arm switch
const int SwitchM = 9; //middle arm switch
const int SwitchR = 8; //right arm switch
const int ledPin = 13; //led to have feedback since we dont have stepper motor yet
int SLState = 0;
int SMState = 0;
int SRState = 0;
int previousArm = 0; //0 is left and 1 is right arm, variable to save the previous arm visited.
int correctArm; //0 is left and 1 is right arm, variable to save the correct Arm animal has to go.

void setup()
{
  //setting up input or output pins
  pinMode(SwitchL, INPUT);
  pinMode(SwitchM, INPUT);
  pinMode(SwitchR, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  //change it to choose randomly an initial arm
  if (previousArm == 0)
  {
    correctArm = 1; //right arm
  }
  else if (previousArm == 1)
  {
    correctArm = 0; //left arm
  }

  SMState = digitalRead(SwitchM);
  SLState = digitalRead(SwitchL);
  SRState = digitalRead(SwitchR);
  if (SMState == HIGH) //this will be important to send the TTLs to the recording system, about the beggining of the trials
  {
  }

  if (SLState == LOW && correctArm == 0)
  {
      //opens the left door and delivery the water at the left
      Serial.println("Correct!");
      digitalWrite(ledPin,HIGH);
      delay(1000);
      digitalWrite(ledPin,LOW);
      previousArm = correctArm;
  }
  else
  {
    digitalWrite(ledPin,LOW);
  }
  
  if (SRState == LOW && correctArm == 1)
  {
    //open the right door and delivery the water at the right
      Serial.println("Correct!");
      digitalWrite(ledPin,HIGH);
      delay(1000);
      digitalWrite(ledPin,LOW);
      previousArm = correctArm;
  }


  //just to check at the serial monitor
  if (correctArm == 0)
  {
    Serial.println("Left Arm");
  }
  else if (correctArm == 1)
  {
    Serial.println("Right Arm");
  }
  

}
