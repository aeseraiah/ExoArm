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
  windowedClassifier();
  // testServo();
}

float ema(int exp, float actual, float alpha){
  return alpha * exp + (1 - alpha) * actual;
}

void driveServo(int pred){

  int SERV_PIN = 9;   // PWM pin
  int SERV_MIN = 52;   // minimum servo angle FLEXION
  int SERV_MAX = 80; // maximum sevo angle EXTENSION

  unsigned long dt_ms = 250; // 16 Hz

  int actual;

  if (pred == EXT) {
    // actual = min(actual + 10, SERV_MAX);
    Serial.println("MOVE EXT");
    actual = SERV_MAX;
    serv.write(actual);
    delay(dt_ms);

  }
  else if (pred == FLX) {
    // actual = max(actual - 10, SERV_MIN);
    Serial.println("MOVE FLX");
    actual = SERV_MIN;
    serv.write(actual);
    delay(dt_ms);
    // Serial.println("MOVE EXT");
    // actual = SERV_MAX;
    // serv.write(actual);
    // delay(dt_ms);
  }
  else if (pred == RST){
    Serial.println("DO NOT MOVE");
    delay(dt_ms);
  }

  prevpred = pred;

}

void driveLinAct(){
  ;
}

void windowedClassifier() {

  // float sensorWindow[100];
  float sensorWindow;


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
  float gap = 50;
  int backtrack = 10;
  int val = 300;
  int windAVG = 50; // 40
  int wind = 100;
  int window;
  int prev_i = 0;

  float actual = 300;
  float ema_actual = 300;


  window = windAVG;

  while(1){
    val = analogRead(analogInPin);
    ema_actual = ema(val, actual, 0.5);
    actual = ema_actual;
    // ema_actual = val;
    // Serial.println(ema_actual);
    if(((gap + ema_actual) < prev_i) || ((-gap + ema_actual) > prev_i)){ // IF ACTION MIGHT BE HAPPENING
      for(int i = 0; i < window; i++) {
        // sensorWindow[i] = analogRead(analogInPin); // save signal, not AVG
        val = analogRead(analogInPin);
        ema_actual = ema(val, actual, 0.5);
        // ema_actual = val;
        // Serial.println("COLLECTING");
        // Serial.println(ema_actual);
        sensorWindow += ema_actual; // running sum, AVG
        actual = ema_actual;
        delay(30);
      }
        // INPUT AMPL INTO CLASSIFER TO GET DIRECTION
      float pred_arg = (sensorWindow / window);
      
      prediction = mclassifier.predict(&pred_arg); // get avg

      // if(prediction == EXT) Serial.println(F("E"));
      // if(prediction == FLX) Serial.println(F("F"));
      // if(prediction == RST) Serial.println(F("R"));
      // if(prediction == SUSTAIN) Serial.println(F("SUSTAIN"));

      // OUTPUT DIGITAL SIGNAL TO SERVO/ LINEAR ACTUATION

      Serial.println(pred_arg);
      if (prediction == FLX){
        if(pred_arg > 600.){
          driveServo(prediction);
        }
        else{
          continue;
        }
      }
      else if( prediction == EXT){
        driveServo(prediction);
      }
      sensorWindow = 0;
      continue;
    }
    else{
      prev_i = val;
      continue;
    }

  }


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

  int predictionArray[6] = {FLX,EXT,FLX,EXT};
  for(int i=0;i<10;i++){
    Serial.println(predictionArray[i]);
    driveServo(predictionArray[i]);

  }
  // driveServo(prediction);
}


  // driveLinAct()