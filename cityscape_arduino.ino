void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

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

