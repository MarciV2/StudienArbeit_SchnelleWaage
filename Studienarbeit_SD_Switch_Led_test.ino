#include <Q2HX711.h>

const byte hx711_data_pin = 3;
const byte hx711_clock_pin = 2;

Q2HX711 hx711(hx711_data_pin, hx711_clock_pin);


void setup() {
  Serial.begin(9600);

}

void loop() {
   
    unsigned long wert=hx711.read();
  
    Serial.println(wert);
 
}
