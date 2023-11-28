#include "affiche.h"
#include <Wire.h>


void setEclairage(int adress,int valeur){
  Wire.beginTransmission(adress);
  if (valeur==1){
    Wire.write(0xFD);//activation r�tro �clairage
  }
  else {
    Wire.write(0xF9);//pas de r�tro �clairage
  }
  Wire.endTransmission();
}

void initAfficheur(int adress){
                                //DB7  DB6  DB5  DB4  DB3  DB2  DB1  DB0
  envoyerCommande(adress,0x35); // 0    0    1    1    0    1    0    1 :fonction set=> H=1(extended instruction set)
  envoyerCommande(adress,0xA0); // 1    0    1    0    0    0    0    0 :set Vlcd=>V=0 (set Va) et voltage=32 ; imp�ratif de choisir Va 
  envoyerCommande(adress,0x34); // 0    0    1    1    0    1    0    0 :fonction set=> DL=1(bus 8 bits), M=1(2 lignes de 16c), SL=0, H=0
  envoyerCommande(adress,0x06); // 0    0    0    0    0    1    1    0 :entry mode set=> I/D=1(increment cursor),S=0(cursor move)  
  envoyerCommande(adress,0x02); // 0    0    0    0    0    0    1    0 :return home=> mise � 0 de l'adresse de la DDRAM
  effacerAfficheur(adress);
  activerAfficheur(adress);
}

void activerAfficheur(int adress){
  envoyerCommande(adress,0x0f); // 0    0    0    0    1    1    1    1 :display control=>D=1(display on),C=1(cursor on),B=1(cursor caractere blink on)
}

void desactiverAfficheur(int adress){
  envoyerCommande(adress,0x0b); // 0    0    0    0    1    0    1    1 :display control=>D=1(display on),C=1(cursor on),B=1(cursor caractere blink on)
}

void effacerAfficheur(int adress){
  int i=0;
  desactiverAfficheur(adress);
  envoyerCommande(adress,0x02); // 0    0    0    0    0    0    1    0 :return home=> mise � 0 de l'adresse de la DDRAM
  Wire.beginTransmission(adress);
  Wire.write(0x40);
  for (i=0;i<38;i++){
    Wire.write(0xa0);
  }
  Wire.endTransmission();
  
  envoyerCommande(adress,0xc0); // 1    1    0   0    0    0    0    0 :set DDRAM=> mise � 0x40 de l'adresse de la DDRAM
  Wire.beginTransmission(adress);
  Wire.write(0x40);
  for (i=0;i<38;i++){
    Wire.write(0xa0);
  }
  Wire.endTransmission();
  envoyerCommande(adress,0x02); // 0    0    0    0    0    0    1    0 :return home=> mise � 0 de l'adresse de la DDRAM
  activerAfficheur(adress);
}
    
void envoyerCommande(int adresse,int valeur){

  Wire.beginTransmission(adresse);
  Wire.write(0x00);
  Wire.write(valeur);
  Wire.endTransmission();
}

void envoyerDonnee(int adresse,int valeur){

  Wire.beginTransmission(adresse);
  Wire.write(0x40);
  Wire.write(valeur);
  Wire.endTransmission();
}

void envoyerMessage(int adress,const char* maChaine, int ligne){
  int i=0;
  int c;
  desactiverAfficheur(adress);
  if (ligne==LIGNE1){
    envoyerCommande(adress,0x02); // 0    0    0    0    0    0    1    0 :return home=> mise � 0 de l'adresse de la DDRAM
   Wire.beginTransmission(adress);
   Wire.write(0x40);
   while(*(maChaine+i)!='\0'){
      c=(int)(*(maChaine+i))+0x80;
      Wire.write(c);
      i++;
    }
   Wire.endTransmission();
  }
  else if (ligne==LIGNE2){
    envoyerCommande(adress,0xC0); // 1    1    0    0    0    0    0    0 :set DDRAM=> va � l'adresse 40 de la DDRAM (2eme ligne)
    Wire.beginTransmission(adress);
    Wire.write(0x40);
    while(*(maChaine+i)!='\0'){
       c=(int)(*(maChaine+i))+0x80;
       Wire.write(c);
       i++;
    }
   Wire.endTransmission();
  }
  activerAfficheur(adress);
}

void blinkAfficheur(int adress,int valeur, unsigned long duree){
  int i;
  for (i=0;i<valeur;i++){
    delay(duree);
    desactiverAfficheur(adress);
    delay(duree);
    activerAfficheur(adress);
    duree=duree-(duree/valeur);
  }
}

void shiftAfficheur(int adress, int valeur){
  int i;
  envoyerCommande(adress,0x0c);     // 0    0    0    0    1    1    0    0 :DisplayCtrl=>D=1(lcd active),C=0 (cursor off), B=0 (blink off)
  envoyerCommande(adress,0x07);     // 0    0    0    0    0    1    1    1 :entry mode set=> I/D=1(incremente), S=1(shift display)
  for(i=0;i<valeur;i++){
    envoyerCommande(adress,0x18);   // 0    0    0    1    1    0    0    0 :Curs_disp_shift=> SC=1 (shift display),RL=0 (left shift)
    delay(200);
  }
  envoyerCommande(adress,0x06);     // 0    0    0    0    0    1    1    0 :entry mode set=> I/D=1(incremente), S=0(cursor move)
  envoyerCommande(adress,0x0c);     // 0    0    0    0    1    1    1    1 :DisplayCtrl=>D=1(lcd active),C=0 (cursor oon), B=0 (blink on)
}
