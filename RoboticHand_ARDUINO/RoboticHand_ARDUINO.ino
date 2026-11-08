#include <Servo.h>

Servo Index_MidServo;
Servo Pinkie_ringServo;
Servo ThumbServo;

int Led1 = 3;
int Led2 = 4;
int winButton = 5;
int lossButton = 6;

const int fist = 180;
const int openD = 0;

bool scissors = false;
bool rock = false;
bool paper = false;

void setup(){
  Index_MidServo.attach(9);
  Pinkie_ringServo.attach(10);
  ThumbServo.attach(11);
  Serial.begin(9600);
  pinMode(Led1, OUTPUT);
  pinMode(Led2, OUTPUT);
  pinMode(winButton, INPUT_PULLUP);
  pinMode(lossButton, INPUT_PULLUP);

  Index_MidServo.write(fist);
  delay(15);
  Pinkie_ringServo.write(fist);
  delay(15);
  ThumbServo.write(fist);
  digitalWrite(Led1, HIGH);
  digitalWrite(Led2, LOW);
}
void loop(){
  delay(20);
  while(digitalRead(winButton) == HIGH){
    // wait until button is pressed
  }
  delay(15);
  digitalWrite(Led2, HIGH);

}