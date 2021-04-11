const int micPin = A0;
const int buttonPin = 23;

const int sampling_frequency = 16000;

const int buf_length = sampling_frequency*3;


uint16_t buf[buf_length];

int buff_index = 0;
long start = -1;
void setup(){
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(buttonPin, INPUT);
    pinMode(micPin, INPUT);
}
void loop(){
    bool isPressed = digitalRead(buttonPin);

    if(isPressed){
        if(start<0){
            start = millis();
            buff_index = 0;

            digitalWrite(LED_BUILTIN, true);
        }
    }
    if(start >=0){
        if(buff_index<buf_length){
            int val = analogRead(micPin);
            buf[buff_index++] = val;

            long expectedtime = start + 1000 * buff_index / sampling_frequency;
            long currenttime = millis();
            if(expectedtime>currenttime){
              delayMicroseconds((expectedtime - currenttime)*1000);
            }
        }else{
            digitalWrite(LED_BUILTIN, false);
        }
        if(!isPressed){
            digitalWrite(LED_BUILTIN, true);
            Serial.print("BUFFSTART");
            Serial.write((uint8_t*)buf, sizeof(buf[0])*buff_index);
            Serial.println("BUFFEND");
            
            digitalWrite(LED_BUILTIN, false);
            start=-1;
        }
    }
    
}
