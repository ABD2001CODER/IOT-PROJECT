#include <Servo.h>
#include <SoftwareSerial.h>// import the serial library
#include <dht11.h> // DHT11 temperature and humidity sesnor 
Servo servo;
SoftwareSerial bluetooth(0, 1); //RX, TX
//#include "Arduino_SensorKit.h"

#define cap_soil_sensorPin A4
#define res_soil_sensorPin A0
#define water_level_sensorPin A1
#define fet_level_sensorPin A3
#define DHT11PIN 12

dht11 DHT11;


const int RELAY1_PIN = 8;
const int RELAY2_PIN = 5;

//cap_soil
const int cap_AirValue = 590;   //you need to replace this value with Value_1
const int cap_WaterValue = 445;  //you need to replace this value with Value_2
int cap_soilMoistureValue = 0;
int cap_soilmoisturepercent=0;

//res_soil
const int res_AirValue = 1024;   //you need to replace this value with Value_1
const int res_WaterValue = 400;  //you need to replace this value with Value_2
int res_soilMoistureValue = 0;
int res_soilmoisturepercent=0;

const int water_level_min = 0;   //you need to replace this value with Value_1
const int water_level_max = 570;  //you need to replace this value with Value_2
int water_level_reading = 0;
int water_level_percent=0;

const int fet_level_min = 0;   //you need to replace this value with Value_1
const int fet_level_max = 510;  //you need to replace this value with Value_2
int fet_level_reading = 0;
int fet_level_percent=0;

int res_setpoint = 60;
int cap_setpoint = 50;
int water_level_setpoint = 40;
int fet_level_setpoint_A = 5;
int fet_level_setpoint_B = 2;
int fet_level_setpoint_C = 2;

const int bouton = 1; // le bouton est connecté à la broche 1 de la carte Adruino
const int relais_pompe = 2; // // le relais est connecté à la broche 2 de la carte Adruino
int etatBouton;

char data = 0; //Variable for storing received data


void setup() {
  // put your setup code here, to run once:
  	
  Serial.begin(9600);
  pinMode(relais_pompe, OUTPUT);
  servo.attach(7);
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);
  digitalWrite(RELAY2_PIN , HIGH);
  pinMode(7, OUTPUT);
  
  

  
  delay(2000);



}

void loop() {
  all_reading();
  int reading ;
 if(Serial.available() > 0)  // Send data only when you receive data:  
   {  //Serial.println("hi bro");
      reading = Serial.read();
  // read the blutooth input
    if( reading == '0'){
      servo.write(0);
      Serial.println("0");
      feeld_A(1);
      delay(1000);

    }else if(reading == '1'){
      servo.write(0);
      Serial.println("1");
      feeld_A(2);
      delay(1000);

    }else if(reading == '2'){
      servo.write(90);
      Serial.println("2");
      feeld_B(1);
      delay(1000);
 
    }else if(reading  == '3'){
      servo.write(90);
      Serial.println("3");
      feeld_B(2);
      delay(1000);

    
    }else if(reading  == '4'){
      Serial.println("4");
      servo.write(180);
      feeld_C(1);
      delay(1000);
    
    }else if(reading == '5'){
      Serial.println("5");
      servo.write(180);
      feeld_C(2);
      delay(1000);
    
    }
  }
  Serial.println("******************");
  delay(5000);



}

int cap_soil(){
  cap_soilMoistureValue = analogRead(cap_soil_sensorPin);  //put Sensor insert into soil
 // Serial.println(cap_soilMoistureValue);
  cap_soilmoisturepercent = map(cap_soilMoistureValue, cap_WaterValue, cap_AirValue, 0, 100);
 // Serial.println(cap_soilmoisturepercent);
  return(cap_soilmoisturepercent);


}
int res_soil(){
  res_soilMoistureValue = analogRead(res_soil_sensorPin);  //put Sensor insert into soil
 // Serial.println(res_soilMoistureValue);
  res_soilmoisturepercent = map(res_soilMoistureValue,res_WaterValue, res_AirValue,  0, 100);
  //Serial.println(res_soilmoisturepercent);
  return(res_soilmoisturepercent);

  
}
int water_level(){
  water_level_reading = analogRead(water_level_sensorPin);  //put Sensor insert into soil
 // Serial.println(water_level_reading);
  water_level_percent = map(water_level_reading, water_level_min, water_level_max, 0, 100);
 // Serial.println(water_level_percent);
  return(water_level_percent);
}

int fet_level(){
  int current_fet_level_reading = analogRead(fet_level_sensorPin);  //put Sensor insert into soil
 // Serial.println(fet_level_reading);
  fet_level_percent = map(current_fet_level_reading, fet_level_min, fet_level_max, 0, 100);
 // Serial.println(fet_level_percent);
  return(fet_level_percent);
}

