
#include <WiFiEsp.h>
#include <WiFiEspClient.h>
#include <WiFiEspUdp.h>
#include "SoftwareSerial.h"
#include <PubSubClient.h>

#define mqtt_server "....." //ip of the server
#define mqtt_user "admin"
#define mqtt_password "admin"
#define your_topic "UFPA/Belem/Proficional/Itec/Bloco novo/A2/Comunicação/temperatura"
#define your_topic2 "UFPA/Belem/Proficional/Itec/Bloco novo/A2/Comunicação/luminosidade"
// analog input of arduino uno
const float LM35 = A0;
const float Ldr = A1;
float temp;// temperature variable
float lum; // luminosity variable
char ssid[] = "....";           // your network SSID (name)
char pass[] = "....";           // your network password
int status = WL_IDLE_STATUS;   // the Wifi radio's status

// Initialize the Ethernet client object
WiFiEspClient espClient;

PubSubClient client(espClient);

SoftwareSerial soft(2,3); // RX, TX
void setup() {
  // initialize serial for debugging
  Serial.begin(9600);
  // initialize serial for ESP module
  soft.begin(115200);
  // initialize ESP module
  WiFi.init(&soft);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  // you're connected now, so print out the data
  Serial.println("You're connected to the network");

  //connect to MQTT server
  client.setServer(mqtt_server, 1883);
  
}
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect, just a name to identify the client
    if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
      Serial.println("connected");
      
      
      
    } else {
      Serial.print("failed, reconect=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

long lastMsg = 0;


void loop() {
  // main code here run repeatedly:
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  long now = millis();

  if (now - lastMsg > 10000) {
    lastMsg = now;
    temp = ((analogRead(LM35))*5/(1023))/0.01;
    lum = (analogRead(Ldr));
    client.publish(your_topic, String( temp).c_str(), true);
    client.publish(your_topic2, String( lum).c_str(), true);
  }
}


 
