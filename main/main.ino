// These constants won't change. They're used to give names to the pins used:
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
const int analogOutPin = 7;  // Analog output pin that the LED is attached to

int sensorValue = 0;  // value read from the pot
int outputValue = 0;  // value output to the PWM (analog out)
int D7 = 7;

float sensorWindow[128]; // init sensor array of window size
int amplitudeWindow;

int prediction = 0;

int EXTENSION = 1;
int FLEXION = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(D7, OUTPUT);
}

void loop() {
  windowedClassifier();
}

void getWindowedAmplitude() {
  // reads EMG signal for 128 samples, 16 comm/s, .0625s and outputs average of the window amplitude
  for(int i = 0; i < 128; i++) {
    sensorValue = analogRead(analogInPin);
    sensorWindow[i] = sensorValue; // save sensor value into array for smoothing? or do smoothing before hand
    amplitudeWindow += sensorValue; // keep running sum of each amplitude for average
  }

  amplitudeWindow /= 128; // get average amplitude for the window
  
}

void getWindowData(){
    for(int i = 0; i < 128; i++) {
    sensorWindow[i] = analogRead(analogInPin);
  }
}

void driveServo(){
  ;
}

void driveLinAct(){
  ;
}

void classifySVM(){
  prediction = predict(sensorWindow);
}

void windowedClassifier() {

  // READ 128 SAMPLE WINDOWS FROM EMG & AVERAGE
  // getWindowedAmplitude();
  getWindowData(); // has window of data saved, pass to SVC CLASSIFIER

  // INPUT AMPL INTO CLASSIFER TO DRIVE ACTUATION
  classifySVM();
  // Serial.println(sensorWindow)
  if(prediction == EXTENSION) Serial.println(EXTENSION);
  if(prediction == FLEXION) Serial.println(FLEXION);
  else Serial.println("FAIL");
  // Serial.println(prediction);
  // OUTPUT DIGITAL SIGNAL TO SERVO/ LINEAR ACTUATION
  driveServo();

  driveLinAct();
  
  int activation_threshold = 75;

  // if(outputValue>activation_threshold){
  //   digitalWrite(D7,1);
  //   delay(1); // pulse 1s
  //   digitalWrite(D7,0);
  // }
  // else{ 
  //   digitalWrite(D7,1);
  //   delay(2); // pulse 2s
  //   digitalWrite(D7,0);
  // }

  // SERIAL OUTPUT FOR DEBUGGING
  // Serial.print("sensor = ");
  // Serial.print(sensorValue);
  // Serial.print("\t output = ");
  // Serial.println(outputValue);

  // wait 100 milliseconds before the next loop for the analog-to-digital
  // converter to settle after the last reading:
  delay(100);
}


// PASTE PREDICTION MODEL BELOW
#pragma once
#include <stdarg.h>

