#ifndef I2C_H
#define I2C_H
/*cette macro permet de placer a 1 le bit d'un registre*/
#ifndef sbi
#define sbi(sfr,bit) (_SFR_BYTE(sfr)|=_BV(bit))
#endif
/*cette macro permet de placer a 0 le bit d'un registre*/
#ifndef cbi
#define cbi(sfr,bit) (_SFR_BYTE(sfr)&=~_BV(bit))
#endif

void initI2C(int vitesse){

  /*division de 64 pour le prediviseur TWPS
  TWPS0=0 et TWPS1=0 ==> TWPS=0 ==> 4^TWPS=1
  TWPS0=0 et TWPS1=1 ==> TWPS=1 ==> 4^TWPS=4
  TWPS0=1 et TWPS1=0 ==> TWPS=2 ==> 4^TWPS=16
  TWPS0=1 et TWPS1=1 ==> TWPS=3 ==> 4^TWPS=64
  0 < TWBR < 255
  SCL=frequence CPU / (16+2*TWBR*4^TWPS)
  d'ou
  489 Hz<SCL<1 000 000 Hz*/
  sbi(TWSR, TWPS0);  //reduction de la vitesse de transmission sur le
  cbi(TWSR, TWPS1);  //bus (ici 4081,63 Hz)
  TWBR=vitesse;		//valeur comprise entre 0 et 255. Une valeur de 122 donne de bon résultats
}
#endif