#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const uint64_t pipeOut = 0xE9E8F0F0E1LL;
RF24 radio(9, 7);

struct Signal {
    byte throttle;
    byte pitch;
    byte roll;
    byte yaw;
};

Signal data;

void ResetData() 
{
    data.throttle = 100;
    data.pitch = 127;
    data.roll = 127;
    data.yaw = 127;
}

void setup()
{
    radio.begin();
    radio.openWritingPipe(pipeOut);
    radio.stopListening();
    ResetData();

    // Initialize serial communication for debugging
    Serial.begin(9600);
}

int mapJoystickValues(int val, int lower, int middle, int upper, bool reverse)
{
    val = constrain(val, lower, upper);
    if ( val < middle )
        val = map(val, lower, middle, -128, 0);
    else
        val = map(val, middle, upper, 0, 127);
    return ( reverse ? -val : val );
}

void loop()
{
    data.throttle = mapJoystickValues( analogRead(A0), 524, 524, 1015, false );
    data.roll = mapJoystickValues( analogRead(A1), 12, 524, 1020, true );
    data.pitch = mapJoystickValues( analogRead(A2), 12, 524, 1020, false );
    data.yaw = mapJoystickValues( analogRead(A3), 12, 524, 1020, true );

    // Print each direction to the terminal
    Serial.print("Throttle: "); Serial.println(data.throttle);
    Serial.print("Roll: "); Serial.println(data.roll);
    Serial.print("Pitch: "); Serial.println(data.pitch);
    Serial.print("Yaw: "); Serial.println(data.yaw);

    radio.write(&data, size of(Signal));
} 
