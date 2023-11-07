#include <SPI.h>
#include <MFRC522.h>
#include <Arduino.h>

#define RST_PIN 9
#define SS_PIN 10
#define LONGITUD 16

int bloque;
MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

void setup() {
    Serial.begin(9600);
    while(!Serial);
    SPI.begin();
    mfrc522.PCD_Init();
    for (byte i = 0; i < 6; i++)
    {
    key.keyByte[i] = 0xFF;
    }
    Serial.println(F(""));
    Serial.println(F("Elija una opción:"));
    Serial.println(F("1. Leer tarjeta"));
    Serial.println(F("2. Escribir bloque"));
    Serial.println(F("3. Leer información de un bloque"));
    Serial.println(F("4. Salir"));
}

void loop() {
    if (Serial.available()) {
        while (!Serial.available());
        char opcion = Serial.read();
        bool resultado = true;
        switch (opcion) {
            case '1':
                resultado = leerTarjeta();
                if(resultado)
                {
                  printf("Leido con éxito");
                }
                break;
                //solo pide UID

            case '2':
                escribirBloque();
                break;
            
            case '3':
                leerBloque();
                break;

            case '4':
              Serial.println(F("Saliendo del programa."));
                while (true) { 
                  //mantener el programa en espera
                }
                break;

            default:
                Serial.println(F("Opción no válida. Elija 1 para leer tarjeta, 2 para escribir, 3 para leer un bloque o 4 para salir."));
                break;
        }
    }
}

bool leerTarjeta() {
  delay(2000);
    if (!mfrc522.PICC_IsNewCardPresent()) {
        Serial.println(F("No se detecta ninguna tarjeta."));
        return false;
    }

    if (!mfrc522.PICC_ReadCardSerial()) {
        Serial.println(F("No se pudo leer la tarjeta."));
        return false;
    }
    
    mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
}

bool escribirBloque() {
  // primero saber la uid de la trajeta, comprobar que la tarjeta se encuentra en contacto, cuando sepa que esta en contacto,
  // ya buscar los bloques i tal, supongo que se tiene que hacer dentro de un while, en plan que si no se detcta la tarjeta uid, se salga
  if (!mfrc522.PICC_IsNewCardPresent()) {
    Serial.println("No se detecta ninguna tarjeta.");
    return;
  }
  if (!mfrc522.PICC_ReadCardSerial()) {
    Serial.println("No se pudo leer la tarjeta.");
    return;
  }

  Serial.println("");
  Serial.println("Ingrese un número de bloque (0-63): ");
  //byte bloque = Serial.readBytesUntil('',)
  
  while (!Serial.available());
  int bloque = Serial.parseInt();

  if (bloque < 0 || bloque > 63) {
    Serial.println("Número de bloque no válido. Debe estar entre 0 y 63.");
    return;
  }

  Serial.println("Ingrese una cadena (máximo 16 caracteres): ");
  while (!Serial.available());
  String cadena = Serial.readString();
  cadena.trim();  // Eliminar espacios en blanco al principio y al final
  if (cadena.length() > 16) {
    Serial.println("La cadena debe tener como máximo 16 caracteres.");
    return;
  }

  byte buferEscritura[LONGITUD];

  for (uint8_t i = 0; i < LONGITUD; i++) {
    buferEscritura[i] = 0; // Inicializa el búfer con ceros
  }
  cadena.getBytes(buferEscritura, LONGITUD);

  MFRC522::StatusCode estado;
  estado = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, bloque, &key, &(mfrc522.uid));
  if (estado != MFRC522::STATUS_OK) {
    Serial.println("Error autenticando");
    Serial.println(mfrc522.GetStatusCodeName(estado));
    return;
  }

  estado = mfrc522.MIFARE_Write(bloque, buferEscritura, LONGITUD);
  if (estado != MFRC522::STATUS_OK) {
    Serial.println("Error escribiendo bloque");
    Serial.println(mfrc522.GetStatusCodeName(estado));
    return;
  }
  Serial.println("Escrito con éxito");
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  return;
}

void leerBloque() {
  delay(2000);
    if (!mfrc522.PICC_IsNewCardPresent()) {
        Serial.println(F("Coloque una tarjeta en el lector."));
        return;
    }

    if (!mfrc522.PICC_ReadCardSerial()) {
        Serial.println(F("No se pudo leer la tarjeta."));
        return;
    }

    Serial.println(F("Escriba el bloque que desea leer (0-63):"));
    int bloque = -1;

    while (!Serial.available());
    bloque = Serial.parseInt();

    char datosLeidos[16];
    if (mfrc522.MIFARE_Read(bloque, (byte*)datosLeidos, 16)) {
        Serial.println(F("Datos leídos del bloque:"));
        int len = strlen(datosLeidos);
        for (int i = 0; i < len; i++) {
          if (datosLeidos[i] != ' '){
            Serial.print(datosLeidos[i]);
          }
        }
    } else {
        Serial.println(F("Error al leer los datos del bloque."));
    }
}
