# Introduction

This sample shows simple console application that gets the gateway device data from the Kanto connector and sends telemetry data for the temoerature rteadings of a simualted Air-Conditioning device.

## Prerequisites

### MQTT Broker configuration

The application does not use any authentiation when connecting to the mqtt broker. Because of this, the local mqtt broker has to be configured to allow anonymous access.

For example, using mosquitto on debian derivative, open the */etc/mosquitto/mosquitoo.conf* file and add the following line

```bash
allow_anonymous true
```

After saving and closing, restart the mosquitto.

### Required libraries

This example application uses two additional libraries

* Paho mqtt - https://github.com/eclipse/paho.mqtt.cpp
* Niels Lohman's JSON library - https://github.com/nlohmann/json

The JSON processing library is already included in the sources - the json.hpp file.

The Paho MQTT has to be built and installed. Please follow the steps described in https://github.com/eclipse/paho.mqtt.cpp?tab=readme-ov-file#building-from-source

## Building

After getting the libraries available, all you have to do is to open a terminal, go to the root folder and execute

```bash
g++ main.cpp -std=c++11 -lpaho-mqttpp3 -lpaho-mqtt3as -o temperature-sensor
```

## Using the application

Start the application with

```bash
./temperature-sensor
```

After succesfully retrieving the device data one should see a debug message in the terminal, like

```bash
Device data received {"deviceId":"<NAMESPACE_ID>:<DEVICE_ID>","tenantId":"<TENANT_ID>"}
```

Then a prompt will appear

```
What would you like to do?
Type send to send message or quit to exit.
```

It is more or less explanatory enough.

If you enter *quit* the application will exit.

If you enter *send* then the application will generate a random value between 20 and 30 and set it as current temperature value in the temperature feature in a Thing named *<FULL_GATEWAY_DEVICE_ID>:AC*
Where *<FULL_GATEWAY_DEVICE_ID>* has the form *<NAMESPACE_ID>:<DEVICE_ID>*

The generated message should look like this

```json
"topic":"<NAMESPACE_ID>/<DEVICE_ID>:AC/things/twin/commands/modify",
{
    "headers":{
        "content-type":"application/json",
        "response-required":false
    },
    "path":"/features/temperature",
    "value":{
        "properties":{
            "currentTemperature":27
        }
    }
}
```