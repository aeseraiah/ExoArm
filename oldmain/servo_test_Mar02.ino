
#include <Servo.h>

// white - signal
// red - 5V
// black - GND

// minimum and maximum servo driving values
// these correspond to 0 deg and appx 1710 deg
// this is almost 5 entire rotations
int MIN = 10;
int MAX = 80;

int SPEED = 20;
int DELAY = 400;

bool HOLD = false;
int H_VAL = 75;

Servo serv;

int pos = 0;

void setup() {
  serv.attach(A0);
}

void loop()
{
  if (HOLD == true)
  {
    serv.write(H_VAL);
  }
  else
  {
    for (pos = MIN; pos <= MAX; pos += SPEED)
    {
      serv.write(pos);
      delay(DELAY);
    }

    for (pos = MAX; pos >= MIN; pos -= SPEED)
    {
      serv.write(pos);
      delay(DELAY);
    }
  }

  
}
