/*
  WiFi Web Server LED Blink

  Um servidor web simples que permite você a piscar o LED interno da ESP32.
  Um endereço de IP será escrito no Monitor Serial, acesse esse endereço em
  seu navegador e lá você poderá controlar o LED conectado no pino 2
  (LED interno) da ESP32.

  Este exemplo foi feito para facilitar o entendimento. A lógica de
  rede está no arquivo 'net.h'.

 created for arduino 25 Nov 2012
 by Tom Igoe

 ported for GEAR
 18.10.2025 by Célio Benjamim
*/
#include <WiFi.h>
#include "net.h" // Inclui a lógica de rede

#define SSID "GEAR_IOT"
#define SENHA "gear_est"
#define LED_PIN 2

// Cria o servidor na porta 80
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT); // Define o pino do LED

  // Conecta-se à rede WiFi
  Serial.print("\nConectando a ");
  Serial.println(SSID);

  WiFi.begin(SSID, SENHA);

  while (WiFi.status() != WL_CONNECTED) { // Tenta se conectar a cada 500ms
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi conectado.");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());

  server.begin(); // Inicia o servidor
}

void loop() {
  WiFiClient client = server.available(); // Escuta por clientes que acessam o endereço

  if (client) { // Se um novo cliente se conectar,
    handleClient(client, LED_PIN); // chama a função do nosso header file para lidar com ele.
  }
}
