#include <CurieBLE.h>

BLEPeripheral blePeripheral;
BLEService customService("19B10000-E8F2-537E-4F6C-D104768A1216");

BLEUnsignedCharCharacteristic sensorCharacteristic("5667f3b1-d6a2-4fb2-a917-4bee580a9c84", BLERead | BLENotify);

void setup() {
  Serial.begin(9600);

  blePeripheral.setLocalName("Curie");
  blePeripheral.setAdvertisedServiceUuid(customService.uuid());

  blePeripheral.addAttribute(customService);
  blePeripheral.addAttribute(sensorCharacteristic);

  sensorCharacteristic.setValue(0);

  blePeripheral.begin();
}

void loop() {
  BLECentral central = blePeripheral.central();

  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());

    while (central.connected()) {
      int sensorValue = analogRead(0);
      sensorCharacteristic.setValue(sensorValue);
    }
  }
}
