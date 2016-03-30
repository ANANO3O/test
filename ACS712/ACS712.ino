#include <Stepper.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

/*
Measuring AC Current Using ACS712
*/
const int sensorIn = A0;
int mVperAmp = 185; // use 100 for 20A Module and 66 for 30A Module


double Voltage = 0;
double VRMS = 0;
double AmpsRMS = 0;


const int stepsPerRevolution = 2048;  
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);
//////////////////////////////////////////////////////////
LiquidCrystal_I2C lcd(0x27,16,2);


void setup()
{ 
    Serial.begin(9600);

    myStepper.setSpeed(10);

    lcd.init();  
    lcd.backlight();

    lcd.print("Hello, world!");
    delay(1000);
    lcd.clear();
}

void loop()
{
    Voltage = getVPP();
    VRMS = (Voltage/2.0) *0.707; 
    AmpsRMS = (VRMS * 1000)/mVperAmp;
    Serial.print(AmpsRMS);
    Serial.println(" Amps RMS");

    lcd.setCursor(0, 0);
    lcd.print(AmpsRMS);
    lcd.print(" Amps RMS");

    if(AmpsRMS <= 0.60)
    {
        myStepper.step(-56);
    }
    else
    {
        myStepper.step(28);
    }
}

float getVPP()
{
    float result;
  
    int readValue;             //value read from the sensor
    int maxValue = 0;          // store max value here
    int minValue = 1024;          // store min value here
  
    uint32_t start_time = millis();
    while((millis()-start_time) < 1000) //sample for 1 Sec
    {
        readValue = analogRead(sensorIn);
        // see if you have a new maxValue
        if (readValue > maxValue) 
        {
            /*record the maximum sensor value*/
            maxValue = readValue;
        }
        if (readValue < minValue) 
        {
            /*record the maximum sensor value*/
            minValue = readValue;
        }
    }

    // Subtract min from max
    result = ((maxValue - minValue) * 5.0)/1024.0;

    return result;
}