/*
  Open Hive | ads1234.ino (untested!!)
  --------------------------------
  sensors:  | Scale

  pins hard coded
  ---------------
  D14 D15 D16  Data, SCL, Power - ADS1231
  D17          (optional) Power load cell and ADS1231
  D2 D3        Mux pins A0, A1
  
  Changelog:
  2017-06-29 Original code provided by Clemens
  2018-02-20 Created this file from : https://community.hiveeyes.org/t/code-fur-ads1232-oder-ads1234/392/2
             changed entry commentary
*/

// -------------------------+------
// variables you can modify | START
// -------------------------+------

// ** load cell characteristic
// ---------------------------

// define here individual values for the used load cell
// this is not type specific! even load cells of the same type /
// model have individual characteristics
//
// - ZeroOffset is the raw sensor value for "0 kg"
//   write down the sensor value of the scale with no load and
//   adjust it here
// - KgDivider is the raw sensor value for a 1 kg weight load
//   add a load with known weight in kg to the scale, note the
//   sesor value, calculate the value for a 1 kg load and adjust
//   it here
const long loadCellZeroOffset = 38623;
// const long loadCellKgDivider  = 22053;
// 1/2 value for single side measurement, so that 1 kg is displayed as 2 kg
const long loadCellKgDivider  = 11026;

// -------------------------+----
// variables you can modify | END
// -------------------------+----

const int loadCellMuxA0 = 2;
const int loadCellMuxA1 = 3;

// libraries

// load cell
#include <ADS1231.h>
ADS1231 loadCell;  // create ADS1231 object

// weight
char weightChar[9];  // should handle +-xxx.xxx and null terminator


void setup () {
  // debug 
  Serial.begin(9600);
 
  // weight / ADS1231
  // pin definition: SCL 15, Data 14, Power 16
  loadCell.attach(15,14,16);
  // power pin load cell and ADS1231: pin 17
  pinMode(17, OUTPUT);
  
  // set mux pins as output
  pinMode(loadCellMuxA0, OUTPUT);
  pinMode(loadCellMuxA1, OUTPUT);
}


// functions for devices
// ---------------------

// weight
#ifdef isScale
  void getWeight() {
    // power ADS1231 and load cell
    digitalWrite(17, HIGH);
    loadCell.power(HIGH);

    delay(2);  // wait for stabilization

    // ADS1231 ready?
    while (!loadCell.check()) {
      loadCell.check();
    }

    // read raw data input of ADS1231
    long weightSensorValue = loadCell.readData();

    // switch off ADS1231 and load cell
    loadCell.power(LOW);
    digitalWrite(17, LOW);

    // calculate weight in kg
    float weightKg = ((float)weightSensorValue - (float)loadCellZeroOffset) / (float)loadCellKgDivider;

    // Write to char array
    dtostrf(weightKg, 8, 3, weightChar);
  }


// Main program
// ------------

void loop () {
  // for ADS1232/4 with 2 load cells 
  // set load cell AIN1
  digitalWrite(loadCellMuxA0, 0); 
  digitalWrite(loadCellMuxA1, 0); 

  // perhaps we need here some delay to wait for stabilization 
  delay(50);  // value has to be tested 
  
  // get weight 
  getWeight();

  // output weight
  Serial.print("weight load cell 1 (AIN1): ");
  Serial.print(weightChar);
  Serial.println(); 

  
  // set load cell AIN2
  digitalWrite(loadCellMuxA0, 0); 
  digitalWrite(loadCellMuxA1, 1); 

  // perhaps we need here some delay to wait for stabilization 
  delay(50);  // value has to be tested 
  
  // get weight 
  getWeight();

  // output weight
  Serial.print("weight load cell 2 (AIN2): ");
  Serial.print(weightChar);
  Serial.println(); 


  
  // additional for ADS1234 with 4 load cells 
  // set load cell AIN3
  digitalWrite(loadCellMuxA0, 1); 
  digitalWrite(loadCellMuxA1, 0); 

  // perhaps we need here some delay to wait for stabilization 
  delay(50);  // value has to be tested 
  
  // get weight 
  getWeight();

  // output weight
  Serial.print("weight load cell 3 (AIN3): ");
  Serial.print(weightChar);
  Serial.println(); 

  
  // set load cell AIN4
  digitalWrite(loadCellMuxA0, 1); 
  digitalWrite(loadCellMuxA1, 1); 
  
  // perhaps we need here some delay to wait for stabilization 
  delay(50);  // value has to be tested 
  
  // get weight 
  getWeight();

  // output weight
  Serial.print("weight load cell 4 (AIN4): ");
  Serial.print(weightChar);
  Serial.println(); 
  
  delay(30 * 1000);  // wait 30 seconds 
  Serial.println();  
}
