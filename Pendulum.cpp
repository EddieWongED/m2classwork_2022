#include <SPI.h>
#include <mcp2515.h>
#include <Arduino.h>
#include <Encoder.h>

struct can_frame canMsgRead;
struct can_frame canMsg200;
MCP2515 mcp2515(10);

const int ENCODER_INPUT_PIN1 = 2;
const int ENCODER_INPUT_PIN2 = 3;
Encoder enc(ENCODER_INPUT_PIN1, ENCODER_INPUT_PIN2);

const float kP = 4;
const float kD = 30;
const int pos_threshold = 70;
int prev_pos = 0;

void setup()
{
    Serial.begin(115200);
    SPI.begin();

    mcp2515.reset();
    mcp2515.setBitrate(CAN_1000KBPS, MCP_8MHZ);
    mcp2515.setNormalMode();

    canMsg200.can_id = 0x200;
    canMsg200.can_dlc = 8;
}

void loop()
{
    if (mcp2515.readMessage(&canMsgRead) == MCP2515::ERROR_OK)
    {
        int16_t current_vel = (int16_t)canMsgRead.data[2] << 8 | canMsgRead.data[3];
        int16_t target_vel;

        int new_pos = enc.read();
        new_pos = new_pos % 4096;
        if (new_pos < -2048)
        {
            new_pos += 4096;
        }
        if (new_pos > 2047)
        {
            new_pos -= 4096;
        };
        if (abs(new_pos) < pos_threshold)
        {
            new_pos = 0;
        };

        if (abs(new_pos) > 400)
        {
            target_vel = 0;
        }
        else
        {
            target_vel = new_pos * kP + (new_pos - prev_pos) * kD;
        }
        prev_pos = new_pos;

        if (target_vel > 2000)
        {
            target_vel = 2000;
        }
        if (target_vel < -2000)
        {
            target_vel = -2000;
        }

        int16_t current_out = (target_vel - current_vel) * 4;
        Serial.print("new_pos: ");
        Serial.print(new_pos);
        Serial.print("\tcurrent_vel: ");
        Serial.print(current_vel);

        int higherByte = current_out >> 8;
        int lowerByte = higherByte << 8 ^ current_out;
        canMsg200.data[0] = higherByte;
        canMsg200.data[1] = lowerByte;
        canMsg200.data[2] = higherByte;
        canMsg200.data[3] = lowerByte;
        canMsg200.data[4] = higherByte;
        canMsg200.data[5] = lowerByte;
        canMsg200.data[6] = higherByte;
        canMsg200.data[7] = lowerByte;

        mcp2515.sendMessage(&canMsg200);
    }

    delayMicroseconds(500);
}