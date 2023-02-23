// Thesis project 2021-2022 code by Phan Quoc Trung (Designing Water Quality Monitoring System Through IOT)
// Fill-in information from your Blynk Template here
#define BLYNK_TEMPLATE_ID "TMPL268CJ-Ep"
#define BLYNK_DEVICE_NAME "Water quality"

#define BLYNK_FIRMWARE_VERSION        "0.1.0"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

#define APP_DEBUG
#define USE_NODE_MCU_BOARD
#define SensorPin A0            //pH meter Analog output to Arduino Analog Input 0
#define Offset 0.3            //deviation compensate PH

#include "BlynkEdgent.h"
#include "OneWire.h" 
#include "DallasTemperature.h"
#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);
unsigned long int avgValue;     //Store the average value of the sensor feedback
int watertemp;

void setup()
{
  Serial.begin(115200);
  delay(100);
  sensors.begin();
  BlynkEdgent.begin();
}

void loop() {
  BlynkEdgent.run();
//Collect Temperature data
 sensors.requestTemperatures();
 Serial.print("Temperature is: ");
 Serial.print(sensors.getTempCByIndex(0)); 
 watertemp = sensors.getTempCByIndex(0);
 Blynk.virtualWrite(V1,watertemp); 
 delay(1000);

//Collect Ph Value data
 int buf[10];                //buffer for read analog
  for(int i=0;i<10;i++)       //Get 10 sample value from the sensor for smooth the value
  { 
    buf[i]=analogRead(SensorPin);
    delay(10);
  }
  for(int i=0;i<9;i++)        //sort the analog from small to large
  {
    for(int j=i+1;j<10;j++)
    {
      if(buf[i]>buf[j])
      {
        int temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  avgValue=0;
  for(int i=2;i<8;i++)                      //take the average value of 6 center sample
    avgValue+=buf[i];
  float phValue=(float)avgValue*5.0/1024/6; //convert the analog into millivolt
  phValue=2.0*phValue+Offset;                      //convert the millivolt into pH value  
  Blynk.virtualWrite(V2,phValue);
  Serial.print("    pH:");  
  Serial.print(phValue,2);
  Serial.println(" ");       
  delay(1000);
}
