#include <SD.h>

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
int pinDHT11 = A2;

SimpleDHT11 dht11;
// end instance fields for DHT11

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

}

void loop() {

  // start temp sensor code
  
  // start working...
  Serial.println("=================================");
  Serial.println("Temp and Humidity reading:");
  
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
  
  
  
  // DHT11 sampling rate is 1HZ.
  delay(3000);

  // end temp sensor code

  // start SD code

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

  // end SD code
    
  
  
  delay(5000); // wait 5 seconds before loop starts again
    
 }

