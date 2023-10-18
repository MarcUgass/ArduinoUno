#include <Arduino.h>
String cadena;
int led = 13;
char* morse[] = {
  ".-",   // A
  "-...", // B
  "-.-.", // C
  "-..",  // D
  ".",    // E
  "..-.", // F
  "--.",  // G
  "....", // H
  "..",   // I
  ".---", // J
  "-.-",  // K
  ".-..", // L
  "--",   // M
  "-.",   // N
  "---",  // O
  ".--.", // P
  "--.-", // Q
  ".-.",  // R
  "...",  // S
  "-",    // T
  "..-",  // U
  "...-", // V
  ".--",  // W
  "-..-", // X
  "-.--", // Y
  "--.."  // Z
};

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
}

void loop() {
  if (Serial.available()) {
    cadena = Serial.readString();
    int len = cadena.length();
    if (len > 100) {
      Serial.println("Cadena demasiado larga");
      return;
    }
    Serial.println("Has escrito: ");
    Serial.println(cadena);
    for (int i = 0; i < len; i++) {
      Letra(cadena.charAt(i));
      delay(2000);
    }
  }
}

void Letra(char letra) {
  if (letra >= 'a' && letra <= 'z') { // minusucla
    char* m = morse[letra - 'a'];
    Led(m);
  } else if (letra >= 'A' && letra <= 'Z'){
    char* m = morse[letra - 'A'];
    Led(m);
  } else if (letra == ' '){
  }
}

void Led(char* m){
  for (int j = 0; m[j] != '\0'; j++) {
      if (m[j] == '.') {
        digitalWrite(led, HIGH);
        delay(500);
      } else if (m[j] == '-') {
        digitalWrite(led, HIGH);
        delay(1000);
      }
      digitalWrite(led, LOW);
      delay(1000);
    }
}