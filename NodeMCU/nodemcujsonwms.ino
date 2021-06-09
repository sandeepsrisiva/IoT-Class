#include <ArduinoJson.h>

#include <DHT.h>
#include <DHT_U.h>

#include<ESP8266WiFi.h> // Nodemcu Library
#include<PubSubClient.h> // MQTT Library

#define ClientID "Nodemcu01" //ClientID Creation

#define dhtpin 5
#define dhttype DHT11

const char *ssid="WiFI_SSIS"; //WiFi Username
const char *password="WIFI_Password"; //WiFi Password
const char *mqttserver="MQTT SERVER"; //MQTT Server
int port=1883; // MQTT Port

char msg[30];
String mesg;
float temp,hum;

WiFiClient espclient; // WiFi client Creation
PubSubClient client(espclient); // Add WiFi client to MQTT Library
DHT dht(dhtpin,dhttype);

void setup() {

     dht.begin();
/****************WiFi Connection Start ********************/
  Serial.begin(9600);
  Serial.println("WiFi Connect with username: "+String(ssid));
  Serial.println("=================================");
  WiFi.begin(ssid,password);
  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print("******");
    delay(150);
  }
  Serial.print("\n WiFi Connected with IP: ");
  Serial.println(WiFi.localIP());

 /****************WiFi Connection End ********************/
 
  client.setServer(mqttserver,port); // Set the MQTT Server


}

void loop() {

  /***********MQTT Server reconnect**************/
  if(!client.connected())
  {
    while(!client.connected())
    {
      Serial.println("Attempting to MQTT Connection");
      if(client.connect(ClientID))
      {

        Serial.println("client Connected with "+String(ClientID));
        
      }
    }
    
        
  }




   temp=dht.readTemperature();
   hum=dht.readHumidity();
 /*** Json Data Creation***/
   StaticJsonDocument<64> doc;
   char output[64];

    doc["deviceid"] = "WMS/BB/R1";
    doc["temp"] = temp;
    doc["hum"] = hum;

    serializeJson(doc, output);

  /***** end the json data *****/  
    Serial.println(output);
    client.publish("wms",output);
    delay(5000);
  
   

  

}
