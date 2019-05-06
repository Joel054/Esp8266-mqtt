//Ultrassonico comunicando com mqtt para mosquitto

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//Lib usada para o ultrassoico
#include <Ultrasonic.h> /*https://github.com/filipeflop/Ultrasonic */

//Define os pinos do ultrassonico para o trigger e echo
#define pino_trigger 2
#define pino_echo 0
//inicializa o sensor nos pinos definidos anteriormente
Ultrasonic ultrasonic(pino_trigger, pino_echo);

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
void UltrassonicSetup();
void UltrassonicLoop();

void setup(){
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println("Iniciando conexao com a rede WiFi..");
  }

  Serial.println("Conectado na rede WiFi!");
}

void loop(){
  //Faz a conexao com o broker MQTT
  reconectabroker();
  UltrassonicLoop();
}

void reconectabroker() {
  //Conexao ao broker MQTT
  client.setServer(mqttServer, mqttPort);
  while (!client.connected())
  {
    Serial.println("Conectando ao broker MQTT...");
    if (client.connect("ESP32Client", mqttUser, mqttPassword ))
    {
      Serial.println("Conectado ao broker!");
    }
    else
    {
      Serial.print("Falha na conexao ao broker - Estado: ");
      Serial.print(client.state());
      delay(1000);
    }
  }
}

void ULtrassonicSetup() {
  Ultrasonic ultrasonic(pino_trigger, pino_echo);
}
void UltrassonicLoop() {
  char topic[20], msg[100];

  //Le as informacoes do sensor em cm
  float cmMsec;
  //converte o tempo de resposta em cm
  long microsec = ultrasonic.timing();
  cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);

  sprintf (topic, "/r/h/e/%d", dev_id); //id referente ao id do canal
  ///coloca a string topic + conteudo da mensagem na string msg
  sprintf(msg,"%.2f",cmMsec);
  //sprintf(msg, "%.2f cm",cmMsec);
  Serial.print(msg);
  Serial.print("\n");
  client.publish(topic, msg); //mensagem que vai pro servidor
  //Exibe informacoes no serial monitor
  Serial.print("Distancia em cm: ");
  Serial.print(cmMsec);
  Serial.print("\n");
  delay(1000);
}

////defines - mapeamento de pinos do NodeMCU
////#define D0    16
////#define D1    5
////#define D2    4
////#define D3    0
////#define D4    2
////#define D5    14
////#define D6    12
////#define D7    13
////#define D8    15
////#define D9    3
////#define D10   1
