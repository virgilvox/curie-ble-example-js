#include <CurieBLE.h>
#include "CurieIMU.h"

BLEPeripheral blePeripheral;
BLEService customService("19B10000-E8F2-537E-4F6C-D104768A1216");

BLEUnsignedCharCharacteristic CharacteristicX("4227f3b1-d6a2-4fb2-a916-3bee580a9c84", BLERead | BLENotify);
BLEUnsignedCharCharacteristic CharacteristicY("5b974f46-6f48-43ee-9a55-4fb009867603", BLERead | BLENotify);
BLEUnsignedCharCharacteristic CharacteristicZ("09a64f10-32b3-497a-93c2-c914f46eba22", BLERead | BLENotify);

unsigned long loopTime = 0;
unsigned long interruptsTime = 0;

void setup() {
  Serial.begin(9600);

  Serial.println("Initializing IMU device...");

  CurieIMU.begin();
  CurieIMU.setAccelerometerRange(4);
  CurieIMU.attachInterrupt(eventCallback);

  CurieIMU.setDetectionThreshold(CURIE_IMU_MOTION, 20);
  CurieIMU.setDetectionDuration(CURIE_IMU_MOTION, 10);
  CurieIMU.interrupts(CURIE_IMU_MOTION);

  blePeripheral.setLocalName("CurieIMU");
  blePeripheral.setAdvertisedServiceUuid(customService.uuid());

  blePeripheral.addAttribute(customService);
  blePeripheral.addAttribute(CharacteristicX);
  blePeripheral.addAttribute(CharacteristicY);
  blePeripheral.addAttribute(CharacteristicZ);

  CharacteristicX.setValue(0);
  CharacteristicY.setValue(0);
  CharacteristicZ.setValue(0);

  blePeripheral.begin();
}

void loop() {
  float ax, ay, az;
  BLECentral central = blePeripheral.central();

  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());

    while (central.connected()) {
      loopTime = millis();
      if(abs(loopTime - interruptsTime) < 1000 ) {
        CurieIMU.readAccelerometerScaled(ax, ay, az);

        CharacteristicX.setValue(ax*100);
        CharacteristicY.setValue(ay*100);
        CharacteristicZ.setValue(az*100);
      }
    }
  }
}

static void eventCallback(void){
  if (CurieIMU.getInterruptStatus(CURIE_IMU_MOTION)) {
    Serial.println("motion detected");
    interruptsTime = millis();
  }
}
