// include the library code for LCD screen:
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

//Declaring variables for the algorithm
unsigned long time;
unsigned long HCtime;
const int TTL1 = 32;
const int TTL2 = 34;
const int TTL3 = 36;
const int TTL4 = 38;
const int lD = 22; //signal to actuate in left door
const int mD = 23; //signal to actuate in middle door
const int rD = 24; //signal to actuate in right door
const int oD = 25; //signal to open the door
const int cD = 26; //signal to close the door
const int done = 27; //read if the motor task was done
const int SwitchHC1 = 5; // home cage sensor
const int SwitchHC2 = 6; // home cage sensor
const int SwitchL = 7; //left arm switch
const int SwitchM = 8; //middle arm switch
const int SwitchR = 9; //right arm switch
const int WaterL = 2;
const int WaterR = 3;
const float SolenoidTime = 1000; //time of open solenoid (in ms)
int Done = 0; //if the door task was done or not state
int Trial = 0; //Number of trials
int Motor; // which motor will open
int State; //next state of the motor, close or open
float percentage = 0; //Percentage of correct trials
int flagEnd = 1;
int flagStart = 0; // it has to start at zero so
int flagMainMenu = 1; //show main menu at the LCD screen
int flagDelayMenu = 0; //show Delay menu and current Delay variable value
int flagTask = 0; //start or pause the task, the variables will continue if paused, to pause press left button
int flagMiddleMotor = 1; // flag the first trial middle motor opening 
int flagHC = 0; //flag if the animal is inside HC
int MenuPos = 0; // Menu arrow position
int SHC1State = 0; //Switch at the home cage stage
int SHC2State = 0; //Switch at the home cage stage
int SLState = 0; //Switch at left arm state
int SMState = 0; //Switch at middle arm state
int SRState = 0; //Switch at right arm state
int previousArm = 0; //0 is left and 1 is right arm, variable to save the previous arm visited.
int correctArm; //0 is left and 1 is right arm, variable to save the correct Arm animal has to go.
int correctTrials = 0;
int wrongTrials = 0;
float Delay = 0; //Delay for the home cage
int flagDelay = 0; //flag for the delay
//For the LCD screen backlight
#define OFF 0x0
#define WHITE 0x7
#define VIOLET 0x5

void setup()
{
  
  //setting up input or output pins
  pinMode(TTL1,OUTPUT);
  pinMode(TTL2,OUTPUT);
  pinMode(TTL3,OUTPUT);
  pinMode(TTL4,OUTPUT);
  pinMode(lD,OUTPUT);
  pinMode(mD,OUTPUT);
  pinMode(rD,OUTPUT);
  pinMode(oD,OUTPUT);
  pinMode(cD,OUTPUT);
  pinMode(done,INPUT);
  pinMode(SwitchL, INPUT);
  pinMode(SwitchM, INPUT);
  pinMode(SwitchR, INPUT);
  pinMode(SwitchHC1, INPUT);
  pinMode(SwitchHC2, INPUT);
  pinMode(WaterL, OUTPUT);
  pinMode(WaterR, OUTPUT);
  digitalWrite(WaterL, LOW);
  digitalWrite(WaterR, LOW);
  
  lcd.begin(16,2); // starting LCD
  lcd.setBacklight(VIOLET);
  lcd.setCursor(0,0);
  lcd.print("Automatic Maze");
  lcd.setCursor(5,1);
  lcd.print("v 0.1");
  delay(1000);
  lcd.clear();
}

