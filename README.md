# esphome-treadmill-f15

![GitHub actions](https://github.com/syssi/esphome-treadmill-f15/actions/workflows/ci.yaml/badge.svg)
![GitHub stars](https://img.shields.io/github/stars/syssi/esphome-treadmill-f15)
![GitHub forks](https://img.shields.io/github/forks/syssi/esphome-treadmill-f15)
![GitHub watchers](https://img.shields.io/github/watchers/syssi/esphome-treadmill-f15)
[!["Buy Me A Coffee"](https://img.shields.io/badge/buy%20me%20a%20coffee-donate-yellow.svg)](https://www.buymeacoffee.com/syssi)

ESPHome component to monitor and control a Sportstech Treadmill F15 via BLE

## Supported devices

* Sportstech Treadmill F15
* Sportstech Treadmill F31

## Requirements

* [ESPHome 2024.6.0 or higher](https://github.com/esphome/esphome/releases).
* Generic ESP32 board

## Installation

You can install this component with [ESPHome external components feature](https://esphome.io/components/external_components.html) like this:
```yaml
external_components:
  - source: github://syssi/esphome-treadmill-f15@main
```

or just use the `esp32-ble-example.yaml` as proof of concept:

```bash
# Install esphome
pip3 install esphome

# Clone this external component
git clone https://github.com/syssi/esphome-treadmill-f15.git
cd esphome-treadmill-f15

# Create a secrets.yaml containing some setup specific secrets
cat > secrets.yaml <<EOF
wifi_ssid: MY_WIFI_SSID
wifi_password: MY_WIFI_PASSWORD

mqtt_host: MY_MQTT_HOST
mqtt_username: MY_MQTT_USERNAME
mqtt_password: MY_MQTT_PASSWORD
EOF

# Validate the configuration, create a binary, upload it, and start logs
esphome run esp32-ble-example.yaml

```

## Example response all sensors enabled

```
[D][treadmill_f15:113]: Notification received (handle 0x12): 02.51.03.0A.00.70.00.1A.00.0A.00.00.00.00.00.38.03 (17)
[D][sensor:124]: 'treadmill-f15 speed': Sending state 1.00000 km/h with 2 decimals of accuracy
[D][sensor:124]: 'treadmill-f15 distance': Sending state 0.02600 km with 4 decimals of accuracy
[D][sensor:124]: 'treadmill-f15 calories': Sending state 10.00000  with 0 decimals of accuracy
[D][sensor:124]: 'treadmill-f15 elapsed time': Sending state 112.00000 s with 0 decimals of accuracy
[D][text_sensor:067]: 'treadmill-f15 operation mode': Sending state 'Running'
[D][binary_sensor:036]: 'treadmill-f15 running': Sending state ON
```

## Protocol

See [protocol.md](docs/protocol.md) for detailed protocol analysis and reverse engineering notes.

## Known issues

None.

## Debugging

If this component doesn't work out of the box for your device please update your configuration to enable the debug output of the BLE component and increase the log level to the see outgoing and incoming BLE traffic:

```yaml
logger:
  level: DEBUG
  logs:
    esp32_ble_client: INFO

treadmill_f15:
  - ble_client_id: client0
    id: treadmill0
    update_interval: 1s
```

## References

* https://github.com/tyge68/fitshow-treadmill/blob/master/src/services/BTService.js
