
#include <Servo.h>

Servo serv;

int SERV_PIN = 9;
int SERV_MIN = 0;
int SERV_MAX = 120;

unsigned long dt_ms = 63; // 16 Hz

int counter = 0;
int val;

float expected = 0.0;
float actual = 0.0;

float alpha = 0.25; // Must be between 0 and 1



void setup() {
  // serv.attach(SERV_PIN);
  Serial.begin(9600);

  randomSeed(analogRead(0));
}


// loop frequency 16Hz
void loop() {
  // Uncomment for specific motion
  // 1) back and forth, 2) flexion, 3) extension, 4) static
  //val = (counter / 25) % 2 == 0 ? 0 : 1;
  // val = 0;
  // val = 1;
  val = random(3);

  if (val == 0) {
    expected = min(expected + 10, SERV_MAX);
  }
  else if (val == 1) {
    expected = max(expected - 10, SERV_MIN);
  }

  actual = alpha * expected + (1 - alpha) * actual;

  // actual = max(actual, SERV_MIN);
  // actual = min(actual, SERV_MAX);
  // serv.write(actual);
  
  Serial.print("Lo:"); Serial.print(SERV_MIN - 10); Serial.print(",");
  Serial.print("Hi:"); Serial.print(SERV_MAX + 10); Serial.print(",");

  Serial.print("Expected:"); Serial.print(expected); Serial.print(",");
  Serial.print("Actual:"); Serial.println(actual);

  counter += 1;
  delay(dt_ms);
}
