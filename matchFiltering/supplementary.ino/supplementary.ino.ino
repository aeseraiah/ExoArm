#include <stdio.h>
#include <stdlib.h>
#include <Servo.h>

/// SAME AS PYTHON
int PEAK_HEIGHT_FLX = 60;
int PEAK_HEIGHT_EXT = 20;
// int PEAK_HEIGHT_FLX = 335;
// int PEAK_HEIGHT_EXT = 295;
int MID = 50;
int WINDOW = 100;
int POOR_PREDICTION_THRESHOLD = 1000000;
int flx_aligned[100];
int ext_aligned[100];
int DC_COMPONENT = 55;
// int DC_COMPONENT = 60;
float alpha = 0.2;


/// THIS FILE ONLY
int count = 0;
const int analogInPin = A0;
const int D7 = 7;
Servo serv;
int EXT = 1;
int FLX = 0;
int RST = 3;
int prevpred = 1;
double *MAX_SIG;


// int FLEX_FILTER[100] = {23, 23, 23, 23, 23, 23, 24, 24, 24, 24, 24, 24, 25, 26, 26, 26, 25, 25, 25, 25, 25, 25, 25, 26, 26, 27, 26, 27, 27, 28, 28, 28, 28, 28, 29, 29, 30, 29, 29, 28, 28, 28, 29, 29, 28, 27, 26, 23, 21, 19, 18, 18, 19, 20, 21, 22, 23, 25, 26, 27, 29, 30, 30, 31, 31, 32, 32, 32, 33, 32, 32, 32, 32, 32, 32, 32, 32, 32, 33, 33, 33, 33, 33, 32, 32, 33, 33, 33, 33, 32, 33, 33, 33, 33, 34, 34, 34, 34, 34, 34};
// int EXT_FILTER[100] = {24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 25, 25, 25, 25, 25, 25, 25, 25, 25, 26, 26, 27, 27, 28, 29, 30, 31, 31, 30, 28, 27, 25, 24, 23, 23, 22, 22, 23, 24, 25, 26, 27, 27, 28, 28, 29, 28, 28, 28, 27, 27, 27, 27, 28, 28, 28, 28, 28, 28, 28, 28, 29, 29, 29, 30, 30, 31, 31, 31, 31, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 33, 33, 33, 33, 33, 33, 33, 33, 33};
// int RST_FILTER[100] = {23, 23, 24, 24, 24, 25, 25, 25, 25, 26, 26, 26, 26, 27, 27, 27, 27, 27, 27, 27, 28, 28, 29, 28, 29, 30, 30, 30, 30, 30, 31, 31, 31, 31, 31, 32, 32, 33, 33, 33, 33, 34, 34, 34, 34, 34, 34, 34, 35, 36, 37, 36, 36, 37, 37, 37, 37, 38, 38, 38, 38, 38, 38, 39, 39, 39, 40, 39, 40, 40, 41, 40, 41, 41, 41, 40, 41, 41, 41, 42, 43, 43, 43, 43, 43, 44, 44, 45, 45, 45, 45, 46, 46, 47, 47, 47, 47, 47, 48, 48};
int FLEX_FILTER[100];
int EXT_FILTER[100];
int RST_FILTER[100];


// DANNY EEPROM STUFF /////////////////////////////////////////////////////
#include <EEPROM.h>

// type used to capture each data point in a sample
// !maximum space in nano is believed to be 1024B!
// in that case, the hard max is 5B per data point
#define USE_TYPE short

// N - number of samples represented in window average
// LO - first sample in window average
// HI - last sample in window average
int BASE = 0;
int LENGTH = 100;

int ADDR_FN  = 0;
int ADDR_FLO = ADDR_FN + sizeof(USE_TYPE);
int ADDR_FHI = ADDR_FLO + (LENGTH - 1) * sizeof(USE_TYPE);

int ADDR_EN = ADDR_FHI + sizeof(USE_TYPE);
int ADDR_ELO = ADDR_EN + sizeof(USE_TYPE);
int ADDR_EHI = ADDR_ELO + (LENGTH - 1) * sizeof(USE_TYPE);

int LINEUP = 50;

int addr; 
USE_TYPE E_val;
USE_TYPE N;

int emg;

int dt_ms = 30;

bool start_fresh = false;
bool align = true;

// END DANNY'S EEPROM /////////////////////////////////////////////////////

