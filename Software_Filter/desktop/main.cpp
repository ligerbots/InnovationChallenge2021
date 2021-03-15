// g++ main.cpp arduinoFFT/arduinoFFT.cpp -o main -Wno-#warnings && ./main && python display_spectrogram.py

#import "arduinoFFT/arduinoFFT.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

std::ofstream outfile;

std::vector<int16_t> read_data(const char* file_name){
    std::ifstream file(file_name);
    std::vector<int16_t> pts;

    while(true){
        int16_t x;
        file>>x;
        if(file.eof()) break;
        pts.push_back(x);
    }

    return pts;
}

const uint16_t sampling_frequency = 16000;
const uint16_t num_samples = 512;
const uint16_t iters = 100;

int sample_index=0;

double real_buf[num_samples];
double imag_buf[num_samples];


arduinoFFT FFT = arduinoFFT(real_buf, imag_buf, num_samples, sampling_frequency);

unsigned long start;


void process_fft(){
    for(int i=0; i<num_samples/2; i++){
        outfile << real_buf[i] << " ";
    }
    outfile<<"\n";
}

void process_sample(int16_t sample){
    real_buf[sample_index++] = sample;
    if(sample_index >= num_samples){
        sample_index=0;

        FFT.Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
        FFT.Compute(FFT_FORWARD);
        FFT.ComplexToMagnitude();

        for(int i=0;i<num_samples;i++){
            imag_buf[i]=0;
        }
        
        process_fft();
    }
}

int main(int argc, char const *argv[]) {
    outfile.open("spectrogram.txt");

    std::vector<int16_t> data = read_data("data.txt");

    for(int i=0;i<data.size();i++){
        process_sample(data[i]);
    }
    outfile.close();
    return 0;
}
