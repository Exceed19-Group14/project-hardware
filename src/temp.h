#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 13

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
void setup(void)
{
  // start serial port
  Serial.begin(115200);
  sensors.begin(); 
}
void loop(void)
{ 
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures();
  Serial.println("DONE");
  Serial.print("Temperature for Device 1 is: ");
  Serial.print(sensors.getTempCByIndex(0)); 
  }