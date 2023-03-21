#include "MotionClassifier.h"
#include <Servo.h>

Servo serv;

Eloquent::ML::Port::SVM mclassifier;

// These constants won't change. They're used to give names to the pins used:
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
const int analogOutPin = 7;  // Analog output pin that the LED is attached to

int outputValue = 0;  // value output to the PWM (analog out)
int D7 = 7;

int prediction = 0;
int prevpred = 1;
int EXT = 1;
int FLX = 0;
int RST = 2;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(D7, OUTPUT);
  serv.attach(9);
}

void loop() {
  // windowedClassifier();
  testServo();
}

void driveServo(int pred){

  int SERV_PIN = 9;   // PWM pin
  int SERV_MIN = 2000;   // minimum servo angle FLEXION
  int SERV_MAX = -20; // maximum sevo angle EXTENSION

  unsigned long dt_ms = 30; // 16 Hz

  int actual;
  // float alpha = 0.25; // Must be between 0 and 1        

  // actual = ema(pred, actual, 0.25);

  if ((pred == EXT) && (prevpred == FLX)) {
    // actual = min(actual + 10, SERV_MAX);
    Serial.println("MOVE EXT");
    actual = SERV_MIN;
    serv.write(actual);
    delay(5000);
  }
  else if ((pred == FLX) && (prevpred == EXT)) {
    // actual = max(actual - 10, SERV_MIN);
    Serial.println("MOVE FLX");
    actual = SERV_MAX;
    serv.write(actual);
    delay(5000);
  }


  // actual = max(actual, SERV_MIN);
  // actual = min(actual, SERV_MAX);


  // Serial.print("Lo:"); Serial.print(SERV_MIN - 10); Serial.print(",");
  // Serial.print("Hi:"); Serial.print(SERV_MAX + 10); Serial.print(",");

  // Serial.print("Expected:"); Serial.print(expected); Serial.print(",");
  // Serial.print("Actual:"); Serial.println(actual);

  // delay(dt_ms);
  prevpred = pred;
}

void driveLinAct(){
  ;
}

void windowedClassifier() {

  // float sensorWindow[100];
  int sensorWindow;


  // READ 100 SAMPLE WINDOWS FROM EMG & AVERAGE
  // getWindowedAmplitude();
  
  // GET WINDOW TO CLASSIFY
  //consecutive
  // for(int i = 0; i < 100; i++) {
  //   // sensorWindow[i] = analogRead(analogInPin); // save signal, not AVG
  //   sensorWindow += analogRead(analogInPin); // running sum, AVG
  //   delay(30);
  // }

 // GET WINDOW TO CLASSIFY
  //consecutive
  float gap = 10;
  int backtrack = 10;
  int val = 0;
  int windAVG = 40;
  int wind = 100;
  int window;
  int prev_i = 0;

  int actual = 0;


  window = windAVG;

  while(1){
    val = analogRead(analogInPin);
    actual = ema(val, actual, 0.1);
    // Serial.println(val);
    if((gap + actual) < prev_i){
      for(int i = 0; i < window; i++) {
        // sensorWindow[i] = analogRead(analogInPin); // save signal, not AVG
        val = analogRead(analogInPin);
        actual = ema(val, actual, 0.1);
        // Serial.println(val);
        sensorWindow += actual; // running sum, AVG
        delay(30);
      }
      break;
    }
    else{
      prev_i = val;
      continue;
    }

  }

  // INPUT AMPL INTO CLASSIFER TO GET DIRECTION
  prediction = mclassifier.predict(sensorWindow/window); // get avg

  if(prediction == EXT) Serial.println(F("E"));
  if(prediction == FLX) Serial.println(F("F"));
  if(prediction == RST) Serial.println(F("R"));
  // if(prediction == SUSTAIN) Serial.println(F("SUSTAIN"));

// OUTPUT DIGITAL SIGNAL TO SERVO/ LINEAR ACTUATION

  driveServo(prediction);
}
void testServo() {

  // float sensorWindow[100];
  int sensorWindow;

 // GET WINDOW TO CLASSIFY
  //consecutive
  float gap = 10;
  int backtrack = 10;
  int val = 0;
  int windAVG = 40;
  int wind = 100;
  int window;
  int prev_i = 0;

  int actual = 0;


  window = windAVG;

  int predictionArray[6] = {FLX,EXT,FLX,EXT,FLX,EXT};
  for(int i=0;i<10;i++){
    Serial.println(predictionArray[i]);
    driveServo(predictionArray[i]);

  }
  // driveServo(prediction);
}

int ema(int exp, int actual, int alpha){
  return alpha * exp + (1 - alpha) * actual;
}
  // driveLinAct()