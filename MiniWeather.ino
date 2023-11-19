/* V1.0  Chris Woodhouse FRAS Feb 21
 *  chris@digitalastrophotography.co.uk
 * very simple environmental module uses Arduino mini and BME sensor
 */
#define version "1.0" // change with revision
#include <BlueDot_BME280.h>  // for reading Bosch BME sensor and calibration

// weather sensor parameters and objects
BlueDot_BME280 bme1;      //bme1 is sensor object
uint8_t bme1Detected = 0;
const uint8_t cmdAmbient = 6;
const uint8_t cmdObject1 = 7;
const uint8_t cmdFlags = 0xf0;
const uint8_t cmdSleep = 0xff;
double tempC = 20.0;   // ambient temp (BME)
double humidity = 50.0;  // humidity (BME)
double pressure = 1000.0; // pressure (BME)


void setup() 
{
Serial.begin(9600);
pinMode(LED_BUILTIN, OUTPUT);
// environment sensor setup

    bme1.parameter.I2CAddress = 0x76;      //I2C Address for Sensor 1 (bme1)
    bme1.parameter.sensorMode = 0b11;     // 11 normal mode 01 forced mode
    bme1.parameter.IIRfilter = 0b010;      // filter factor 16             //IIR Filter for Sensor 1
    bme1.parameter.standby = 0b101;        // added another parameter to control standby time
    bme1.parameter.humidOversampling = 0b100;   // filter factor 4 (default value)
    bme1.parameter.tempOversampling = 0b100;  // filter factor 4 (default value)
    bme1.parameter.pressOversampling = 0b100;  // filter factor 4 (default value)
    // initial sensor activation and sampling
    detectBME();
    ReadBME();
}

void loop() 
{
  // put your main code here, to run repeatedly:
  ReadBME(); // read tempC, humidity and pressure every 5 seconds
  delay(5000);  // 5 second delay
  FTDi();
  digitalWrite(LED_BUILTIN, HIGH);
  delay(10);
  digitalWrite(LED_BUILTIN, LOW);
}

// routine to output formatted string over FTDi
// all strings representing doubles
// "$tempC,humidity,pressure#"

void FTDi(void)
{
    Serial.print('$');  // start character
    Serial.print(tempC,2); // from BME  (2 D.P.)
    Serial.print(',');
    Serial.print(humidity,2); // from BME
    Serial.print(',');
    Serial.print(pressure,2); // from BME
    Serial.print('#');  // terminal character
}
// try to establish contact with BME sensor - sets OK flag
void detectBME(void)
{
    if (bme1.init() != 0x60)
    {
        //Serial.println(F("Ops! First BME280 Sensor not found!"));
        bme1Detected = 0;
    }
    else
    {
        //Serial.println(F("First BME280 Sensor detected!"));
        bme1Detected = 1;
    }
}

// read environmental values from BME sensor
void ReadBME(void)
{
    if (bme1Detected)
    {
    tempC = bme1.readTempC();
    humidity = bme1.readHumidity();
    pressure = bme1.readPressure();
    }
}
