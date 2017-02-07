
#include <SPI.h>      
#include <Mirf.h>     
#include <nRF24L01.h> 
#include <MirfHardwareSpiDriver.h> 
typedef struct{
  int gaz;
  int refX;
  int refY;
  

}Cmd;

  Cmd msg;

int ancien_gaz = msg.gaz;
void setup() {
  Serial.begin(9600);
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  Mirf.cePin = 9; // Broche CE sur D9
  Mirf.csnPin = 10; // Broche CSN sur D10
  Mirf.spi = &MirfHardwareSpi; // On veut utiliser le port SPI hardware
  Mirf.init(); // Initialise la bibliothèque

  Mirf.channel = 1; // Choix du canal de communication (128 canaux disponibles, de 0 à 127)
  Mirf.payload = sizeof(Cmd); // Taille d'un message (maximum 32 octets)
  Mirf.config(); // Sauvegarde la configuration dans le module radio

  Mirf.setTADDR((byte *) "nrf01"); // Adresse de transmission
  Mirf.setRADDR((byte *) "nrf02"); // Adresse de réception
}

void loop() {
    msg.gaz = map(analogRead(A0),0,1023,0,100);
    msg.refX = map(analogRead(A1),0,1023,-90,90);
    msg.refY = map(analogRead(A2),0,1023,-90,90);

    Mirf.send((byte *)&msg);
    while(Mirf.isSending());
    Serial.println("TX");
    

}
