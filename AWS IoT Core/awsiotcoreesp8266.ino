#include<ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <time.h>
#include "secr.h"
#include<DHT.h>
#define emptyString String()
#define dhtpin 5
#define sensor DHT11


DHT dht(dhtpin,sensor);


const int MQTT_PORT = 8883;
const char MQTT_SUB_TOPIC[] = "$aws/things/" THINGNAME "/awstodev";
const char MQTT_PUB_TOPIC[] = "$aws/things/" THINGNAME "/devtoaws";

#ifdef USE_SUMMER_TIME_DST
uint8_t DST = 1;
#else
uint8_t DST = 0;
#endif

WiFiClientSecure espclient;

#ifdef ESP8266
BearSSL::X509List cert(cacert);
BearSSL::X509List client_crt(client_cert);
BearSSL::PrivateKey key(privkey);
#endif

PubSubClient client(espclient);
unsigned long lastMillis = 0;
time_t now;
time_t nowish = 1510592825;

void NTPConnect()
{
  Serial.print("Setting time using SNTP");
  configTime(TIME_ZONE * 3600, DST * 3600, "pool.ntp.org", "time.nist.gov");
  now = time(nullptr);
  while (now < nowish)
  {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("done!");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));
}

void messageReceived(char *topic, byte* payload, unsigned int length)
{
  Serial.print("Received [");
  Serial.print(topic);
  Serial.print("]: ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  StaticJsonDocument<256> doc;

DeserializationError error = deserializeJson(doc,payload, length);

if (error) {
  Serial.print(F("deserializeJson() failed: "));
  Serial.println(error.f_str());
  return;
}

const char* deviceid = doc["deviceid"];
const char* status =doc["status"];
Serial.println(deviceid);
Serial.println(status);
String st=String(status);
if(st=="on")
{
  digitalWrite(16,0);
  Serial.println("LED ON");
  sendData("Light ON");
}
if(st=="off")
{
  digitalWrite(16,1);
  Serial.println("LED OFF");
  sendData("Light OFF");
}

}



void connectToMqtt(bool nonBlocking = false)
{
  Serial.print("MQTT connecting ");
  while (!client.connected())
  {
    if (client.connect(THINGNAME))
    {
      Serial.println("connected!");
      if (!client.subscribe(MQTT_SUB_TOPIC))
        pubSubErr(client.state());
    }
    else
    {
      Serial.print("failed, reason -> ");
      pubSubErr(client.state());
      if (!nonBlocking)
      {
        Serial.println(" < try again in 5 seconds");
        delay(5000);
      }
      else
      {
        Serial.println(" <");
      }
    }
    if (nonBlocking)
      break;
  }
}


void connectToWiFi(String init_str)
{
  if(init_str!=emptyString)
  {
    Serial.print(init_str);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }
    Serial.print("\n IP:");
    Serial.println(WiFi.localIP());
  }
}
void checkWiFiThenMQTT(void)
{
  connectToWiFi("Checking WiFi");
  connectToMqtt();
}
void sendData(String str)
{
//  float temp,hum;
//  temp=dht.readTemperature();
//  hum=dht.readHumidity();
  StaticJsonDocument<96> doc;

  doc["deviceid"] = "nodemcu";
  doc["stautus"] = str;
//  doc["temp"] = temp;
//  doc["hum"] = hum;
  char output[96];
  serializeJson(doc, output);
  Serial.println(output);
  if (!client.publish(MQTT_PUB_TOPIC, output, false))
    pubSubErr(client.state());
  
}


void pubSubErr(int8_t MQTTErr)
{
  if (MQTTErr == MQTT_CONNECTION_TIMEOUT)
    Serial.print("Connection tiemout");
  else if (MQTTErr == MQTT_CONNECTION_LOST)
    Serial.print("Connection lost");
  else if (MQTTErr == MQTT_CONNECT_FAILED)
    Serial.print("Connect failed");
  else if (MQTTErr == MQTT_DISCONNECTED)
    Serial.print("Disconnected");
  else if (MQTTErr == MQTT_CONNECTED)
    Serial.print("Connected");
  else if (MQTTErr == MQTT_CONNECT_BAD_PROTOCOL)
    Serial.print("Connect bad protocol");
  else if (MQTTErr == MQTT_CONNECT_BAD_CLIENT_ID)
    Serial.print("Connect bad Client-ID");
  else if (MQTTErr == MQTT_CONNECT_UNAVAILABLE)
    Serial.print("Connect unavailable");
  else if (MQTTErr == MQTT_CONNECT_BAD_CREDENTIALS)
    Serial.print("Connect bad credentials");
  else if (MQTTErr == MQTT_CONNECT_UNAUTHORIZED)
    Serial.print("Connect unauthorized");
}
void setup() {
  pinMode(16,OUTPUT);
  Serial.begin(9600);
  dht.begin();
  delay(2000);
  WiFi.hostname(THINGNAME);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  connectToWiFi(String("Attempting to connect to SSID: ") + String(ssid));
  NTPConnect();
  espclient.setTrustAnchors(&cert);
  espclient.setClientRSACert(&client_crt, &key);
  client.setServer(MQTT_HOST, MQTT_PORT);
  client.setCallback(messageReceived);
  connectToMqtt();
}

void loop() {
  now = time(nullptr);
  if (!client.connected())
  {
    checkWiFiThenMQTT();
    
  }
  else
  {
    client.loop();
//    if (millis() - lastMillis > 5000)
//    {
//      lastMillis = millis();
//      sendData();
//    }
  }
  

}
