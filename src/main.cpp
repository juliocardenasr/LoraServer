#include <Arduino.h>

#include <Ticker.h>
Ticker  tickerLed;

/*
********************************************************************************
  Led variables
********************************************************************************
*/
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
  
}

void loop() {
 
}

