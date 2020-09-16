// 7 Segmentanzeige - Pins für segmentmuster
//    -      Pin3(Bit0)
//   | |     Pin7(Bit1)     Pin4(Bit2)
//    -      Pin5(Bit3)
//   | |     Pin11(Bit4)    Pin12(Bit5)
//    -   .  Pin10(Bit6)    Pin13(bit7)

int lmPin = 0; // LM35 Analog-Pin

byte segmentmuster[10] = {  // Jedes Segmentmuster repraesentiert eine Ziffer
  B01110111, // Ausleuchtung fuer die 0
  B00100100, // Ausleuchtung fuer die 1
  B01011101, // Ausleuchtung fuer die 2
  B01101101, // Ausleuchtung fuer die 3
  B00101110, // Ausleuchtung fuer die 4
  B01101011, // Ausleuchtung fuer die 5
  B01111011, // Ausleuchtung fuer die 6
  B00100101, // Ausleuchtung fuer die 7
  B01111111, // Ausleuchtung fuer die 8
  B01101111};// Ausleuchtung fuer die 9

int PwmPin = 6; // Pin zum Ansteuern des MosFet per PWM

int PinArray[] = {10, 12, 11, 5, 4, 7, 3};  // Pins der 7 Segmentanzeige ohne Punkt
byte EinerPin = 2, ZehnerPin = 9;

// Pins als Output setzen
void setup(){
  pinMode(lmPin, INPUT); 
  for(int i = 0; i < 7; i++){
    pinMode(PinArray[i], OUTPUT);
    pinMode(EinerPin, OUTPUT);
    pinMode(ZehnerPin, OUTPUT);
  }
  Serial.begin(9600); // open the serial port at 9600 bps:
}

void loop(){
  int temperatur = (int)TemperaturCelsius()+.5; // Korrektur fuer Rundung
  Serial.println(temperatur);  
  if ((temperatur > 14) and (temperatur < 18)){  // Erst ab 15 Grad Heizen, sonst aufwaermen durch Zimmertemperatur
    heizung_stufe_6();
    Serial.println("HS6");
  }
  else if ((temperatur > 17) and (temperatur < 21)){
    heizung_stufe_5();
    Serial.println("HS5");
  }
  else if (temperatur == 21){
    heizung_stufe_4();
    Serial.println("HS4");
  }
  else if ((temperatur == 22) or (temperatur == 23)){
    heizung_stufe_3();
    Serial.println("HS3");
  }
  else if ((temperatur > 23) and (temperatur < 25)){
    heizung_stufe_2();
    Serial.println("HS2");
  }
    else if ((temperatur > 24) and (temperatur < 26)){
    heizung_stufe_1();
    Serial.println("HS1");
  }
  else{
    heizung_stufe_0();
  }
  
  anzeige(temperatur);

}

void anzeige(int wert){
  byte einer, zehner;
  zehner = int(wert / 10); // Ziffer der ZehnerPin berechnen
  einer = wert - zehner * 10; // Ziffer der EinerPin berechnen

  //EinerPin schalten
  digitalWrite(EinerPin, HIGH);
  digitalWrite(ZehnerPin, LOW);
  aktivierung(einer);

  //ZehnerPin schalten
  digitalWrite(EinerPin, LOW);
  digitalWrite(ZehnerPin, HIGH);
  aktivierung(zehner);
}

void aktivierung(int a){
  for(int j = 6; j >=0; j--) //Bits fuer Segmente
  digitalWrite(PinArray[6 - j], bitRead(segmentmuster[a], j) == 1?HIGH:LOW);
  delay(12); // 12ms warten
}

float TemperaturCelsius(){   
   float AD_Wert = analogRead(lmPin);
   float Prozent = AD_Wert/1023.0;
   float Volt = Prozent*5.0;
   return 100.0*Volt;
}

// Heizstufen-PWM-Werte zwischen 0 und 255 möglich.
// Temperaturen duerfen für die Urzeitkrebse i.d.R nicht unter 20 oder über 26 Grad liegen, 
// also Vorsicht und mit dem eigenen Heizpad vorher ohne Tierchen testen!

// Durch verschiedene Abstufungen wird fuer ein etwas schnelleres Erwaermen nach einem Wasserwechsel gesorgt
// PwmPin Werte aus praktischer Erprobung ohne Tierchen
void heizung_stufe_0(){ 
  analogWrite(PwmPin, 0);
}
void heizung_stufe_1(){
  analogWrite(PwmPin, 100);
}
void heizung_stufe_2(){
  analogWrite(PwmPin, 140);
}
void heizung_stufe_3(){
  analogWrite(PwmPin, 165);
}
void heizung_stufe_4(){
  analogWrite(PwmPin, 170);
}
void heizung_stufe_5(){
  analogWrite(PwmPin, 175);
}
void heizung_stufe_6(){
  analogWrite(PwmPin, 180);
}
