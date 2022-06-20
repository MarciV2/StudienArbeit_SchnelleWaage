#include <Q2HX711.h>

const byte hx711_data_pin = 3;
const byte hx711_clock_pin = 2;

Q2HX711 hx711(hx711_data_pin, hx711_clock_pin);

int switchPin=8;

//LED
int ledRed=6;
int ledGreen=5;
int ledBlue=7;

//Status-Beschreibungen:
//Grün:  Bereit zum Aufnehmen
//Blau:  Aufnahme läuft
//Rot:   Fehler

// SD-Card
#include <SPI.h>
#include <SdFat.h>
SdFat SD;
File myFile;
int millisecs=0;

//Time
#include <Time.h> 
#include <DS1307RTC.h>

boolean stateRecording=false;

void setLED(int red, int green, int blue){
  analogWrite(ledRed,   red);
  analogWrite(ledGreen, green);
  analogWrite(ledBlue,  blue);
}

void error(){
  setLED(255,0,0);
  Serial.println("ERROR!");
  while(1);
}

void setup() {
  Serial.begin(9600);
  pinMode(switchPin,INPUT_PULLUP);
  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledBlue, OUTPUT);

  setSyncProvider(RTC.get);


  //SD-Karte prüfen
  if (!SD.begin(10)){
    Serial.println("SD initialization failed!");
    error();
  }
  
  Serial.println("SD initialization done.");
  //Bereit für Aufnahme -> LED grün
  setLED(0,255,0);
  
}

void loop() {
if(digitalRead(switchPin)){
  if(!stateRecording)
  {
      stateRecording=true;
      //led auf blau
      setLED(0,0,255);

      millisecs=millis();
      
      String str="Messung_";
      str+=hour();
      str+="_";
      str+=minute();
      str+="_";
      str+=second();
      str+=".csv";
  
      myFile = SD.open(str, FILE_WRITE);
      Serial.print("Recording starting, filename: ");
      Serial.println(str);
      myFile.println("Zeit [ms];Abs. Messwert;Kal. Messwert");
  }

  
    //Datei beschreiben
  if(myFile){
      unsigned long wert=hx711.read();
      unsigned long wertCal=(wert-8297724)/1800;
      if(wertCal>2000000) wertCal=0;
      myFile.print(millis()-millisecs);
      myFile.print(";");
      myFile.print(wert);
      myFile.print(";");
      myFile.println(wertCal);
      Serial.print(wert);
      Serial.print(":");
      Serial.println(wertCal);

  }else{
      error();
  }

}else
{
  if(stateRecording)
    {
       stateRecording=false;
       setLED(0,255,0);
       myFile.flush();
       myFile.close();

        myFile = SD.open(".trash", FILE_WRITE);
        if(myFile)
          myFile.println(".");
          myFile.close();
       Serial.println("Recording stopping");
   }
  }
}
