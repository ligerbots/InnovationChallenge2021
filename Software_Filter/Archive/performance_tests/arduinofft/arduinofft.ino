
#include "arduinoFFT.h"

arduinoFFT FFT = arduinoFFT();

const uint16_t samples = 512; 
const uint16_t iters = 100; 

double initValues[samples];

double vReal[samples];
double vImag[samples];

unsigned long start;

void setup() {
    Serial.begin(115200);
    while(!Serial);
    Serial.println("Ready");
    for(int i=0;i<samples;i++) {
        initValues[i] = random(-100,100);
    }
}

void loop() {
    start = micros();
    for (int i = 0; i < iters; i++) {
        for (uint16_t i = 0; i < samples; i++) {
            vReal[i] = initValues[i];
            vImag[i] = 0.0;
        }
    
        FFT.Windowing(vReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
        FFT.Compute(vReal, vImag, samples, FFT_FORWARD);
        
        FFT.ComplexToMagnitude(vReal, vImag, samples); /* Compute magnitudes */
    }
    float timePerFFT = (micros() - start) / 1000000.0/ iters;
    Serial.print("FFT/sec: ");
    Serial.print((1.0 / timePerFFT));
    Serial.println();
    Serial.print("samples/sec: ");
    Serial.print((1.0 / timePerFFT * samples));
    Serial.println();   
    
    
}
