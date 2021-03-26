#include "arduinoFFT.h"

#define __ESP32__
#include "process.h"

TaskHandle_t sampleTask;
const int micPin = A0;
const int defaultAnalogValue = 1955;

volatile int sampler_buffer[process::num_samples];
volatile bool isSampling = true;

void sampler(void* ptr){
    while(true){
        if(isSampling){
            long start = micros();
            for(int i=0;i<process::num_samples;i++){
                long expectedtime = start + 1000000 * i / process::sampling_frequency;
                long currenttime = micros();
                if(expectedtime>currenttime){
                    delayMicroseconds(expectedtime-currenttime);
                }

                sampler_buffer[i] = analogRead(micPin);
            }
            isSampling=false;
        }
        delay(1);
    }
}

void setup(){
    Serial.begin(115200);
    xTaskCreatePinnedToCore(
        sampler, /* Function to implement the task */
        "sampler", /* Name of the task */
        10000,  /* Stack size in words */
        NULL,  /* Task input parameter */
        0,  /* Priority of the task */
        &sampleTask,  /* Task handle. */
        0); /* Core where the task should run */
    process::init();
}
void loop(){
    if(!isSampling){
        for(int i=0;i<process::num_samples;i++){
            process::real_buf[i]=sampler_buffer[i];
        }
        isSampling=true;
        process::process_fft();
    }
    delay(1);
}
