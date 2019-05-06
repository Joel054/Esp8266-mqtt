//Leitura do DHT11 enviando os dados por mqtt para broker mosquitto
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//DTH -> sensor de umidade e temperatura
#include "DHT.h"
DHT dht;

const char* ssid = "Hackathon";
const char* password =  "hackathon";
const char* mqttServer = "192.168.0.13";
const int mqttPort = 1883;
const char* mqttUser = " ";
const char* mqttPassword = " ";

WiFiClient espClient;
PubSubClient client(espClient);

char mensagem[30];
//identificador do topico
#define dev_id 30

//prototipos
void dhtSetup();
void dhtLoop();

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println("Iniciando conexao com a rede WiFi..");
  }
  Serial.println("Conectado na rede WiFi!");
  dhtSetup();
}

void loop(){
  //Faz a conexao com o broker MQTT
  reconectabroker();
  dhtLoop();
  //Aguarda 5 segundos para enviar uma nova mensagem
  //delay(5000);
}

void reconectabroker() {
  //Conexao ao broker MQTT
  client.setServer(mqttServer, mqttPort);
  while (!client.connected()){
    Serial.println("Conectando ao broker MQTT...");
    if (client.connect("ESP32Client", mqttUser, mqttPassword )){
      Serial.println("Conectado ao broker!");
    }else{
      Serial.print("Falha na conexao ao broker - Estado: ");
      Serial.print(client.state());
      delay(1000);
    }
  }
}

void dhtSetup() {
  dht.setup(2);
}
void dhtLoop() {
  char topic[20], msg[100];
  float umidade = dht.getHumidity(); /* Get humidity value */
  float temperatura = dht.getTemperature(); /* Get temperature value */
  sprintf (topic, "/r/h/m/%d", dev_id); //TODO: trocar id
  Serial.print("umidade:");
  Serial.print(umidade);
  Serial.print("temperatura:");
  Serial.print(temperatura);
  Serial.print("\t");
  sprintf(msg, "%.1f c temperatura %.1f umidade ", temperatura, umidade);
  Serial.print(msg);
  Serial.print("\n");
  client.publish(topic, msg); //mensagem que vai pro servidor
  delay(2000);
}
