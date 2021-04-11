#define __ESP32__

//#define DEBUGSAMPLE
#define DEBUGTIMING
void setActivationStatus(bool isActivated);

#include "process.h"
#include "esp32-hal-adc.h"
/*
WIRING:
A0/GPIO36 to MIC OUT

GPIO34 to RXD
GPIO35 to TXD
*/

TaskHandle_t sampleTask;
const int micPin = A0;

const int radioRxPin = 10;
const int radioTxPin = 9;

const int buttonPin = 23;

#ifdef DEBUGSAMPLE
  const int buf_length = process::sampling_frequency*3;

  uint16_t debug_buf[buf_length];
  int buff_index = 0;
  bool was_pressed = false;
#endif

volatile int sampler_buffer[process::num_samples];
volatile int sampler_index = 0;



const int timer_count=1000000/process::sampling_frequency;
hw_timer_t* timer = NULL;

void sampler(void* ptr) {
  long start = millis();
  long cnt=0;

  timer = timerBegin(3, 80, true); // 1/(80MHZ/80) = 1us
  timerWrite(timer, 0);
  timerStart(timer);
  
  while (true) {
    timerRead(timer); // for some reason timerRead returns wrong value without being called first
    if(timerRead(timer)>timer_count){
        timerWrite(timer, timerRead(timer)-timer_count);
        cnt++;

        if(sampler_index<process::num_samples){
          sampler_buffer[sampler_index++]=analogRead(micPin);
        }
    }

    /*
    if(start+1000<millis()){
      Serial.print("freq: ");
      Serial.print(cnt);
      Serial.print(" expected freq: ");
      Serial.println(1000000/timer_count);
      cnt = 0;
      start = millis();  

    }*/
  }
}



bool previouslyActivated = false;
void setActivationStatus(bool isActivated){
    if(previouslyActivated!=isActivated){
        Serial2.write(isActivated?255:0);
        previouslyActivated=isActivated;
    }
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, radioTxPin, radioRxPin);
  
  xTaskCreatePinnedToCore(
    sampler, /* Function to implement the task */
    "sampler", /* Name of the task */
    10000,  /* Stack size in words */
    NULL,  /* Task input parameter */
    0,  /* Priority of the task */
    &sampleTask,  /* Task handle. */
    0); /* Core where the task should run */
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(micPin, INPUT);
  pinMode(buttonPin, INPUT);

  process::init();
  disableCore0WDT();
  disableCore1WDT();
  
}
#ifdef DEBUGTIMING
  long last_process = millis();
  long frequency_start = millis();
  long frequency_count = 0;

#endif

long start;


void loop() {
  if(sampler_index>=process::num_samples){      
    #ifdef DEBUGTIMING
      long start = millis();
    #endif

    #ifdef DEBUGSAMPLE
      bool isPressed = digitalRead(buttonPin);
      if((!was_pressed)&&isPressed){
        digitalWrite(LED_BUILTIN, true);
        start=millis();
      }
      if(isPressed){
        for (int i = 0; i < process::num_samples; i++) {
          if(buff_index>=buf_length){
            digitalWrite(LED_BUILTIN, false);
            break;
          }
          debug_buf[buff_index++] = sampler_buffer[i];
        }
        sampler_index=0;

      }
      if(was_pressed&&(!isPressed)){
        digitalWrite(LED_BUILTIN, true);
        Serial.print("BUFFSTART");
        Serial.write((uint8_t*)debug_buf, sizeof(debug_buf[0])*buff_index);
        Serial.println("BUFFEND");
        digitalWrite(LED_BUILTIN, false);
        buff_index=0;
      }
      was_pressed=isPressed;
          
    #else
      for (int i = 0; i < process::num_samples; i++) {
        process::real_buf[i] = sampler_buffer[i];
      }
      
      sampler_index=0;
      process::process_fft();
    #endif

    #ifdef DEBUGTIMING
      long process = millis() - start;
      long idle = start - last_process;
      last_process = millis();
      
      frequency_count++;

      if(frequency_start+1000<millis()){
        Serial.print("process: ");
        Serial.print(process);
        Serial.print(" idle: ");
        Serial.print(idle);
        Serial.print(" duty cycle: ");
        Serial.print(process*100/(process+idle));
        Serial.println("%");
      
        Serial.print("freq: ");
        Serial.print(frequency_count);
        Serial.print(" expected freq: ");
        Serial.println(process::sampling_frequency/process::num_samples);
        frequency_count = 0;
        frequency_start = millis();  
      }
      
    #endif

    
  }
}
