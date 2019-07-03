// declaração das bibliotecas necessárias
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
 
// declaração dos pinos e variáveis que serão utilizadas
#define ONE_WIRE_BUS 24
int PIN_VALVE_1 = 9;
int PIN_VALVE_2 = 8;
int PIN_HOTASS = 10;
int alturaAgua = 0;

const byte ROWS = 4;
const byte COLS = 4;
byte rowPins[ROWS] = { 44, 45, 46, 47 };
byte colPins[COLS] = { 48, 49, 50, 51 };
unsigned long time;

// variáveis necessárias para realizar operações com temperatura e tratar os dados do dispaly
float temperaturaLida = 0;
float temperaturaDesejada = 25;
char temp[6];
char customKey;
String byteRecebido;
int index;

char hexaKeys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
{ '4', '5', '6', 'B' },
{ '7', '8', '9', 'C' },
{ '*', '0', '#', 'D' }
};


LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3, POSITIVE); // criação do objeto lcd

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); // criação do objeto keypad


// Define uma instancia do oneWire para comunicacao com o sensor
OneWire oneWire(ONE_WIRE_BUS);

 

 
DallasTemperature sensors(&oneWire);
DeviceAddress sensor1;
 
void setup()
{

  //delay(3000);
  lcd.begin(16, 2);
  // definição e setagem de algumas portas utilizadas
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

  lcd.setBacklight(HIGH); // liga o backlight do display
  sensors.requestTemperatures(); // solicita ao sensor para medir a temperatura
  temperaturaLida = sensors.getTempC(sensor1); // armazena a temperatura (em celsius) em 'temperaturalida'

  alturaAgua = analogRead(A2); // faz a leitura de A2 para verificar se a água chegou ao limite do tanque

  // printa os dados obtidos para posterior análise dos mesmos
  Serial.print(temperaturaLida); 
  time = millis();
  Serial.print(',');
  Serial.print(time);
  Serial.print(',');
  Serial.println(alturaAgua);


  // mostra a temperatura desejada e a atual no display
  lcd.setCursor(0, 0);
  lcd.print("T. alvo: ");
  lcd.print(temperaturaDesejada);
  lcd.setCursor(0,1);
  lcd.print("T. atual: ");
  lcd.print(temperaturaLida);

  customKey = customKeypad.getKey();
  
  // trecho de código responsável pela mudança de temperatura quando o botão '*' é pressionado
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

  // caso o valor em 'alturaAgua' esteja acima do limite, ativa a válvula do tanque de baixo
  if(alturaAgua >= 15){
    digitalWrite(PIN_VALVE_2, LOW);
  }

  // caso o valor em 'alturaAgua' esteja abaixo do limite, ativa a válvula do tanque de baixo
  if(alturaAgua < 15){
    digitalWrite(PIN_VALVE_2, HIGH);
  }

  // se a temperatura lida for menor que a desejada, ativa o aquecedor e desliga a válvula do tanque superior
  if(temperaturaLida < temperaturaDesejada){
    digitalWrite(PIN_VALVE_1, HIGH);
    digitalWrite(PIN_HOTASS, LOW);
  }

  // se a temperatura lida for maior que a desejada, desliga o aquecedor e ativa a válvula do tanque superior
  if(temperaturaLida > temperaturaDesejada){
    digitalWrite(PIN_VALVE_1, LOW);
    digitalWrite(PIN_HOTASS, HIGH);
  }

  delay(50);
}
