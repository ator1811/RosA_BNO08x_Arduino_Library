/*
  Using the BNO08x IMU

  This example shows how to use the Tare features of the sensor.
  It outputs the roll, pitch and yaw.
  Type letters into the terminal to execute Tare commands.

  Example : Tare function
  By: Sofian Audry
  Date: March 2nd, 2022

  Based on: Example9-Calibrate
  By: Nathan Seidle
  SparkFun Electronics
  Date: December 21st, 2017
  SparkFun code, firmware, and software is released under the MIT License.
	Please see LICENSE.md for further details.

  Originally written by Nathan Seidle @ SparkFun Electronics, December 28th, 2017

  Adjusted by Pete Lewis @ SparkFun Electronics, June 2023 to incorporate the
  CEVA Sensor Hub Driver, found here:
  https://github.com/ceva-dsp/sh2

  Also, utilizing code from the Adafruit BNO08x Arduino Library by Bryan Siepert
  for Adafruit Industries. Found here:
  https://github.com/adafruit/Adafruit_BNO08x

  Also, utilizing I2C and SPI read/write functions and code from the Adafruit
  BusIO library found here:
  https://github.com/adafruit/Adafruit_BusIO

  Hardware Connections:
  Plug the sensor into IoT RedBoard via QWIIC cable.
  Serial.print it out at 115200 baud to serial monitor.

  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/22857
*/

#include <Wire.h>

#include "SparkFun_BNO08x_Arduino_Library.h"  // Click here to get the library: http://librarymanager/All#SparkFun_BNO08x
BNO08x myIMU;

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("BNO08x Read Example");

  //Once magnetic field is 2 or 3, run the Save DCD Now command
  Serial.println(F("'t' to tare according to xyz"));
  Serial.println(F("'z' to tare according to z axis only"));
  Serial.println(F("'s' to save tare settings"));
  Serial.println(F("'r' to reset/clear tare orientation registry"));
  Serial.println(F("Output in form x, y, z, in uTesla"));

  delay(1000); // you have one second to read the menu in the terminal.
  // or simply look at the code above, and take all the time you want.

  Wire.begin();

  if (myIMU.begin() == false) {
    Serial.println("BNO08x not detected at default I2C address. Check your jumpers and the hookup guide. Freezing...");
    while (1)
      ;
  }
  Serial.println("BNO08x found!");

  // Wire.setClock(400000); //Increase I2C data rate to 400kHz

  setReports();

  Serial.println("Reading events");
  delay(100);
}

// Here is where you define the sensor outputs you want to receive
void setReports(void) {
  Serial.println("Setting desired reports");
  if (myIMU.enableRotationVector() == true) {
    Serial.println(F("Rotation vector enabled"));
    Serial.println(F("Output in form roll, pitch, yaw"));
  } else {
    Serial.println("Could not enable rotation vector");
  }
}

void loop() {
  if(Serial.available())
  {
    byte incoming = Serial.read();

    switch (incoming) {
      case 't':
        if(myIMU.tareNow()) {Serial.println("TareXYZ Success");}
        else {Serial.print ("TareXYZ Failure");}
        break;
      case 'z':
        if(myIMU.tareNow(true)) {Serial.println("TareZ Success");}
        else {Serial.print ("TareZ Failure");}
        break;
      case 's':
        if(myIMU.saveTare()) {Serial.println("Save Tare Success");}
        else {Serial.print ("Save Tare Failure");}
        break;
      case 'r':
        if(myIMU.clearTare()) {Serial.println("Clear Tare Success");}
        else {Serial.print ("Clear Tare Failure");}
        break;
      default:;
    }
  }

  delay(10);

  if (myIMU.wasReset()) {
    Serial.print("sensor was reset ");
    setReports();
  }

  // Has a new event come in on the Sensor Hub Bus?
  if (myIMU.getSensorEvent() == true) {

    // is it the correct sensor data we want?
    if (myIMU.getSensorEventID() == SENSOR_REPORTID_ROTATION_VECTOR) {

    float roll = (myIMU.getRoll()) * 180.0 / PI; // Convert roll to degrees
    float pitch = (myIMU.getPitch()) * 180.0 / PI; // Convert pitch to degrees
    float yaw = (myIMU.getYaw()) * 180.0 / PI; // Convert yaw / heading to degrees

    Serial.print(roll, 1);
    Serial.print(F(","));
    Serial.print(pitch, 1);
    Serial.print(F(","));
    Serial.print(yaw, 1);

    Serial.println();
    }
  }
}
