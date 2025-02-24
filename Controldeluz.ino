#include <WiFi.h>
#include <WebServer.h>  // Cambié ESP32WebServer.h por WebServer.h

// Configura tu red Wi-Fi
const char *ssid = "tuSSID";
const char *password = "tuContraseñaWiFi";

// Pin del LED (o la luz que quieras controlar)
const int ledPin = 2;  // Usa el pin que tengas conectado al LED

// Crea una instancia del servidor web
WebServer server(80);  // Usa el puerto 80 por defecto

void setup() {
  // Inicia el puerto serie
  Serial.begin(115200);
  
  // Conéctate a la red Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando al Wi-Fi...");
  }
  Serial.println("Conectado al Wi-Fi");

  // Imprime la IP del ESP32
  Serial.print("Dirección IP del ESP32: ");
  Serial.println(WiFi.localIP());
  
  // Configura el pin del LED como salida
  pinMode(ledPin, OUTPUT);
  
  // Ruta para ajustar la intensidad del LED
  server.on("/set_brightness", HTTP_GET, []() {
    if (server.hasArg("intensity")) {
      int intensity = server.arg("intensity").toInt();  // Obtén el valor de intensidad
      intensity = constrain(intensity, 0, 255);  // Asegúrate de que esté entre 0 y 255
      analogWrite(ledPin, intensity);  // Ajusta el brillo del LED
      server.send(200, "text/plain", "Intensidad del LED ajustada a: " + String(intensity));
    } else {
      server.send(400, "text/plain", "Falta el parámetro 'intensity'");
    }
  });
  
  // Inicia el servidor
  server.begin();
}

void loop() {
  server.handleClient();  // Procesa las solicitudes HTTP
}
