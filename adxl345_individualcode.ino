#include <Wire.h>  // Wire library - used for I2C communication
// SDA A4 SCL A5
int ADXL345 = 0x53; // The ADXL345 sensor I2C address

String X_out, Y_out, Z_out;  // Outputs
float x,y,z;
float getWireRead(){
  float value= ( Wire.read() | Wire.read() << 8);
  return value;
}

void setup() {
  Serial.begin(9600); // Initiate serial communication for printing the results on the Serial monitor
  Wire.begin(); // Initiate the Wire library
  // Set ADXL345 in measuring mode
  Wire.beginTransmission(ADXL345); // Start communicating with the device 
  Wire.write(0x2D); // Access/ talk to POWER_CTL Register - 0x2D
  // Enable measurement
  Wire.write(8); // (8dec -> 0000 1000 binary) Bit D3 High for measuring enable 
  Wire.endTransmission();
}

void loop() {
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
  Serial.print(x);
  Serial.print("\t");
  Serial.print(y);
  Serial.print("\t");
  Serial.print(z);
  Serial.println();
  delay(100);
}
