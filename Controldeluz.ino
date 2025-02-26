#include <Wire.h>
#include <WiFi.h>
#include <WebServer.h>
#include "SSD1306Wire.h"        // Librería para la pantalla OLED con I2C

// Definir el pin del relé (en lugar de una bomba)
const int relayPin = 5; // Cambia esto al pin que estás usando para controlar la luz

// Inicializar la pantalla OLED con la dirección I2C 0x3C
SSD1306Wire display(0x3C, SDA, SCL);

// Configuración de WiFi
const char *ssid = "RED WIFI"; // Nombre de tu red WiFi
const char *password = "CONTRASEÑA DE LA RED WIFI"; // Contraseña de tu red WiFi

// Configuración de IP estática
IPAddress local_ip(192, 168, 1, 100);   // Dirección IP estática
IPAddress gateway(192, 168, 1, 1);      // Dirección IP del gateway (router)
IPAddress subnet(255, 255, 255, 0);     // Máscara de subred

// Crear un servidor web en el puerto 80
WebServer server(80);

// Variables para la intensidad de la luz (PWM)
int lightIntensity = 0;  // Valor inicial de la intensidad de la luz (0 a 255)

// Ruta principal (pantalla de control)
void handleRoot() {
  String html = "<html><body>";
  html += "<h1>Control de Luz</h1>";
  html += "<p>Intensidad de la Luz: <input type='range' id='intensidad' min='0' max='255' value='" + String(lightIntensity) + "' onchange='enviar()'/></p>";
  html += "<p>Intensidad: <span id='valor'>" + String(lightIntensity) + "</span></p>";
  html += "<button onclick='enviar()'>Actualizar</button>";
  html += "<script>";
  html += "function enviar() {";
  html += "  var intensidad = document.getElementById('intensidad').value;";
  html += "  document.getElementById('valor').innerText = intensidad;";
  html += "  var url = '/update?intensidad=' + intensidad;";
  html += "  fetch(url);";
  html += "}"; 
  html += "</script>";
  html += "</body></html>";
  
  server.send(200, "text/html", html);
}

// Ruta para actualizar la intensidad de la luz
void handleUpdate() {
  if (server.hasArg("intensidad")) {
    lightIntensity = server.arg("intensidad").toInt();  // Obtener el valor de la intensidad desde la URL
    analogWrite(relayPin, lightIntensity);  // Ajustar la intensidad de la luz (PWM)
    
    String message = "Intensidad de la luz: " + String(lightIntensity) + "<br>";
    server.send(200, "text/html", message);  // Confirmar la actualización en la página web
  } else {
    server.send(400, "text/html", "Faltan parámetros.");
  }
}

void setup() {
  Serial.begin(9600); // Inicialización de la conexión en serie para la depuración

  // Configurar el pin del relé como salida (PWM)
  pinMode(relayPin, OUTPUT);

  // Inicializar la pantalla OLED
  display.init();
  display.setContrast(255);  // Ajustar el contraste de la pantalla
  display.clear();           // Limpiar la pantalla

  // Mostrar un mensaje inicial en la pantalla OLED
  display.drawString(0, 0, "Conectando...");
  display.display();

  // Conectar el ESP32 a la red WiFi con IP estática
  WiFi.config(local_ip, gateway, subnet);  // Configuración de la IP estática
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Conectado a WiFi");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());  // Imprimir la IP local del ESP32

  // Mostrar la dirección IP en la pantalla OLED
  display.clear();
  display.drawString(0, 0, "WiFi Conectado");
  display.drawString(0, 10, "IP: " + WiFi.localIP().toString());
  display.display();

  // Configurar las rutas del servidor web
  server.on("/", handleRoot);       // Ruta principal
  server.on("/update", handleUpdate);  // Ruta para actualizar la intensidad
  server.begin();
}

void loop() {
  server.handleClient();  // Manejar las solicitudes del cliente

  // Mostrar el valor de la intensidad de la luz en la pantalla OLED
  display.clear();
  display.drawString(0, 0, "Intensidad: " + String(lightIntensity));
  display.display();
}

