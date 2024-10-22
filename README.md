# cp-iot-02
1. Introdução
Este projeto visa automatizar o controle de um ventilador com base na temperatura ambiente. A comunicação entre os dispositivos é feita através do protocolo MQTT, utilizando o broker Mosquitto. O sistema inclui uma placa Arduino conectada a um sensor DHT11 que coleta dados de temperatura e umidade, e uma placa ESP32 que controla um relé. A comunicação entre o Arduino e o broker MQTT é gerenciada através do Node-RED, que atua como um intermediário.

2. Componentes Utilizados
Placa Arduino Uno: Coleta os dados do sensor de temperatura e umidade DHT11.
Sensor DHT11: Mede a temperatura e a umidade do ambiente.
Node-RED: Gerencia o fluxo de comunicação entre o Arduino e o broker MQTT.
Placa ESP32 DevKit V1: Recebe os dados de temperatura do broker MQTT e controla o relé para ligar ou desligar o ventilador.
Relé JQC3F-05VDC-C: Controla o estado do ventilador (ligado/desligado).
Ventilador: O dispositivo controlado pelo relé.
Broker MQTT (Mosquitto): Gerencia a comunicação entre o Node-RED e a ESP32.

3. Arquitetura do Sistema
Fluxo de comunicação:
O Arduino Uno coleta a temperatura e a umidade usando o sensor DHT11.
Os dados são enviados do Arduino para o Node-RED através da comunicação serial.
O Node-RED processa os dados e publica no broker Mosquitto MQTT.
A ESP32 se conecta ao broker MQTT e escuta os dados de temperatura.
Com base nos dados de temperatura, a ESP32 ativa ou desativa o relé para controlar o ventilador.

Diagrama de Comunicação:
Arduino + DHT11 → Node-RED → Mosquitto Broker → ESP32 → Relé → Ventilador

5. Configuração no Node-RED
Fluxo Node-RED:
Entrada Serial: Recebe os dados do Arduino (porta serial).
Processamento de Dados: Converte os dados em um formato JSON adequado para envio via MQTT.
Saída MQTT: Publica os dados no tópico MQTT configurado.

Tópicos utilizados:
Tópico de temperatura: esp32/temperatura
Tópico de controle manual: esp32/control


7. Funcionamento e Lógica
O Arduino lê os dados do sensor DHT11 e envia para o Node-RED via comunicação serial.
O Node-RED processa os dados e publica no broker Mosquitto.
A ESP32 ouve o tópico MQTT e, quando a temperatura ultrapassa 25°C, ativa o relé para ligar o ventilador. Quando a temperatura cai abaixo de 25°C, o relé é desativado.


8. Conclusão
Este projeto integra Arduino, ESP32 e Node-RED para automatizar o controle de um ventilador. O Node-RED serve como middleware para processar os dados do Arduino e publicá-los no broker MQTT. O sistema pode ser expandido para incluir mais sensores e atuadores, oferecendo flexibilidade para aplicações IoT mais complexas.


