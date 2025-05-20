#include <EEPROM.h>

#define uint unsigned int
#define uchar unsigned char
#define ulong unsigned long

#define EEPROM_NAME_SIZE 72
#define NAME_SIZE EEPROM_NAME_SIZE / 2
#define PORT_COUNT 7
#define EEPROM_MAGIC_VALUE 122
#define EEPROM_WRITES

const uint8_t PORTS[PORT_COUNT] = {6, 3, 4, 5, 7, 9, 8};

//char NAMES[PORT_COUNT][EEPROM_NAME_SIZE] = {"Port 1", "Port 2", "Port 3", "Port 4", "Port 5", "Port 6", "Port 7"};
uint state = 0;

// TODO: Rewrite this absolute dogshit
String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++){
    if (data.charAt(i) == separator || i == maxIndex) {
        found++;
        strIndex[0] = strIndex[1] + 1;
        strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

uint bit_on(uint num, uint bit) {
  return num | (1 << bit);
}

uint bit_off(uint num, uint bit) {
  return num & ~(1 << bit);
}

bool is_bit_on(uint num, uint bit) {
  return (num & (1 << bit)) != 0;
}

ulong lastPing;

void fwinfo() {
  Serial.print("AstroUSB-Nano v0.0.1-beta ");
  Serial.print(__DATE__);
  Serial.print(" ");
  Serial.println(__TIME__);
  Serial.flush();
}

void setup() {
  state = EEPROM.read(0);

  for (int i = 0; i < PORT_COUNT; i++) {
    digitalWrite(PORTS[i], is_bit_on(state, i) == 0 ? HIGH : LOW);
    pinMode(PORTS[i], OUTPUT);
  }

  Serial.begin(115200);
  fwinfo();

  lastPing = millis();
}

void process(String data) {
  data.trim();
  String command = getValue(data, ';', 0);
  if (command == "") {
    Serial.println("INVALID_COMMAND");
    Serial.flush();
    return;
  }
  if (command == "FWINFO") {
    fwinfo();
    return;
  } else if (command == "PING") {
    Serial.println("PONG");
    Serial.flush();
    return;
  } else if (command == "ACK") {
    Serial.println("OK");
    Serial.flush();
    return;
  }

  // Zahtevan je 2. argument
  String t = getValue(data, ';', 1);
  uint target = t.toInt();
  if (t.length() == 0 || !isDigit(t.charAt(0)) || target >= PORT_COUNT) {
    Serial.println("INVALID_PORT");
    Serial.flush();
    return;
  }

  if (command == "ON") {
    state = bit_on(state, target);
#ifdef EEPROM_WRITES
    EEPROM.write(0, state);
#endif
    digitalWrite(PORTS[target], LOW);
    Serial.println("OK");
  } else if (command == "OFF") {
    state = bit_off(state, target);
#ifdef EEPROM_WRITES
    EEPROM.write(0, state);
#endif
    digitalWrite(PORTS[target], HIGH);
    Serial.println("OK");
  } else if (command == "STATEGET") {
    Serial.println(is_bit_on(state, target));
  }

  Serial.flush();
}

//String data;

void loop() {
  if (!Serial) {
    lastPing = millis();
    Serial.println("PING");
    Serial.flush();
    delay(2000);
  } else {
    String data = Serial.readStringUntil('\n');
    if (data.length() > 0) {
      process(data);
      //Serial.print("RX: ");
      //Serial.println(data);
    }
  }
}