int predict(float *x) {
  float kernels[12] = { 0 };
  float decisions[1] = { 0 };
  int votes[2] = { 0 };
  kernels[0] = compute_kernel(x,   4.0  , 5.0  , 6.0  , 7.0  , 8.0  , 8.0  , 9.0  , 9.0  , 10.0  , 11.0  , 11.0  , 12.0  , 12.0  , 13.0  , 13.0  , 13.0  , 13.0  , 13.0  , 13.0  , 14.0  , 13.0  , 14.0  , 14.0  , 14.0  , 14.0  , 14.0  , 14.0  , 14.0  , 14.0  , 14.0  , 14.0  , 14.0  , 14.0  , 13.0  , 13.0  , 14.0  , 13.0  , 13.0  , 13.0  , 13.0  , 13.0  , 13.0  , 12.0  , 13.0  , 12.0  , 12.0  , 12.0  , 11.0  , 12.0  , 11.0  , 11.0  , 11.0  , 11.0  , 11.0  , 11.0  , 10.0  , 11.0  , 10.0  , 10.0  , 10.0  , 10.0  , 10.0  , 10.0  , 9.0  , 10.0  , 10.0  , 9.0  , 9.0  , 9.0  , 10.0  , 9.0  , 10.0  , 9.0  , 9.0  , 9.0  , 9.0  , 9.0  , 8.0  , 9.0  , 8.0  , 8.0  , 8.0  , 8.0  , 9.0  , 8.0  , 9.0  , 9.0  , 9.0  , 9.0  , 9.0  , 9.0  , 8.0  , 8.0  , 8.0  , 8.0  , 8.0  , 9.0  , 8.0  , 8.0  , 8.0 );
  kernels[1] = compute_kernel(x,   2.0  , 3.0  , 3.0  , 5.0  , 6.0  , 7.0  , 8.0  , 8.0  , 9.0  , 9.0  , 10.0  , 10.0  , 11.0  , 11.0  , 10.0  , 11.0  , 11.0  , 11.0  , 12.0  , 11.0  , 12.0  , 12.0  , 12.0  , 13.0  , 12.0  , 13.0  , 13.0  , 13.0  , 13.0  , 13.0  , 13.0  , 13.0  , 13.0  , 12.0  , 13.0  , 12.0  , 12.0  , 13.0  , 12.0  , 12.0  , 12.0  , 12.0  , 12.0  , 11.0  , 11.0  , 11.0  , 11.0  , 12.0  , 12.0  , 11.0  , 12.0  , 11.0  , 12.0  , 11.0  , 11.0  , 11.0  , 10.0  , 10.0  , 10.0  , 10.0  , 10.0  , 11.0  , 10.0  , 10.0  , 10.0  , 10.0  , 10.0  , 10.0  , 11.0  , 10.0  , 10.0  , 10.0  , 9.0  , 9.0  , 9.0  , 9.0  , 9.0  , 9.0  , 10.0  , 9.0  , 9.0  , 9.0  , 9.0  , 9.0  , 9.0  , 9.0  , 9.0  , 9.0  , 9.0  , 8.0  , 9.0  , 8.0  , 9.0  , 8.0  , 9.0  , 8.0  , 8.0  , 8.0  , 8.0  , 8.0 );
  kernels[2] = compute_kernel(x,   3.0  , 4.0  , 5.0  , 7.0  , 8.0  , 10.0  , 10.0  , 10.0  , 11.0  , 11.0  , 11.0  , 12.0  , 11.0  , 12.0  , 12.0  , 12.0  , 12.0  , 11.0  , 12.0  , 12.0  , 12.0  , 12.0  , 12.0  , 11.0  , 12.0  , 12.0  , 12.0  , 12.0  , 12.0  , 12.0  , 12.0  , 12.0  , 12.0  , 11.0  , 12.0  , 11.0  , 11.0  , 11.0  , 10.0  , 11.0  , 11.0  , 10.0  , 11.0  , 11.0  , 10.0  , 11.0  , 11.0  , 11.0  , 11.0  , 10.0  , 10.0  , 10.0  , 10.0  , 10.0  , 9.0  , 9.0  , 10.0  , 9.0  , 10.0  , 9.0  , 10.0  , 9.0  , 9.0  , 10.0  , 10.0  , 10.0  , 10.0  , 10.0  , 10.0  , 9.0  , 10.0  , 9.0  , 9.0  , 9.0  , 9.0  , 9.0  , 9.0  , 9.0  , 9.0  , 9.0  , 9.0  , 9.0  , 9.0  , 9.0  , 9.0  , 9.0  , 8.0  , 9.0  , 8.0  , 8.0  , 8.0  , 9.0  , 9.0  , 9.0  , 9.0  , 9.0  , 9.0  , 8.0  , 9.0  , 9.0 );
  kernels[3] = compute_kernel(x,   4.0  , 5.0  , 6.0  , 8.0  , 9.0  , 10.0  , 11.0  , 12.0  , 13.0  , 13.0  , 14.0  , 15.0  , 15.0  , 15.0  , 15.0  , 14.0  , 15.0  , 14.0  , 14.0  , 14.0  , 13.0  , 13.0  , 13.0  , 13.0  , 13.0  , 12.0  , 13.0  , 13.0  , 12.0  , 13.0  , 12.0  , 12.0  , 12.0  , 12.0  , 12.0  , 12.0  , 11.0  , 12.0  , 12.0  , 11.0  , 12.0  , 12.0  , 12.0  , 12.0  , 11.0  , 11.0  , 11.0  , 10.0  , 11.0  , 11.0  , 10.0  , 10.0  , 9.0  , 10.0  , 9.0  , 10.0  , 10.0  , 9.0  , 10.0  , 9.0  , 10.0  , 10.0  , 9.0  , 10.0  , 10.0  , 10.0  , 10.0  , 9.0  , 10.0  , 10.0  , 10.0  , 10.0  , 9.0  , 9.0  , 9.0  , 9.0  , 9.0  , 9.0  , 9.0  , 9.0  , 10.0  , 9.0  , 9.0  , 9.0  , 9.0  , 10.0  , 9.0  , 9.0  , 9.0  , 8.0  , 9.0  , 8.0  , 9.0  , 9.0  , 9.0  , 8.0  , 9.0  , 9.0  , 9.0  , 9.0 );
  kernels[4] = compute_kernel(x,   7.0  , 7.0  , 7.0  , 6.0  , 7.0  , 6.0  , 7.0  , 7.0  , 7.0  , 8.0  , 9.0  , 9.0  , 9.0  , 10.0  , 11.0  , 12.0  , 13.0  , 14.0  , 15.0  , 15.0  , 16.0  , 16.0  , 16.0  , 17.0  , 16.0  , 16.0  , 16.0  , 15.0  , 15.0  , 14.0  , 14.0  , 15.0  , 14.0  , 14.0  , 14.0  , 13.0  , 14.0  , 14.0  , 13.0  , 14.0  , 13.0  , 14.0  , 13.0  , 13.0  , 13.0  , 12.0  , 12.0  , 12.0  , 11.0  , 11.0  , 12.0  , 11.0  , 12.0  , 11.0  , 11.0  , 11.0  , 11.0  , 11.0  , 11.0  , 10.0  , 11.0  , 11.0  , 10.0  , 11.0  , 10.0  , 11.0  , 11.0  , 10.0  , 11.0  , 10.0  , 10.0  , 10.0  , 10.0  , 10.0  , 10.0  , 9.0  , 10.0  , 10.0  , 10.0  , 10.0  , 10.0  , 10.0  , 10.0  , 10.0  , 10.0  , 10.0  , 10.0  , 10.0  , 9.0  , 10.0  , 9.0  , 9.0  , 9.0  , 9.0  , 9.0  , 9.0  , 9.0  , 8.0  , 9.0  , 8.0 );
  kernels[5] = compute_kernel(x,   5.0  , 5.0  , 6.0  , 5.0  , 6.0  , 5.0  , 5.0  , 5.0  , 5.0  , 6.0  , 6.0  , 6.0  , 6.0  , 6.0  , 6.0  , 6.0  , 7.0  , 9.0  , 13.0  , 14.0  , 14.0  , 13.0  , 12.0  , 11.0  , 10.0  , 8.0  , 8.0  , 7.0  , 8.0  , 7.0  , 8.0  , 8.0  , 9.0  , 10.0  , 11.0  , 14.0  , 15.0  , 18.0  , 20.0  , 22.0  , 23.0  , 23.0  , 24.0  , 24.0  , 23.0  , 24.0  , 24.0  , 23.0  , 23.0  , 22.0  , 22.0  , 22.0  , 21.0  , 21.0  , 21.0  , 20.0  , 21.0  , 20.0  , 20.0  , 19.0  , 19.0  , 19.0  , 18.0  , 18.0  , 19.0  , 18.0  , 18.0  , 18.0  , 17.0  , 17.0  , 16.0  , 16.0  , 16.0  , 15.0  , 16.0  , 15.0  , 15.0  , 15.0  , 15.0  , 14.0  , 14.0  , 13.0  , 14.0  , 14.0  , 14.0  , 14.0  , 13.0  , 13.0  , 13.0  , 12.0  , 13.0  , 12.0  , 12.0  , 13.0  , 12.0  , 12.0  , 12.0  , 11.0  , 11.0  , 11.0 );
  kernels[6] = compute_kernel(x,   7.0  , 7.0  , 6.0  , 7.0  , 6.0  , 7.0  , 7.0  , 9.0  , 10.0  , 12.0  , 14.0  , 15.0  , 17.0  , 18.0  , 19.0  , 20.0  , 21.0  , 21.0  , 22.0  , 22.0  , 22.0  , 22.0  , 22.0  , 22.0  , 22.0  , 21.0  , 21.0  , 21.0  , 21.0  , 21.0  , 20.0  , 20.0  , 20.0  , 19.0  , 19.0  , 18.0  , 17.0  , 16.0  , 16.0  , 15.0  , 14.0  , 13.0  , 12.0  , 12.0  , 11.0  , 11.0  , 10.0  , 10.0  , 9.0  , 8.0  , 8.0  , 8.0  , 7.0  , 7.0  , 7.0  , 6.0  , 6.0  , 6.0  , 6.0  , 5.0  , 5.0  , 5.0  , 5.0  , 4.0  , 4.0  , 4.0  , 4.0  , 4.0  , 4.0  , 4.0  , 4.0  , 3.0  , 3.0  , 3.0  , 3.0  , 3.0  , 3.0  , 3.0  , 3.0  , 3.0  , 3.0  , 3.0  , 3.0  , 3.0  , 2.0  , 2.0  , 2.0  , 2.0  , 2.0  , 2.0  , 2.0  , 2.0  , 2.0  , 2.0  , 2.0  , 2.0  , 2.0  , 2.0  , 2.0  , 2.0 );
  kernels[7] = compute_kernel(x,   10.0  , 13.0  , 14.0  , 16.0  , 18.0  , 19.0  , 21.0  , 22.0  , 23.0  , 24.0  , 24.0  , 24.0  , 26.0  , 25.0  , 25.0  , 25.0  , 25.0  , 25.0  , 25.0  , 24.0  , 24.0  , 23.0  , 23.0  , 22.0  , 21.0  , 20.0  , 19.0  , 18.0  , 17.0  , 16.0  , 15.0  , 14.0  , 14.0  , 13.0  , 12.0  , 11.0  , 11.0  , 10.0  , 10.0  , 9.0  , 9.0  , 8.0  , 8.0  , 8.0  , 7.0  , 7.0  , 7.0  , 6.0  , 6.0  , 6.0  , 6.0  , 5.0  , 5.0  , 5.0  , 5.0  , 4.0  , 4.0  , 4.0  , 4.0  , 4.0  , 4.0  , 4.0  , 3.0  , 3.0  , 3.0  , 3.0  , 3.0  , 3.0  , 3.0  , 3.0  , 3.0  , 3.0  , 3.0  , 3.0  , 3.0  , 3.0  , 2.0  , 2.0  , 2.0  , 2.0  , 2.0  , 2.0  , 2.0  , 2.0  , 2.0  , 2.0  , 2.0  , 2.0  , 2.0  , 2.0  , 2.0  , 2.0  , 2.0  , 2.0  , 2.0  , 2.0  , 2.0  , 2.0  , 2.0  , 2.0 );
  kernels[8] = compute_kernel(x,   7.0  , 7.0  , 7.0  , 7.0  , 7.0  , 8.0  , 9.0  , 11.0  , 12.0  , 15.0  , 17.0  , 18.0  , 20.0  , 20.0  , 21.0  , 22.0  , 22.0  , 23.0  , 23.0  , 23.0  , 24.0  , 23.0  , 23.0  , 22.0  , 21.0  , 21.0  , 20.0  , 19.0  , 19.0  , 18.0  , 17.0  , 16.0  , 15.0  , 15.0  , 14.0  , 13.0  , 12.0  , 12.0  , 11.0  , 10.0  , 10.0  , 9.0  , 9.0  , 8.0  , 8.0  , 8.0  , 7.0  , 7.0  , 7.0  , 6.0  , 6.0  , 6.0  , 5.0  , 5.0  , 5.0  , 5.0  , 5.0  , 5.0  , 4.0  , 4.0  , 4.0  , 4.0  , 4.0  , 4.0  , 3.0  , 3.0  , 3.0  , 3.0  , 3.0  , 3.0  , 3.0  , 3.0  , 3.0  , 3.0  , 3.0  , 3.0  , 3.0  , 3.0  , 2.0  , 2.0  , 2.0  , 2.0  , 2.0  , 2.0  , 2.0  , 2.0  , 2.0  , 2.0  , 2.0  , 2.0  , 2.0  , 2.0  , 2.0  , 2.0  , 2.0  , 2.0  , 2.0  , 2.0  , 2.0  , 2.0 );
  kernels[9] = compute_kernel(x,   12.0  , 14.0  , 15.0  , 16.0  , 18.0  , 19.0  , 20.0  , 20.0  , 21.0  , 22.0  , 22.0  , 22.0  , 22.0  , 21.0  , 21.0  , 21.0  , 20.0  , 20.0  , 19.0  , 19.0  , 19.0  , 18.0  , 18.0  , 18.0  , 17.0  , 18.0  , 17.0  , 17.0  , 16.0  , 15.0  , 15.0  , 14.0  , 14.0  , 14.0  , 13.0  , 13.0  , 12.0  , 12.0  , 12.0  , 12.0  , 12.0  , 11.0  , 11.0  , 10.0  , 9.0  , 8.0  , 8.0  , 8.0  , 8.0  , 8.0  , 8.0  , 8.0  , 8.0  , 8.0  , 7.0  , 7.0  , 7.0  , 7.0  , 6.0  , 6.0  , 6.0  , 6.0  , 5.0  , 5.0  , 5.0  , 5.0  , 5.0  , 4.0  , 4.0  , 4.0  , 4.0  , 4.0  , 4.0  , 4.0  , 3.0  , 3.0  , 3.0  , 3.0  , 3.0  , 3.0  , 3.0  , 3.0  , 3.0  , 3.0  , 3.0  , 3.0  , 3.0  , 2.0  , 3.0  , 2.0  , 2.0  , 3.0  , 2.0  , 2.0  , 2.0  , 2.0  , 2.0  , 2.0  , 2.0  , 2.0 );
  kernels[10] = compute_kernel(x,   8.0  , 8.0  , 8.0  , 8.0  , 8.0  , 8.0  , 8.0  , 7.0  , 7.0  , 7.0  , 7.0  , 7.0  , 7.0  , 8.0  , 8.0  , 8.0  , 9.0  , 9.0  , 10.0  , 11.0  , 13.0  , 14.0  , 15.0  , 16.0  , 16.0  , 18.0  , 18.0  , 19.0  , 19.0  , 19.0  , 20.0  , 20.0  , 20.0  , 20.0  , 19.0  , 18.0  , 18.0  , 17.0  , 17.0  , 17.0  , 16.0  , 16.0  , 16.0  , 16.0  , 16.0  , 15.0  , 15.0  , 15.0  , 14.0  , 15.0  , 14.0  , 13.0  , 14.0  , 13.0  , 12.0  , 12.0  , 11.0  , 11.0  , 11.0  , 11.0  , 11.0  , 10.0  , 10.0  , 10.0  , 9.0  , 9.0  , 8.0  , 8.0  , 7.0  , 7.0  , 7.0  , 8.0  , 8.0  , 8.0  , 8.0  , 8.0  , 8.0  , 7.0  , 7.0  , 6.0  , 6.0  , 6.0  , 6.0  , 6.0  , 6.0  , 7.0  , 6.0  , 6.0  , 7.0  , 6.0  , 7.0  , 6.0  , 7.0  , 6.0  , 6.0  , 6.0  , 6.0  , 6.0  , 6.0  , 6.0 );
  kernels[11] = compute_kernel(x,   7.0  , 7.0  , 7.0  , 7.0  , 7.0  , 7.0  , 8.0  , 9.0  , 11.0  , 11.0  , 13.0  , 14.0  , 15.0  , 17.0  , 18.0  , 19.0  , 21.0  , 21.0  , 22.0  , 23.0  , 22.0  , 23.0  , 23.0  , 23.0  , 24.0  , 24.0  , 24.0  , 24.0  , 23.0  , 24.0  , 23.0  , 23.0  , 23.0  , 22.0  , 22.0  , 21.0  , 20.0  , 20.0  , 20.0  , 19.0  , 19.0  , 18.0  , 18.0  , 18.0  , 17.0  , 17.0  , 16.0  , 16.0  , 15.0  , 15.0  , 14.0  , 14.0  , 13.0  , 14.0  , 13.0  , 13.0  , 13.0  , 13.0  , 13.0  , 12.0  , 12.0  , 12.0  , 11.0  , 11.0  , 11.0  , 10.0  , 10.0  , 10.0  , 9.0  , 9.0  , 9.0  , 9.0  , 8.0  , 8.0  , 8.0  , 8.0  , 7.0  , 7.0  , 7.0  , 7.0  , 7.0  , 7.0  , 7.0  , 7.0  , 7.0  , 7.0  , 6.0  , 7.0  , 6.0  , 6.0  , 6.0  , 7.0  , 6.0  , 6.0  , 6.0  , 6.0  , 6.0  , 6.0  , 6.0  , 6.0 );
  float decision = 8.34465e-07;
  decision = decision - ( + kernels[0] * -1.0  + kernels[1] * -1.0  + kernels[2] * -1.0  + kernels[3] * -1.0  + kernels[4] * -1.0  + kernels[5] * -1.0 );
  decision = decision - ( + kernels[6] * 1.0  + kernels[7] * 1.0  + kernels[8] * 1.0  + kernels[9] * 1.0  + kernels[10] * 1.0  + kernels[11] * 1.0 );

  return decision > 0 ? 0 : 1;
}

