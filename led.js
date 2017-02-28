var noble = require('noble');

// Search only for the Service UUID of the device (remove dashes)
var serviceUuids = ['20B10010E8F2537E4F6CD104768A1214'];

// Search only for the led charateristic
var characteristicUuids = ['19B10011E8F2537E4F6CD104768A1214'];

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
        var ledCharacteristic = characteristics[0];

        var bufferToSend = new Buffer(1);
        var byte = 0;

        setInterval(function() {
           bufferToSend.writeUInt8((byte ^= 1)); // you can pass this any integer, we just do this to alternate 0/1
           ledCharacteristic.write(bufferToSend, false);
        }, 1000);

      });
    });
  });
});