int ema(int exp, int actual, float alpha){
  // smooths the signal based on alpha value
  // 0.1 is large smoothing, 0.5 is less smoothing, 1 is basically none
  // float value 0 to 1
  // Serial.println(( (alpha * (float)(exp)) + ((1 - alpha) * (float)(actual))) );
  // Serial.println((int)( (alpha * (float)(exp)) + ((1 - alpha) * (float)(actual))) );
  return (int) ( (alpha * (float)(exp)) + ((1 - alpha) * (float)(actual)) );
}

void convolve_avg(int* A, int* B, double *AVG_VAL){
  // convolve signals A and B and return the average of the convolution

  double large_conv = 0;
  double large_avg = 0;
  *AVG_VAL = 0; // zero out avg
  for(int i = 0; i < WINDOW-1; i++)
  {
    large_conv = 0;
    for(int j = 0; j < WINDOW-1; j++){ // THIS MAY NOT WORK, HAD TO DO IT TO USE INT INSTEAD OF
      large_conv += ((double)(A[i-j]) * (double)(B[j])); // compute current convolution
    }
    // Serial.print("large_conv: ");
    // Serial.print(large_conv);
    // Serial.print("\n");

    // CHECK IF CURRENT CONV IS THE MAX VAL, MAYBE ALSO MAKE ONE FOR AVG
    *AVG_VAL += large_conv;
    *AVG_VAL /= 2;

  }

  Serial.println(*AVG_VAL);
  // cout<<"AVG VALUE: "<<(*AVG_VAL)<<endl;
}

void convolve_max(int* A, int* B, double *MAX_VAL){
  // convolve signals A and B and return the maximum of the convolution
  // aligned signal is A, filter is B

  double large_conv = 0;
  double prev_conv = 0;
  double large_max = 0;
  *MAX_VAL = 0; // zero out max
  for(int i = 0; i < WINDOW-1; i++)
  {
    large_conv = 0;
    for(int j = 0; j < WINDOW-1; j++){ // THIS MAY NOT WORK, HAD TO DO IT TO USE INT INSTEAD OF
      large_conv += ((double)(A[i-j]) * (double)(B[j])); // compute current convolution
    }
    // CHECK IF CURRENT CONV IS THE MAX VAL, MAYBE ALSO MAKE ONE FOR AVG
    if ((large_conv > prev_conv) && (large_conv > large_max)){
        large_max = large_conv;
    }
    prev_conv = large_conv;
  }
  *MAX_VAL = large_max;
  Serial.println(*MAX_VAL);
  // cout<<"MAX VALUE: "<<(*MAX_VAL)<<endl;
}

int find_peaks(int *data, int max_height){
  // iterate through data file, comparing each pt 
  // until max peak obtained larger than peak_height 

  int MAX_INDEX = 0;
  int PREV = data[0]; //handle both negative and pos peak starts
  for(int i = 0; i < WINDOW-1; i++){
    if ((data[i] >= PREV) && (data[i] >= max_height) && (data[i] >= data[MAX_INDEX])){
      MAX_INDEX = i;
    }
  }
  return MAX_INDEX;
}

