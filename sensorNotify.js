var noble = require('noble');

// Search only for the Service UUID of the device (remove dashes)
var serviceUuids = ['19B10000E8F2537E4F6CD104768A1216'];

// Search only for sensor charateristic
var characteristicUuids = ['5667f3b1d6a24fb2a9174bee580a9c84'];

// start scanning when bluetooth is powered on
noble.on('stateChange', function(state) {
  if (state === 'poweredOn') {
    noble.startScanning(serviceUuids);
  } else {
    noble.stopScanning();
  }
});

// Search for BLE peripherals
noble.on('discover', function(peripheral) {
  peripheral.connect(function(error) {
    console.log('connected to peripheral: ' + peripheral.uuid);
    // Only discover the service we specified above
    peripheral.discoverServices(serviceUuids, function(error, services) {
      var service = services[0];
      console.log('discovered service');

      service.discoverCharacteristics(characteristicUuids, function(error, characteristics) {
        console.log('discovered characteristics');
        // Assign Characteristic
        var sensorCharacteristic = characteristics[0];

        sensorCharacteristic.subscribe(function(error) {
          if(error) console.log(error);
        });

        // Handle data events for characteristic
        sensorCharacteristic.on('data', function(data, isNotification) {
          console.log('Sensor Value', data.readUInt8(0))
        });

      });
    });
  });
});
