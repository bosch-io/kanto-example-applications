#include <mqtt/client.h>
#include <iostream>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;
int main() 
{
    string ip = "localhost:1883";
    
    string id = "temperature_sensor";
    
    mqtt::client client(ip, id);

    client.connect();

    client.subscribe("edge/thing/response");
    mqtt::message_ptr deviceDataRequestMessagePointer = mqtt::make_message("edge/thing/request", "");

    client.publish(deviceDataRequestMessagePointer);

    auto deviceDataResponseMessagePointer = client.consume_message();

    if (!deviceDataResponseMessagePointer) 
    {
        cout << "No device data available" << endl;
        return 1;
    }

    client.unsubscribe("edge/thing/response");
    cout << "Device data received " << deviceDataResponseMessagePointer->get_payload_str() << endl;

    json deviceData = json::parse(deviceDataResponseMessagePointer->get_payload_str());
   
    string tenantId = deviceData["tenantId"];
    string gatewayDeviceId = deviceData["deviceId"];
    string airConditionerDeviceId = gatewayDeviceId + ":AC";

    string deviceIdInNamespaceNotation = airConditionerDeviceId;
    int namespaceSeparatorIndex = deviceIdInNamespaceNotation.find(":");
    deviceIdInNamespaceNotation.replace(namespaceSeparatorIndex, 1, "/");

    string telemetryTopic = "t/" + tenantId + "/" + airConditionerDeviceId;
    string userResponse;

    json telemetryMessagePayload = {
        {"topic", deviceIdInNamespaceNotation + "/things/twin/commands/modify"},
        {"headers", {
            {"content-type", "application/json"},
            {"response-required", false}
        }},
        {"path", "/features/temperature"},
        {"value", {{
            "properties", {{
                "currentTemperature", 0
            }}
        }}}
    };

    cout << "Payload Template ::" << telemetryMessagePayload << endl;
    do 
    {
        cout << "What would you like to do?\nType send to send message or quit to exit." << endl;
        cin >> userResponse;
        if (userResponse == "send") 
        {
            srand(time(nullptr));
            int currentTemperature = 20 + (rand() % 10);
            telemetryMessagePayload["value"]["properties"]["currentTemperature"] = currentTemperature;

            cout << "Sending message:: " << endl;
            cout << "topic:: " << telemetryTopic << " message " << telemetryMessagePayload << endl;
            mqtt::message_ptr telemetryMessage = mqtt::make_message(telemetryTopic, telemetryMessagePayload.dump());
            client.publish(telemetryMessage);
        }

    } while (userResponse != "quit");

    cout << "Bye Bye" << endl;
    return 0;
}