#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <Arduino.h>
#include <WiFi.h>

const int ledPin = 22;
const int buttonPin = 5;
const int buzzerPin = 4;
const int recvPin = 23;
const int sendPin = 18;
bool lastButtonState = HIGH;

IRrecv irrecv(recvPin);
IRsend irsend(sendPin);
decode_results results;

unsigned long storedIRCode = 0;

void setup()
{
  Serial.begin(115200);

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  irrecv.enableIRIn();
  irsend.begin();

  Serial.println("Setup start");
}

void loop()
{

  if (irrecv.decode(&results) )
  {

    Serial.println("Código recebido:");
    Serial.println(results.value, HEX);

    if (results.value != 0xFFFFFFFF) {                               // Evita códigos de repetição
      storedIRCode = results.value; // Armazena o código
      Serial.print("Código armazenado: 0x");
      Serial.println(storedIRCode, HEX);
    }

    irrecv.resume();
  }

  bool currentButtonState = digitalRead(buttonPin);

  // Detecta quando botão é pressionado' (mudança de HIGH para LOW)
  if (lastButtonState == HIGH && currentButtonState == LOW && storedIRCode != 0xFFFFFFFF)
  {
    digitalWrite(ledPin, HIGH);
    digitalWrite(ledPin, HIGH);
    digitalWrite(buzzerPin, HIGH);

    if (storedIRCode != 0)
    {
      unsigned long irCode = storedIRCode;
      irsend.sendNEC(irCode, 32); // Envia o código IR armazenado
      String message = "Code sent: 0x" + String(irCode, HEX);
      Serial.println(message);

      digitalWrite(ledPin, LOW);
      digitalWrite(buzzerPin, LOW);

      delay(300);
    }
    else
    {
      Serial.println("Sem Código Armazenado!");
    }
  }

  lastButtonState = currentButtonState;
}
