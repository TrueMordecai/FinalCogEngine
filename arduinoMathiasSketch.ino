#include "Adafruit_VL53L0X.h"
#include "Wire.h"
#include "vl53l0x_api.h"

// First Lox adress for the multiple sensor
#define firstLoxAddress 0x30

// Shutdown Address ? (maybe not)
#define firstLoxSht 7

// CHANGE IT IF YOU WANT OTHER SENSOR !
#define sensorNumber 3


// Also add the equivalent of constructor here, should have a cleaner way to do it tho
Adafruit_VL53L0X *loxArray[sensorNumber] = {new Adafruit_VL53L0X(), new Adafruit_VL53L0X(), new Adafruit_VL53L0X()};
VL53L0X_RangingMeasurementData_t measurementsDatas[sensorNumber];

// Set the adress and id of the sensors, refers to https://learn.adafruit.com/adafruit-vl53l0x-micro-lidar-distance-sensor-breakout/arduino-code for further informations. Might not work with multiple other sensors
void setID()
{
    for (char i = 0; i != sensorNumber; i++)
        digitalWrite(7 + i, LOW);
    for (char i = 0; i != sensorNumber; i++)
        digitalWrite(7 + i, HIGH);
    for (char i = 0; i != sensorNumber; i++) {
        if (i == 0) {
            digitalWrite(firstLoxSht, HIGH);
        } else {
            digitalWrite(7 + i, LOW);
        }
    }
    for (int i = 0; i != sensorNumber; i++) {
        if (i >= 1)
          digitalWrite(firstLoxSht + i, HIGH); // Obscure things, might don't understand this why only the first ??
        if (!loxArray[i]->begin(firstLoxAddress + i, false, &Wire, Adafruit_VL53L0X::VL53L0X_SENSE_LONG_RANGE)) { // Security but useless in real life             
            Serial.print(F("Failed to boot : "));
            Serial.print(firstLoxAddress + i);
            while(1);
            }
    }
}

void readSensorsAndWrite()
{
    for (char i = 0; i != sensorNumber; i++)
          loxArray[i]->rangingTest(&measurementsDatas[i], false); // pass in 'true' to get debug data printout! Fill measurementsDatas with data
    for (char i = 0; i != sensorNumber; i++) {
        if (measurementsDatas[i].RangeStatus == 0) // if not out of range
            Serial.print(measurementsDatas[i].RangeMilliMeter );
        else
            Serial.print(2999); // If out of range then it print 2500
        Serial.print(';');
    }
    Serial.print('\n');
}

void setup()
{
    Serial.begin(115200);
    pinMode(13, OUTPUT);
    while (!Serial) // Wait until serial port opens for native USB devices
        delay(1);
    for (char i = 0; i != sensorNumber; i++) // No idea
        pinMode(firstLoxSht + i, OUTPUT);
    for (char i = 0; i != sensorNumber; i++) // No idea too
        digitalWrite(firstLoxSht + i, LOW);
    setID();
}

void loop()
{
    readSensorsAndWrite();
    
    if (Serial.available() > 0) { // No idea for this block, comment it and it still work very well.
        int incomingByte=0;
        incomingByte = Serial.read();
    }
}
