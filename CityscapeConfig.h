// Bluetooth Service Constants
#define GATTSERV_CITYSCAPE "0xcc00"
#define GATTCHAR_BOARDSSTATE "0xcc01"
#define GATTCHAR_BOARDPOWERSTATES "0xcc02"
#define GATTCHAR_BOARDBUILDINGSTATES "0xcc03"
#define GATTCHAR_BOARDTRANSITSTATES "0xcc04"
#define GATTCHAR_BOARDINFO "0xcc05"

// Board Type Representation Constants
const byte POWER_BASE = 0x00;

const byte POWER_SOLAR = 0x00;
const byte POWER_WIND = 0x01;
const byte POWER_COAL = 0x02;

const byte BUILDING_BASE = 0x20;

const byte BUILDING_HOUSE_SMALL = 0x00;
const byte BUILDING_HOUSE_LARGE = 0x01;
const byte BUILDING_APARTMENT_SMALL = 0x02;
const byte BUILDING_APARTMENT_LARGE = 0x03;

const byte TRANSIT_BASE = 0x40;

const byte TRANSIT_BUS_STOP = 0x00;

// Board Pin Layout Constants
const byte NUM_POWER = 0;
const byte POWER_PINS[] = {}; //ex {13, 14, 15}
const byte POWER_POTPINS[] = {}; //ex {A0, A0, A18} to indicate no potentiometer for the first two powersources, and pin 18 for the 3rd
const byte POWER_TYPES[] = {}; //ex {POWER_SOLAR, POWER_SOLAR, POWER_WIND} to indicate the first 2 are solar and third is wind
const byte POWER_ISON[] = {}; //ex {HIGH, LOW, LOW} to indicate that when voltage is HIGH for the first, and LOW for the last two, the state is on

const byte NUM_BUILDING = 4;
const byte BUILDING_PINS[] = {25,26,27,28};

const byte NUM_TRANSIT = 0;
const byte TRANSIT_PINS[] = {};