//where everything runs
void loop()
{
  lcd.setCursor(0,0);
  uint8_t buttons = lcd.readButtons();
  
  if (flagMainMenu == 1) // Call the main menu function
  {
    MainMenu(MenuPos);
  }
  if (flagDelayMenu == 1) // Call the Delay menu function
  {
    DelayMenu(buttons);
    if ((buttons == BUTTON_SELECT) || (buttons == BUTTON_LEFT)){
      flagDelayMenu = 0;
      flagMainMenu = 1;
      MenuPos = 1;
      lcd.clear();
      buttons = 0; //I have to clear the buttons variable because it activates another if right after this one.
      delay(400); //I'm inserting some delay after some statements due to bounce effect from mechanical keys
    }
  }
  

  if ((buttons == BUTTON_DOWN) & flagMainMenu)
  {
    MenuPos = 1;
  }

  if ((buttons == BUTTON_UP) & flagMainMenu)
  {
    MenuPos = 0;
  }

  if ((buttons == BUTTON_SELECT) & flagMainMenu == 1 & (MenuPos == 1)){
    flagMainMenu = 0;
    flagDelayMenu = 1;
    lcd.clear();
  }
  
  if ( (buttons == BUTTON_SELECT) & (flagMainMenu == 1) & (MenuPos == 0)){
      flagTask = 1;
      flagMainMenu = 0;
      flagDelayMenu = 0;
  }
  if (flagTask == 1){
    Task(Delay);
  }
  if ((flagTask == 1) & (buttons == BUTTON_LEFT)){
    flagTask = 0;
    flagMainMenu = 1;
    MenuPos = 0;
    lcd.clear();
  }
  Done = digitalRead(done);
  if (Done){
    digitalWrite(lD,LOW);
    digitalWrite(mD,LOW);
    digitalWrite(rD,LOW);
    digitalWrite(oD,LOW);
    digitalWrite(cD,LOW);
  }
}  

//functions to keep code clean

void MainMenu(int MenuPos)//Function for the Main Menu
{
  if (MenuPos == 0)
  {
      lcd.print("Start <-");
      lcd.setCursor(0,1);
      lcd.print("Delay   ");
  }
  else if (MenuPos == 1)
  {
      lcd.print("Start   ");
      lcd.setCursor(0,1);
      lcd.print("Delay <-");
  }
}

void DelayMenu(uint8_t ChangeDelay) //Function for the Delay Menu
{
    lcd.print("Delay (in s)");
    lcd.setCursor(0,1);
    lcd.print(Delay);
    if(ChangeDelay == BUTTON_UP){
      lcd.print("          ");
      Delay++;
      delay(100);
    }
    if((ChangeDelay == BUTTON_DOWN) && (Delay > 0)){
      lcd.setCursor(0,1); //This is extremly important to come back the cursor and erase de digit, specially going down because can be remianing of decimals and the user can get confused
      lcd.print("          ");
      Delay--;
      delay(100);
    }
}

void MotorControl(int motor, int opcl){ //Function to control motors opening and close
  if (motor == 0){ // left motor
    digitalWrite(lD,HIGH);
  
    if (opcl == 1){ // open door
      digitalWrite(oD,HIGH);
    }
    else if(opcl == 0){ // close door
      digitalWrite(cD,HIGH);
    }
  }
  else if (motor == 1){ //middle motor
    digitalWrite(mD,HIGH);
    if (opcl == 1){ // open door
      digitalWrite(oD,HIGH);
    }
    else if(opcl == 0){ // close door
      digitalWrite(cD,HIGH);
    }
  }
  else if(motor == 2){ //right motor
    digitalWrite(rD,HIGH);
    if (opcl == 1){ // open door
      digitalWrite(oD,HIGH);
    }
    else if(opcl == 0){ // close door
      digitalWrite(cD,HIGH);
    }
  }
}

