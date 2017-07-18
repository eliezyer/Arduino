// include the library code for LCD screen:
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
#include <Stepper.h>
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

//Declaring variables for the algorithm
const int SwitchL = 8; //left arm switch
const int SwitchM = 9; //middle arm switch
const int SwitchR = 10; //right arm switch
const int stepsPerRevolution = 50; //stepper motor revolution, 360 deg would be 200 steps

int Trial = 0; //Number of trials
float percentage = 0; //Percentage of correct trials
int flagEnd = 1;
int flagStart = 0; // it has to start at zero so
int flagMainMenu = 1; //show main menu at the LCD screen
int flagDelayMenu = 0; //show Delay menu and current Delay variable value
int flagTask = 0; //start or pause the task, the variables will continue if paused, to pause press left button
int flagMiddleMotor = 1; // flag the first trial middle motor opening 
int MenuPos = 0; // Menu arrow position
int SLState = 0; //Switch at left arm state
int SMState = 0; //Switch at middle arm state
int SRState = 0; //Switch at right arm state
int previousArm = 0; //0 is left and 1 is right arm, variable to save the previous arm visited.
int correctArm; //0 is left and 1 is right arm, variable to save the correct Arm animal has to go.
int correctTrials = 0;
int wrongTrials = 0;
int Delay = 0; //Delay for the home cage

//For the LCD screen backlight
#define OFF 0x0

//initialize stepper for each arm
Stepper MotorL(stepsPerRevolution,22,23,24,25); //Left arm stepper motor
Stepper MotorM(stepsPerRevolution,26,27,28,29); //Middle arm stepper motor
Stepper MotorR(stepsPerRevolution,30,31,32,33); //Right arm stepper motor

void setup()
{
  //setting the motor speed at 60 rpm
  MotorL.setSpeed(30);
  MotorM.setSpeed(30);
  MotorR.setSpeed(30);
  
  //setting up input or output pins
  pinMode(SwitchL, INPUT);
  pinMode(SwitchM, INPUT);
  pinMode(SwitchR, INPUT);
  Serial.begin(9600);
  
  lcd.begin(16,2); // starting LCD
  lcd.setBacklight(OFF);
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
      delay(250); //I'm inserting some delay after some statements due to bounce effect from mechanical keys
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

void Task(int Delay){
      //lcd.clear();
      //change it to choose randomly an initial arm
      if (previousArm == 0){
        correctArm = 1; //right arm
      }
      else if (previousArm == 1){
        correctArm = 0; //left arm
      }

      if(Trial == 0 && flagMiddleMotor == 1){
        MotorM.step(-stepsPerRevolution);
        flagMiddleMotor = 0;
      }
      SMState = digitalRead(SwitchM);
      SLState = digitalRead(SwitchL);
      SRState = digitalRead(SwitchR);
      if (SMState == LOW && flagEnd == 1){ //this will be important to send the TTLs to the recording system, about the beggining of the trials
          Trial++;
          flagEnd = 0;
          flagStart = 1;
          delay(1000);
          MotorM.step(stepsPerRevolution);
      }
    
      if (flagStart == 1){
         if (SLState == LOW && correctArm == 0){
           //opens the left door and reward
              /*
              INSERT CODE TO OPEN THE LEFT DOOR AND DELIVERY REWARD
              */
           MotorL.step(stepsPerRevolution);
           previousArm = correctArm;
           flagEnd = 1;
           flagStart = 0;
           correctTrials++;
           percentage = correctTrials*100/Trial;
           delay(1000);
           MotorL.step(-stepsPerRevolution);
           delay(500);
           MotorM.step(-stepsPerRevolution);
         }
         else if (SLState == LOW && correctArm == 1){
          //opens the left door and do not reward
               /*
              INSERT CODE TO OPEN THE LEFT DOOR ONLY
              */
            MotorL.step(stepsPerRevolution);
            flagEnd = 1;
            flagStart = 0;
            wrongTrials++;
            percentage = correctTrials*100/Trial;
            delay(1000);
            MotorL.step(-stepsPerRevolution);
            delay(500);
            MotorM.step(-stepsPerRevolution);
          }
      
          if (SRState == LOW && correctArm == 1){
            //open the right door and delivery the water at the right
              /*
              INSERT CODE TO OPEN THE RIGHT DOOR AND DELIVERY REWARD
              */
              MotorR.step(-stepsPerRevolution);
              previousArm = correctArm;
              flagEnd = 1;
              flagStart = 0;
              correctTrials++;
              percentage = correctTrials*100/Trial;
              delay(1000);
              MotorR.step(stepsPerRevolution);
              delay(500);
              MotorM.step(-stepsPerRevolution);
          }
          else if (SRState == LOW && correctArm == 0){
              //open the left and do not reward
              /*
              INSERT CODE TO OPEN THE RIGHT DOOR ONLY
              */
              MotorR.step(-stepsPerRevolution);
              flagEnd = 1;
              flagStart = 0;
              wrongTrials++;
              percentage = correctTrials*100/Trial;
              delay(1000);
              MotorR.step(stepsPerRevolution);
              delay(500);
              MotorM.step(-stepsPerRevolution);
          }
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
