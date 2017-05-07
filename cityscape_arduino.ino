void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}

//Bluetooth Lifecycle
#define CITYSCAPESERVICE 0xCC00

int32_t csServiceId;
int32_t csBoardStateCharId;
int32_t csBoardUpdateCharId;

int initializeBluetoothService() {
  /* Change the device name to make it easier to find */
  Serial.println(F("Setting device name to 'Cityscape Board': "));

  if (! ble.sendCommandCheckOK(F("AT+GAPDEVNAME=Cityscape Board")) ) {
    error(F("Could not set device name?"));
    return 1;
  }

  /* Add the Cityscape Service definition */
  /* Service ID should be 1 */
  Serial.println(F("Adding the Cityscape Service definition (UUID = CITYSCAPESERVICE): "));
  success = ble.sendCommandWithIntReply( F("AT+GATTADDSERVICE=UUID=CITYSCAPESERVICE"), &csServiceId);
  if (! success) {
    error(F("Could not add Cityscape service"));
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

