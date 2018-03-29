/*
 * Author: Eliezyer Fermino de Oliveira - 2017
 * Writing this code to automatize TTL recorded from the w-maze.
 * which is just the t-maze with the back from the home cage blocked
 * Later I plan to add the deliver of water automatic but for now
 * this should work.
 */

// include the library code:

#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
#define OFF 0x7
#define RED 0x1

//declaring variables
const int waterL = 26;
const int waterM = 28;
const int waterR = 30;
const int switchL = 5; //left arm sensor
const int switchM = 6; //middle arm sensor
const int switchR = 7; //right arm sensor
const int leftTTL = 8; // to signal to intan the left arm was acessed
const int midTTL = 9; // to signal to intan the middle arm was acessed
const int rightTTL = 10; // to signal to intan the right arm was acessed
const int reward = 11; //correct trials will send 1 s pulses output
int previous = 1; //variable to store previous arm visited, can assume values 0 (left), 1 (middle) and  2 (right)
int previousOB = 2; //variable to store previous outbound arm
int SLstate;
int SMstate;
int SRstate;
int R = 0; //Reward to show at the lcd screen
int N = 0; //Next arm 0 -> left | 1 -> middle | 2-> right
int timeArm;

//parameters to show in the lcd screen
int oBtrials = 0; // # of outbound trials 
int iBtrials = 0; // # of inbound trials
int oBerror = 0; // # of outbound errors
int iBerror = 0; // # of inbound errors
float overAll = 0; // variable to calculate overall performance


//For the LCD screen backlight


void setup() {
  pinMode(switchL, INPUT);
  pinMode(switchM, INPUT);
  pinMode(switchR, INPUT);
  pinMode(leftTTL, OUTPUT);
  pinMode(midTTL, OUTPUT);
  pinMode(rightTTL, OUTPUT);
  pinMode(reward, OUTPUT);
  pinMode(waterL, OUTPUT);
  pinMode(waterM, OUTPUT);
  pinMode(waterR, OUTPUT);

  lcd.begin(16,2); // starting LCD
  lcd.setBacklight(OFF);
  lcd.setCursor(0,0);
  lcd.print("Automatic W-Maze");
  lcd.setCursor(5,1);
  lcd.print("v 0.1");
  delay(1000);
  lcd.clear();
}

void loop() {
  int time = millis();
  lcd.setCursor(0,0);
  lcd.print("N:");
  if (N == 0) {
    lcd.print("L ");
  }
  else if (N == 1){
    lcd.print("M ");
  }
  else if (N == 2){
    lcd.print("R ");
  }
  
  lcd.print("R:");
  if (R == 0){
    lcd.print("N ");
  }
  else if(R == 1){
    lcd.print("Y ");
  }
  lcd.print("T:");
  overAll = (oBtrials+iBtrials);
  lcd.print(overAll);
  lcd.setCursor(0,1);
  lcd.print("t:");
  lcd.print((time - timeArm)/1000);
  if ((time - timeArm)/1000>=20){
    lcd.setBacklight(RED);
  }
  else{
      lcd.setBacklight(OFF);
  }
  lcd.setCursor(5,1);
  lcd.print("OB:");
  float oBperc = 100*(oBtrials-oBerror)/(oBtrials);
  lcd.print(oBperc);
  lcd.setCursor(11,1);
  lcd.print("IB:");
  float iBperc = 100*(iBtrials-iBerror)/(iBtrials);
  lcd.print(iBperc);
  task();
}


void task() {

    SLstate = digitalRead(switchL);
    SMstate = digitalRead(switchM);
    SRstate = digitalRead(switchR);

    //visiting the left arm
    if (SLstate == LOW && (previous == 1 && previousOB == 2)){
      R = 1;
      N = 1;
      oBtrials++;
      digitalWrite(leftTTL,HIGH);
      digitalWrite(reward,HIGH);
      digitalWrite(waterL,HIGH);
      delay(1000);
      digitalWrite(reward,LOW);
      digitalWrite(leftTTL,LOW);
      delay(500);
      digitalWrite(waterL,LOW);
      previousOB = 0;
      previous = 0;
      timeArm = millis();
    }
    else  if (SLstate == LOW && (previous == 1 && previousOB == 0)){
      R = 0;
      N = 1;
      oBtrials++;
      oBerror++;
      digitalWrite(leftTTL,HIGH);
      delay(1000);
      digitalWrite(leftTTL,LOW);
      previousOB = 0;
      previous = 0;
      timeArm = millis();
    }
    else if (SLstate == LOW && (previous == 2)){ //inbound error
      R = 0;
      N = 1;
      iBtrials++;
      iBerror++;
      digitalWrite(leftTTL,HIGH);
      delay(1000);
      digitalWrite(leftTTL,LOW);
      previousOB = 0;
      previous = 0;
      timeArm = millis();
    }
    
    //visiting the middle arm
    if (SMstate == LOW && (previous == 0 || previous == 2))
    {
      if (previous == 0){
        N = 2;
      }
      else if (previous == 2){
        N = 0;
      }
      R = 1;
      iBtrials++;
      digitalWrite(midTTL,HIGH);
      digitalWrite(reward,HIGH);
      digitalWrite(waterM,HIGH);
      delay(1000);
      digitalWrite(reward,LOW);
      digitalWrite(midTTL,LOW);
      delay(500);
      digitalWrite(waterM,LOW);
      timeArm = millis();
      
      previous = 1;
    }

    //visiting the right arm
    if (SRstate == LOW && (previous == 1 && previousOB == 0)) //correct trial
    {
      R = 1;
      N = 1;
      oBtrials++;
      digitalWrite(rightTTL,HIGH);
      digitalWrite(reward,HIGH);
      digitalWrite(waterR,HIGH);
      delay(1000);
      digitalWrite(reward,LOW);
      digitalWrite(rightTTL,LOW);
      delay(500);
      digitalWrite(waterR,LOW);
      timeArm = millis();
      previousOB = 2;
      previous = 2;      
    }
    else if (SRstate == LOW && (previous == 1 && previousOB == 2))    { //outbound error
      R = 0;
      N = 1;
      oBtrials++;
      oBerror++;
      digitalWrite(rightTTL,HIGH);
      delay(1000);
      digitalWrite(rightTTL,LOW);
      previousOB = 2;
      previous = 2;
      timeArm = millis();
    }
    else if (SRstate == LOW && (previous == 0)){ // inbound error
      R = 0;
      N = 1;
      iBtrials++;
      iBerror++;
      digitalWrite(rightTTL,HIGH);
      delay(1000);
      digitalWrite(rightTTL,LOW);
      previousOB = 2;
      previous = 2;
      timeArm = millis();
    }
}
