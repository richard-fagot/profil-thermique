// Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS D3

OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

void setup(void)
{
 // start serial port
 Serial.begin(9600);
 Serial.println("Dallas Temperature IC Control Library Demo");

 // Start up the library
 sensors.begin();
}

void loop(void)
{ 
 // call sensors.requestTemperatures() to issue a global temperature 
 // request to all devices on the bus
 Serial.print("Requesting temperatures...");
 sensors.requestTemperatures(); // Send the command to get temperatures
 Serial.println("DONE");
 // After we got the temperatures, we can print them here.
 // We use the function ByIndex, and as an example get the temperature from the first sensor only.
 Serial.print("Temperature for the device 1 (index 0) is: ");
 Serial.println(sensors.getTempCByIndex(0)); 
 delay(2000);
}
