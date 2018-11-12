//sd library
#include <SD.h>
//barameter libraries
#include <SFE_BMP180.h>
#include <Wire.h>
//temp library
#include <SimpleDHT.h>

// start instance fields for SD card reading and writing
int chipSelect = 3;
File file;
// end instance fields for SD

// start instance fields for DHT11 temperature and humidity sensor
// for DHT11, 
//      VCC: 5V or 3V
//      GND: GND
//      DATA: A2
SimpleDHT11 dht11;
// end instance fields for DHT11
int pinDHT11 = A2;

//instance fields for BMP_180
SFE_BMP180 pressure;
double baseline;
#define ALTITUDE 216.0 // Altitude of SparkFun's HQ in Boulder, CO. in meters
//end instance for BMP


//runs when arduino starts
void setup() {
  Serial.begin(9600);

// SD card should be plugged into 4 on arduino
  pinMode(chipSelect, OUTPUT);
  if(!SD.begin(chipSelect)){
    Serial.println("DIDNT WORK");
  }

  //setting up file columnn headers
  file = SD.open("test2.txt", FILE_WRITE);
  if(file){
    file.println("T, H");
    file.close();
  }

  //check barameter to see if working
    if (pressure.begin())
      Serial.println("BMP180 init success");
    else
    {
      // Oops, something went wrong, this is usually a connection problem,
      // see the comments at the top of this sketch for the proper connections.
  
      Serial.println("BMP180 init fail (disconnected?)\n\n");
      while(1); // Pause forever.
    }

    // start working...
    Serial.println("=================================");
    Serial.println("Temp\tHumidity\tPressure\tAltitude");

}

void loop() {

  // start temp sensor code
  

  
  // read without samples.
  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(pinDHT11, &temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err="); Serial.println(err);delay(5000);
    return;
  }

  
  // casting to int and printing readings to screen
  temperature = (int)temperature;
  humidity = (int)humidity;
  
  //Serial.print(humidity);
  
  // DHT11 sampling rate is 1HZ.
  //delay(3000);

  // end temp sensor code

  // start SD code
/*
  file = SD.open("test2.txt", FILE_WRITE);
  if(file){
    file.print(temperature); file.print(", "); file.print(humidity); file.print("\n");
    file.close();
  }
  else{
    Serial.println("could not open file (writing)");
  }


  file = SD.open("test2.txt");
  if(file){
    Serial.println("--- Reading start ---");
    while(file.available()){
      Serial.write(file.read());
    }
    file.close();
    Serial.println("--- Reading end ---");
  }
  else{
    Serial.println("could not open file (reading)\n");
  }
*/
  // end SD code
  char status;
  double T,P,p0,a;

  // Loop here getting pressure readings every 10 seconds.

  // If you want sea-level-compensated pressure, as used in weather reports,
  // you will need to know the altitude at which your measurements are taken.
  // We're using a constant called ALTITUDE in this sketch:
  
  Serial.println();
  //Serial.print("provided altitude: ");
  //Serial.print(ALTITUDE,0);
  //Serial.print(" meters, ");
  //Serial.print(ALTITUDE*3.28084,0);
  //Serial.println(" feet");
  
  // If you want to measure altitude, and not pressure, you will instead need
  // to provide a known baseline pressure. This is shown at the end of the sketch.

  // You must first get a temperature measurement to perform a pressure reading.
  
  // Start a temperature measurement:
  // If request is successful, the number of ms to wait is returned.
  // If request is unsuccessful, 0 is returned.

  status = pressure.startTemperature();
  if (status != 0)
  {
    // Wait for the measurement to complete:
    delay(status);

    // Retrieve the completed temperature measurement:
    // Note that the measurement is stored in the variable T.
    // Function returns 1 if successful, 0 if failure.

    status = pressure.getTemperature(T);
    if (status != 0)
    {
      // Print out the measurement:
      //Serial.print("temperature: ");
      //Serial.print(T,2);
      //Serial.print(" deg C, ");
      Serial.print((9.0/5.0)*T+32.0,2);
      Serial.print("F\t\t");
      
      // Start a pressure measurement:
      // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
      // If request is successful, the number of ms to wait is returned.
      // If request is unsuccessful, 0 is returned.
      
      Serial.print(humidity);Serial.print("\t");
      
      status = pressure.startPressure(3);
      if (status != 0)
      {
        // Wait for the measurement to complete:
        delay(status);

        // Retrieve the completed pressure measurement:
        // Note that the measurement is stored in the variable P.
        // Note also that the function requires the previous temperature measurement (T).
        // (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
        // Function returns 1 if successful, 0 if failure.

        status = pressure.getPressure(P,T);
        if (status != 0)
        {
          // Print out the measurement:
          //Serial.print("absolute pressure: ");
         // Serial.print(P,2);
          //Serial.print(" mb, ");
          Serial.print(P*0.0295333727,2);
          Serial.print(" inHg\t");

          // The pressure sensor returns abolute pressure, which varies with altitude.
          // To remove the effects of altitude, use the sealevel function and your current altitude.
          // This number is commonly used in weather reports.
          // Parameters: P = absolute pressure in mb, ALTITUDE = current altitude in m.
          // Result: p0 = sea-level compensated pressure in mb

          p0 = pressure.sealevel(P,ALTITUDE); // we're at 1655 meters (Boulder, CO)
          //Serial.print("relative (sea-level) pressure: ");
          //Serial.print(p0,2);
          //Serial.print(" mb, ");
          //Serial.print(p0*0.0295333727,2);
          //Serial.println(" inHg");

          // On the other hand, if you want to determine your altitude from the pressure reading,
          // use the altitude function along with a baseline pressure (sea-level or other).
          // Parameters: P = absolute pressure in mb, p0 = baseline pressure in mb.
          // Result: a = altitude in m.

          a = pressure.altitude(P,p0);
          //Serial.print("computed altitude: ");
          Serial.print(a,0);
          Serial.print(" meters, ");
          Serial.print(a*3.28084,0);
          Serial.println(" feet");
        }
        else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error starting pressure measurement\n");
    }
    else Serial.println("error retrieving temperature measurement\n");
  }
  else Serial.println("error starting temperature measurement\n");

  
  
  delay(2000); // wait 5 seconds before loop starts again
    
 }
