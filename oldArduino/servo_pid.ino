
#include <Servo.h>

Servo serv;

int SERV_PIN = 9;
int SERV_MAX = 120;

int counter = 0; int val;

float expected = 0.0; float actual = 0.0;
float err[] = {0.0, 0.0, 0.0};

//////////////////////////////////
// Modulate Values Below
//////////////////////////////////

float Ku = 0.0;
float Tu = 0.0;

float Kp = 0.0;     // 1 - 0.5Ku;   0.45Ku;     0.60Ku;
float Ki = 0.0;     // 2 - ;        0.54Ku/Tu;  1.2Ku/Tu;
float Kd = 0.0;     // 3 - ;        ;           3KuTu/40;

unsigned long dt = 63;

//////////////////////////////////

float coeff_0 = Kp + Ki*dt + Kd/dt;
float coeff_1 = -Kp - 2*Kd/dt;
float coeff_2 = Kd/dt;


void setup() {
  serv.attach(SERV_PIN);
}


// buf of 128
// loop frequency 16Hz
void loop() {
  // Uncomment for specific motion
  // 1) back and forth, 2) flexion, 3) extension, 4) static
  // val = (count / 75) % 2 == 0 ? 0 : 1;
  // val = 0;
  // val = 1;
  // exp = val ? max(exp - 10, 0) : min(exp + 10, SERV_MAX);
  expected = 90;

  err[2] = err[1];
  err[1] = err[0];
  err[0] = expected - actual;

  actual = actual + coeff_0 * err[0] + coeff_1 * err[1] + coeff_2 * err[2];
  actual = max(actual, 0)
  actual = min(actual, SERV_MAX)
  serv.write(actual);

  counter += 1;
  delay(dt);
}
