#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Keypad.h>
 
// Porta do pino de sinal do DS18B20
#define ONE_WIRE_BUS 24
int PIN_VALVE_1 = 9;
int PIN_VALVE_2 = 8;
int PIN_HOTASS = 10;
int alturaAgua = 0;

const byte ROWS = 4;
const byte COLS = 4;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

char hexaKeys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
{ '4', '5', '6', 'B' },
{ '7', '8', '9', 'C' },
{ '*', '0', '#', 'D' }
};

byte rowPins[ROWS] = { 44, 45, 46, 47 };
byte colPins[COLS] = { 48, 49, 50, 51 };

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

unsigned long time;
 
// Define uma instancia do oneWire para comunicacao com o sensor
OneWire oneWire(ONE_WIRE_BUS);

 
// Armazena temperaturas minima e maxima
float temperaturaLida = 0;
float temperaturaDesejada = 25;
char temp[6];
char customKey;
String byteRecebido;
int index;
 
DallasTemperature sensors(&oneWire);
DeviceAddress sensor1;
 
void setup()
{

  delay(3000);
  lcd.begin(16, 2);

  pinMode(PIN_VALVE_1,OUTPUT);
  pinMode(PIN_VALVE_2,OUTPUT);
  pinMode(PIN_HOTASS,OUTPUT);

  digitalWrite(PIN_VALVE_1, HIGH);
  digitalWrite(PIN_VALVE_2, HIGH);
  digitalWrite(PIN_HOTASS, HIGH);

  
  Serial.begin(9600);
  sensors.begin();
  // Localiza e mostra enderecos dos sensores
  Serial.println("Localizando sensores DS18B20...");
  Serial.print("Foram encontrados ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" sensores.");
  if (!sensors.getAddress(sensor1, 0)) 
     Serial.println("Sensores nao encontrados !"); 
  Serial.println();
  Serial.println();
}
 
 
void loop()
{
  sensors.requestTemperatures();
  temperaturaLida = sensors.getTempC(sensor1);

  alturaAgua = analogRead(A2);

  
  Serial.print(temperaturaLida);
  time = millis();
  Serial.print(',');
  Serial.print(time);
  Serial.print(',');
  Serial.println(alturaAgua);


  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T. alvo: ");
  lcd.print(temperaturaDesejada);
  lcd.setCursor(0,1);
  lcd.print("T. atual: ");
  lcd.print(temperaturaLida);

  customKey = customKeypad.getKey();
  if (customKey == '*') {
    
    Serial.println("Botão de mudança de temperatura acionado");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Insira: ");
    index = 0;
    //;
    customKey = customKeypad.getKey();

    while (customKey != '#') {
      customKey = customKeypad.getKey();

      if (customKey && customKey != '#') {
            //Serial.println(customKey);
            temp[index] = customKey;
            lcd.print(customKey);
            //Serial.print(customKey);
            index++;
            if (index == 2)
            {
              temp[index] = ".";
              lcd.print(".");
              Serial.print(".");
            }
          }
    }

    temperaturaDesejada = atof (temp);
    temperaturaDesejada = temperaturaDesejada/100;

    Serial.println(" ");
    Serial.println("-------------- TEMPERATURA ALTERADA --------------");
    Serial.println(temperaturaDesejada);
    Serial.println(" ");

    time = 0;

  }


  if(alturaAgua >= 15){
    digitalWrite(PIN_VALVE_2, LOW);
  }

  if(alturaAgua < 15){
    digitalWrite(PIN_VALVE_2, HIGH);
  }


  if(temperaturaLida < temperaturaDesejada){
    digitalWrite(PIN_VALVE_1, HIGH);
    digitalWrite(PIN_HOTASS, LOW);
  }

  if(temperaturaLida > temperaturaDesejada){
    digitalWrite(PIN_VALVE_1, LOW);
    digitalWrite(PIN_HOTASS, HIGH);
  }

  //Serial.println(temperaturaDesejada);
  delay(50);
}
