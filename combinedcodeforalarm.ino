/* adxl345 için pin a4 sda a5 scl*/
#include <SoftwareSerial.h>
#include <TinyGPSPlus.h>
#define rxPin 7 // sim800ltx nanodaD7ya gider
#define txPin 8 // sim800lrx nanodaD8ya gider
SoftwareSerial sim800L(rxPin, txPin);

String buff, sendingdatasms;
char recsms;
short con1 = -1, con2 = -1, con3 = -1, con4 = -1;

#include <Wire.h>   // Wire library - used for I2C communication
int ADXL345 = 0x53; // The ADXL345 sensor I2C address
String longi, lati;
double longid, latid;
String X_out, Y_out, Z_out; // Outputs
float x, y, z, xq = 0, yq = 0, zq = 0;
int trigger=0;
float getWireRead()
{
  float value = (Wire.read() | Wire.read() << 8);
  return value;
}
// GPS pins:
static const int RXPin = 5, TXPin = 6; // gps rxD3e yazılım rx4 , gps tx D4e yazılım rx3
static const uint32_t GPSBaud = 9600;  // must 9600

unsigned long timer1, timer2, timer3;
// The TinyGPSPlus object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

void setup()
{
  // Begin serial communication with Arduino and Arduino IDE (Serial Monitor) GSM
  Serial.begin(9600); // Begin serial communication with Arduino and Arduino IDE (Serial Monitor) GSM

  Serial.begin(115200); // Begin serial communication with Arduino and Arduino IDE (Serial Monitor) GSM
  // GPS:
  // Serial.begin(19200);  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor) GSM

  timer1 = millis();
  timer2 = millis();
  timer3 = millis();

  // Begin serial communication with Arduino and SIM800L
  sim800L.begin(19200);

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
  sim800L.println("ATS7=12");
  waitForResponse();
  sim800L.println("AT+COPS=?");
  waitForResponse();
  // sim800L.println("AT+COPS?");
  // waitForResponse();
  sim800L.println("AT+CBC?");
  waitForResponse();

  // Serial.begin(4800); // Initiate serial communication for printing the results on the Serial monitor
  Wire.begin(); // Initiate the Wire library
  // Set ADXL345 in measuring mode
  Wire.beginTransmission(ADXL345); // Start communicating with the device
  Wire.write(0x2D);                // Access/ talk to POWER_CTL Register - 0x2D
  // Enable measurement
  Wire.write(8); // (8dec -> 0000 1000 binary) Bit D3 High for measuring enable
  Wire.endTransmission();
  delay(3000);
  Wire.beginTransmission(ADXL345);
  Wire.write(0x32); // Start with register 0x32 (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(ADXL345, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
  xq = getWireRead();
  yq = getWireRead();
  zq = getWireRead();
  /*Serial.print("xq=");
  Serial.print(xq);
  Serial.print("yq=");
  Serial.print(yq);
  Serial.print("zq=");
  Serial.print(zq);*/
  Wire.endTransmission();

  ss.begin(GPSBaud);

  Serial.println("Setup completed.");
}

