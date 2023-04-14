#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
// #include <ctype.h>
#include <stdint.h>
#include <cmath>
using namespace std;

float ema(int exp, float actual, float alpha){
  return alpha * exp + (1 - alpha) * actual;
}

double PEAK_HEIGHT_FLX = 335;
double PEAK_HEIGHT_EXT = 295;
int MID = 50;
int WINDOW = 100;
int POOR_PREDICTION_THRESHOLD = 1000000;

double flx_aligned[100];
double ext_aligned[100];
double CONVOLUTION[100];


double convolve(double* A, double* B){

  for(int i = 0; i < WINDOW; i++)
  {
    CONVOLUTION[i] = 0;
    for(int j = 0; j < WINDOW; j++){
      CONVOLUTION[i] += A[i-j] * B[j];
    }
  }
  return 1;
}

double find_peaks(double *data, double max_height){
  // iterate through data file, comparing each pt until peak obtained larger than peak_height
  int MAX_INDEX = 0;
  double PREV = 0;
  for(int i = 0; i < WINDOW; i++){
    cout<<"DATA"<<i<<":"<<data[i]<<'\n';
    if ((data[i] > PREV) && (data[i] > max_height)){
      MAX_INDEX = i;
    }
  }
  cout<<"MAX_INDEX:"<<MAX_INDEX<<"\n\n";
  return MAX_INDEX;
}

void align_signal(double *data){
  double max_height;
  int peak_index, gap;
  double DC_COMPONENT = 330;
  double neg_data[100];
  
  max_height = PEAK_HEIGHT_FLX;
  peak_index = find_peaks(data, max_height); // find maximum peak
  if (peak_index){
    if (peak_index > MID){
        gap = peak_index - MID;

        for(int i = 0; i < (WINDOW - gap); i++){
          flx_aligned[i] = data[gap]; // save starting at gap up to window end
          gap++;
        }
        for(int i = gap; i < WINDOW; i++){
          flx_aligned[i] = DC_COMPONENT; // make the rest of the window DC component flat
        }

    }
    else if (peak_index <= MID){
        gap = MID - peak_index;

        for(int i = 0; i < gap; i++){
          flx_aligned[i] = DC_COMPONENT; // save starting at gap up to window end
        }
        for(int i = 0; i < (WINDOW-gap); i++){
          flx_aligned[gap] = data[i]; // make the rest of the window DC component flat
          gap++;
        }
    }
    else{
      for(int i = 0; i < WINDOW; i++){
        flx_aligned[i] = data[i]; // IF NO PEAKS ARE FOUND AT THE SPECIFIED HEIGHT, RETURN ORIGINAL WAVE
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
          ext_aligned[i] = data[gap]; // save starting at gap up to window end
          gap++;
        }
        for(int i = gap; i < WINDOW; i++){
          ext_aligned[i] = DC_COMPONENT; // make the rest of the window DC component flat
        }

    }
    else if (peak_index <= MID){
        gap = MID - peak_index;

        for(int i = 0; i < gap; i++){
          ext_aligned[i] = DC_COMPONENT; // save starting at gap up to window end
        }
        for(int i = 0; i < (WINDOW-gap); i++){
          ext_aligned[gap] = data[i]; // make the rest of the window DC component flat
          gap++;
        }
    }
    else{
      for(int i = 0; i < WINDOW; i++){
        ext_aligned[i] = data[i]; // IF NO PEAKS ARE FOUND AT THE SPECIFIED HEIGHT, RETURN ORIGINAL WAVE
      }
    }
  }

}

double find_max(double *data){
  int MAX_VALUE = 0;
  double PREV = 0;
  for(int i = 0; i < WINDOW; i++){
    if ((data[i] > PREV)){
      MAX_VALUE = i;
    }
  }
  return MAX_VALUE;
}

