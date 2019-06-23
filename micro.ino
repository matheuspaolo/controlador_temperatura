 
#include <OneWire.h>
#include <DallasTemperature.h>
 
// Porta do pino de sinal do DS18B20
#define ONE_WIRE_BUS 3
int PIN_VALVE_1 = 6;
int PIN_VALVE_2 = 5;
int PIN_HOTASS = 7;
int alturaAgua = 0;

unsigned long time;
 
// Define uma instancia do oneWire para comunicacao com o sensor
OneWire oneWire(ONE_WIRE_BUS);

 
// Armazena temperaturas minima e maxima
float temperaturaLida = 0;
float temperaturaDesejada = 37;
String byteRecebido;
 
DallasTemperature sensors(&oneWire);
DeviceAddress sensor1;
 
void setup()
{

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

  alturaAgua = analogRead(A1);

  Serial.print(temperaturaLida);
  time = millis();
  Serial.print(',');
  Serial.print(time);
  Serial.print(',');
  Serial.println(alturaAgua);


  if(alturaAgua >= 15){
    digitalWrite(PIN_VALVE_2, LOW);
  }

  if(alturaAgua < 15){
    digitalWrite(PIN_VALVE_2, LOW);
  }


  if(temperaturaLida < temperaturaDesejada){
    digitalWrite(PIN_VALVE_1, HIGH);
    digitalWrite(PIN_HOTASS, LOW);
  }

  if(temperaturaLida > temperaturaDesejada){
    digitalWrite(PIN_VALVE_1, LOW);
    digitalWrite(PIN_HOTASS, HIGH);
  }


  delay(1000);
}
