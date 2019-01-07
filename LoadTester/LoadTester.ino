/*
  Load Tester by Jackson Borneman
  Based off of code by Donald Cook
  Required Librarys:
  --SimpleTimer library by Marcello Romani avalable at https://github.com/schinken/SimpleTimer
*/
#include <SimpleTimer.h>
//Declare Switch pins
const int randomSwitchPin = 2;
const int alarmSwitchPin = 3;
const int tamperSwitchPin = 4;
const int troubleSwitchPin = 5;

#define meanActivateAlarmPin A1 //Pot pins for adjusting the mean duration, comment me to deactivate
#define meanActivateTamperPin A2 //Pot pins for adjusting the mean duration, comment me to deactivate
#define meanActivateTroublePin A3 //Pot pins for adjusting the mean duration, comment me to deactivate
const int mulitplyer = 25; //Multiplyer for input from pot
/*Declare Mean Time
  The Mean time is the middle time of the random number set that gets generated
*/
unsigned long meanActivateAlarm = 25000;
unsigned long meanActivateTamper = 25000;
unsigned long meanActivateTrouble = 25000;
unsigned long meanDeactivate = 1000;

SimpleTimer timer1; //The timers for driving relays each supports 8 running processes
SimpleTimer timer2;
SimpleTimer timer3;
SimpleTimer timer4;

bool constantTime = true; //Tied to switch. False for randomized times, True for static times.
bool triggerAlarm = false; //Tied to switch, specifies whether or not to tigger the alarm signals
bool triggerTamper = false; //Tied to switch, specifies whether or not to tigger the tamper signals
bool triggerTrouble = false; //Tied to switch, specifies whether or not to tigger the trouble signals

const int alarmPins[] = {22, 25, 28, 31, 34, 37, 40, 43, 46, 49}; //Pin definitions for the alarm signal. First is alarm 1, second alarm 2, etc..
const int tamperPins[] = {23, 26, 29, 32, 35, 38, 41, 44, 47, 50}; //Pin definitions for the tamper signal. First is alarm 1, second alarm 2, etc..
const int troublePins[] = {24, 27, 30, 33, 36, 39, 42, 45, 48, 51}; //Pin definitions for the trouble signal. First is alarm 1, second alarm 2, etc..

//Declare Activate function arrays
typedef void (*activateFunction)();//Create a object of functions
const activateFunction activateAlarm[10] = {&activateAlarm0, &activateAlarm1, &activateAlarm2, &activateAlarm3, &activateAlarm4, &activateAlarm5, &activateAlarm6, &activateAlarm7, &activateAlarm8, &activateAlarm9};
const activateFunction activateTamper[10] = {&activateTamper0, &activateTamper1, &activateTamper2, &activateTamper3, &activateTamper4, &activateTamper5, &activateTamper6, &activateTamper7, &activateTamper8, &activateTamper9};
const activateFunction activateTrouble[10] = {&activateTrouble0, &activateTrouble1, &activateTrouble2, &activateTrouble3, &activateTrouble4, &activateTrouble5, &activateTrouble6, &activateTrouble7, &activateTrouble8, &activateTrouble9};

const int numberOfSets = 10;//Number of alarm points
bool stateAlarm[10], stateTamper[10], stateTrouble[10]; //Declare Alarm, Tamper, and Trouble State tracker

