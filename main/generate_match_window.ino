

#include <EEPROM.h>

// type used to capture each data point in a sample
// !maximum space in nano is believed to be 1024B!
// in that case, the hard max is 5B per data point
#define USE_TYPE short

// N - number of samples represented in window average
// LO - first sample in window average
// HI - last sample in window average
int BASE = 0;
int LENGTH = 100;

int ADDR_FN  = 0
int ADDR_FLO = ADDR_FN + sizeof(USE_TYPE);
int ADDR_FHI = ADDR_FLO + (LENGTH - 1) * sizeof(USE_TYPE);

int ADDR_EN = ADDR_FHI + sizeof(USE_TYPE);
int ADDR_ELO = ADDR_EN + sizeof(USE_TYPE);
int ADDR_EHI = ADDR_ELO + (LENGTH - 1) * sizeof(USE_TYPE);
///////////////////////////////////////////////////////

int addr; 
USE_TYPE val;
USE_TYPE N;

const uint8_t in_pin = A0;
int emg;

int dt_ms = 30;

bool start_fresh = false;

void setup()
{
  Serial.begin(9600);
  pinMode(in_pin, INPUT);

  if(start_fresh)
  {
    mem_wipe();
  }
}

void mem_wipe()
{
  for (int i = 0; i < EEPROM.length() ; i++)
  {
    EEPROM.update(i, 0)
  }
}

void loop()
{
  if( Serial.available() > 0 )
  {
    char in_char = Serial.read();

    // First cases loop <LENGTH> times with delay <dt_ms> (nom. 100, 30ms).
    // Loop iterates over the address space, stepping by sizeof datapoints.
    // It reads the old value from mem, expands it to maintain an accurate
    // average, and folds a new sensor value into the average window.
    switch (in_char)
    {
      case 'f':
        EEPROM.get(ADDR_FN, N);

        for (addr = ADDR_FLO; addr <= ADDR_FHI; addr += sizeof(USE_TYPE))
        {
          EEPROM.get(addr, val);
          val *= N;

          emg = analogRead(in_pin);
          val += emg;
          val /= N + 1;

          EEPROM.put(addr, val);
          delay(dt_ms);
        }

        N += 1;
        EEPROM.put(ADDR_FN, N);        
        break;

      case 'e':
        EEPROM.get(ADDR_EN, N);

        for (addr = ADDR_ELO; addr <= ADDR_EHI; addr += sizeof(USE_TYPE))
        {
          EEPROM.get(addr, val);
          val *= N;

          emg = analogRead(in_pin);
          val += emg;
          val /= N + 1;

          EEPROM.put(addr, val);
          delay(dt_ms);
        }

        N += 1;
        EEPROM.put(ADDR_EN, N);        
        break;

      // Returns N followed by window; of flex, then extend (in csv format)
      case 'm':
        for(addr = ADDR_FN; addr <= ADDR_FHI; addr += sizeof(USE_TYPE))
        {
          EEPROM.get(addr, val);
          Serial.print(val);
          Serial.print(",");
        }
        Serial.println();

        for(addr = ADDR_EN; addr <= ADDR_EHI; addr += sizeof(USE_TYPE))
        {
          EEPROM.get(addr, val);
          Serial.print(val);
          Serial.print(",");
        }
        Serial.println();
        break;

      // User operated ROM wipe
      case '0':
        mem_wipe();
        break;

      default:
        // any default
        break;
    }
  }
}






















