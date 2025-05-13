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

  if (irrecv.decode(&results))
  {

    Serial.println("Código recebido:");
    Serial.println(results.value, HEX);
    // Serial.println(resultToHumanReadableBasic(&results));

    irrecv.resume();
  }

  bool currentButtonState = digitalRead(buttonPin);

  // Detecta quando botão é pressionado' (mudança de HIGH para LOW)
  if (lastButtonState == HIGH && currentButtonState == LOW)
  {
    digitalWrite(ledPin, HIGH);
    digitalWrite(buzzerPin, HIGH);

    unsigned long irCode = 0x20DF10EF;
    irsend.sendNEC(irCode, 32);         // Envia o código IR
    // irsend.sendKelon(0x3000007056, 48); // LIGA O AR CONDICIONADO
    // irsend.sendKelon(0x3000007156, 48); // AUMENTA O AR CONDICIONADO
    // irsend.sendKelon(0x3000007056, 48); // DIMINUI O AR CONDICIONADO
    // irsend.sendKelon(0x5200007456, 48); // MODO O AR CONDICIONADO
    // irsend.sendKelon(0x25200007456, 48); // OCILAR O AR CONDICIONADO

    String message = "Code sent: 0x" + String(irCode, HEX);
    Serial.println(message);
    digitalWrite(ledPin, LOW);
    digitalWrite(buzzerPin, LOW);
  }

  lastButtonState = currentButtonState;
}
