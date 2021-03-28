const int flexPin = A0; //pin A0 to read analog input
#include <Ethernet.h>
#include <SPI.h>
#include <UbidotsEthernet.h>
#include <dht.h>

int fan_state=0;
int light_state=0;

dht DHT;

#define TEMPID  "5912e50376254212ae6115dc"
#define HUMID  "5912e59676254212ae612312"
#define LIGHTID  "5912e4f976254212a8e24c5a"
#define FANCID "5912e52276254212b4b210b8"
#define LIGHTCID  "5912e53076254212a60853f3"
#define TFID "5912e5d376254212a8e25fb8"
#define LEDID "5912e5cc76254212a6086210"
#define MCID "5916a4a276254269c54081d1"
//#define MCID2 "5912e6db76254212b4b23348"

#define TOKEN  "CSSxcAHjhepIFTVj6uszEUazlrIaU8"

const int analogIn1 = A3;
const int analogIn2 = A4;
int mVperAmp =100; // use 100 for 20A Module and 66 for 30A Module
int RawValue1= 0;
int RawValue2= 0;
int ACSoffset = 2500; 
double Voltage1 = 0;
double Amps1 = 0;
double Voltage2 = 0;
double Amps2 = 0;

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 0, 177);

Ubidots client(TOKEN);
 
int value;
#define fanRelay 7
#define lightRelay 8
 
int flexcount = 0;
int control = 0;
 
 
void setup(){
  Serial.begin(9600);     
  pinMode(lightRelay,OUTPUT);
  pinMode(fanRelay,OUTPUT);
/*  if (Ethernet.begin(mac) == 0)
    {
    Serial.println("Failed to configure Ethernet using DHCP");
    Ethernet.begin(mac, ip);
    } */
  
}
 
void loop(){
    
     int ldr=analogRead(A1);
    Serial.print("LDR = ");
    Serial.print(ldr);
        DHT.read11(A2);
    Serial.print("\nTemperature = ");
    Serial.print(DHT.temperature);
    
RawValue1 = analogRead(analogIn1);
Voltage1 = (RawValue1 / 1023.0) * 5000; // Gets you mV
Amps1 = ((Voltage1 - ACSoffset) / mVperAmp);
Amps1 = ((128*Amps1)+293.28)/24.4 +0.171;
RawValue2 = analogRead(analogIn2);
Voltage2 = (RawValue2 / 1023.0) * 5000; // Gets you mV
Amps2 = ((Voltage2 - ACSoffset) / mVperAmp);
Amps2 = ((128*Amps2)+293.28)/24.4 +0.171;

fan_state=digitalRead(fanRelay);
light_state=digitalRead(lightRelay);

if(light_state==1)
{
Serial.print("Raw Value(Light) = " ); // shows pre-scaled value 
Serial.print(RawValue1); 
Serial.print("\t mV = "); // shows the voltage measured 
Serial.print(Voltage1,3); // the '3' after voltage allows you to display 3 digits after decimal point
Serial.print("\t mAmps = "); // shows the voltage measured 
Serial.println(Amps1,3); // the '3' after voltage allows you to display 3 digits after decimal point
}
else
{
Serial.print("Raw Value(Light) = 0" ); // shows pre-scaled value 
Voltage1=0;
Serial.print("\t mV = 0"); // shows the voltage measured 
Amps1=0;
Serial.print("\t mAmps = 0"); // shows the voltage measured 

}
if(fan_state==1)
{
Serial.print("Raw Value(Fan) = " ); // shows pre-scaled value 
Serial.print(RawValue2); 
Serial.print("\t mV = "); // shows the voltage measured 
Serial.print(Voltage2,3); // the '3' after voltage allows you to display 3 digits after decimal point
Serial.print("\t mAmps = "); // shows the voltage measured 
Serial.println(Amps2,3); 
}
else
{
 Serial.print("Raw Value(Fan) = 0" ); // shows pre-scaled value 
Voltage2=0;
//Serial.print(RawValue1); 
Serial.print("\t mV = 0"); // shows the voltage measured 
//Serial.print(Voltage1,3);
Amps2=0;// the '3' after voltage allows you to display 3 digits after decimal point
Serial.print("\t mAmps = 0"); // shows the voltage measured 
//Serial.println(Amps1,3); // the '3' after voltage allows you to display 3 digits after decimal point   
}
   // delay(2000);
    float humidity = DHT.humidity;
    float temperature = DHT.temperature;
    client.add(TEMPID, temperature);
    client.add(LIGHTID, ldr);
    client.add(FANCID, Amps2);
    client.add(LIGHTCID, Amps1);
    client.add(HUMID, humidity);
    client.sendAll();
    
    int tempfanstatus = client.getValue(TFID);
    int ledstatus = client.getValue(LEDID);
    Serial.print("Fan Status :");
    Serial.println(fan_state);
    Serial.print("Light Status :");
    Serial.println(light_state);


int sw=client.getValue(MCID);
  Serial.print("mcstatus =");
  Serial.println(sw);

  if(sw == 0)
  {   
  value = analogRead(flexPin);         //Read and save analog value from potentiometer
  Serial.print("\nFlex Sensor = ");
  Serial.print(value);                //Print value
 
    if(value>650)
    {
     flexcount++;
    }     

    if(flexcount%2==1)
    {
      digitalWrite(lightRelay,HIGH);
      digitalWrite(fanRelay,HIGH); 
      control=1;
    }
    if(flexcount%2==0)
    {
      
      digitalWrite(lightRelay,LOW);
      digitalWrite(fanRelay,LOW);
      control=0;
    }
   


    
   /* int pir=digitalRead(2);
    if(pir==1)
    {
      Serial.println("\nMotion Detected");
    }
    else if(pir==0)
    {
      Serial.println("\nNo Motion Detected");
    } */




    if(control==0)
    {
   
             if(DHT.temperature>35)
              {
                Serial.print("     Temperature is High");
                digitalWrite(fanRelay,HIGH);
              }
             else
              {
               Serial.println("      Temperature is Low");
               digitalWrite(fanRelay,LOW);
              }

             if(ldr<500)
              {
               Serial.println("    Light Available");
               digitalWrite(lightRelay,LOW);
              }
             else
             {
              Serial.println("    No Light");
              digitalWrite(lightRelay,HIGH);
             }
    }
       client.add(TFID, fan_state);
    client.add(LEDID, light_state);
    client.sendAll();

  }
  else if(sw==1)
  {
    if(tempfanstatus==1)
    {
      digitalWrite(fanRelay,HIGH);
    }
    else
    {
      digitalWrite(fanRelay,LOW);
    }
    if(ledstatus==1)
    {
      digitalWrite(lightRelay,HIGH);
    }
    else
    {
      digitalWrite(lightRelay,LOW);
    }
  } 
  delay(2000);
} 
