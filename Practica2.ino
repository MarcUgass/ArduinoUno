#include <SPI.h>
#include <Arduino.h>
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <Wire.h>

SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

String userName = "";
uint8_t id = 0;

void setup() {
  Serial.begin(9600);
  //while (!Serial);
  Serial.println("Inicio de programa");

  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("Sensor de huella dactilar encontrado!");
  } else {
    Serial.println("Sensor de huella dactilar no encontrado.");
    while(1);
  }

}

void loop() {
  Serial.println("Elija una opción:");
  Serial.println("1 - Registro de usuario");
  Serial.println("2 - Reconocimiento de usuario");

  while (!Serial.available());

  char opcion = Serial.read();

  if (opcion == '1') {
    registro();
  } else if (opcion == '2') {
    reconocimiento();
  } else {
    Serial.println("Opción no válida");
  }
}

void registro() {
  Serial.println("Ingrese el nombre y apellidos del usuario:");

  while (!Serial.available());

  userName = Serial.readString();
  userName.trim();

  int p = -1;
  Serial.print("Esperando validar usuario con el id: #"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      break;
    case FINGERPRINT_NOFINGER:
      break;
    default:
      Serial.println("Error");
      break;
    }
  }

    p = finger.image2Tz(1);
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Usuario registrado!");
      break;
    default:
      Serial.println("Error");
      return p;
  }
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Guardado!");
  } else {
    Serial.println("Error");
    return p;
  }
  id +=1;
  
}

void reconocimiento() {
  Serial.println("Coloque el dedo en el sensor...");
  delay(2000);
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("Huella dactilar no encontrada.");
      return p;
    default:
      Serial.println("Error");
      return p;
  }

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      break;
    default:
      Serial.println("Error");
      return p;
  }

  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Se ha encontrado una coincidencia!");
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("No se ha encontrado una coincidencia!");
    return p;
  } else {
    Serial.println("Error");
    return p;
  }

  Serial.println("Bienvenido " + userName +"!"); //Serial.print(finger.fingerID);
}