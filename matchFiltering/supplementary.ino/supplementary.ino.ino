#include <stdio.h>
#include <stdlib.h>

///
int PEAK_HEIGHT_FLX = 30;
int PEAK_HEIGHT_EXT = 20;
// int PEAK_HEIGHT_FLX = 335;
// int PEAK_HEIGHT_EXT = 295;
int MID = 50;
int WINDOW = 100;
int POOR_PREDICTION_THRESHOLD = 1000000;
int flx_aligned[100];
int ext_aligned[100];
int DC_COMPONENT = 24;
// int DC_COMPONENT = 60;
float alpha = 0.4;
/// 
int count = 0;
const int analogInPin = A0;
int delay_Hz = 30; // change back to 30 after debugging

int FLEX_FILTER[100] = {23, 23, 23, 23, 23, 23, 24, 24, 24, 24, 24, 24, 25, 26, 26, 26, 25, 25, 25, 25, 25, 25, 25, 26, 26, 27, 26, 27, 27, 28, 28, 28, 28, 28, 29, 29, 30, 29, 29, 28, 28, 28, 29, 29, 28, 27, 26, 23, 21, 19, 18, 18, 19, 20, 21, 22, 23, 25, 26, 27, 29, 30, 30, 31, 31, 32, 32, 32, 33, 32, 32, 32, 32, 32, 32, 32, 32, 32, 33, 33, 33, 33, 33, 32, 32, 33, 33, 33, 33, 32, 33, 33, 33, 33, 34, 34, 34, 34, 34, 34};
int EXT_FILTER[100] = {24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 25, 25, 25, 25, 25, 25, 25, 25, 25, 26, 26, 27, 27, 28, 29, 30, 31, 31, 30, 28, 27, 25, 24, 23, 23, 22, 22, 23, 24, 25, 26, 27, 27, 28, 28, 29, 28, 28, 28, 27, 27, 27, 27, 28, 28, 28, 28, 28, 28, 28, 28, 29, 29, 29, 30, 30, 31, 31, 31, 31, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 33, 33, 33, 33, 33, 33, 33, 33, 33};
int RST_FILTER[100] = {23, 23, 24, 24, 24, 25, 25, 25, 25, 26, 26, 26, 26, 27, 27, 27, 27, 27, 27, 27, 28, 28, 29, 28, 29, 30, 30, 30, 30, 30, 31, 31, 31, 31, 31, 32, 32, 33, 33, 33, 33, 34, 34, 34, 34, 34, 34, 34, 35, 36, 37, 36, 36, 37, 37, 37, 37, 38, 38, 38, 38, 38, 38, 39, 39, 39, 40, 39, 40, 40, 41, 40, 41, 41, 41, 40, 41, 41, 41, 42, 43, 43, 43, 43, 43, 44, 44, 45, 45, 45, 45, 46, 46, 47, 47, 47, 47, 47, 48, 48};
// int FLEX_FILTER[100] = {298, 298, 298, 298, 298, 298, 296, 294, 291, 287, 284, 279, 274, 267, 260, 252, 244, 235, 228, 220, 211, 201, 190, 178, 164, 152, 140, 127, 117, 110, 108, 113, 124, 140, 160, 179, 197, 216, 237, 260, 284, 309, 332, 355, 379, 404, 429, 453, 476, 495, 511, 526, 538, 549, 558, 566, 572, 576, 578, 578, 577, 574, 570, 566, 561, 555, 550, 544, 538, 531, 524, 517, 511, 504, 498, 491, 485, 478, 472, 466, 460, 454, 448, 443, 438, 432, 427, 422, 418, 414, 409, 405, 402, 397, 394, 391, 388, 384, 381, 378};
// int EXT_FILTER[100] = {433, 430, 427, 423, 419, 416, 413, 411, 412, 412, 412, 412, 414, 420, 427, 433, 434, 431, 424, 413, 395, 375, 352, 328, 306, 288, 277, 272, 273, 279, 288, 300, 313, 326, 340, 353, 363, 373, 380, 386, 391, 396, 400, 405, 409, 413, 416, 419, 423, 427, 430, 433, 436, 437, 438, 437, 435, 433, 430, 428, 425, 423, 420, 416, 412, 407, 402, 397, 392, 387, 381, 376, 371, 367, 363, 359, 355, 350, 347, 344, 342, 339, 337, 334, 332, 331, 330, 328, 327, 325, 324, 323, 322, 322, 321, 320, 319, 319, 319, 318};
// int RST_FILTER[100] = {322, 322, 321, 321, 320, 320, 319, 319, 318, 318, 318, 318, 317, 317, 317, 317, 317, 317, 317, 316, 317, 316, 316, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 318, 318, 318, 319, 319, 319, 319, 320, 320, 320, 321, 321, 321, 321, 322, 322, 322, 322, 322, 322, 322, 322, 322, 322, 323, 323, 323, 323, 324, 324, 324, 324, 325, 325, 325, 325, 325, 325, 326, 326, 326, 327, 327, 327, 327, 327, 327, 327, 327, 327, 328, 328, 328, 328, 328, 328, 328, 328, 328, 329, 329, 329, 329, 329, 330};


int ema(int exp, int actual, float alpha){
  return (int) (alpha * (float)exp + (1 - alpha) * (float)actual);
}

void convolve_avg(int* A, int* B, int *AVG_VAL){

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
    large_avg += large_conv;
    large_avg /= 2;

  }
  *AVG_VAL = (int)(round(large_avg));
  Serial.println(*AVG_VAL);
  // cout<<"AVG VALUE: "<<(*AVG_VAL)<<endl;
}

