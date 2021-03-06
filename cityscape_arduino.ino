#include <Arduino.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"

#include "BluefruitConfig.h"
#include "CityscapeConfig.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  Serial.println(F("Cityscape Board Microcontroller"));
  Serial.println(F("---------------------------------------------------"));
  randomSeed(micros());
  
  initializePins();
  initializeBluetoothService();
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

int32_t csServiceId;
int32_t csBoardSStateCharId;
int32_t csBoardPowerStatesCharId;
int32_t csBoardBuildingStatesCharId;
int32_t csBoardTransitStatesCharId;
int32_t csBoardInfoCharId;

boolean bluetoothInitialized = false;

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
  Serial.println(F("Adding the Cityscape Service definition (UUID = " GATTSERV_CITYSCAPE "): "));
  success = ble.sendCommandWithIntReply( F("AT+GATTADDSERVICE=UUID=" GATTSERV_CITYSCAPE), &csServiceId);
  if (! success) {
    error(F("Could not add Cityscape service"));
  }

  /* Add the Structure State characteristic */
  //TODO: Min/max len
  Serial.println(F("Adding the Structure State characteristic (UUID =" GATTCHAR_BOARDSSTATE "): "));
  success = ble.sendCommandWithIntReply( F("AT+GATTADDCHAR=UUID=" GATTCHAR_BOARDSSTATE ", PROPERTIES=0x12, MIN_LEN=1, MAX_LEN=20, DATATYPE=2"), &csBoardSStateCharId);
    if (! success) {
    error(F("Could not add Structure State characteristic"));
  }

  /* Add the Board Info characteristic */
  //TODO: Min/max len
  Serial.println(F("Adding the Board Info characteristic (UUID =" GATTCHAR_BOARDINFO "): "));

  char boardInfoCommand [150] = {0};
  sprintf(boardInfoCommand, "AT+GATTADDCHAR=UUID=" GATTCHAR_BOARDINFO ", PROPERTIES=0x02, MIN_LEN=1, MAX_LEN=20, DATATYPE=2, DATATYPE=2, VALUE=00-%02X-%02X-%02X", NUM_POWER, NUM_BUILDING,NUM_TRANSIT);
  
  success = ble.sendCommandWithIntReply( boardInfoCommand, &csBoardInfoCharId);
  if (! success) {
    error(F("Could not add Board Info characteristic"));
  }

  /* Add the Cityscape Service to the advertising data (needed for Nordic apps to detect the service) */
  Serial.print(F("Adding Cityscape Service UUID to the advertising payload: "));
  ble.sendCommandCheckOK( F("AT+GAPSETADVDATA=02-01-06-07-02-00-cc-05-cc-0a-18") );

  /* Reset the device for the new service setting changes to take effect */
  Serial.print(F("Performing a SW reset (service changes require a reset): "));
  ble.reset();

  Serial.println();

  bluetoothInitialized=true;
}

//Structure Type Constants:

byte updateNum = 0x00;

int onStructure(byte combinedType, byte id, bool state, byte level) {
  // bluetooth handling here. return 1 on success and 0 on fail maybe

  Serial.print(F("Board updated: \nType:"));
  Serial.println(combinedType, BIN);
  Serial.print(F("Coord:"));
  Serial.println(id);
  Serial.print(F("State:"));
  Serial.println(state);
  Serial.print(F("Level:"));
  Serial.println(level);
  Serial.print(F("UpdateNum:"));
  Serial.println(updateNum, HEX);

  if(bluetoothInitialized) {
    /* Command is sent when \n (\r) or println is called */
    /* AT+GATTCHAR=CharacteristicID,value */
    
    ble.print( F("AT+GATTCHAR=") );
    ble.print( csBoardSStateCharId );
    //Characteristic
    char combinedValue[15] = {0};
    //Add +1 to the type value if state is true
    sprintf(combinedValue, "00-%02X-%02X-%02X-%02X", updateNum,(state? combinedType | B10000000: combinedType & B01111111),id, level);
    ble.print( F(","));
    ble.println(combinedValue);
    
    ble.print( F("\n") );
  
    /* Check if command executed OK */
    if ( !ble.waitForOK() )
    {
      Serial.println(F("Failed to get response!"));
      return 1;
    }
    //increment by 1 on success
    updateNum++;
    return 0;
  } else {
    Serial.println(F("Bluetooth not initialized!"));
    return 2;
  }
}

int onPowerPlant(byte subtype, byte id, bool state, byte level) {
  return onStructure(subtype+POWER_BASE, id, state, level);
}

int onBuilding(byte subtype, byte coord, bool state) {
  return onStructure(subtype+BUILDING_BASE, coord, state, 0);
}

int onTransit(byte subtype, byte coord, bool state) {
  return onStructure(subtype+TRANSIT_BASE, coord, state, 0);
}


//Wiring + states

bool power_state[NUM_BUILDING] = {};
byte power_level[NUM_BUILDING] = {};

bool building_state[NUM_BUILDING] = {0,0,0,0}; //TODO

bool transit_state[NUM_BUILDING] = {};

void initializePins() {
  for(byte i = 0; i < NUM_BUILDING; i++) {
    pinMode(BUILDING_PINS[i], INPUT);
  }
  for(byte i = 0; i < NUM_POWER; i++) {
    pinMode(POWER_PINS[i], INPUT);
    pinMode(POWER_POTPINS[i], INPUT);
  }
  for(byte i = 0; i < NUM_TRANSIT; i++) {
    pinMode(TRANSIT_PINS[i], INPUT);
  }
}



//Main loop

void loop() {
  // put your main code here, to run repeatedly:
  for(byte i = 0; i < NUM_BUILDING; i++) {
    bool currentState = digitalRead(BUILDING_PINS[i]) == LOW;
    if (currentState != building_state[i]) {
      building_state[i] = currentState;
      onBuilding(BUILDING_HOUSE_SMALL, i, currentState);
    }
  }
  for(byte i = 0; i < NUM_TRANSIT; i++) {
    bool currentState = digitalRead(TRANSIT_PINS[i]) == LOW;
    if (currentState != transit_state[i]) {
      transit_state[i] = currentState;
      onTransit(TRANSIT_BUS_STOP, i, currentState);
    }
  }
  for(byte i = 0; i < NUM_POWER; i++) {
    bool currentState = digitalRead(POWER_PINS[i]) == POWER_ISON[i];
    byte currentLevel = (POWER_POTPINS[i] == 0) ? 255 : (analogRead(POWER_POTPINS[i]) / 4);
    if (currentState != power_state[i]) {
      power_state[i] = currentState;
      
      onPowerPlant(POWER_TYPES[i], i, currentState, currentLevel);
    }
  }
}