void setup() {
  Serial.begin(9600);
  Serial.println("HELLO WORLD");
  //initalize Switch pins
  pinMode(alarmSwitchPin, INPUT_PULLUP);
  pinMode(tamperSwitchPin, INPUT_PULLUP);
  pinMode(troubleSwitchPin, INPUT_PULLUP);
  pinMode(randomSwitchPin, INPUT_PULLUP);
  randomSeed(analogRead(0)); //Seed random number generator
  srand(random(2000000000));
  
  for (int i = 0; i < numberOfSets; i++) { //initalize outputs and start processes
    pinMode(alarmPins[i], OUTPUT);
    stateAlarm[i]=1;
    activateAlarm[i]();
    pinMode(tamperPins[i], OUTPUT);
    stateTamper[i]=1;
    activateTamper[i]();
    pinMode(troublePins[i], OUTPUT);
    stateTrouble[i]=1;
    activateTrouble[i]();
    //Serial.print(i);
  }
  //Test if the pot pins are declared then set their mode
#ifdef meanActivateAlarmPin
  pinMode(meanActivateAlarmPin,INPUT);
#endif
#ifdef meanActivateTamperPin
  pinMode(meanActivateTamperPin,INPUT);
#endif
#ifdef meanActivateTroublePin
  pinMode(meanActivateTroublePin,INPUT);
#endif
  Serial.println("\nStartup Finished");
}
void loop() {
  //checks the status of the switches in order to switch on and off the diffrent modes
  triggerAlarm = digitalRead(alarmSwitchPin);
  triggerTamper = digitalRead(tamperSwitchPin);
  triggerTrouble = digitalRead(troubleSwitchPin);
  constantTime = !digitalRead(randomSwitchPin);

  //Test if the pot pins are decalred then update the Mean Time
#ifdef meanActivateAlarmPin
  meanActivateAlarm = mulitplyer * analogRead(meanActivateAlarmPin);
  Serial.print("meanActivateAlarm =");
  Serial.println(meanActivateAlarm);
#endif
#ifdef meanActivateTamperPin
  meanActivateTamper = mulitplyer * analogRead(meanActivateTamperPin);
  Serial.print("meanActivateTamper =");
  Serial.println(meanActivateTamper);
#endif
#ifdef meanActivateTroublePin
  meanActivateTrouble = mulitplyer * analogRead(meanActivateTroublePin);
  Serial.print("meanActivateTrouble =");
  Serial.println(meanActivateTrouble);
#endif

  //Runs all timers to see if the activation time has passed
  timer1.run();
  timer2.run();
  timer3.run();
  timer4.run();
}
unsigned long randomize(unsigned long mean) {
  if (constantTime == false) {
    double U = (double) ((double) rand() / (double) RAND_MAX);
    unsigned long randomVariable = -1 * log(U) * mean;
    //if(randomVariable < 11000){
    //Serial.print(randomVariable);
    //randomVariable = randomize(mean);
    //}
    return randomVariable;
  }
  else {
    return mean;
  }
}
void activateAlarm0() {
  if (stateAlarm[0]) { //Reset Alarm
    if (triggerAlarm) { //If the alarms are active
      digitalWrite(alarmPins[0], LOW);
    }
    timer1.setTimeout(randomize(meanDeactivate), activateAlarm0);
    stateAlarm[0] = false;
  }
  else { //Trigger alarm
    digitalWrite(alarmPins[0], HIGH);
    timer1.setTimeout(randomize(meanActivateAlarm), activateAlarm0);
    stateAlarm[0] = true;
  }
}
void activateAlarm1() {
  if (stateAlarm[1]) { //Reset Alarm
    if (triggerAlarm) { //If the alarms are active
      digitalWrite(alarmPins[1], LOW);
    }
    timer1.setTimeout(randomize(meanDeactivate), activateAlarm1);
    stateAlarm[1] = false;
  }
  else { //Trigger alarm
    digitalWrite(alarmPins[1], HIGH);
    timer1.setTimeout(randomize(meanActivateAlarm), activateAlarm1);
    stateAlarm[1] = true;
  }
}
void activateAlarm2() {
  if (stateAlarm[2]) { //Reset Alarm
    if (triggerAlarm) { //If the alarms are active
      digitalWrite(alarmPins[2], LOW);
    }
    timer1.setTimeout(randomize(meanDeactivate), activateAlarm2);
    stateAlarm[2] = false;
  }
  else { //Trigger alarm
    digitalWrite(alarmPins[2], HIGH);
    timer1.setTimeout(randomize(meanActivateAlarm), activateAlarm2);
    stateAlarm[2] = true;
  }
}
void activateAlarm3() {
  if (stateAlarm[3]) { //Reset Alarm
    if (triggerAlarm) { //If the alarms are active
      digitalWrite(alarmPins[3], LOW);
    }
    timer1.setTimeout(randomize(meanDeactivate), activateAlarm3);
    stateAlarm[3] = false;
  }
  else { //Trigger alarm
    digitalWrite(alarmPins[3], HIGH);
    timer1.setTimeout(randomize(meanActivateAlarm), activateAlarm3);
    stateAlarm[3] = true;
  }
}
void activateAlarm4() {
  if (stateAlarm[4]) { //Reset Alarm
    if (triggerAlarm) { //If the alarms are active
      digitalWrite(alarmPins[4], LOW);
    }
    timer1.setTimeout(randomize(meanDeactivate), activateAlarm4);
    stateAlarm[4] = false;
  }
  else { //Trigger alarm
    digitalWrite(alarmPins[4], HIGH);
    timer1.setTimeout(randomize(meanActivateAlarm), activateAlarm4);
    stateAlarm[4] = true;
  }
}
void activateAlarm5() {
  if (stateAlarm[5]) { //Reset Alarm
    if (triggerAlarm) { //If the alarms are active
      digitalWrite(alarmPins[5], LOW);
    }
    timer1.setTimeout(randomize(meanDeactivate), activateAlarm5);
    stateAlarm[5] = false;
  }
  else { //Trigger alarm
    digitalWrite(alarmPins[5], HIGH);
    timer1.setTimeout(randomize(meanActivateAlarm), activateAlarm5);
    stateAlarm[5] = true;
  }
}
void activateAlarm6() {
  if (stateAlarm[6]) { //Reset Alarm
    if (triggerAlarm) { //If the alarms are active
      digitalWrite(alarmPins[6], LOW);
    }
    timer1.setTimeout(randomize(meanDeactivate), activateAlarm6);
    stateAlarm[6] = false;
  }
  else { //Trigger alarm
    digitalWrite(alarmPins[6], HIGH);
    timer1.setTimeout(randomize(meanActivateAlarm), activateAlarm6);
    stateAlarm[6] = true;
  }
}
void activateAlarm7() {
  if (stateAlarm[7]) { //Reset Alarm
    if (triggerAlarm) { //If the alarms are active
      digitalWrite(alarmPins[7], LOW);
    }
    timer1.setTimeout(randomize(meanDeactivate), activateAlarm7);
    stateAlarm[7] = false;
  }
  else { //Trigger alarm
    digitalWrite(alarmPins[7], HIGH);
    timer1.setTimeout(randomize(meanActivateAlarm), activateAlarm7);
    stateAlarm[7] = true;
  }
}
void activateAlarm8() {
  if (stateAlarm[8]) { //Reset Alarm
    if (triggerAlarm) { //If the alarms are active
      digitalWrite(alarmPins[8], LOW);
    }
    timer2.setTimeout(randomize(meanDeactivate), activateAlarm8);
    stateAlarm[8] = false;
  }
  else { //Trigger alarm
    digitalWrite(alarmPins[8], HIGH);
    timer2.setTimeout(randomize(meanActivateAlarm), activateAlarm8);
    stateAlarm[8] = true;
  }
}
void activateAlarm9() {
  if (stateAlarm[9]) { //Reset Alarm
    if (triggerAlarm) { //If the alarms are active
      digitalWrite(alarmPins[9], LOW);
    }
    timer2.setTimeout(randomize(meanDeactivate), activateAlarm9);
    stateAlarm[9] = false;
  }
  else { //Trigger alarm
    digitalWrite(alarmPins[9], HIGH);
    timer2.setTimeout(randomize(meanActivateAlarm), activateAlarm9);
    stateAlarm[9] = true;
  }
}
void activateTamper0() {
  if (stateTamper[0]) { //Reset Tamper
    if (triggerTamper) { //If the alarms are active
      digitalWrite(tamperPins[0], LOW);
    }
    timer2.setTimeout(randomize(meanDeactivate), activateTamper0);
    stateTamper[0] = false;
  }
  else { //Trigger Tamper
    digitalWrite(tamperPins[0], HIGH);
    timer2.setTimeout(randomize(meanActivateTamper), activateTamper0);
    stateTamper[0] = true;
  }
}
void activateTamper1() {
  if (stateTamper[1]) { //Reset Tamper
    if (triggerTamper) { //If the alarms are active
      digitalWrite(tamperPins[1], LOW);
    }
    timer2.setTimeout(randomize(meanDeactivate), activateTamper1);
    stateTamper[1] = false;
  }
  else { //Trigger Tamper
    digitalWrite(tamperPins[1], HIGH);
    timer2.setTimeout(randomize(meanActivateTamper), activateTamper1);
    stateTamper[1] = true;
  }
}
void activateTamper2() {
  if (stateTamper[2]) { //Reset Tamper
    if (triggerTamper) { //If the alarms are active
      digitalWrite(tamperPins[2], LOW);
    }
    timer2.setTimeout(randomize(meanDeactivate), activateTamper2);
    stateTamper[2] = false;
  }
  else { //Trigger Tamper
    digitalWrite(tamperPins[2], HIGH);
    timer2.setTimeout(randomize(meanActivateTamper), activateTamper2);
    stateTamper[2] = true;
  }
}
void activateTamper3() {
  if (stateTamper[3]) { //Reset Tamper
    if (triggerTamper) { //If the alarms are active
      digitalWrite(tamperPins[3], LOW);
    }
    timer2.setTimeout(randomize(meanDeactivate), activateTamper3);
    stateTamper[3] = false;
  }
  else { //Trigger Tamper
    digitalWrite(tamperPins[3], HIGH);
    timer2.setTimeout(randomize(meanActivateTamper), activateTamper3);
    stateTamper[3] = true;
  }
}
void activateTamper4() {
  if (stateTamper[4]) { //Reset Tamper
    if (triggerTamper) { //If the alarms are active
      digitalWrite(tamperPins[4], LOW);
    }
    timer2.setTimeout(randomize(meanDeactivate), activateTamper4);
    stateTamper[4] = false;
  }
  else { //Trigger Tamper
    digitalWrite(tamperPins[4], HIGH);
    timer2.setTimeout(randomize(meanActivateTamper), activateTamper4);
    stateTamper[4] = true;
  }
}
void activateTamper5() {
  if (stateTamper[5]) { //Reset Tamper
    if (triggerTamper) { //If the alarms are active
      digitalWrite(tamperPins[5], LOW);
    }
    timer2.setTimeout(randomize(meanDeactivate), activateTamper5);
    stateTamper[5] = false;
  }
  else { //Trigger Tamper
    digitalWrite(tamperPins[5], HIGH);
    timer2.setTimeout(randomize(meanActivateTamper), activateTamper5);
    stateTamper[5] = true;
  }
}
void activateTamper6() {
  if (stateTamper[6]) { //Reset Tamper
    if (triggerTamper) { //If the alarms are active
      digitalWrite(tamperPins[6], LOW);
    }
    timer3.setTimeout(randomize(meanDeactivate), activateTamper6);
    stateTamper[6] = false;
  }
  else { //Trigger Tamper
    digitalWrite(tamperPins[6], HIGH);
    timer3.setTimeout(randomize(meanActivateTamper), activateTamper6);
    stateTamper[6] = true;
  }
}
void activateTamper7() {
  if (stateTamper[7]) { //Reset Tamper
    if (triggerTamper) { //If the alarms are active
      digitalWrite(tamperPins[7], LOW);
    }
    timer3.setTimeout(randomize(meanDeactivate), activateTamper7);
    stateTamper[7] = false;
  }
  else { //Trigger Tamper
    digitalWrite(tamperPins[7], HIGH);
    timer3.setTimeout(randomize(meanActivateTamper), activateTamper7);
    stateTamper[7] = true;
  }
}
void activateTamper8() {
  if (stateTamper[8]) { //Reset Tamper
    if (triggerTamper) { //If the alarms are active
      digitalWrite(tamperPins[8], LOW);
    }
    timer3.setTimeout(randomize(meanDeactivate), activateTamper8);
    stateTamper[8] = false;
  }
  else { //Trigger Tamper
    digitalWrite(tamperPins[8], HIGH);
    timer3.setTimeout(randomize(meanActivateTamper), activateTamper8);
    stateTamper[8] = true;
  }
}
void activateTamper9() {
  if (stateTamper[9]) { //Reset Tamper
    if (triggerTamper) { //If the alarms are active
      digitalWrite(tamperPins[9], LOW);
    }
    timer3.setTimeout(randomize(meanDeactivate), activateTamper9);
    stateTamper[9] = false;
  }
  else { //Trigger Tamper
    digitalWrite(tamperPins[9], HIGH);
    timer3.setTimeout(randomize(meanActivateTamper), activateTamper9);
    stateTamper[9] = true;
  }
}
void activateTrouble0() {
  if (stateTrouble[0]) { //Reset Trouble
    if (triggerTrouble) { //If the troubles are active
      digitalWrite(troublePins[0], LOW);
    }
    timer3.setTimeout(randomize(meanDeactivate), activateTrouble0);
    stateTrouble[0] = false;
  }
  else { //Trigger trouble
    digitalWrite(troublePins[0], HIGH);
    timer3.setTimeout(randomize(meanActivateTrouble), activateTrouble0);
    stateTrouble[0] = true;
  }
}
void activateTrouble1() {
  if (stateTrouble[1]) { //Reset Trouble
    if (triggerTrouble) { //If the troubles are active
      digitalWrite(troublePins[1], LOW);
    }
    timer3.setTimeout(randomize(meanDeactivate), activateTrouble1);
    stateTrouble[1] = false;
  }
  else { //Trigger trouble
    digitalWrite(troublePins[1], HIGH);
    timer3.setTimeout(randomize(meanActivateTrouble), activateTrouble1);
    stateTrouble[1] = true;
  }
}
void activateTrouble2() {
  if (stateTrouble[2]) { //Reset Trouble
    if (triggerTrouble) { //If the troubles are active
      digitalWrite(troublePins[2], LOW);
    }
    timer3.setTimeout(randomize(meanDeactivate), activateTrouble2);
    stateTrouble[2] = false;
  }
  else { //Trigger trouble
    digitalWrite(troublePins[2], HIGH);
    timer3.setTimeout(randomize(meanActivateTrouble), activateTrouble2);
    stateTrouble[2] = true;
  }
}
void activateTrouble3() {
  if (stateTrouble[3]) { //Reset Trouble
    if (triggerTrouble) { //If the troubles are active
      digitalWrite(troublePins[3], LOW);
    }
    timer3.setTimeout(randomize(meanDeactivate), activateTrouble3);
    stateTrouble[3] = false;
  }
  else { //Trigger trouble
    digitalWrite(troublePins[3], HIGH);
    timer3.setTimeout(randomize(meanActivateTrouble), activateTrouble3);
    stateTrouble[3] = true;
  }
}
void activateTrouble4() {
  if (stateTrouble[4]) { //Reset Trouble
    if (triggerTrouble) { //If the troubles are active
      digitalWrite(troublePins[4], LOW);
    }
    timer4.setTimeout(randomize(meanDeactivate), activateTrouble4);
    stateTrouble[4] = false;
  }
  else { //Trigger trouble
    digitalWrite(troublePins[4], HIGH);
    timer4.setTimeout(randomize(meanActivateTrouble), activateTrouble4);
    stateTrouble[4] = true;
  }
}
void activateTrouble5() {
  if (stateTrouble[5]) { //Reset Trouble
    if (triggerTrouble) { //If the troubles are active
      digitalWrite(troublePins[5], LOW);
    }
    timer4.setTimeout(randomize(meanDeactivate), activateTrouble5);
    stateTrouble[5] = false;
  }
  else { //Trigger trouble
    digitalWrite(troublePins[5], HIGH);
    timer4.setTimeout(randomize(meanActivateTrouble), activateTrouble5);
    stateTrouble[5] = true;
  }
}
void activateTrouble6() {
  if (stateTrouble[6]) { //Reset Trouble
    if (triggerTrouble) { //If the troubles are active
      digitalWrite(troublePins[6], LOW);
    }
    timer4.setTimeout(randomize(meanDeactivate), activateTrouble6);
    stateTrouble[6] = false;
  }
  else { //Trigger trouble
    digitalWrite(troublePins[6], HIGH);
    timer4.setTimeout(randomize(meanActivateTrouble), activateTrouble6);
    stateTrouble[6] = true;
  }
}
void activateTrouble7() {
  if (stateTrouble[7]) { //Reset Trouble
    if (triggerTrouble) { //If the troubles are active
      digitalWrite(troublePins[7], LOW);
    }
    timer4.setTimeout(randomize(meanDeactivate), activateTrouble7);
    stateTrouble[7] = false;
  }
  else { //Trigger trouble
    digitalWrite(troublePins[7], HIGH);
    timer4.setTimeout(randomize(meanActivateTrouble), activateTrouble7);
    stateTrouble[7] = true;
  }
}
void activateTrouble8() {
  if (stateTrouble[8]) { //Reset Trouble
    if (triggerTrouble) { //If the troubles are active
      digitalWrite(troublePins[8], LOW);
    }
    timer4.setTimeout(randomize(meanDeactivate), activateTrouble8);
    stateTrouble[8] = false;
  }
  else { //Trigger trouble
    digitalWrite(troublePins[8], HIGH);
    timer4.setTimeout(randomize(meanActivateTrouble), activateTrouble8);
    stateTrouble[8] = true;
  }
}
void activateTrouble9() {
  if (stateTrouble[9]) { //Reset Trouble
    if (triggerTrouble) { //If the troubles are active
      digitalWrite(troublePins[9], LOW);
    }
    timer4.setTimeout(randomize(meanDeactivate), activateTrouble9);
    stateTrouble[9] = false;
  }
  else { //Trigger trouble
    digitalWrite(troublePins[9], HIGH);
    timer4.setTimeout(randomize(meanActivateTrouble), activateTrouble9);
    stateTrouble[9] = true;
  }
}
