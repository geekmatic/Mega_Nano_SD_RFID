void func_i2c()
{
  byte high[2];
  byte error;

  Wire.beginTransmission(15);
  error = Wire.endTransmission();
  if (error == 0)
  {
      Wire.beginTransmission(15);   
      Wire.write(mfrc522.uid.uidByte[0]);
      Serial.print(mfrc522.uid.uidByte[0], HEX);
      Wire.write(mfrc522.uid.uidByte[1]);
      Serial.print(mfrc522.uid.uidByte[1], HEX);
      Wire.write(mfrc522.uid.uidByte[2]);
      Serial.print(mfrc522.uid.uidByte[2], HEX);
      Wire.write(mfrc522.uid.uidByte[3]);
      Serial.println(mfrc522.uid.uidByte[3], HEX);

      Wire.endTransmission();   
      
      Wire.requestFrom(15, 1, 1); // adr, bytes, 1-stop connection

      while (Wire.available()) {
        c = Wire.read();
        Serial.println(c);
        
      }
      Serial.println("");
  }
}
