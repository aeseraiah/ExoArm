
#include <Servo.h>

// white - signal
// red - 5V
// black - GND

// minimum and maximum servo driving values
// these correspond to 0 deg and appx 1710 deg
// this is almost 5 entire rotations
int MIN = 0;
int MAX = 180;

bool HOLD = true;

Servo serv;

int pos = 0;

void setup() {
  serv.attach(9);
}

void loop()
{
  if (HOLD == true)
  {
    serv.write(0);
  }
  else
  {
    for (pos = 0; pos <= 180; pos += 1)
    {
      serv.write(pos);
      delay(100);
    }

    for (pos = 180; pos >= 0; pos -= 1)
    {
      serv.write(pos);
      delay(100);
    }
  }

  
}
