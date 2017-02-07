#include <Servo.h>

#include <SPI.h>      
#include <Mirf.h>     
#include <nRF24L01.h> 
#include <MirfHardwareSpiDriver.h> 



typedef struct{
  int angle;  
  int angle2;
  int angle3;
}Cmd;

Cmd msg;
Servo myservo;
Servo myservo2;
Servo myservo3;
int ancien_angle = 0; 
int ancien_angle2 = 0;
int ancien_angle3 = 0;
void setup() {
  pinMode(3,OUTPUT);
  pinMode(7,INPUT);
  Mirf.cePin = 9; // Broche CE sur D9
  Mirf.csnPin = 10; // Broche CSN sur D10
  Mirf.spi = &MirfHardwareSpi; // On veut utiliser le port SPI hardware
  Mirf.init(); // Initialise la bibliothèque

  Mirf.channel = 1; // Choix du canal de communication (128 canaux disponibles, de 0 à 127)
  Mirf.payload = sizeof(Cmd); // Taille d'un message (maximum 32 octets)
  Mirf.config(); // Sauvegarde la configuration dans le module radio

  Mirf.setTADDR((byte *) "nrf02"); // Adresse de transmission
  Mirf.setRADDR((byte *) "nrf01"); // Adresse de réception
  Serial.begin(9600);

  Serial.println("start");

  myservo.attach(6);
  myservo2.attach(5);
  myservo3.attach(4);

  myservo.write(0);
  myservo2.write(0);
  myservo3.write(0);
  
}

void loop() {
  
while(Mirf.dataReady()){
  Mirf.getData((byte *) &msg);
if(msg.angle != ancien_angle){
    Serial.println(msg.angle);
    ancien_angle = msg.angle;
    
    myservo.write(ancien_angle);
  }
  if(msg.angle2 != ancien_angle2){
    Serial.println(msg.angle2);
    ancien_angle2 = msg.angle2;
    
    myservo2.write(ancien_angle2);
  }
  if(msg.angle3 != ancien_angle3){
    Serial.println(msg.angle3);
    ancien_angle3 = msg.angle3;
    
    myservo3.write(ancien_angle3);
  }
  }



}


