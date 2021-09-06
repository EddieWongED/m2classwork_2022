#include <Arduino.h>
#include <SPI.h>
#include "../lib/arduino-mcp2515/mcp2515.h"

struct can_frame canMsg;
MCP2515 mcp2515(10);

void setup()
{
    Serial.begin(115200);
    mcp2515.reset();
    mcp2515.setBitrate(CAN_1000KBPS, MCP_8MHZ);
    mcp2515.setNormalMode();
}

void loop()
{
    if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK)
    {
        Serial.print("ID: ");
        Serial.println(canMsg.can_id, HEX);
        Serial.print("DLC: ");
        Serial.println(canMsg.can_dlc, HEX);

        int pos = canMsg.data[0] << 8 | canMsg.data[1];
        int vel = canMsg.data[2] << 8 | canMsg.data[3];
        int torqueCurrent = canMsg.data[4] << 8 | canMsg.data[5];
        int temperture = canMsg.data[6];

        Serial.print("Position: ");
        Serial.println(pos, DEC);
        Serial.print("Velocity: ");
        Serial.println(vel, DEC);
        Serial.print("Torque Current: ");
        Serial.println(torqueCurrent, DEC);
        Serial.print("Temperture: ");
        Serial.println(temperture, DEC);
    }
}