double match_filter_prediction(double *d){
    // THIS IS CONVERTED TO ARDUINO CODE/CPP


  double FLEX_FILTER[100] = {298, 298, 298, 298, 298, 298, 296, 294, 291, 287, 284, 279, 274, 267, 260, 252, 244, 235, 228, 220, 211, 201, 190, 178, 164, 152, 140, 127, 117, 110, 108, 113, 124, 140, 160, 179, 197, 216, 237, 260, 284, 309, 332, 355, 379, 404, 429, 453, 476, 495, 511, 526, 538, 549, 558, 566, 572, 576, 578, 578, 577, 574, 570, 566, 561, 555, 550, 544, 538, 531, 524, 517, 511, 504, 498, 491, 485, 478, 472, 466, 460, 454, 448, 443, 438, 432, 427, 422, 418, 414, 409, 405, 402, 397, 394, 391, 388, 384, 381, 378};
  double EXT_FILTER[100] = {433, 430, 427, 423, 419, 416, 413, 411, 412, 412, 412, 412, 414, 420, 427, 433, 434, 431, 424, 413, 395, 375, 352, 328, 306, 288, 277, 272, 273, 279, 288, 300, 313, 326, 340, 353, 363, 373, 380, 386, 391, 396, 400, 405, 409, 413, 416, 419, 423, 427, 430, 433, 436, 437, 438, 437, 435, 433, 430, 428, 425, 423, 420, 416, 412, 407, 402, 397, 392, 387, 381, 376, 371, 367, 363, 359, 355, 350, 347, 344, 342, 339, 337, 334, 332, 331, 330, 328, 327, 325, 324, 323, 322, 322, 321, 320, 319, 319, 319, 318};
  double RST_FILTER[100] = {322, 322, 321, 321, 320, 320, 319, 319, 318, 318, 318, 318, 317, 317, 317, 317, 317, 317, 317, 316, 317, 316, 316, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 318, 318, 318, 319, 319, 319, 319, 320, 320, 320, 321, 321, 321, 321, 322, 322, 322, 322, 322, 322, 322, 322, 322, 322, 323, 323, 323, 323, 324, 324, 324, 324, 325, 325, 325, 325, 325, 325, 326, 326, 326, 327, 327, 327, 327, 327, 327, 327, 327, 327, 328, 328, 328, 328, 328, 328, 328, 328, 328, 329, 329, 329, 329, 329, 330};

  double out_flx_aligned[100];
  double out_ext_aligned[100];
  double out_rst_aligned[100];

  double MAX_FLX;
  double MAX_EXT;
  double MAX_RST;
  double FLX_OR_EXT[2];
  double DIFF_EXT_RST;
  double DIFF_FLX_RST;
  double AVG_DIFF;
  

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

  DIFF_EXT_RST = abs(MAX_RST - MAX_EXT);
  DIFF_FLX_RST = abs(MAX_RST - MAX_FLX);

  AVG_DIFF = (DIFF_EXT_RST + DIFF_FLX_RST) / 2; 
  
  if (AVG_DIFF < POOR_PREDICTION_THRESHOLD){
      PRED = MAX_RST;
  }


//flexion = 0, extension = 1, sustain = 2, rest = 3
  if (PRED == MAX_FLX){
    cout<<"FLEX"<<"\n";
    return 0;
  }
  else if (PRED == MAX_EXT){
    cout<<"EXT"<<"\n";
    return 1;
  }
  else if (PRED == MAX_RST){
    cout<<"REST"<<"\n";
    return 3;
  }

  return 4;
}


int main() {

  double sensorWindow[100];
  // double sensorWindow;


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
  double data[100];


  double gap = 10;
  double val = 300;
  int windAVG = 40;
  int window = 40;
  int prev_i = 0;

  double actual = 0;
  double ema_actual = 0;
  double prediction = 0;

    // READ IN SIGNAL FROM FILE
    using namespace std;


    // string fname = "../data/CoolTerm Capture 2023-02-07 10-29-08.txt";
    string fname = "../data/CoolTerm Capture 2023-02-13 18-31-26.txt";

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
//     for(int i=0;i<content.size();i++)
//     {
//         for(int j=0;j<content[i].size();j++)
//         {
//             cout<<content[i][j]<<":  ";
//             cout<<j<<"  ";
//         }
//         cout<<"\r\n";
//     }

// j is element (1 has data)
// i is row


int i = 0;
while (i<content.size()){
    // cout<<i<<"\n";
    double val =  stof(content[i][1]); // holds current emg
    ema_actual = ema(val, actual, 0.1);
    actual = ema_actual;

    if(((gap + ema_actual) < prev_i) || ((-gap + ema_actual) > prev_i)){ // IF ACTION MIGHT BE HAPPENING
      for(int j = 1; j < window; j++) {
        val =  stof(content[i+j][1]);
        ema_actual = ema(val, actual, 0.1);
        sensorWindow[j-1] = ema_actual; // running sum, AVG
        actual = ema_actual;
        i++;
      }
      i++;
      // cout << i << " NEW VALUE OF I";
    //     // INPUT AMPL INTO CLASSIFER TO GET DIRECTION
      prediction =   match_filter_prediction(sensorWindow); // get avg
      cout<<"PREDICTION: " << prediction << "\n";

    //   // OUTPUT DIGITAL SIGNAL TO SERVO/ LINEAR ACTUATION

    //   driveServo(prediction);
    //   continue;
    // }
    // else{
    //   prev_i = val;
    //   continue;
    // }

    }
    i++;

}
return 0;
}

