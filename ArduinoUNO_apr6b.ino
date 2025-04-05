#include <OneWire.h>
#include <DallasTemperature.h>
#define SensorPin A0
#define ONE_WIRE_BUS 7
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

int buf[10],temp;
unsigned long int avgValue;
String valueString = "";
String PH_value = ""; 
char buff2[10];


void setup() {
  Serial.begin(9600);
  sensors.begin();

}

void loop() {
  // Read temperature
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);
  PH_Value();

  Serial.print("Ph:");
  //Serial.println(analogRead(A0));
  //Serial.println("Ph:12.53,temp:21.50");
  Serial.print(PH_value);
  Serial.print(",temp:");
  Serial.println(tempC);
  delay(1000);

}



void PH_Value()
{
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
        temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  avgValue=0;
  for(int i=2;i<8;i++)                      //take the average value of 6 center sample
  avgValue+=buf[i];
  float phValue=(float)avgValue*3.3/1024/6; //convert the analog into millivolt
  phValue=((3.5*phValue + 0.4) - 14) * -1;                      //convert the millivolt into pH value
  
  PH_value =  dtostrf(phValue, 4, 2, buff2);  //4 is mininum width, 6 is precision
  //Serial.println(PH_value );
  valueString = "";

}

