#include <Arduino.h>

#include <Ticker.h>


/*
********************************************************************************
  Led variables
********************************************************************************
*/
Ticker tickerLed;
int pinLed;
int indexLed;
int lenLed;
String msgLed;

void actualizeLed() {
  if (indexLed >= lenLed) indexLed = 0;
  if (msgLed.charAt(indexLed) == '1') 
  {
    digitalWrite(pinLed, HIGH);
  }
  else
  {
    digitalWrite(pinLed, LOW);
  }
  indexLed++;
}

/*
********************************************************************************
  Clock variables
********************************************************************************
*/
Ticker  tickerClock;

unsigned long elapsedSeconds;
int seconds;
int minutes;
int hours;
String stime;

void actualizeClock() 
{
    elapsedSeconds++;
    seconds++;

    if (seconds > 59) 
    {
        seconds = 0;
        minutes++;
        if (minutes > 59) 
        {
            minutes = 0;
            hours++;
            if (hours > 23) 
            {
                hours = 0;
            }
        }
    }

    stime = "";
    if (hours < 10) 
    {
        stime = stime + "0";
    }
    stime = stime + String(hours);
    stime = stime + ":";
    if (minutes < 10) 
    {
        stime = stime + "0";
    }
    stime = stime + String(minutes);
    stime = stime + ":";
    if (seconds < 10) 
    {
        stime = stime + "0";
    }
    stime = stime + String(seconds);
}

void setup() {
  Serial.begin(115200);
  delay(3000);
  Serial.println("Serial               : OK");

  // initialize led variables
  pinLed = LED_BUILTIN;
  pinMode(pinLed, OUTPUT);
  //.--- ..- .-.. .. --- .---- ..--- ...--   JULIO123
  msgLed = "01011101110111001010111001011101010010100111011101110010111011101110111001010111011101110010101011101110";
  lenLed = msgLed.length();
  indexLed = 0;

  tickerLed.attach_ms(120, actualizeLed);
  Serial.println("Led variables        : OK");

  // initialize clock variables
  elapsedSeconds = 1;
  seconds = 0;
  minutes = 0;
  hours   = 0;
  stime   = "00:00:00";

  tickerClock.attach(1, actualizeClock);
  Serial.println("Clock variables      : OK"); 

  
}

void loop() {
  
}

