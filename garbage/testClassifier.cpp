#include "MotionClassifiercopy.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
// #include <ctype.h>
#include <stdint.h>
#include <cmath>

Eloquent::ML::Port::SVM mclassifier;

float ema(int exp, float actual, float alpha){
  return alpha * exp + (1 - alpha) * actual;
}

int main() {

  // float sensorWindow[100];
  double sensorWindow;


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
  double gap = 10;
  int val = 300;
  int windAVG = 40;
  int window = 40;
  int prev_i = 0;

  double actual = 0;
  double ema_actual = 0;
  double prediction = 0;

    // READ IN SIGNAL FROM FILE
    using namespace std;


    string fname = "../data/CoolTerm Capture 2023-02-07 10-29-08.txt";

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

//PRINT EVERYTHING
    for(int i=0;i<content.size();i++)
    {
        for(int j=0;j<content[i].size();j++)
        {
            cout<<content[i][j]<<":  ";
            cout<<j<<"  ";
        }
        cout<<"\r\n";
    }

// j is element (1 has data)
// i is row


int i = 0;
while (i<content.size()){
    // cout<<i<<"\n";
    double val =  stof(content[i][1]); // holds current emg
    ema_actual = ema(val, actual, 0.1);
    actual = ema_actual;

    if(((gap + ema_actual) < prev_i) || ((-gap + ema_actual) > prev_i)){ // IF ACTION MIGHT BE HAPPENING
      // cout<<i<<" ";
      // cout<<"BLOAT\n";
      for(int j = 1; j < window; j++) {
        val =  stof(content[i+j][1]);
        ema_actual = ema(val, actual, 0.1);
        sensorWindow += ema_actual; // running sum, AVG
        actual = ema_actual;
        i++;
      }
      i++;
      // cout << i << " NEW VALUE OF I";
    //     // INPUT AMPL INTO CLASSIFER TO GET DIRECTION
      float pred_arg = (sensorWindow / window);
      cout<<"AVG: " << pred_arg << "\n";
      prediction = mclassifier.predict(&pred_arg); // get avg
      cout<<"PREDICTION: " << prediction << "\n";

      sensorWindow = 0;

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

