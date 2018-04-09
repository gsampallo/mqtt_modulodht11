#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT11.h>

const char* ssid = "wifi";
const char* password = "clave";
const char* mqtt_server = "198.41.30.241";

const char* keyDevice = "DHTMODULO1";

int devicePin = 2;

DHT11 dht11(devicePin); 

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  pinMode(devicePin, INPUT);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);  
}

void setup_wifi() {

  delay(10);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void callback(char* topic, byte* payload, unsigned int length) {

}

void reconnect() {

  while (!client.connected()) {
    if (client.connect("dht11modulo")) {
      client.publish("test", keyDevice);
      client.subscribe(keyDevice);
    } else {
      delay(5000);
    }
  }
}

int tellstate = 0;

void loop() {
  if (!client.connected()) {
    reconnect();
    //delay(5000);
    client.publish("casa", keyDevice);
  } 
  
  client.loop();

  if ( (millis() - tellstate) > 5000 ) {
    int err;
    float temp, humi;
    if((err=dht11.read(humi, temp))==0) {
      client.publish("temperatura",String(temp).c_str()); 
      client.publish("humedad",String(humi).c_str()); 
    }
    tellstate = millis();
  }
  delay(500);

}
