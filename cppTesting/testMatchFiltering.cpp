#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <stdint.h>
#include <cmath>
using namespace std;

int PEAK_HEIGHT_FLX = 30;
int PEAK_HEIGHT_EXT = 20;
int MID = 50;
int WINDOW = 100;
int flx_aligned[100];
int ext_aligned[100];
int DC_COMPONENT = 24;
float alpha = 0.4;

int FLEX_FILTER[100] = {23, 23, 23, 23, 23, 23, 24, 24, 24, 24, 24, 24, 25, 26, 26, 26, 25, 25, 25, 25, 25, 25, 25, 26, 26, 27, 26, 27, 27, 28, 28, 28, 28, 28, 29, 29, 30, 29, 29, 28, 28, 28, 29, 29, 28, 27, 26, 23, 21, 19, 18, 18, 19, 20, 21, 22, 23, 25, 26, 27, 29, 30, 30, 31, 31, 32, 32, 32, 33, 32, 32, 32, 32, 32, 32, 32, 32, 32, 33, 33, 33, 33, 33, 32, 32, 33, 33, 33, 33, 32, 33, 33, 33, 33, 34, 34, 34, 34, 34, 34};
int EXT_FILTER[100] = {24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 25, 25, 25, 25, 25, 25, 25, 25, 25, 26, 26, 27, 27, 28, 29, 30, 31, 31, 30, 28, 27, 25, 24, 23, 23, 22, 22, 23, 24, 25, 26, 27, 27, 28, 28, 29, 28, 28, 28, 27, 27, 27, 27, 28, 28, 28, 28, 28, 28, 28, 28, 29, 29, 29, 30, 30, 31, 31, 31, 31, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 33, 33, 33, 33, 33, 33, 33, 33, 33};
int RST_FILTER[100] = {23, 23, 24, 24, 24, 25, 25, 25, 25, 26, 26, 26, 26, 27, 27, 27, 27, 27, 27, 27, 28, 28, 29, 28, 29, 30, 30, 30, 30, 30, 31, 31, 31, 31, 31, 32, 32, 33, 33, 33, 33, 34, 34, 34, 34, 34, 34, 34, 35, 36, 37, 36, 36, 37, 37, 37, 37, 38, 38, 38, 38, 38, 38, 39, 39, 39, 40, 39, 40, 40, 41, 40, 41, 41, 41, 40, 41, 41, 41, 42, 43, 43, 43, 43, 43, 44, 44, 45, 45, 45, 45, 46, 46, 47, 47, 47, 47, 47, 48, 48};


int ema(int exp, int actual, float alpha){

  return (int) (alpha * (float)exp + (1 - alpha) * (float)actual);
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
    // THIS IS CONVERTED TO ARDUINO CODE/CPP

  double MAX_FLX;
  double MAX_EXT;
  double MAX_RST;

  int PRED;

  align_signal(d); // ALIGN AS YOU CONVOLVE

  convolve_max(flx_aligned, FLEX_FILTER, &MAX_FLX);
  cout<<"MAX FLX: "<<MAX_FLX<<endl;

  convolve_max(ext_aligned, EXT_FILTER, &MAX_EXT);
  cout<<"MAX EXT: "<<MAX_EXT<<endl;

  // convolve_max(ext_aligned, RST_FILTER, &MAX_RST);
  // cout<<"MAX RST: "<<MAX_RST<<endl;


  PRED = (MAX_FLX>MAX_EXT ? MAX_FLX:MAX_EXT); // FLEX OR EXTEND HIGHER CORRELATION


//flexion = 0, extension = 1, sustain = 2, rest = 3
  if (PRED == MAX_FLX){
    cout<<"PREDICTION: FLEX"<<"\n";
    return 0;
  }
  else if (PRED == MAX_EXT){
    cout<<"PREDICTION: EXT"<<"\n";
    return 1;
  }
  else if (PRED == MAX_RST){
    cout<<"PREDICTION: REST"<<"\n";
    return 3;
  }

  return -1;
}

int main() {

  int sensorWindow[WINDOW];

  int gap = 10;
  int val = 0;
  int prev_i = 0;

  int actual = 0; // CHANGES DC OFFSET DUE TO EMA
  int ema_actual = 0;
  int prediction = 0;
  int count = 0;

    // READ IN SIGNAL FROM FILE
    using namespace std;


    // string fname = "../data/CoolTerm Capture 2023-02-07 10-29-08.txt";
    string fname = "../data/lp_hp_Data/CoolTerm Capture 2023-04-19 14-57-55.txt";

    vector<vector<string> > content;
    vector<string> row;
    string line, word;

    fstream file (fname, ios::in);
    if(file.is_open())
    {
        while(getline(file, line))
        {
            row.clear();

            stringstream str(line);

            while(getline(str, word, '\t'))
                row.push_back(word);
            content.push_back(row);
        }
    }
    else
        cout<<"Could not open the file\n";

// //PRINT EVERYTHING
    // for(int i=0;i<content.size();i++)
    // {
    //     for(int j=0;j<content[i].size();j++)
    //     {
    //         cout<<content[i][j]<<":  ";
    //         cout<<j<<"  ";
    //     }
    //     cout<<"\r\n";
    // }

// j is element (1 has data)
// i is row

int i = 0;
int short_window = 0;

while (i<content.size()){
    // cout<<"CURRENT i VALUE: "<<i<<"\n";
    int val =  stof(content[i][1]); // holds current emg
    ema_actual = ema(val, actual, alpha);
    actual = ema_actual;

    // cout<<"PREV VAL"<<prev_i<<endl;
    if(((gap + ema_actual) < prev_i) || ((-gap + ema_actual) > prev_i)){ // IF ACTION MIGHT BE HAPPENING
      // fill sensor window, smooth with ema
      int j;
      for(j = 1; j < WINDOW; j++) {
        // cout<<"CURRENT i VALUE: "<<i+j<<"\n";
        if((i+j) >= content.size()){
          cout<<"WINDOW TOO SHORT"<<i+j<<"\n";
          short_window = 1;
          break;
        }
        val =  stof(content[i+j][1]);
        ema_actual = ema(val, actual, alpha);
        // cout<<ema_actual<<'\n';
        sensorWindow[j-1] = ema_actual; 
        actual = ema_actual;
      }

      //  MAKE PREDICTION ON WINDOW
      if(short_window == 0){
          prediction =   match_filter_prediction(sensorWindow); // get avg
          count++;
          i = (i+j);
      }
      else{
        prediction = -1;
        break;
      }
    
      cout<<"FINAL PREDICTION: " << prediction << "\n";
    }

    // save previous val for signal detection and incr i to next value
    prev_i = val;
    i++;

}
cout<<"TOTAL PREDICTIONS: "<< count<<'\n';
return 0;
}