void loop()
{
  String receivedsms;
  while (sim800L.available() > 0)
  {
    recsms = sim800L.read();
    Serial.print(recsms);
    receivedsms.concat(recsms);
    con1 = receivedsms.indexOf("Ac");
    if (con1 != -1)
    {
      Serial.println("Wow got that 1");
      sendingdatasms = "Ac trigerred";
        send_sms();
        waitForResponse();
      con1 = -1;
      Serial.println("Ac requested");
    }
    con2 = receivedsms.indexOf("Kapat");
    if (con2 != -1)
    {
      Serial.println("Wow got that 2");
      sendingdatasms = "Kapat trigerred";
        send_sms();
        waitForResponse();
      con2 = -1;
      Serial.println("Kapat requested");
    }
    con3 = receivedsms.indexOf("Konum");
    if (con3 != -1)
    {

      Serial.println("Konum request");
      // sendingdatasms="con3 trigerred";
      // GPS below;
      if (millis() - timer1 > 5000)
      {
        smartDelay(1000);
        timer1 = millis();
        int loopcount = 0;
        while ((gps.charsProcessed() < 1600) && (loopcount < 100))
        {
          smartDelay(1000);

          latid = gps.location.lat();
          longid = gps.location.lng();
          loopcount++;
          Serial.print(loopcount);
        }
        loopcount = 0;
        Serial.println(latid);
        if (latid != longid)
        {
          lati = String(latid, 6);
          longi = String(longid, 6);
        }
        sendingdatasms = "https://www.google.com/maps/search/(" + (lati) + "," + (longi) + ")";
        if (millis() > 5000 && gps.charsProcessed() < 10)
        {

          Serial.println(F("No GPS data received: check wiring"));
        }

        // Serial.println();
        Serial.println(sendingdatasms);
        // Serial.println();
        // Serial.println( gps.charsProcessed());
        // Serial.println();
        // GPS ABOVE

        send_sms();
        waitForResponse();
        con3 = -1;
      }
    }

    // GPS below
    /*static const double LONDON_LAT = 51.508131, LONDON_LON = -0.128002;
    latid=gps.location.lat();
    longid=gps.location.lng();
    lati=String(latid,6);
    longi=String(longid,6);
    sendingdatasms="https://www.google.com/maps/search/("+(lati)+","+(longi)+")";
   //Serial.println();
   //Serial.println(sendingdatasms);

   /*Serial.print("https://www.google.com/maps/search/(");
   printFloat(gps.location.lat(), gps.location.isValid(), 8, 6);
   Serial.print(",");
   printFloat(gps.location.lng(), gps.location.isValid(), 8, 6);
   Serial.print(")");*/
    /*
     unsigned long distanceKmToLondon =
       (unsigned long)TinyGPSPlus::distanceBetween(
         gps.location.lat(),
         gps.location.lng(),
         LONDON_LAT,
         LONDON_LON) / 1000;
         //printFloat()

     double courseToLondon =
       TinyGPSPlus::courseTo(
         gps.location.lat(),
         gps.location.lng(),
         LONDON_LAT,
         LONDON_LON);


     const char *cardinalToLondon = TinyGPSPlus::cardinal(courseToLondon);

     //Serial.println();

     smartDelay(0);

     if (millis() > 5000 && gps.charsProcessed() < 10)
       Serial.println(F("No GPS data received: check wiring"));
   //GPS above*/
  }
  // === Read acceleromter data === //
  Wire.beginTransmission(ADXL345);
  Wire.write(0x32); // Start with register 0x32 (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(ADXL345, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
  x = getWireRead();
  y = getWireRead();
  z = getWireRead();
  X_out = x;
  Y_out = y;
  Z_out = z;
  if (((x - xq > 50) && (x > xq)) || ((x - xq < -50) && (x < xq)))
  {
    // Serial.println();
    Serial.println("aaaaaaaaaa   ");
    trigger=1;
    // Serial.println(x-xq);
    // Serial.println();
  }
  if (((y - yq > 50) && (y > yq)) || ((y - yq < -50) && (y < yq)))
  {
    // Serial.println();
    Serial.println("bbbbbbbbbbb   ");
    trigger=1;
    // Serial.println(y-yq);
    // Serial.println();
  }
  //////////////////////////////////////////////////////
  if (((z - zq > 50) && (z > zq)) || ((z - zq < -50) && (z < zq)))
  {
    // Serial.println();
    Serial.println("cccccccc   ");
    Serial.println(z - zq);
    trigger=1;
    // Serial.println();

    // GPS below;
  }
  if (millis() < 5000 ){
    trigger=0;
  }
   if ((millis() - timer3> 50000)){
    
      smartDelay(1000);
      timer3 = millis();
   }
  if (trigger==1){
    if (millis() - timer1 > 5000)
    {

      smartDelay(1000);
      timer1 = millis();
      timer3 = millis();
      int loopcount = 0;
      while ((gps.charsProcessed() < 1600) && (loopcount < 100))
      {
        smartDelay(1000);
        latid = gps.location.lat();
        longid = gps.location.lng();
        loopcount++;
        Serial.println(loopcount);
      }
      loopcount = 0;
      if (latid != longid)
      {
        lati = String(latid, 6);
        longi = String(longid, 6);
      }
      sendingdatasms = "https://www.google.com/maps/search/(" + (lati) + "," + (longi) + ")";
      if (millis() > 5000 && gps.charsProcessed() < 10)
      {

        Serial.println(F("No GPS data received: check wiring"));
      }

      // Serial.println();
      Serial.println(sendingdatasms);
      // Serial.println();
      // Serial.println( gps.charsProcessed());
      // Serial.println();
      // GPS ABOVE
    }

    if ((con4 == -1) && (millis() - timer2 > 20000))
    {
      // waitForResponse();

      make_call();
      waitForResponse();
      send_sms();
      waitForResponse();
      Serial.println();
      Serial.println(sendingdatasms);
      Serial.println();
      timer2 = millis();
      Serial.println("Testing con4");
      // send_sms();
      // con4=0;sadece 1 kez tetiklenmesi için
    }
    delay(100);
    Serial.println();
    Serial.println("birisi dokundu");
    Serial.println();
  }
  ///////////////////////////////////////////////////////////7
  /*Serial.print(x);
  Serial.print("\t");
  Serial.print(y);
  Serial.print("\t");
  Serial.print(z);
  Serial.println();
  delay(100);*/
}
void Serialcom()
{

  while (Serial.available())
  {

    sim800L.write(Serial.read());
  }

  while (sim800L.available())
  {
    Serial.write(sim800L.read());
  }
}

