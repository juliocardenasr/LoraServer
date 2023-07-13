#include <Arduino.h>
/*
********************************************************************************
  Performance variables
********************************************************************************
*/
unsigned long cycles;
unsigned long average;
String saverage;

/*
********************************************************************************
  Time variables
********************************************************************************
*/
#include <Ticker.h>
Ticker tickerLed;
Ticker tickerClock;

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

/*
********************************************************************************
  Clock variables
********************************************************************************
*/
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

/*
**************************************************************************************************
  I2C variables
**************************************************************************************************
*/
#define PinSDA 21
#define PinSCL 22
#include <Wire.h>

/*
********************************************************************************
  KY040 variables Rotatory encoder
********************************************************************************
*/
#define CLK_PIN 36
#define DT_PIN  39
#define SW_PIN  34

int  lastEncoded     = 0;                // Valor anterior del encoder
bool buttonPressed   = false; 
bool optionChanged   = false;
int  counter         = 0; 
int  optionMin       = 0;
int  optionMax       = 3;
int  option          = 0; 

void handleButtonPress() {
  buttonPressed = true;                  // Marca el botón como presionado
}

void updateEncoder() {
  int MSB = digitalRead(CLK_PIN);    
  int LSB = digitalRead(DT_PIN);       
  int encoded = (MSB << 1) | LSB;
  int optionNew;

  if (encoded != lastEncoded) {  // aqui se descartan los siguientes estados Ob1111 Ob0101 b0000 b1010
    int sum = (lastEncoded << 2) | encoded;
    if (sum == 0b0011 || sum == 0b0110 || sum == 0b1001 || sum == 0b1100) {
      // estos estados no son validos, por lo tanto se ignoran
    }
    else {
      if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) {
        // giro a la derecha
        counter++;
      }
      else {
        // sum == 0b1110 || sum == 0b1000 || sum == 0b0001 || sum == 0b0111
        // giro a la izquierda
        counter--;
      }
      if (counter % 4 == 0) {
        optionNew = counter / 4;
        if (optionNew != option) {
          option = optionNew;
          optionChanged = true;
          if (option < optionMin) { 
            option = optionMax;
            counter = optionMax * 4;
          } 
          if (option > optionMax){ 
            option = optionMin;
            counter = 0;
          }
        }
      }
      lastEncoded = encoded; 
    } 
  }
  
}

void setup() {
  // initialize Serial Port
  Serial.begin(115200);
  delay(3000);
  Serial.println("Serial               : OK");

  // initialize performance Variables
  cycles = 0;
  average = 0;
  Serial.println("Performance variables: OK");

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

  //Initialize the I2C bus
  Wire.begin(PinSDA,PinSCL); 
  Serial.println("I2C Bus              : OK");

  // initialize the rotatory encoder KY40 variables
  attachInterrupt(digitalPinToInterrupt(CLK_PIN), updateEncoder,     CHANGE); 
  attachInterrupt(digitalPinToInterrupt(DT_PIN),  updateEncoder,     CHANGE);   
  attachInterrupt(digitalPinToInterrupt(SW_PIN),  handleButtonPress, FALLING);
  lastEncoded = digitalRead(CLK_PIN);                     // Lee el estado del pin CLK
  lastEncoded = (lastEncoded << 1) | digitalRead(DT_PIN); // Combina los estados de CLK y DT en un solo valor
  counter   = 0;
  optionMin = 0;
  optionMax = 3;
  Serial.println("Rotatory encoder     : OK"); 
}

void loop() {
  if (buttonPressed) {
    Serial.print(stime);
    Serial.println(" el boton fue presionado");
    buttonPressed = false;
  }
  if (optionChanged) {
    Serial.print(stime);
    Serial.print(" ");
    Serial.println(option);
    optionChanged = false;
  }
  cycles++;
  average = cycles / elapsedSeconds;
}

