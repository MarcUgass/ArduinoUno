#include <SPI.h>
#include <Arduino.h>
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <Wire.h>

SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup() {
   Serial.begin(9600);
  while (!Serial);
  Serial.println("Adafruit Finger Print Sensor Test");

  if (finger.verifyPassword()) {
    Serial.println("Sensor de huella dactilar encontrado!");
  } else {
    Serial.println("Verificación de contraseña incorrecta");
  }

}

void loop() {
  Serial.println("Elija una opción:");
  Serial.println("1 - Alta de usuario");
  Serial.println("2 - Identificación de usuario");

  while (!Serial.available());

  char choice = Serial.read();

  if (choice == '1') {
    enrollUser();
  } else if (choice == '2') {
    identifyUser();
  } else {
    Serial.println("Opción no válida");
  }
}

void enrollUser() {
  Serial.println("Ingrese el nombre y apellidos del usuario:");

  while (!Serial.available());

  String userName = Serial.readString();
  userName.trim();

  Serial.println("Coloque el dedo en el sensor...");

  uint8_t id = 0;

  while (!finger.getImage());
  finger.image2Tz();
  id = finger.storeModel(0);

  if (id != FINGERPRINT_OK) {
    Serial.println("Error al almacenar la huella dactilar");
  } else {
    Serial.println("Huella dactilar almacenada exitosamente");
  }
}

void identifyUser() {
  Serial.println("Coloque el dedo en el sensor...");

  while (!finger.getImage());
  finger.image2Tz();

  int id = finger.fingerFastSearch();

  if (id == FINGERPRINT_OK) {
    Serial.println("Usuario reconocido");
    Serial.println("Bienvenido!");
  } else {
    Serial.println("Acceso no autorizado");
  }
}
