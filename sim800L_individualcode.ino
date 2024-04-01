
#include <SoftwareSerial.h>

#define rxPin 7//sim800ltx nanodaD7ya gider
#define txPin 8//sim800lrx nanodaD8ya gider
SoftwareSerial sim800L(rxPin,txPin); 

String buff,sendingdatasms;
char recsms;
short con1=-1,con2=-1,con3=-1;
void setup()
{
  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(115200);
  
  //Begin serial communication with Arduino and SIM800L
  sim800L.begin(115200);

  Serial.println("Initializing...");
  
  sim800L.println("AT");
  waitForResponse();

  sim800L.println("ATE1");
  waitForResponse();

  sim800L.println("AT+CMGF=1");
  waitForResponse();

  sim800L.println("AT+CNMI=1,2,0,0,0");
  waitForResponse();
  sim800L.println("AT+CSQ");
  waitForResponse();
  sim800L.println("AT+CREG?");
  waitForResponse();
  sim800L.println("AT+COPS=?");
  waitForResponse();
  sim800L.println("AT+COPS?");
  waitForResponse();
  sim800L.println("AT+CBC?");
  waitForResponse();
  delay(5000);
}

void loop()
{
  
String receivedsms;
  while(sim800L.available()>0){
    recsms=sim800L.read();
    Serial.print(recsms);
    receivedsms.concat(recsms);
    con1=receivedsms.indexOf("Ac");
    if (con1!=-1){
      Serial.println("Wow got that 1");
      sendingdatasms="con1 trigerred";
      send_sms();
      ReceiveMode();
      con1=-1;
      Serial.println("con1 resetted");
    }
    con2=receivedsms.indexOf("Kapat");
    if (con2!=-1){
      Serial.println("Wow got that 2");
      sendingdatasms="con2 trigerred";
      send_sms();
      ReceiveMode();
      con2=-1;
      Serial.println("con2 resetted");
    }
    //con3=receivedsms.indexOf("Konum");
    con3=-1;
    if (con3==-1){
      Serial.println("Wow got that 3");
      sendingdatasms="con3 trigerred";
      send_sms();
      ReceiveMode();
      delay(5000);
      con3=1;
      Serial.println("con3 resetted");
    }
    
  }
}
void Serialcom(){

  while(Serial.available())  {

    sim800L.write(Serial.read());
  }
  
  while(sim800L.available()){
  Serial.write(sim800L.read());
  }
}

void send_sms(){
  Serial.println("Sending sms rn");
  sim800L.print("AT+CMGS=\"+yournumberhere\"\r");
  delay(100);
  sim800L.print(sendingdatasms);
  delay(100);
  sim800L.write(0x1A);
  delay(100);
  sim800L.println();
  delay(100);
}


void make_call(){
  sim800L.println("ATD+yournumberhere;");
  waitForResponse();
}

void waitForResponse(){
  delay(500);
  while(sim800L.available()){
    Serial.println(sim800L.readString());
  }
}

void ReceiveMode(){
  sim800L.println("AT");
  Serialcom();
  sim800L.println("AT+CMGF=1");
  Serialcom();
  sim800L.println("AT+CNMI=2,2,0,0,0");
  Serialcom();
  
}

/* *************************************************
 * SMS SENT STATUS
 * SMS SENT     -> OK
 * SMS NOT SENT -> ERROR or +CMS ERROR
 * *************************************************
AT+CMGS="+yournumberhere"
> It is easy to send text messages.
+CMGS: 12
OK
The value in the information response, 12, is the message reference number allocated to the SMS 
text message. The final result code OK indicates the sending of the SMS text message was successful. 
If the execution of the +CMGS AT command fails, the GSM/GPRS modem or mobile phone will return 
either the final result code ERROR or +CMS ERROR.
 */
