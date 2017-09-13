/*
 * Author: Eliezyer Fermino de Oliveira - 2017
 * Writing this code to automatize TTL recorded from the w-maze.
 * which is just the t-maze with the back from the home cage blocked
 * Later I plan to add the deliver of water automatic but for now
 * this should work.
 */

//declaring variables
const int switchL = 5; //left arm sensor
const int switchM = 6; //middle arm sensor
const int switchR = 7; //right arm sensor
const int leftTTL = 8; // to signal to intan the left arm was acessed
const int midTTL = 9; // to signal to intan the middle arm was acessed
const int rightTTL = 10; // to signal to intan the right arm was acessed
const int reward = 11; //correct trials will send 1 s pulses output
int previous = 0; //variable to store previous arm visited, can assume values 0 (left), 1 (middle) and  2 (right)
int previousOB = 0; //variable to store previous outbound arm
int SLstate;
int SMstate;
int SRstate;

//parameters to show in the lcd screen
int Trials = 0; // # total trials
int oBtrials = 0; // # outbound trials 
int iBtrials = 0;
void setup() {
  pinMode(switchL, INPUT);
  pinMode(switchM, INPUT);
  pinMode(switchR, INPUT);
  pinMode(leftTTL, OUTPUT);
  pinMode(midTTL, OUTPUT);
  pinMode(rightTTL, OUTPUT);
  pinMode(reward, OUTPUT);
  Serial.begin(9600);
}

void loop() {
 task()
}


void task() {

    SLstate = digitalRead(switchL);
    SMstate = digitalRead(switchM);
    SRstate = digitalRead(switchR);

    //visiting the left arm
    if (SLstate == LOW && (previous == 1 && previousOB == 2)){
      Serial.println("Correct left trial");
      digitalWrite(leftTTL,HIGH);
      digitalWrite(reward,HIGH);
      delay(1000);
      digitalWrite(reward,LOW);
      digitalWrite(leftTTL,LOW);
      previousOB = 0;
      previous = 0;
    }
    else  if (SLstate == LOW && (previous == 1 && previousOB == 0)){
      Serial.println("Wrong left trial outbound error");
      digitalWrite(leftTTL,HIGH);
      delay(1000);
      digitalWrite(leftTTL,LOW);
      previousOB = 0;
      previous = 0;
    }
    else if (SLstate == LOW && (previous == 2)){
      Serial.println("Wrong left trial inbound error");
      digitalWrite(leftTTL,HIGH);
      delay(1000);
      digitalWrite(leftTTL,LOW);
      previousOB = 0;
      previous = 0;
    }
    //visiting the middle arm
    if (SMstate == LOW && (previous == 0 || previous == 2))
    {
      Serial.println("Correct mid trial");
      digitalWrite(midTTL,HIGH);
      digitalWrite(reward,HIGH);
      delay(1000);
      digitalWrite(reward,LOW);
      digitalWrite(midTTL,LOW);
      
      previous = 1;
    }

    //visiting the right arm
    if (SRstate == LOW && (previous == 1 && previousOB == 0)) //correct trial
    {
      Serial.println("Correct right trial");
      digitalWrite(rightTTL,HIGH);
      digitalWrite(reward,HIGH);
      delay(1000);
      digitalWrite(reward,LOW);
      digitalWrite(rightTTL,LOW);
      previousOB = 2;
      previous = 2;      
    }
    else if (SRstate == LOW && (previous == 1 && previousOB == 2))    { //outbound error
      Serial.println("Wrong right trial outbound error");
      digitalWrite(rightTTL,HIGH);
      delay(1000);
      digitalWrite(rightTTL,LOW);
      previousOB = 2;
      previous = 2;
    }
    else if (SRstate == LOW && (previous == 0)){
      Serial.println("Wrong right trial inbound error");
      digitalWrite(rightTTL,HIGH);
      delay(1000);
      digitalWrite(rightTTL,LOW);
      previousOB = 2;
      previous = 2;
    }
}
}

