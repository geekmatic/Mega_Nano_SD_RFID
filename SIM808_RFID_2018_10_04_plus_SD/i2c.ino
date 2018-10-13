void requestEvent() 
{  
  Wire.write(kategor);
}

void receiveEvent(int bytes)
{
  String str5="";
  byte g=0, h=0;
  byte i=0;
  kategor = 0;
  
  while((otime[i]>0)&&(i<bufsize-1)){
    i++;
    h = i;
  }
  
  while (Wire.available()) { // slave may send less than requested
    byte c = Wire.read(); // receive a byte as character  
    if(c < 0x10) str5+="0";
    
    okod[h][g] = c;
    str5 += String(okod[h][g], HEX);
    otime[h] = now.unixtime();
    g++;
  }
  if (str5!=""){
    Serial.println(str5);
 /*   
    Serial.print(otime[0]);
    Serial.print(" ");
    Serial.print(otime[1]);
    Serial.print(" ");
    Serial.print(otime[2]);
    Serial.print(" ");
    Serial.print(otime[3]);
    Serial.print(" ");
    Serial.print(otime[4]);
    Serial.print(" ");
    Serial.print(otime[5]);
    Serial.print(" ");
    Serial.print(otime[6]);
    Serial.print(" ");
    Serial.print(otime[7]);
    Serial.print(" ");
    Serial.print(otime[8]);
    Serial.print(" ");
    Serial.print(otime[9]);    
    Serial.println(" ");
    */
    if (SD.exists(str5)) {
      Serial.println("e takiy");
      kategor = 1;
    } else {
      kategor = 2;
      Serial.println(str5);
      Serial.println("str doesn't exist.");
    }
    Serial.println(otime[0]);
    
  }
 
}