void Task(float Delay){ //task structure function
      //lcd.clear();
      //change it to choose randomly an initial arm
      if (previousArm == 0){
        correctArm = 1; //right arm
      }
      else if (previousArm == 1){
        correctArm = 0; //left arm
      }

      if(Trial == 0 && flagMiddleMotor == 1){ //opening the middle doOr at the first trial
        MotorControl(1,1);
        flagMiddleMotor = 0;
      }
      SMState = digitalRead(SwitchM);
      SLState = digitalRead(SwitchL);
      SRState = digitalRead(SwitchR);
      SHC1State = digitalRead(SwitchHC1);
      SHC2State = digitalRead(SwitchHC2);
      if (SMState == LOW && flagEnd == 1 && flagHC == 0){ //this will be important to send the TTLs to the recording system, about the beggining of the trials
          Trial++;
          flagEnd = 0;
          flagStart = 1;
          MotorControl(1,0);
          digitalWrite(TTL2,HIGH);
          delay(1000);// WE SHOULD PUT 2 SENSORS TO READ ANIMAL POSITION ALONG THE MAIN ARM
          digitalWrite(TTL2,LOW);
      }
          
      if (flagStart == 1){
         if (SLState == LOW && correctArm == 0){
           //opens the left door and reward
              /*
              INSERT CODE TO OPEN THE LEFT DOOR AND DELIVERY REWARD
              */

           previousArm = correctArm;
           flagEnd = 1;
           flagStart = 0;
           flagHC = 1;
           correctTrials++;
           percentage = correctTrials*100/Trial;
           Motor = 0;
           State = 1;
           MotorControl(Motor,State);
           digitalWrite(WaterL,HIGH);
           digitalWrite(TTL1,HIGH);
           digitalWrite(TTL4,HIGH);  
           delay(SolenoidTime);
           digitalWrite(TTL1,LOW);
           digitalWrite(TTL4,LOW);             
           digitalWrite(WaterL,LOW);
         }
         else if (SLState == LOW && correctArm == 1){
          //opens the left door and do not reward
               /*
              INSERT CODE TO OPEN THE LEFT DOOR ONLY
              */
            flagEnd = 1;
            flagStart = 0;
            flagHC = 1;
            Motor = 0;
            State = 1;
            wrongTrials++;
            percentage = correctTrials*100/Trial;
            MotorControl(Motor,State);
            digitalWrite(TTL1,HIGH);
            delay(1000);
            digitalWrite(TTL1,LOW);
          }
      
          if (SRState == LOW && correctArm == 1){
            //open the right door and delivery the water at the right
              /*
              INSERT CODE TO OPEN THE RIGHT DOOR AND DELIVERY REWARD
              */
              
              previousArm = correctArm;
              flagEnd = 1;
              flagStart = 0;
              flagHC = 1;
              Motor = 2;
              State = 1;
              correctTrials++;
              percentage = correctTrials*100/Trial;
              MotorControl(Motor,State);
              digitalWrite(TTL3,HIGH);
              digitalWrite(TTL4,HIGH);  
              digitalWrite(WaterR,HIGH);
              delay(SolenoidTime);
              digitalWrite(WaterR,LOW);
              digitalWrite(TTL3,LOW);
              digitalWrite(TTL4,LOW);  
          }
          else if (SRState == LOW && correctArm == 0){
              //open the left and do not reward
              /*
              INSERT CODE TO OPEN THE RIGHT DOOR ONLY
              */
              flagEnd = 1;
              flagStart = 0;
              flagHC = 1;
              Motor = 2;
              State = 1;
              wrongTrials++;
              percentage = correctTrials*100/Trial;
              MotorControl(Motor,State);
              digitalWrite(TTL3,HIGH);
              delay(1000);
              digitalWrite(TTL3,LOW);
          }
    }
    if (flagHC == 1 && !flagDelay){
      if(SHC1State == LOW || SHC2State == LOW){
        MotorControl(Motor,(State-1)*-1);
        HCtime = millis();
        delay(1500); //time delay of door closing or opening
        flagDelay = 1;
      }
    }
    
    if (flagDelay){
      DelayFunction(HCtime);
    }
      
    //Prints task info at the LCD screen
    lcd.setCursor(0,0);
    lcd.print("Trial: ");
    lcd.print(Trial);
    lcd.setCursor(0,1);
    lcd.print("% Correct: ");
    lcd.print(percentage);
    lcd.print("     ");
}

void DelayFunction(unsigned long HCtime){
  time = millis();
  if ((time - HCtime) >= Delay*1000){
  flagHC = 0;
  MotorControl(1,1);
  flagDelay = 0; 
  }
}
