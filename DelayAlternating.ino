//Declaring variables for the algorithm
const int SwitchL = 10; //left arm switch
const int SwitchM = 9; //middle arm switch
const int SwitchR = 8; //right arm switch
const int ledPin = 13; //led to have feedback since we dont have stepper motor yet
int Trial = 0;
int flagEnd = 1;
int flagStart = 0; // it has to start at zero so
int SLState = 0;
int SMState = 0;
int SRState = 0;
int previousArm = 0; //0 is left and 1 is right arm, variable to save the previous arm visited.
int correctArm; //0 is left and 1 is right arm, variable to save the correct Arm animal has to go.
int correctTrials = 0;
int wrongTrials = 0;

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
  if (SMState == LOW && flagEnd == 1) //this will be important to send the TTLs to the recording system, about the beggining of the trials
  {
      Trial++;
      flagEnd = 0;
      flagStart = 1;
  }

if (flagStart == 1)
{
  if (SLState == LOW && correctArm == 0)
  {
      //opens the left door and delivery the water at the left
      delay(1000);
      
      previousArm = correctArm;
      flagEnd = 1;
      flagStart = 0;
      correctTrials++;
  }
  else if (SLState == LOW && correctArm == 1)
  {
      delay(1000);
      flagEnd = 1;
      flagStart = 0;
      wrongTrials++;
  }
  
  if (SRState == LOW && correctArm == 1)
  {
    //open the right door and delivery the water at the right
      digitalWrite(ledPin,HIGH);
      delay(1000);
      digitalWrite(ledPin,LOW);
      
      previousArm = correctArm;
      flagEnd = 1;
      flagStart = 0;
      correctTrials++;
  }
  else if (SRState == LOW && correctArm == 0)
  {
      delay(1000);
      flagEnd = 1;
      flagStart = 0;
      wrongTrials++;
  }
}


  //just to check at the serial monitor
  if (correctArm == 0)
  {
    Serial.print("Left Arm  - ");
    Serial.print("Trial: ");
    Serial.print(Trial);
    Serial.print(" Correct: ");
    Serial.print(correctTrials);
    Serial.print(" Wrong: ");
    Serial.print(wrongTrials);
    Serial.print(" Score (%): ");
    Serial.println(correctTrials*100/Trial);
  }
  else if (correctArm == 1)
  {
    Serial.print("Right Arm  - ");
    Serial.print("Trial: ");
    Serial.print(Trial);
    Serial.print(" Correct: ");
    Serial.print(correctTrials);
    Serial.print(" Wrong: ");
    Serial.print(wrongTrials);
    Serial.print(" Score (%): ");
    Serial.println(correctTrials*100/Trial);
  }
  

}