void align_signal(int *data){
  // align the signal to the mid point based on the maximum peak,
  // if this works, remove ext_aligned and just use a single aligned signal variable

  int max_height;
  int peak_index, gap, endpoint;

  int neg_data[WINDOW];
  
  max_height = PEAK_HEIGHT_FLX;

  peak_index = find_peaks(data, max_height); // find maximum peak

  if (peak_index){
    if (peak_index > MID){
        gap = peak_index - MID;

        for(int i = 0; i < (WINDOW-1 - gap); i++){
          flx_aligned[i] = data[gap]; // save starting at gap up to window end
          ext_aligned[i] = data[gap]; // save starting at gap up to window end
          gap++;
        }
        for(int i = gap; i < WINDOW-1; i++){
          flx_aligned[i] = DC_COMPONENT; // make the rest of the window DC component flat
          ext_aligned[i] = DC_COMPONENT; // make the rest of the window DC component flat
        }

    }
    else if (peak_index <= MID){
        gap = MID - peak_index;

        for(int i = 0; i < gap; i++){
          flx_aligned[i] = DC_COMPONENT; // make the beginning of the window DC component flat
          ext_aligned[i] = DC_COMPONENT; // make the beginning of the window DC component flat
        }
        endpoint = (WINDOW-1-gap);
        for(int i = 0; i < endpoint; i++){
          flx_aligned[gap] = data[i]; // make from gap to window end the data 
          ext_aligned[gap] = data[i]; // make from gap to window end the data 
          gap++;
        }
    }
    else{
      for(int i = 0; i < WINDOW-1; i++){
        flx_aligned[i] = data[i]; // IF NO PEAKS ARE FOUND AT THE SPECIFIED HEIGHT, RETURN ORIGINAL WAVE
        ext_aligned[i] = data[i]; // IF NO PEAKS ARE FOUND AT THE SPECIFIED HEIGHT, RETURN ORIGINAL WAVE
      }
    }
  }

  // max_height = PEAK_HEIGHT_EXT;
  // for(int i = 0; i < WINDOW-1; i++){
  //   neg_data[i] = -(data[i]);
  // }

  // peak_index = find_peaks(neg_data, -max_height); // find minimum peak

  // if (peak_index){
  //   if (peak_index > MID){
  //       gap = peak_index - MID;

  //       for(int i = 0; i < (WINDOW-1 - gap); i++){
  //         ext_aligned[i] = data[gap]; // save starting at gap up to window end
  //         gap++;
  //       }
  //       for(int i = gap; i < WINDOW-1; i++){
  //         ext_aligned[i] = DC_COMPONENT; // make the rest of the window DC component flat
  //       }

  //   }
  //   else if (peak_index <= MID){
  //       gap = MID - peak_index;

  //       for(int i = 0; i < gap; i++){
  //         ext_aligned[i] = DC_COMPONENT; // save as dc component up to gap
  //       }
  //       endpoint = WINDOW-1-gap;
  //       for(int i = 0; i < endpoint; i++){
  //         ext_aligned[gap] = data[i]; // make rest of signal align with window
  //         gap++;
  //       }
  //   }
  //   else{
  //     for(int i = 0; i < WINDOW-1; i++){
  //       ext_aligned[i] = data[i]; // IF NO PEAKS ARE FOUND AT THE SPECIFIED HEIGHT, RETURN ORIGINAL WAVE
  //     }
  //   }
  // }

}

int match_filter_prediction(int *d){
  // take a WINDOW size int array
  // align the signal to match the filter alignment
  // convolve the signal with the flexion and extension filter
  // return a number corresponding to FLEXION or EXTENSION based on convolution

  double MAX_FLX;
  double MAX_EXT;
  double MAX_RST;

  double PRED;

  // Serial.println("ALIGNING");
  align_signal(d); // ALIGN AS YOU CONVOLVE

  // CONVOLVE AND RETURN MAX?

  // convolve_avg(flx_aligned, FLEX_FILTER, &MAX_FLX);
  convolve_max(flx_aligned, FLEX_FILTER, &MAX_FLX);

  // convolve_avg(ext_aligned, EXT_FILTER, &MAX_EXT);
  convolve_max(ext_aligned, EXT_FILTER, &MAX_EXT);

  // convolve_avg(flx_aligned, EXT_FILTER, &MAX_EXT);
  // convolve_max(ext_aligned, EXT_FILTER, &MAX_EXT);

  Serial.println("END CONVOLUTION");

  Serial.println(*MAX_SIG);
  // if (MAX_FLX < 300000 || MAX_EXT < 300000){
  //   Serial.println("Low Similarity, No prediction");
  //   return -1;
  // }
  PRED = (abs(MAX_FLX - MAX_EXT) > 4000 ? MAX_FLX:MAX_EXT);

  // PRED = (MAX_FLX>MAX_EXT ? MAX_FLX:MAX_EXT); // FLEX OR EXTEND HIGHER CORRELATION

  //flexion = 0, extension = 1, sustain = 2, rest = 3

  // RETURN THE PREDICTION BASED ON MAX VALUE
  if (PRED == MAX_FLX){
    return FLX;
  }
  else if (PRED == MAX_EXT){
    return EXT;
  }
  else if (PRED == MAX_RST){
    return RST;
  }

  return -1;
}

void setup(){
  // initialize Serial port and analog pins
  // fill the filters using data saved to EEPROM

  Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(D7, OUTPUT);
  serv.attach(9);
  Serial.println("SETUP DONE");
  Serial.println("CALIBRATING...");
  mem_fill_filter();
  Serial.println("FILTERS LOADED");

  Serial.println("FLEXION FILTER");
  for(int i = 0; i < WINDOW-1; i++){
    Serial.println(FLEX_FILTER[i]);
    delay(dt_ms);
  }
  Serial.println("EXTENSION FILTER");
  for(int i = 0; i < WINDOW-1; i++){
    Serial.println(EXT_FILTER[i]);
    delay(dt_ms);
  }
  
}

