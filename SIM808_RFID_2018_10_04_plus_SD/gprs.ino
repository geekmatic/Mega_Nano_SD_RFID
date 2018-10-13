void func_gprs() {
  if (millis() - prev_ms_gprs >= 20000){
    prev_ms_gprs = millis();
//    DateTime now = RTC.now();
//    Serial.println(now.unixtime());
    str1 = "1.php/";
    
    if (dosendgps==1){
      str1 += "L1=";
      str1 += String(longitude, 5);
      str1 += ";L2=";
      str1 += String(latitude, 5);
      str1 += ";";
      dosendgps = 0;
    }
    if (dosendbalans==1){
      str1 += "B=";
      str1 += String(balans, 2);
      str1 += ";";
    }
    for (int i=0; i < bufsize; i++){
      if (otime[i]>0) {
        str1 += otime[i];
        str1 += "_";
        str1 += String(okod[i][0], HEX);
        str1 += String(okod[i][1], HEX);
        str1 += String(okod[i][2], HEX);
        str1 += String(okod[i][3], HEX);
        str1 += ";";
      }
    }
    for (int i=0; i < bufsize; i++){
      otime[i] = 0;
      okod[i][0] = 0;
      okod[i][1] = 0;
      okod[i][2] = 0;
      okod[i][3] = 0;
    }

    A6command("AT+HTTPPARA=\"URL\",\"bee.in.ua/" + str1 + "\"", "OK", "ERROR", 10000, 1);
    A6command("AT+HTTPPARA=\"CID\",1", "OK", "ERROR", 10000, 1);
  
    A6command("AT+HTTPACTION=0", "+HTTPACTION:", "ERROR", 20000, 1);
    
    if (signal_level_str.indexOf("+HTTPACTION: 0,200,")==-1){
      A6command("AT+HTTPTERM", "HTTPTERM", "ERROR", 20000, 1);
      A6command("AT+SAPBR=0,1", "+SAPBR:", "ERROR", 2000, 1);

      A6command("AT+SAPBR=1,1", "+SAPBR:", "ERROR", 2000, 1); //start up the connection
      A6command("AT+HTTPINIT", "OK", "ERROR", 5000, 1);
      A6command("AT+HTTPPARA=\"URL\",\"bee.in.ua/" + str1 + "\"", "OK", "ERROR", 10000, 1);
      A6command("AT+HTTPPARA=\"CID\",1", "OK", "ERROR", 10000, 1);
      A6command("AT+HTTPACTION=0", "+HTTPACTION:", "ERROR", 20000, 1);
      if (signal_level_str.indexOf("+HTTPACTION: 0,200,")==-1){
        return;
      }
    }
    signal_level_str = "";
    A6command("AT+HTTPREAD", "HTTPREAD", "ERROR", 20000, 1);
    parse_dani(signal_level_str);
  }  
}
  
void parse_dani(String &outstr)
{
  //if (outstr.indexOf("a=")>-1){
    outstr = outstr.substring(outstr.indexOf('\n')+1, outstr.length());
    outstr = outstr.substring(outstr.indexOf('\n')+1, outstr.length());
    outstr = outstr.substring(outstr.indexOf('\n')+1, outstr.length());
    String outstr1 = outstr.substring(0, outstr.indexOf('\n'));
    outstr = outstr.substring(outstr.indexOf('\n')+1, outstr.length());
    Serial.println(outstr1);
    Serial.println("and");
    Serial.println(outstr);
    String outstr2;
    while(outstr1.length()>7){
      outstr2 = outstr1.substring(0, outstr1.indexOf(";"));
      outstr1 = outstr1.substring(outstr1.indexOf(";")+1, outstr1.length());
      Serial.println(outstr2);
      noInterrupts();
      if (!SD.exists(outstr2)) {
        myFile = SD.open(outstr2, FILE_WRITE);
        myFile.close();
      }  
      interrupts();
    }
    Serial.println(" ");
    while(outstr.length()>7){
      outstr2 = outstr.substring(0, outstr.indexOf(";"));
      outstr = outstr.substring(outstr.indexOf(";")+1, outstr.length());
      Serial.println(outstr2);
      noInterrupts();
      if (SD.exists(outstr2)) {
        SD.remove(outstr2);
      }
      interrupts();
    }

 // }
  
}
  
byte A6waitFor(String response1, String response2, uint16_t timeOut) {
  uint16_t entry = 0;
  uint16_t count = 0;
  String reply = A6read();
  byte retVal = 99;
  do {
    reply = A6read();
    delay(1);
    entry ++;
  } while ((reply.indexOf(response1) + reply.indexOf(response2) == -2) && entry < timeOut );
  if (entry >= timeOut) {
    retVal = TIMEOUT;
  } else {
    if (reply.indexOf(response1) + reply.indexOf(response2) > -2) retVal = OK;
    else retVal = NOTOK;
  }
  signal_level_str = reply;
  return retVal;
}

byte A6command(String command, String response1, String response2, uint16_t timeOut, uint16_t repetitions) {
  byte returnValue = NOTOK;
  byte count = 0;
  
  while (count < repetitions && returnValue != OK) {
    SIM808board.println(command);
    
    if (A6waitFor(response1, response2, timeOut) == OK) {
      returnValue = OK;
    } else {returnValue = NOTOK;}
    count++;
  }
  
  return returnValue;
}

String A6read() {
  char c;
  String reply = "";
  if (SIM808board.available())  {
    reply = SIM808board.readString();
  }

  if (reply!=""){
    Serial.print("Reply: ");
    Serial.println(reply);
  }
  
  //signal_level_str = reply;
  return reply;
}


