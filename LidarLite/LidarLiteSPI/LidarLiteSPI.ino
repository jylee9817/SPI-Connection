/* 
http://pulsedlight3d.com
This sketch demonstrates getting distance with the LIDAR-Lite Sensor
It utilizes the 'Arduino I2C Master Library' from DSS Circuits:
http://www.dsscircuits.com/index.php/articles/66-arduino-i2c-master-library 
You can find more information about installing libraries here:
http://arduino.cc/en/Guide/Libraries
*/

#include <I2C.h>
#include <SPI.h>

#define    LIDARLite_ADDRESS   0x62          // Default I2C Address of LIDAR-Lite.
#define    RegisterMeasure     0x00          // Register to write to initiate ranging.
#define    MeasureValue        0x04          // Value to initiate ranging.
#define    RegisterHighLowB    0x8f          // Register to get both High and Low bytes in 1 call.
#define    maxSamples          0x0a          // Smoothing algorithm samples
#define    offset              5          // sensor offset

#define    SCK_PIN  13
#define    MISO_PIN 12
#define    MOSI_PIN 11
#define    SS_PIN   10

int readings[maxSamples];
int readIndex = 0;
int total = 0;
int average = 0;

volatile byte pos;
volatile boolean process_it;

void setup()
{
  Serial.begin(9600); //Opens serial connection at 9600bps.     

  //have to send on master in *slave out*
  pinMode(MOSI_PIN, INPUT);
  pinMode(MISO_PIN, OUTPUT);
  pinMode(SCK_PIN, INPUT);
  pinMode(SS_PIN, INPUT);
  
  //start the SPI library
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV16); //Div 2 to 128 by doubles
  SPI.beginTransaction(SPISettings(500000, MSBFIRST, SPI_MODE0));
  
  SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR1);
  byte clr;
  clr = SPSR;
  clr = SPDR;

  //Start the I2C library
  I2c.begin(); // Opens & joins the irc bus as master
  delay(100); // Waits to make sure everything is powered up before sending or receiving data  
  I2c.timeOut(50); // Sets a timeout to ensure no locking up of sketch if I2C communication fails
  
  for(int thisReading = 0; thisReading < maxSamples; thisReading++)
  {
    readings[thisReading] = 0;
  }
}

void loop()
{
  //enable Slave Select
  digitalWrite(SS_PIN, LOW);
  
  // Write 0x04 to register 0x00
  uint8_t nackack = 100; // Setup variable to hold ACK/NACK resopnses     
  while (nackack != 0) // While NACK keep going (i.e. continue polling until sucess message (ACK) is received )
  { 
    nackack = I2c.write(LIDARLite_ADDRESS, RegisterMeasure, MeasureValue); // Write 0x04 to 0x00
    delay(1); // Wait 1 ms to prevent overpolling
  }

  byte distanceArray[2]; // array to store distance bytes from read function
  
  // Read 2byte distance from register 0x8f
  nackack = 100; // Setup variable to hold ACK/NACK resopnses     
  while (nackack != 0)// While NACK keep going (i.e. continue polling until sucess message (ACK) is received )
  { 
    nackack = I2c.read(LIDARLite_ADDRESS, RegisterHighLowB, 2, distanceArray); // Read 2 Bytes from LIDAR-Lite Address and store in array
    delay(1); // Wait 1 ms to prevent overpolling
  }
  // Serial.println(distanceArray[0]);
  // Serial.println(distanceArray[1]);
  total = total - readings[readIndex];
  readings[readIndex] = (distanceArray[0] << 8) + distanceArray[1];  // Shift high byte [0] 8 to the left and add low byte [1] to create 16-bit int

  total = total + readings[readIndex];
  readIndex = readIndex + 1;
  if(readIndex >= maxSamples)
  {
    readIndex = 0;
  }

   average = (total / maxSamples)-offset;
   //Serial.write(average); Does this code needed for RoboRIO transfer?
   Serial.println(average);
   SPI.transfer(average);
   delay(1);
    
  // Print Distance
  //Serial.println(distance); //unit that it prints out is roughly in centimeters(cm)
}