/**
* Compute kernel between feature vector and support vector.
* Kernel type: rbf
*/
float compute_kernel(float *x, ...) {
  va_list w;
  va_start(w, 100);
  float kernel = 0.0;

  for (uint16_t i = 0; i < 100; i++) {
      kernel += pow(x[i] - va_arg(w, double), 2);
  }

  return exp(-1e-10 * kernel);
}


void simpleAinPWMout() {
  // read the analog in value:
  sensorValue = analogRead(analogInPin);
  // map it to the range of the analog out:
  outputValue = map(sensorValue, 0, 1023, 0, 255);

  int activation_threshold = 75;

  if(outputValue>activation_threshold){
    digitalWrite(D7,1);
    delay(1); // pulse 1s
    digitalWrite(D7,0);
  }
  else{ 
    digitalWrite(D7,1);
    delay(2); // pulse 2s
    digitalWrite(D7,0);
  }
  // change the analog out value:
  analogWrite(analogOutPin, outputValue);

  // print the results to the Serial Monitor:
  Serial.print("sensor = ");
  Serial.print(sensorValue);
  Serial.print("\t output = ");
  Serial.println(outputValue);

  // wait 2 milliseconds before the next loop for the analog-to-digital
  // converter to settle after the last reading:
  delay(2);
}