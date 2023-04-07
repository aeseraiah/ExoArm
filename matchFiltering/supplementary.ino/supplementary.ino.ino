#include <stdio.h>
#include <stdlib.h>

int PEAK_HEIGHT_FLX = 335;
int PEAK_HEIGHT_EXT = 295;
int MID = 50;
int WINDOW = 100;
int POOR_PREDICTION_THRESHOLD = 1000000;

int flx_aligned[100];
int ext_aligned[100];
int CONVOLUTION[100];

int convolve(int* A, int* B){

  for(int i = 0; i < WINDOW; i++)
  {
    CONVOLUTION[i] = 0;
    for(int j = 0; j < WINDOW; j++){
      CONVOLUTION[i] += A[i-j] * B[j];
    }
  }
  return 1;
}

int find_peaks(int *data, int max_height){
  // iterate through data file, comparing each pt until peak obtained larger than peak_height
  int MAX_INDEX = 0;
  int PREV = 0;
  for(int i = 0; i < WINDOW; i++){
    if ((data[i] > PREV) && (data[i] > max_height)){
      MAX_INDEX = i;
    }
  }

  Serial.print(MAX_INDEX);

  return MAX_INDEX;
}

void align_signal(int *data){
  int max_height;
  int peak_index, gap;
  int flex_new_wave[100];
  int ext_new_wave[100];
  int DC_COMPONENT = 330;
  int neg_data[100];
  
  max_height = PEAK_HEIGHT_FLX;
  peak_index = find_peaks(data, max_height); // find maximum peak
  if (peak_index){
    if (peak_index > MID){
        gap = peak_index - MID;

        for(int i = 0; i < (WINDOW - gap); i++){
          flex_new_wave[i] = data[gap]; // save starting at gap up to window end
          gap++;
        }
        for(int i = gap; i < WINDOW; i++){
          flex_new_wave[i] = DC_COMPONENT; // make the rest of the window DC component flat
        }

    }
    else if (peak_index <= MID){
        gap = MID - peak_index;

        for(int i = 0; i < gap; i++){
          flex_new_wave[i] = DC_COMPONENT; // save starting at gap up to window end
        }
        for(int i = 0; i < (WINDOW-gap); i++){
          flex_new_wave[gap] = data[i]; // make the rest of the window DC component flat
          gap++;
        }
    }
    else{
      for(int i = 0; i < WINDOW; i++){
        flex_new_wave[i] = data[i]; // IF NO PEAKS ARE FOUND AT THE SPECIFIED HEIGHT, RETURN ORIGINAL WAVE
      }
    }
  }

  max_height = PEAK_HEIGHT_EXT;
  for(int i = 0; i < WINDOW; i++){
    neg_data[i] = -data[i];
  }
  peak_index = find_peaks(neg_data, -max_height); // find maximum peak
  
  if (peak_index){
    if (peak_index > MID){
        gap = peak_index - MID;

        for(int i = 0; i < (WINDOW - gap); i++){
          ext_new_wave[i] = data[gap]; // save starting at gap up to window end
          gap++;
        }
        for(int i = gap; i < WINDOW; i++){
          ext_new_wave[i] = DC_COMPONENT; // make the rest of the window DC component flat
        }

    }
    else if (peak_index <= MID){
        gap = MID - peak_index;

        for(int i = 0; i < gap; i++){
          ext_new_wave[i] = DC_COMPONENT; // save starting at gap up to window end
        }
        for(int i = 0; i < (WINDOW-gap); i++){
          ext_new_wave[gap] = data[i]; // make the rest of the window DC component flat
          gap++;
        }
    }
    else{
      for(int i = 0; i < WINDOW; i++){
        ext_new_wave[i] = data[i]; // IF NO PEAKS ARE FOUND AT THE SPECIFIED HEIGHT, RETURN ORIGINAL WAVE
      }
    }
  }

  return flex_new_wave, ext_new_wave;
}

int find_max(int *data){
  int MAX_VALUE = 0;
  int PREV = 0;
  for(int i = 0; i < WINDOW; i++){
    if ((data[i] > PREV)){
      MAX_VALUE = i;
    }
  }
  return MAX_VALUE;
}

int match_filter_prediction(int *d){
    // THIS IS CONVERTED TO ARDUINO CODE/CPP


  int FLEX_FILTER[100];
  int EXT_FILTER[100];
  int RST_FILTER[100];
  int out_flx_aligned[100];
  int out_ext_aligned[100];
  int out_rst_aligned[100];

  int MAX_FLX;
  int MAX_EXT;
  int MAX_RST;
  int FLX_OR_EXT[2];

  float PRED;

  align_signal(d); // ALIGN AS YOU CONVOLVE

  if(convolve(flx_aligned, FLEX_FILTER)){
    for(int i = 0; i < WINDOW; i++){
      out_flx_aligned[i] = CONVOLUTION[i]; // CONVOLVE FLX ALIGNED W/ FLX FILTER & EXT ALIGNED W/ EXT FILTER
    }
  }
  if(convolve(ext_aligned, EXT_FILTER)){
    for(int i = 0; i < WINDOW; i++){
      out_ext_aligned[i] = CONVOLUTION[i]; // CONVOLVE FLX ALIGNED W/ FLX FILTER & EXT ALIGNED W/ EXT FILTER
    }
  }
  if(convolve(ext_aligned, RST_FILTER)){
    for(int i = 0; i < WINDOW; i++){
      out_rst_aligned[i] = CONVOLUTION[i]; // CONVOLVE RST FILTER WITH EITHER ALIGNMENT
    }
  }

  MAX_FLX = find_max(out_flx_aligned);
  MAX_EXT = find_max(out_ext_aligned);
  MAX_RST = find_max(out_rst_aligned);

  FLX_OR_EXT[0] = MAX_FLX;
  FLX_OR_EXT[1] = MAX_EXT;

  PRED = find_max(FLX_OR_EXT);

  // AVG_DIFF = np.average(np.abs(np.diff([MAX_FLX, MAX_EXT, MAX_RST])))

  // DIFF_EXT_RST = np.diff([MAX_RST, MAX_EXT])
  // DIFF_FLX_RST = np.diff([MAX_RST, MAX_FLX])
  
  // if AVG_DIFF < POOR_PREDICTION_THRESHOLD{
  //     PRED = MAX_RST
  // }
}

void setup(){
  Serial.begin(9600);
}

void loop(){
  int data[100];
}
