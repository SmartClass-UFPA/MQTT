#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define wifi_ssid "ssid"  // wifi network name
#define wifi_password "password"  

#define mqtt_server "host/ip" // ip of the machine you are using
#define mqtt_user "admin"
#define mqtt_password "admin"

#define your_topic "UFPA/Belem/Proficional/Itec/Bloco novo/A2/Comunicação/temperatura"
// variables to the topic
int valor lido = 0;
float temp = 0;
// setting the client 
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200); //initialize the seial of the board used in the work velocity
  setup_wifi();
  client.setServer(mqtt_server, 1883);// setting the machine to became the server (machine ip , broker port)
  
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);
  WiFi.begin(wifi_ssid, wifi_password);
// in the case of failed conection this function will print points
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    // If you do not want to use a username and password, change next line to
    // if (client.connect("ESP8266Client")) {
    if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
// time counter
long lastMsg = 0;
int cont = 0;

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();

  if (now - lastMsg > 10000) {
    lastMsg = now;
// pino A0 is the only analogic read , with a 3.2 v max voltage
    valor Lido = analogRead(A0);
    temp =  (valor lido * (3,3/1023))*100
// publish in the form (topic , payload,
    client.publish(your_topic, String(temp).c_str(), true);

  }
}
