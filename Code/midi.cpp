
/*

Backend code for a MIDI step for the IVC Cameras

In arguments:
    Command         - Note off, Note on, Aftertouch, Continuous controller, Patch change, Channel pressure, Pitch bend, System message
    Channel         - [0, 15]
    Parameter one   - [0, 127], alt. rewrite to accept 0 255 and rescale below
    Parameter two   - [0, 127]

*/

#include <string>

#define     CHANNEL_START           0x00
#define     CHANNEL_END             0x0F
#define     DATA_BYTES_START        0x00
#define     DATA_BYTES_END          0x7F

struct MIDI_MessageType
{
    std::string name;
    unsigned char command;
    unsigned short params;
};

enum MIDI_MessageEnums
{
    NOTE_OFF = 0,
    NOTE_ON,
    AFTERTOUCH,
    CONTINUOUS_CONTROLLER,
    PATCH_CHANGE,
    CHANNEL_PRESSURE,
    PITCH_BEND,
    SYSTEM_MESSAGE
};

enum MIDI_ErrorCodes
{
    UNKNOWN_COMMAND = 0,
    CHANNEL_ERROR,
    PARAMETER_ONE_ERROR,
    PARAMETER_TWO_ERROR,
};

int main()
{
    int index;
    MIDI_MessageType msgTypes[] = {
        {"Note off",                0x80,   2},
        {"Note on",                 0x90,   2},
        {"Aftertouch",              0xA0,   2},
        {"Continuous controller",   0xB0,   2},
        {"Patch change",            0xC0,   2},
        {"Channel pressure",        0xD0,   1},
        {"Pitch bend",              0xE0,   2},
        {"System message",          0xF0,   0}};

    // Get the three inputs to the step
    unsigned char command, channel;
    unsigned char[2] param;

    // CHECK INPUTS
    if (command < 0 || command > 7)
        return UNKNOWN_COMMAND;

    if (channel < CHANNEL_START || channel > CHANNEL_END)
        return CHANNEL_ERROR;

    for (index = 0; index < 2 ; index++)
    {
        if (param[index] < DATA_BYTES_START || param[index] > DATA_BYTES_END)
            return PARAMETER_ONE_ERROR + index;
    }

    // Print a log message
    put_console(200, "Inputs ok, creating %s", msgTypes[command].name);

    // Create the MIDI_Message
    unsigned char * outData = new unsigned char[msgTypes[command].params+1];
    outData[0] = msgTypes[command].command + channel;
    for (index = 0; index < msgTypes[command].params; index++)
    {
        outData[1+index] = param[index];
    }

    // Send the MIDI message over serial
    serial.write(outData, 3);
}
