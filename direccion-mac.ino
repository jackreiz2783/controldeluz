#include <Wire.h>
#include "SSD1306Wire.h"
#include "BluetoothSerial.h"

// Inicializar la pantalla OLED
SSD1306Wire display(0x3c, SDA, SCL);

// Crear una instancia de BluetoothSerial
BluetoothSerial SerialBT;

void setup() {
  // Inicializar la pantalla OLED
  display.init();
  display.setContrast(255);
  
  // Inicializar el Bluetooth
  SerialBT.begin("ESP32_Hidroponia"); // Nombre del dispositivo Bluetooth
  
  // Mostrar un mensaje de bienvenida en la pantalla OLED
  display.drawString(0, 0, "Bienvenido a Hidroponia");
  display.display();
  delay(2000);
  
  // Obtener la dirección MAC Bluetooth del ESP32
  uint8_t mac[6];
  SerialBT.getBtAddress(mac);
  
  // Convertir la dirección MAC a una cadena
  String macAddress = String(mac[0], HEX) + ":" +
                      String(mac[1], HEX) + ":" +
                      String(mac[2], HEX) + ":" +
                      String(mac[3], HEX) + ":" +
                      String(mac[4], HEX) + ":" +
                      String(mac[5], HEX);
  
  // Mostrar la dirección MAC en la pantalla OLED
  display.clear();
  display.drawString(0, 0, "Dirección MAC:");
  display.drawString(0, 16, macAddress);
  display.display();
}

void loop() {
  // Código principal del loop aquí
}
