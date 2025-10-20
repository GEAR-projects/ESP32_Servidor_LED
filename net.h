#ifndef NET_H
#define NET_H

#include <WiFi.h>

/**
 * @brief Lida com a requisição de um cliente web.
 * Esta função lê a requisição HTTP do cliente, envia uma página HTML como resposta
 * e controla o pino do LED baseado na URL acessada.
 * A página HTML agora também exibe o estado atual do LED (LIGADO ou DESLIGADO).
 * @param client O objeto WiFiClient que representa a conexão com o cliente.
 * @param ledPin O número do pino do LED que será controlado.
 */
void handleClient(WiFiClient& client, int ledPin) {
  Serial.println("Novo Cliente.");
  String currentLine = "";       // Armazena uma linha da requisição HTTP
  
  while (client.connected()) {   // Fica em loop enquanto o cliente está conectado
    if (client.available()) {    // Se há bytes para ler do cliente,
      char c = client.read();    // leia um byte,
      Serial.write(c);           // e o imprima no monitor serial.
      
      // Se o byte for uma nova linha, processamos a linha que acabamos de receber
      if (c == '\n') {
        
        // Se a linha atual estiver em branco, significa que recebemos duas novas linhas seguidas.
        // Isso indica o fim da requisição HTTP do cliente, então podemos enviar uma resposta.
        if (currentLine.length() == 0) {
          
          String ledState;
          String ledStateColor;
          if (digitalRead(ledPin) == HIGH) {
            ledState = "LIGADO";
            ledStateColor = "#4CAF50"; // Verde
          } else {
            ledState = "DESLIGADO";
            ledStateColor = "#f44336"; // Vermelho
          }

          // O conteúdo do site é montado em uma string para incluir o estado atual do LED.
          String htmlContent = "<!DOCTYPE html><html>"
                               "<head><title>Controle de LED - ESP32</title>"
                               "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
                               "<meta charset=\"UTF-8\">"
                               "<link rel=\"icon\" href=\"data:,\">"
                               "<style>"
                               "html { font-family: Helvetica, Arial, sans-serif; display: inline-block; margin: 0px auto; text-align: center;}"
                               "body { margin-bottom: 40px; }"
                               "h1 { color: #0F3376; }"
                               "p { font-size: 1.5rem; }"
                               ".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px; "
                               "text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer; border-radius: 12px;}"
                               ".button2 {background-color: #f44336;}"
                               ".state-box { border: 2px solid #ccc; border-radius: 12px; padding: 10px; margin: 20px auto; max-width: 300px; }"
                               ".state { font-weight: bold; color: " + ledStateColor + "; }"
                               "footer { background-color: #f2f2f2; text-align: center; padding: 25px 0px 25px 0px; position: fixed; left: 0; bottom: 0; width: 100%; font-size: 0.9rem; color: #555; }"
                               "footer a { color: #0F3376; text-decoration: none; font-weight: bold; }"
                               "footer a:hover { text-decoration: underline; }"
                               "</style></head>"
                               "<body><h1>Servidor Web ESP32</h1>"
                               "<div class=\"state-box\">"
                               "<p>O LED está: <span class=\"state\">" + ledState + "</span></p>" // Exibe o estado
                               "</div>"
                               "<p><a href=\"/H\"><button class=\"button\">LIGAR</button></a></p>"
                               "<p><a href=\"/L\"><button class=\"button button2\">DESLIGAR</button></a></p>"
                               "<footer>"
                               "© 2025 | Desenvolvido por <a href=\"https://github.com/celiobenj\" target=\"_blank\">Célio Benjamim</a> para o projeto <a href=\"https://instagram.com/gear_est\" target=\"_blank\">GEAR</a>."
                               "</footer>"
                               "</body></html>";


          // Cabeçalhos HTTP
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html; charset=UTF-8");
          client.println("Connection: close");
          client.println();

          // Envia a página HTML para o navegador
          client.println(htmlContent);
          break; // Sai do loop while
        } else { // Se não for uma linha em branco,
          currentLine = ""; // limpa a variável para receber a próxima linha.
        }
      } else if (c != '\r') { // Se não for um caractere de retorno de carro,
        currentLine += c; // anexa o caractere à linha atual.
      }

      // Verifica se a requisição foi para LIGAR ou DESLIGAR o LED
      if (currentLine.endsWith("GET /H")) {
        digitalWrite(ledPin, HIGH);
      }
      if (currentLine.endsWith("GET /L")) {
        digitalWrite(ledPin, LOW);
      }
    }
  }
  
  // Fecha a conexão
  client.stop();
  Serial.println("Cliente Desconectado.");
}

#endif