void loop(){
  // MAIN LOOP that samples analog pin waiting for a spike in the signal
  // indicating potential movement. 'gap' variable determines threshold for detection
  // loop then records WINDOW size signal, and sends it to a prediction fn
  // 
  // 

  int sensorWindow[WINDOW];
  int gap = 5;
  int val = 0;
  int prev_i;

  int prediction = 0;
  int i;

  int ema_actual; 

  while(1){
    // may have an issue with missing part of wave before drop, maybe save a buffer of 5 at all times?
    val = analogRead(analogInPin);
    ema_actual = DC_COMPONENT;
    ema_actual = ema(val, ema_actual, alpha); // do ema as read in

    // if the beginning of the loop
    if(count == 0){
      prev_i = ema_actual;
      // Serial.println("BEGINNING OF CNT");
    }

    // Serial.println(ema_actual);


    // if a movement is detected
    if(((gap + ema_actual) < prev_i) || ((-gap + ema_actual) > prev_i)){
      Serial.println("MOVEMENT DETECTED...");
      *MAX_SIG = 0;
      // read in a 100 smaple window
      for(i = 0; i < WINDOW; i++) {
        val = analogRead(analogInPin);
        ema_actual = DC_COMPONENT;
        ema_actual = ema(val, ema_actual, alpha);

        sensorWindow[i] = ema_actual; // save the window into sensorWindow

        if(*MAX_SIG < ema_actual){
          *MAX_SIG = ema_actual;
        }

        // Serial.println(ema_actual); // print the value

        delay(dt_ms); // delay to allow time for ADC
      }

      // send new window to the prediction model 
      Serial.println("PREDICTING...");
      prediction = match_filter_prediction(sensorWindow);

      // process the prediction and move the servo
      if (prediction == EXT){
        Serial.println("PREDICTION : EXTENSION");
        // driveServo(prediction);
      }
      else if (prediction == FLX){
        Serial.println("PREDICTION : FLEXION");
        // driveServo(prediction);
      }
      else{
        Serial.println("PREDICTION : REST");
      }
    }

    // otherwise continue to poll for movement
    else if (count%100==0){
      Serial.println("POLLING...");
      count = 0;
    }

    // Serial.println(prev_i);
    prev_i = ema_actual;
    count++;
    delay(dt_ms);
  }
}

void mem_fill_filter()
// get filters saved to EEPROM from memory 
// and save into global variables
{ 
  int buffer = DC_COMPONENT - 23;
  int i = 0;
  for(addr = ADDR_FN; addr <= ADDR_FHI; addr += sizeof(USE_TYPE))
  {
    EEPROM.get(addr, E_val);
    FLEX_FILTER[i] = E_val+buffer;
    i++;
  }

  i = 0;
  for(addr = ADDR_EN; addr <= ADDR_EHI; addr += sizeof(USE_TYPE))
  {
    EEPROM.get(addr, E_val);
    EXT_FILTER[i] = E_val+buffer;
    i++;
  }

}

void driveServo(int pred){

  int SERV_PIN = 9;   // PWM pin
  int SERV_MIN = 20;   // minimum servo angle FLEXION
  int SERV_MAX = 160; // maximum sevo angle EXTENSION

  // unsigned long dt_ms = 250; // 16 Hz
  unsigned long servo_dt_ms = 1500; // 16 Hz

  int actual;

  if (pred == EXT) {
    // actual = min(actual + 10, SERV_MAX);
    Serial.println("MOVE EXT");
    actual = SERV_MAX;
    serv.write(actual);
    delay(servo_dt_ms);

  }
  else if (pred == FLX) {
    // actual = max(actual - 10, SERV_MIN);
    Serial.println("MOVE FLX");
    actual = SERV_MIN;
    serv.write(actual);
    delay(servo_dt_ms);
    // Serial.println("MOVE EXT");
    // actual = SERV_MAX;
    // serv.write(actual);
    // delay(servo_dt_ms);
  }
  else if (pred == RST){
    Serial.println("DO NOT MOVE");
    delay(servo_dt_ms);
  }

  prevpred = pred;

}
