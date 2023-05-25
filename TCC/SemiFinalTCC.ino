#include <LiquidCrystal.h>
#include <DHT.h>
#include <Servo.h>

#define DHTPIN 7
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
Servo servo1, servo2;

const int cooler1RelayPin = 22;
const int cooler2RelayPin = 24;
const int cooler3RelayPin = 26;
const int cooler4RelayPin = 28;
const int servo1Pin = 9;
const int servo2Pin = 8;

const float temperaturaIdeal = 28.0;
const float umidadeIdeal = 82;
const float IQAIdeal = 100.0;

int intervalo = 5000;
unsigned long ultimaAtualizacao = 0;
int secaoAtual = 0;

void setup() {
  Serial.begin(9600);
  dht.begin();
  lcd.begin(16, 2);
  pinMode(cooler1RelayPin, OUTPUT);
  pinMode(cooler2RelayPin, OUTPUT);
  pinMode(cooler3RelayPin, OUTPUT);
  pinMode(cooler4RelayPin, OUTPUT);
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
  
  lcd.setCursor(0, 0);
  lcd.print("Sensor MQ-135");
  lcd.setCursor(0, 1);
  lcd.print("e DHT22");
  delay(2000);
}

void loop() {
  float temperatura = dht.readTemperature();
  float umidade = dht.readHumidity();
  int valorMQ135 = analogRead(A0);





  
  // Controle dos coolers
  if (temperatura < 28.0) {
  digitalWrite(cooler1RelayPin, HIGH);
  digitalWrite(cooler2RelayPin, HIGH);
 
  } 
  
  else {
  digitalWrite(cooler1RelayPin, LOW);
  digitalWrite(cooler2RelayPin, LOW);

  }
  
  if (valorMQ135 < 100.0) {
  digitalWrite(cooler3RelayPin, HIGH);
  digitalWrite(cooler4RelayPin, HIGH);
 
  } 
  
  else {
  digitalWrite(cooler3RelayPin, LOW);
  digitalWrite(cooler4RelayPin, LOW);
  }

 
  // Controle do servo 1 (umidade)
if (umidade < umidadeIdeal && servo1.read() != 111) {
  servo1.write(110); // Gira para 90 graus se não estiver na posição desejada
  delay(1200);
  servo1.write(98);
  delay(300);
  servo1.write(111);
  
} else if (umidade >= umidadeIdeal && servo1.read() != 112) {
  servo1.write(110); // Gira para 90 graus se não estiver na posição desejada
  delay(1200);
  servo1.write(98);
  delay(300);
  servo1.write(110);
  delay(1200);
  servo1.write(98);
  delay(300);
  servo1.write(112);
}





  // Controle do servo 2 (janelas)
if (temperatura < temperaturaIdeal && servo2.read() != 90) {
  servo2.write(90); // Gira para 90 graus se não estiver na posição desejada
} else if (temperatura >= temperaturaIdeal && servo2.read() != 0) {
  servo2.write(0); // Gira para 0 grau se não estiver na posição desejada
}





  if (millis() - ultimaAtualizacao >= intervalo) {
    ultimaAtualizacao = millis();
    secaoAtual = (secaoAtual + 1) % 2;
    lcd.clear();
    lcd.setCursor(0, 0);
    
    switch (secaoAtual) {
      case 0:
        lcd.print("Temp: ");
        lcd.print(temperatura);
        lcd.print(" C");
        Serial.print("Temperatura: ");
        Serial.print(temperatura);
        Serial.println(" °C");
        lcd.setCursor(0, 1);
        lcd.print("Umidade: ");
        lcd.print(umidade);
        lcd.print(" %");
        Serial.print("Umidade: ");
        Serial.print(umidade);
        Serial.println(" %");
        break;
      case 1:
        lcd.print("IQA: ");
        lcd.print(valorMQ135);
        Serial.print("IQA: ");
        Serial.println(valorMQ135);
        break;
    }
  }
}
