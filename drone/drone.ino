#include <Gyro.h>
#include <Servo.h>
#include <SPI.h>      
#include <Mirf.h>     
#include <nRF24L01.h> 
#include <MirfHardwareSpiDriver.h> 

Servo moteur1;
Servo moteur2;
Servo moteur3;
Servo moteur4;

Gyro drone;

int vitesseM1;
int vitesseM2;
int vitesseM3;
int vitesseM4;

int refX = 0;
int refY = 0;

typedef struct{
  int gaz;
  int refX;
  int refY;

}Cmd;

Cmd msg;

int gaz;
void setup() {
  Serial.begin(9600);
  drone.begin();
  moteur1.attach(7);
  moteur2.attach(6);
  moteur3.attach(5);
  moteur4.attach(4);

  moteur1.write(0);
  moteur2.write(0);
  moteur3.write(0);
  moteur4.write(0);
  delay(1000);

  init_radio();
}

void loop() {
int angleX = map(drone.getX(),0,180,-90,90) - refX;
int angleY = map(drone.getY(),0,180,-90,90) - refY;
  
vitesseM1 = gaz * map(0 + angleX + angleY,-180,180,0,150);
vitesseM2 = gaz * map(0 + angleX - angleY,-180,180,0,150);
vitesseM3 = gaz * map(0 - angleX - angleY,-180,180,0,150);
vitesseM4 = gaz * map(0 - angleX + angleY,-180,180,0,150);

moteur1.write(map(vitesseM1,0,10000,0,255));
moteur2.write(map(vitesseM2,0,10000,0,255));
moteur3.write(map(vitesseM3,0,10000,0,255));
moteur4.write(map(vitesseM4,0,10000,0,255));

if(Mirf.dataReady()){
  Serial.println("RX");
  Mirf.getData((byte *) &msg);
  gaz = msg.gaz;
  refX = msg.refX;
  refY = msg.refY;
  
}
}

void init_radio(){

  Mirf.cePin = 9; // Broche CE sur D9
  Mirf.csnPin = 10; // Broche CSN sur D10
  Mirf.spi = &MirfHardwareSpi; // On veut utiliser le port SPI hardware
  Mirf.init(); // Initialise la bibliothèque

  Mirf.channel = 1; // Choix du canal de communication (128 canaux disponibles, de 0 à 127)
  Mirf.payload = sizeof(Cmd); // Taille d'un message (maximum 32 octets)
  Mirf.config(); // Sauvegarde la configuration dans le module radio

  Mirf.setTADDR((byte *) "nrf02"); // Adresse de transmission
  Mirf.setRADDR((byte *) "nrf01"); // Adresse de réception
}