void convolve_max(int* A, int* B, double *MAX_VAL){

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
  // iterate through data file, comparing each pt until peak obtained larger than peak_height
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
          gap++;
        }
        for(int i = gap; i < WINDOW-1; i++){
          flx_aligned[i] = DC_COMPONENT; // make the rest of the window DC component flat
        }

    }
    else if (peak_index <= MID){
        gap = MID - peak_index;

        for(int i = 0; i < gap; i++){
          flx_aligned[i] = DC_COMPONENT; // make the beginning of the window DC component flat
        }
        endpoint = (WINDOW-1-gap);
        for(int i = 0; i < endpoint; i++){
          flx_aligned[gap] = data[i]; // make from gap to window end the data 
          gap++;
        }
    }
    else{
      for(int i = 0; i < WINDOW-1; i++){
        flx_aligned[i] = data[i]; // IF NO PEAKS ARE FOUND AT THE SPECIFIED HEIGHT, RETURN ORIGINAL WAVE
      }
    }
  }

  max_height = PEAK_HEIGHT_EXT;
  for(int i = 0; i < WINDOW-1; i++){
    neg_data[i] = -(data[i]);
  }

  peak_index = find_peaks(neg_data, -max_height); // find minimum peak

  if (peak_index){
    if (peak_index > MID){
        gap = peak_index - MID;

        for(int i = 0; i < (WINDOW-1 - gap); i++){
          ext_aligned[i] = data[gap]; // save starting at gap up to window end
          gap++;
        }
        for(int i = gap; i < WINDOW-1; i++){
          ext_aligned[i] = DC_COMPONENT; // make the rest of the window DC component flat
        }

    }
    else if (peak_index <= MID){
        gap = MID - peak_index;

        for(int i = 0; i < gap; i++){
          ext_aligned[i] = DC_COMPONENT; // save as dc component up to gap
        }
        endpoint = WINDOW-1-gap;
        for(int i = 0; i < endpoint; i++){
          ext_aligned[gap] = data[i]; // make rest of signal align with window
          gap++;
        }
    }
    else{
      for(int i = 0; i < WINDOW-1; i++){
        ext_aligned[i] = data[i]; // IF NO PEAKS ARE FOUND AT THE SPECIFIED HEIGHT, RETURN ORIGINAL WAVE
      }
    }
  }

}

int match_filter_prediction(int *d){

  double MAX_FLX;
  double MAX_EXT;
  double MAX_RST;

  double PRED;

  Serial.println("ALIGNING");
  align_signal(d); // ALIGN AS YOU CONVOLVE

  // CONVOLVE AND RETURN MAX?
  Serial.println("CONVOLVING FLEX");
  convolve_max(flx_aligned, FLEX_FILTER, &MAX_FLX);
  Serial.print("MAX FLEX: ");
  Serial.print(MAX_FLX);
  Serial.print("\n");

  Serial.println("CONVOLVING EXT");
  convolve_max(ext_aligned, EXT_FILTER, &MAX_EXT);
  Serial.print("MAX_EXT: ");
  Serial.print(MAX_EXT);
  Serial.print("\n");

  // Serial.println("CONVOLVING REST");
  // convolve_max(ext_aligned, RST_FILTER, MAX_RST);
  // Serial.print("MAX_RST: ");
  // Serial.print(MAX_RST);
  // Serial.print("\n");


  Serial.println("END CONVOLUTION");

  PRED = (MAX_FLX>MAX_EXT ? MAX_FLX:MAX_EXT); // FLEX OR EXTEND HIGHER CORRELATION

  //flexion = 0, extension = 1, sustain = 2, rest = 3

  // RETURN THE PREDICTION BASED ON MAX VALUE
  if (PRED == MAX_FLX){
    return 0;
  }
  else if (PRED == MAX_EXT){
    return 1;
  }
  else if (PRED == MAX_RST){
    return 3;
  }

  return -1;
}

void setup(){
  Serial.begin(9600);
  pinMode(A0, INPUT);
  Serial.println("SETUP DONE");
}

void loop(){
  int sensorWindow[WINDOW];
  int gap = 10;
  int val = DC_COMPONENT;
  int prev_i;

  int actual = 0; // CHANGING THIS AFFECTS THE DC OFFSET
  int ema_actual = 0;
  int prediction = 0;
  int i;

  // may have an issue with missing part of wave before drop, maybe save a buffer of 5 at all times?
  val = analogRead(analogInPin);
  ema_actual = ema(val, actual, alpha);

  if(count == 0){
    prev_i = ema_actual;
  }
  // Serial.println(ema_actual);
  if(((gap + ema_actual) < prev_i) | ((-gap + ema_actual) > prev_i)){
    Serial.println("MOVEMENT DETECTED...");
    for(i = 0; i < WINDOW; i++) {
      val = analogRead(analogInPin);
      ema_actual = ema(val, actual, alpha);
      sensorWindow[i] = ema_actual;
      // Serial.println(sensorWindow[i]);
      delay(delay_Hz);
    }
    Serial.println("PREDICTING...");
    prediction = match_filter_prediction(sensorWindow);
    if (prediction == 1){
      Serial.println("PREDICTION : EXTENSION");
    }
    else if (prediction == 0){
      Serial.println("PREDICTION : FLEXION");
    }
    else{
      Serial.println("PREDICTION : REST");
    }
  }
  else if (count%50==1){
    Serial.println("POLLING...");
  }
  // Serial.println(prev_i);
  prev_i = ema_actual;
  count++;
  delay(delay_Hz);

  
}
