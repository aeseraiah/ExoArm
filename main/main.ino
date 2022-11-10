// These constants won't change. They're used to give names to the pins used:
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
const int analogOutPin = 7;  // Analog output pin that the LED is attached to

int sensorValue = 0;  // value read from the pot
int outputValue = 0;  // value output to the PWM (analog out)
int D7 = 7;

int sensorWindow[128]; // init sensor array of window size
int amplitudeWindow;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(D7, OUTPUT);
}

void loop() {
  windowedClassifier();
}

void getSensorValue() {
  // sensorValue = analogRead(analogInPin);
  // // outputValue = map(sensorValue, 0, 1023, 0, 255);
  // outputValue = sensorValue; // use raw sensor value for now
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

void driveServo(){
  ;
}

void driveLinAct(){
  ;
}

void classifySVM(){
  ;
}

void windowedClassifier() {

  // READ 128 SAMPLE WINDOWS FROM EMG & AVERAGE
  getWindowedAmplitude();

  // INPUT AMPL INTO CLASSIFER TO DRIVE ACTUATION
  classifySVM();

  // OUTPUT DIGITAL SIGNAL TO SERVO/ LINEAR ACTUATION
  driveServo();

  driveLinAct();
  
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

  // SERIAL OUTPUT FOR DEBUGGING
  Serial.print("sensor = ");
  Serial.print(sensorValue);
  Serial.print("\t output = ");
  Serial.println(outputValue);

  // wait 100 milliseconds before the next loop for the analog-to-digital
  // converter to settle after the last reading:
  delay(100);
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