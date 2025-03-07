#include <WiFi.h>
#include "camera.h"
#include "http_post.h"
#include "camera_pins.h"

#define LED_PIN 4
#define SENSOR_PIN 13

bool estadoSensor = false;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(SENSOR_PIN, INPUT);

  if (!setupCamera()) {
    Serial.println("Falha ao inicializar a câmera!");
    while (true)
      ;
  }
  Serial.println("ESP32-CAM AI-Thinker iniciada com sucesso.");

  // Configuração do WiFi
  const char* ssid = "AP03";
  const char* password = "fabio123eu";
  WiFi.begin(ssid, password);
  WiFi.setSleep(false);

  Serial.print("WiFi connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

void loop() {
    int leitura = digitalRead(SENSOR_PIN);

    if (leitura == HIGH) {          // Movimento detectado
        digitalWrite(LED_PIN, HIGH); // Liga o LED

        if (!estadoSensor) {
            Serial.println("Movimento Detectado");
            estadoSensor = true;

            // Garante que uma nova foto seja capturada
            camera_fb_t* foto = nullptr;
            esp_camera_fb_return(foto); // Libera qualquer imagem anterior

            delay(500); // Pequeno delay para evitar captura corrompida

            foto = capturarFoto(); // Captura uma nova foto

            if (foto != nullptr) {
                Serial.println("Nova foto capturada! Enviando...");
                enviarFoto(foto); // Envia a nova foto
                esp_camera_fb_return(foto); // Libera a imagem da memória
            } else {
                Serial.println("Falha ao capturar nova foto.");
            }
        }

        delay(2000);                 // Mantém o LED ligado por 2 segundos
        digitalWrite(LED_PIN, LOW);  // Apaga o LED
        Serial.println("LED apagado");
        delay(10000);  // Aguarda 10 segundos antes de permitir outra detecção
        Serial.println("Aguardando nova detecção...");
        estadoSensor = false;
    }
}
