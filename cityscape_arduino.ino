#include <Arduino.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"

#include "BluefruitConfig.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println(F("Cityscape Board Microcontroller"));
  Serial.println(F("---------------------------------------------------"));

  randomSeed(micros());
}

void loop() {
  // put your main code here, to run repeatedly:

}

//Utility
void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

//Bluetooth Lifecycle
SoftwareSerial bluefruitSS = SoftwareSerial(BLUEFRUIT_SWUART_TXD_PIN, BLUEFRUIT_SWUART_RXD_PIN);
Adafruit_BluefruitLE_UART ble(bluefruitSS, BLUEFRUIT_UART_MODE_PIN,
                      BLUEFRUIT_UART_CTS_PIN, BLUEFRUIT_UART_RTS_PIN);

#define GATTSERV_CITYSCAPE 0xCC00
#define GATTCHAR_BOARDSSTATE 0xCC01
#define GATTCHAR_BOARDPOWERSTATES 0xCC02
#define GATTCHAR_BOARDBUILDINGSTATES 0xCC03
#define GATTCHAR_BOARDTRANSITSTATES 0xCC04

int32_t csServiceId;
int32_t csBoardSStateCharId;
int32_t csBoardPowerStatesCharId;
int32_t csBoardBuildingStatesCharId;
int32_t csBoardTransitStatesCharId;

int initializeBluetoothService() {
  boolean success;

  /* Initialise the module */
  Serial.print(F("Initialising the Bluefruit LE module: "));

  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  Serial.println( F("OK!") );

  /* Perform a factory reset to make sure everything is in a known state */
  Serial.println(F("Performing a factory reset: "));
  if (! ble.factoryReset() ){
       error(F("Couldn't factory reset"));
  }

  /* Disable command echo from Bluefruit */
  ble.echo(false);

  Serial.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  ble.info();
  
  /* Change the device name to make it easier to find */
  Serial.println(F("Setting device name to 'Cityscape Board': "));

  if (! ble.sendCommandCheckOK(F("AT+GAPDEVNAME=Cityscape Board")) ) {
    error(F("Could not set device name?"));
    return 1;
  }

  /* Add the Cityscape Service definition */
  /* Service ID should be 1 */
  Serial.println(F("Adding the Cityscape Service definition (UUID = GATTSERV_CITYSCAPE): "));
  success = ble.sendCommandWithIntReply( F("AT+GATTADDSERVICE=UUID=GATTSERV_CITYSCAPE"), &csServiceId);
  if (! success) {
    error(F("Could not add Cityscape service"));
  }

  /* Add the Structure State characteristic */
  /* Chars ID for Measurement should be 1 */
  //TODO: Min/max len
  Serial.println(F("Adding the Structure State characteristic (UUID = GATTCHAR_BOARDSSTATE): "));
  success = ble.sendCommandWithIntReply( F("AT+GATTADDCHAR=UUID=GATTCHAR_BOARDSSTATE, PROPERTIES=0x10, MIN_LEN=1, MAX_LEN=20, VALUE=00-00, DESCRIPTION=Structure State"), &csBoardSStateCharId);
    if (! success) {
    error(F("Could not add Structure State characteristic"));
  }
}

//Structure Type Constants:
const int POWER_SOLAR = 0;
const int POWER_WIND = 1;
const int POWER_COAL = 2;

const int BUILDING_HOUSE_SMALL = 0;
const int BUILDING_HOUSE_LARGE = 1;
const int BUILDING_APARTMENT_SMALL = 2;
const int BUILDING_APARTMENT_LARGE = 3;

const int TRANSIT_BUS_STOP = 0;

int onPowerPlant(int type, int id, bool state, float level) {
  // bluetooth handling here. return 1 on success and 0 on fail maybe
}

int onBuilding(int type, int coord, bool state) {
  // bluetooth handling here. return 1 on success and 0 on fail maybe
}

int onTransit(int type, int coord, bool state) {
  // bluetooth handling here. return 1 on success and 0 on fail maybe
}

