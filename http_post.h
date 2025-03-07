#ifndef HTTP_POST_H
#define HTTP_POST_H

#include <WiFi.h>
#include <HTTPClient.h>

const char* serverUrl = "http://192.168.15.42:8080/api/logs";

void enviarFoto(camera_fb_t* foto) {
    if (foto == nullptr) {
        Serial.println("Erro: Foto inválida.");
        return;
    }

    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi desconectado. Tentando reconectar...");
        WiFi.reconnect();
        delay(3000);
        if (WiFi.status() != WL_CONNECTED) {
            Serial.println("Falha ao reconectar. Cancelando envio.");
            return;
        }
    }

    HTTPClient http;
    WiFiClient client;

    Serial.println("Testando conexão com o servidor...");
    if (!client.connect("192.168.15.42", 8080)) { 
        Serial.println("Erro: Não foi possível conectar ao servidor!");
        return;
    }
    Serial.println("Conectado ao servidor!");

    Serial.println("Iniciando envio da imagem...");
    if (!http.begin(client, serverUrl)) {
        Serial.println("Falha ao iniciar a conexão HTTP.");
        return;
    }

    String boundary = "----ESP32Boundary";
    String head = "--" + boundary + "\r\n";
    head += "Content-Disposition: form-data; name=\"sensor_id\"\r\n\r\n1\r\n";
    head += "--" + boundary + "\r\n";
    head += "Content-Disposition: form-data; name=\"mensagem\"\r\n\r\nMovimento detectado\r\n";
    head += "--" + boundary + "\r\n";
    head += "Content-Disposition: form-data; name=\"imagem\"; filename=\"foto.jpg\"\r\n";
    head += "Content-Type: image/jpeg\r\n\r\n";

    String tail = "\r\n--" + boundary + "--\r\n";

    int contentLength = head.length() + foto->len + tail.length();
    Serial.printf("Tamanho total da requisição: %d bytes\n", contentLength);

    if (contentLength > 150000) {  
        Serial.println("Erro: Foto muito grande para envio!");
        esp_camera_fb_return(foto);
        return;
    }

    http.addHeader("Content-Type", "multipart/form-data; boundary=" + boundary);
    http.addHeader("Content-Length", String(contentLength));

    Serial.println("Enviando foto...");
    
    // Enviando a requisição completa em partes
    int httpResponseCode = http.POST(head + String((char*)foto->buf, foto->len) + tail);

    if (httpResponseCode > 0) {
        Serial.println("Resposta do servidor: " + http.getString());
    } else {
        Serial.printf("Erro ao enviar a imagem. Código HTTP: %d\n", httpResponseCode);
    }

    http.end();
    Serial.println("Envio finalizado.");
}

#endif