void send_sms()
{
  delay(100);
  Serial.println("Sending sms rn");
  sim800L.println();
  sim800L.print("AT+CMGS=\"+yournumberhere\"\r");
  delay(100);
  sim800L.print(sendingdatasms);
  delay(100);
  sim800L.write(0x1A);
  delay(100);
  sim800L.println();
  delay(100);
}

void make_call()
{
  delay(100);
  sim800L.println("ATD+yournumberhere;");
  waitForResponse();
  delay(100);
}

void waitForResponse()
{
  delay(500);
  while (sim800L.available())
  {
    Serial.println(sim800L.readString());
    // Serial.println(sim800L.read());
  }
}

void ReceiveMode()
{
  sim800L.println("AT");
  waitForResponse();
  sim800L.print("AT+CNMI=2,2,0,0,0");
  waitForResponse();
}
// gps below V

// This custom version of delay() ensures that the gps object
// is being "fed".
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while ((ss.available()) && (millis() - start < ms))
      gps.encode(ss.read());
  } while (millis() - start < ms);
}

static void printFloat(float val, bool valid, int len, int prec)
{
  if (!valid)
  {
    while (len-- > 1)
      Serial.print('*');
    Serial.print(' ');
  }
  else
  {
    Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3
                         : vi >= 10    ? 2
                                       : 1;
    for (int i = flen; i < len; ++i)
      Serial.print(' ');
  }
  smartDelay(0);
}

static void printInt(unsigned long val, bool valid, int len)
{
  char sz[32] = "*****************";
  if (valid)
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  for (int i = strlen(sz); i < len; ++i)
    sz[i] = ' ';
  if (len > 0)
    sz[len - 1] = ' ';
  Serial.print(sz);
  smartDelay(0);
}

static void printDateTime(TinyGPSDate &d, TinyGPSTime &t)
{
  if (!d.isValid())
  {
    Serial.print(F("********** "));
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d/%02d/%02d ", d.month(), d.day(), d.year());
    Serial.print(sz);
  }

  if (!t.isValid())
  {
    Serial.print(F("******** "));
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d:%02d:%02d ", t.hour(), t.minute(), t.second());
    Serial.print(sz);
  }

  printInt(d.age(), d.isValid(), 5);
  smartDelay(0);
}

static void printStr(const char *str, int len)
{
  int slen = strlen(str);
  for (int i = 0; i < len; ++i)
    Serial.print(i < slen ? str[i] : ' ');
  smartDelay(0);
}
// GPS ABOVE A

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
