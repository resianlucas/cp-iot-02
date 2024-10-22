#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// Defina as credenciais Wi-Fi
const char* ssid = "SSID";        
const char* password = "PASSWORD";  

// Configurações do MQTT
const char* mqtt_server = "test.mosquitto.org";
const char* mqtt_topic_temp = "esp32/temperatura";  // Tópico para temperatura
const char* mqtt_topic_control = "esp32/control";   // Tópico para controle do botão
WiFiClient espClient;
PubSubClient client(espClient);

// Pino do relé
const int relayPin = 5;
bool controlByTemperature = true; // Estado do botão: controlar pela temperatura?

void setup() {
  Serial.begin(115200);
  
  // Configura o pino do relé
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW); // Inicialmente desligado

  // Conecta ao Wi-Fi
  setup_wifi();

  // Configura o cliente MQTT
  client.setServer(mqtt_server, 1883);
  client.setCallback(mqttCallback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando-se à rede Wi-Fi ");
  Serial.print(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Wi-Fi conectado");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop até que esteja conectado
  while (!client.connected()) {
    Serial.print("Tentando se conectar ao MQTT...");
    // Tenta se conectar sem autenticação
    if (client.connect("ESP32Client748653")) {
      Serial.println("conectado");
      // Inscreva-se nos tópicos
      client.subscribe(mqtt_topic_temp);      // Inscrever no tópico de temperatura
      client.subscribe(mqtt_topic_control);   // Inscrever no tópico de controle do botão
    } else {
      Serial.print("Falha na conexão, código de erro: ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String messageTemp;
  for (int i = 0; i < length; i++) {
    messageTemp += (char)payload[i];
  }

  // Verificar em qual tópico a mensagem foi recebida
  if (strcmp(topic, mqtt_topic_temp) == 0) {
    // Processar temperatura
    DynamicJsonDocument doc(1024);
    if (deserializeJson(doc, messageTemp) == DeserializationError::Ok) {
      float temperatura = doc["temperatura"]; // Obtém o valor da temperatura

      Serial.print("Temperatura recebida: ");
      Serial.println(temperatura);

      if (controlByTemperature) {
        // Controlar o relé pela temperatura
        if (temperatura < 25.0) {
          Serial.println("Temperatura abaixo do limite, desligando o relé");
          digitalWrite(relayPin, HIGH); // Desliga o relé
        } else {
          Serial.println("Temperatura acima do limite, ligando o relé");
          digitalWrite(relayPin, LOW); // Liga o relé
        }
      } else {
        Serial.println("Botão está desligado, mantendo o relé desligado");
        digitalWrite(relayPin, HIGH); // Relé sempre desligado quando o botão está off
      }
    } else {
      Serial.println("Falha ao fazer o parse do JSON");
    }

  } else if (strcmp(topic, mqtt_topic_control) == 0) {
    // Processar comando do botão
    if (messageTemp == "on") {
      controlByTemperature = true;
      Serial.println("Botão ligado: Controle de temperatura ativado.");
    } else if (messageTemp == "off") {
      controlByTemperature = false;
      Serial.println("Botão desligado: Relé desligado.");
      digitalWrite(relayPin, HIGH); // Desligar o relé imediatamente
    }
  }
}