void temp_hemidity(){

  Serial.println();

  int chk = DHT11.read(DHT11PIN);

  Serial.print("Humidity (%): ");
  Serial.println((float)DHT11.humidity, 2);

  Serial.print("Temperature  (C): ");
  Serial.println((float)DHT11.temperature, 2);

  delay(2000);

  
}

void bluetooth_read(){
   if(Serial.available() > 0)  // Send data only when you receive data:  
   {  
      data = Serial.read();        //Read the  incoming  data and store it into variable data  
      Serial.print(data);
      return (data);
   }


}
void bluetooth_write(int input){

  bluetooth.print(input);
  bluetooth.print(";");
  Serial.println(input);
  delay(20);


}

void all_reading(){
  int cap_soilmoisturepercent = cap_soil();
  bluetooth.print("feeld B soil moisture persentage = ");
  bluetooth.print(cap_soilmoisturepercent);
  bluetooth.print(";");
  Serial.print("feeld B soil moisture persentage = ");
  Serial.println(cap_soilmoisturepercent);

  int  res_soilmoisturepercent = res_soil();
  bluetooth.print("feeld c soil moisture persentage = ");
  bluetooth.print(res_soilmoisturepercent);
  bluetooth.print(";");
  Serial.print("feeld c soil moisture persentage = ");
  Serial.println(res_soilmoisturepercent);


  int  water_level_percent = water_level();
  bluetooth.print("feeld A Water level persentage = ");
  bluetooth.print(water_level_percent);
  bluetooth.print(";");
  Serial.print("feeld A Water level persentage = ");
  Serial.println(water_level_percent);

  int fet_level_percent = fet_level();
  bluetooth.print("fertilizer level persentage = ");
  bluetooth.print(fet_level_percent);
  bluetooth.print(";");
  Serial.print("fertilizer level persentage = ");
  Serial.println(fet_level_percent);

  temp_hemidity();
  bluetooth.print(";");


}

void feeld_A(int motor_num){
  
  water_level_percent = water_level();
  if(motor_num == 1){
    digitalWrite(RELAY1_PIN , HIGH);
    while(water_level_percent <= water_level_setpoint){ 
      delay(100);
      water_level_percent = water_level();
       delay(100);
    }
    delay(5000);
    digitalWrite(RELAY1_PIN , LOW);

  }else if(motor_num == 2){
    int current_fet_level_percent = fet_level();
    digitalWrite(RELAY2_PIN , LOW);
    int new_fet_level_percent;
    int gap;
    while(gap <= fet_level_setpoint_A){  
      new_fet_level_percent = fet_level();
      gap = new_fet_level_percent - current_fet_level_percent;
      delay(500);
    } 
    digitalWrite(RELAY2_PIN , HIGH);

  }
  
}

void feeld_B(int motor_num){
  
  int cap_soilmoisturepercent = cap_soil();
  if(motor_num == 1){
    digitalWrite(RELAY1_PIN , HIGH);
    while(cap_soilmoisturepercent <= cap_setpoint){  
      cap_soilmoisturepercent = cap_soil();
      delay(500);
    }
    digitalWrite(RELAY1_PIN , LOW);

  }else if(motor_num == 2){
    int current_fet_level_percent = fet_level();
    digitalWrite(RELAY2_PIN , LOW);
    int new_fet_level_percent;
    int gap;
    while(gap <= fet_level_setpoint_B){  
    new_fet_level_percent = fet_level();
    gap = new_fet_level_percent - current_fet_level_percent;
    delay(500);
    } 
    digitalWrite(RELAY2_PIN , HIGH);

  }
  
}

void feeld_C(int motor_num){
  
  int  res_soilmoisturepercent = res_soil();
  if(motor_num == 1){
    digitalWrite(RELAY1_PIN , HIGH);
    while(res_soilmoisturepercent >= res_setpoint){  
      res_soilmoisturepercent = res_soil();
      delay(500);
    }
    digitalWrite(RELAY1_PIN , LOW);

  }else if(motor_num == 2){
    int current_fet_level_percent = fet_level();
    digitalWrite(RELAY2_PIN , LOW);
    int new_fet_level_percent;
    int gap;
    while(gap <= fet_level_setpoint_C){  
    new_fet_level_percent = fet_level();
    gap = new_fet_level_percent - current_fet_level_percent;
    delay(1000);
    } 
    digitalWrite(RELAY2_PIN , HIGH);

  }
  
}


/*void motor(){
  etatBouton = digitalRead(bouton);
  if (etatBouton == 0) // We press the button
  {
    digitalWrite(relais_pompe, HIGH); // the water pump fills the bottle
  }else{
    digitalWrite(relais_pompe, LOW); // the water pump stops
  }
}*/