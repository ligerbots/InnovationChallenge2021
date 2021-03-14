#include <NRFLite.h>

const static uint8_t RADIO_ID = 0;
const static uint8_t DESTINATION_RADIO_ID = 0;
const static uint8_t PIN_RADIO_MOMI = 0; // MOMI connected to pin 0 of Digispark
const static uint8_t PIN_RADIO_SCK = 2; // SCK connected to pin 2 of Digispark

struct RadioPacket
{
    uint8_t FromRadioId;
    bool whistle;
};

NRFLite _radio; // Initialize radio
RadioPacket _radioData; 

void setup()
{
  
    Serial.begin(115200);

    if (!_radio.initTwoPin(RADIO_ID, PIN_RADIO_MOMI, PIN_RADIO_SCK)) // Note usage of 'initTwoPin' rather than 'init'.
    {
        Serial.println("Cannot communicate with radio");
        while (1){
        } // Wait here forever
    }

    _radioData.FromRadioId = RADIO_ID;
}

void loop()
{
    //_radioData.OnTimeMillis = millis();

    // Serial.print("Sending ");
    // Serial.print(_radioData.OnTimeMillis);
    // Serial.print(" ms");

    if (_radio.send(DESTINATION_RADIO_ID, &_radioData, sizeof(_radioData)))
    {
        Serial.println("...Success");
    }
    else
    {
        Serial.println("...Failed");
        _radioData.FailedTxCount++;
    }

    delay(10);
}

