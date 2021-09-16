#include <Arduino.h>
#include <SPI.h>
#include <mcp2515.h>

struct can_frame canMsg200;
struct can_frame canMsg1FF;
MCP2515 mcp2515(10);

void setup()
{
    canMsg200.can_id = 0x200;
    canMsg200.can_dlc = 8;
    canMsg200.data[0] = 0x00;
    canMsg200.data[1] = 0x00;
    canMsg200.data[2] = 0x00;
    canMsg200.data[3] = 0x00;
    canMsg200.data[4] = 0x00;
    canMsg200.data[5] = 0x00;
    canMsg200.data[6] = 0x00;
    canMsg200.data[7] = 0x00;

    canMsg1FF.can_id = 0x1FF;
    canMsg1FF.can_dlc = 8;
    canMsg1FF.data[0] = 0x00;
    canMsg1FF.data[1] = 0xFF;
    canMsg1FF.data[2] = 0x00;
    canMsg1FF.data[3] = 0x00;
    canMsg1FF.data[4] = 0x00;
    canMsg1FF.data[5] = 0x00;
    canMsg1FF.data[6] = 0x00;
    canMsg1FF.data[7] = 0x00;

    Serial.begin(115200);

    mcp2515.reset();
    mcp2515.setBitrate(CAN_1000KBPS, MCP_8MHZ);
    mcp2515.setNormalMode();
}

void loop()
{
    mcp2515.sendMessage(&canMsg200);
    mcp2515.sendMessage(&canMsg1FF);

    delay(1);
}