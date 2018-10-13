void rfidtimer() {
  Timer1.initialize(600000);
  Timer1.attachInterrupt(func_timer);
}  

void func_timer() {
  if (dosend==0){
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Dump debug info about the card; PICC_HaltA() is automatically called
//  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
  str="";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if(mfrc522.uid.uidByte[i] < 0x10) str+="0";
    
    str+=String(mfrc522.uid.uidByte[i], HEX);
  } 
  Serial.println(str);
  
  dosend = 1;
  c = 0;
  }
}
