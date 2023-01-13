#include <Arduino.h>

/*
  A simple Tachometer Program made to report the rpm and 
  number of revolutions to an LCD screen with an I2C adapter.
  Uses the hall effect and a 3144 sensor to detect revolutions.
  Developed on an Arduino Uno Rev 3 and adapted to an Arduino Nano.
*/

// include the library code:
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display


//define variables
const int ledPin = 5;
const int buttonPin = 3;
float value = 0;
float revpermin = 0;
int rev = -1;
int rpm;
int oldtime = 0;
int time;
//bool catcher = false;

//Interrupt service routine
void isr()
{
  rev ++;
  revpermin ++;
}

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.init();
  lcd.backlight();
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Resetting...");
  digitalWrite(2, HIGH);  //Instead if using a pull up resistor
  attachInterrupt(0, isr, RISING);  //Attaching the interrupt

}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  delay(2000); // 2 second delay
  detachInterrupt(0); //Detaches the interrupt while calculating
  time = millis() - oldtime;  //Finds the time
  rpm = (revpermin / time) * 60000; //Calculates the rpm
  oldtime = millis(); //Saves the current time
  revpermin = 0;

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("RPM:");
  lcd.setCursor(5,0);
  lcd.print(rpm);
  lcd.setCursor(0, 1);
  lcd.print("Revolutions:");
  lcd.setCursor(12,1);
  lcd.print(rev);

  attachInterrupt(0, isr, RISING);
}

