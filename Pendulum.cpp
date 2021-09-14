#include <Arduino.h>
#include <SPI.h>
#include "../lib/arduino-mcp2515/mcp2515.h"
#include "../lib/Encoder/Encoder.h"

const int set_pos = 0;
const int set_pos_max = 50;
const int set_pos_min = -50;

struct can_frame canMsg200;
struct can_frame canMsg1FF;
struct can_frame canMsgRead;

const int ENCODER_INPUT_PIN1 = 2;
const int ENCODER_INPUT_PIN2 = 3;
Encoder myEnc(ENCODER_INPUT_PIN1, ENCODER_INPUT_PIN2);

MCP2515 mcp2515(10);

const int Kp = 12;
const int Ki = 0;
const int Kd = 500;

unsigned long previousTime = millis();
double elapsedTime;
double lastError;
double cumError = 0;
double input, output, setPoint;

boolean stop = false;

int computeCurrent(int new_pos)
{
    unsigned long currentTime = millis();
    double elapsedTime = (double)(currentTime - previousTime);

    double error = set_pos - new_pos;
    cumError += error * elapsedTime;
    double rateError = (error - lastError) / elapsedTime;

    Serial.print("  error: ");
    Serial.print(error);
    Serial.print("  cumError: ");
    Serial.print(cumError);
    Serial.print("  rateError: ");
    Serial.print(rateError);

    int current = Kp * error + Kd * rateError;

    lastError = error;
    previousTime = currentTime;

    return current * -1;
}

void setMotorCurrent(int current)
{
    int higherByte = current >> 8;
    int lowerByte = higherByte << 8 ^ current;
    canMsg200.data[2] = higherByte;
    canMsg200.data[3] = lowerByte;
    mcp2515.sendMessage(&canMsg200);
    mcp2515.sendMessage(&canMsg1FF);
}

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
    canMsg1FF.data[1] = 0x00;
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
    if (stop == false)
    {
        int new_pos = myEnc.read();
        Serial.print("new_pos: ");
        Serial.print(new_pos);

        int current = computeCurrent(new_pos);
        Serial.print("  current: ");
        Serial.print(current);

        int motor_vel = 0;
        if (mcp2515.readMessage(&canMsgRead) == MCP2515::ERROR_OK)
        {
            motor_vel = canMsgRead.data[2] << 8 | canMsgRead.data[3];
            Serial.print("  motor_vel: ");
            Serial.print(motor_vel, DEC);
        };

        if (new_pos >= 500 || new_pos <= -500)
        {
            stop = true;
            Serial.println("\nTerminated Due to Bar Position");
        }
        else if (current >= 12000 || current <= -12000)
        {
            stop = true;
            Serial.println("\nTerminated Due to Too High Current");
        }
        else if (motor_vel >= 2000 || motor_vel <= -2000)
        {
            stop = true;
            Serial.println("\nTerminated Due to Too High Speed");
        }
        else
        {
            setMotorCurrent(current);
            Serial.println();
        };
    }
    else
    {
        setMotorCurrent(0);
    }
}