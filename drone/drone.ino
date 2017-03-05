#include <Gyro.h>
#include <Servo.h>
#include <SPI.h>      
#include <Mirf.h>     
#include <nRF24L01.h> 
#include <MirfHardwareSpiDriver.h> 

Servo moteur1;
Servo moteur2;
//Servo moteur3;
//Servo moteur4;

Gyro drone;

int vitesseM1;
int vitesseM2;
int vitesseM3;
int vitesseM4;

int refX = 0;
int refY = 4;

typedef struct{
  int gaz;
  int refX = 0;
  int refY = 0;

}Cmd;

Cmd msg;

int gaz = 50;
void setup() {
  Serial.begin(9600);
  drone.begin();


  delay(10);
  moteur2.attach(6);
  moteur1.attach(7);


  
  moteur1.write(40);
  
  moteur2.write(40);
  delay(5000);


  

 


  init_radio();
}

void loop() {
  int angleX = 0;
  int angleY;
  //int angleY = map(drone.getX(),0,180,-90,90) - refY;
 for(int i=0; i<3;i++){
    angleY += map(drone.getX(),0,180,-90,90) - refY;
    delayMicroseconds(1);
 }
  
angleY /= 3;

//conversion angle en RAD
double angleYrad = (angleY*PI)/180;
double angleXrad = (angleX*PI)/180;

//calcul des sinus des angles X et Y
double sinAngleXrad = sin(angleXrad);
double sinAngleYrad = sin(angleYrad);

Serial.println(sinAngleYrad);
vitesseM1 = gaz * map(0 + (sinAngleXrad*30) + (sinAngleYrad*30),-200,200,0,100);
vitesseM2 = gaz * map(0 + (sinAngleXrad*30) - (sinAngleYrad*30),-200,200,0,100);

//vitesseM3 = gaz * map(0 - angleX - angleY,-180,180,0,100);
//vitesseM4 = gaz * map(0 - angleX + angleY,-180,180,0,100);

moteur1.write(map(vitesseM1,0,10000,40,220));
moteur2.write(map(vitesseM2,0,10000,40,220));
//moteur3.write(map(vitesseM3,0,10000,0,180));
//moteur4.write(map(vitesseM4,0,10000,0,180));

if(Mirf.dataReady()){
  Mirf.getData((byte *) &msg);
  gaz = msg.gaz;
  //refX = msg.refX;
  //refY = msg.refY;
 // Serial.println(gaz);

  
}
//test_batterie();
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

/*void test_batterie(){
  double U = map(analogRead(A0),0,1023,0,2000);
  U /= 100;

  if(U <= 9.20){
    while(U <= 9.20){
      U = map(analogRead(A0),0,1023,0,2000);
      U /= 100;
    digitalWrite(A1,0);
    delay(100);
    digitalWrite(A1,1);
    delay(100);
    }
  }
  else digitalWrite(A1,1);
  
}*/


