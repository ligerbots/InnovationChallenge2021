// python generate_data.py -n 1 -l 20 -os "sound.wav" data/A1*
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

const float freq_cutoff_low = 2000;
const float freq_cutoff_high = 4000;
const float freq_neighbor = 500;

const int low_index = freq_cutoff_low * num_samples / sampling_frequency;
const int high_index = freq_cutoff_high * num_samples / sampling_frequency;
const int neighbor_index = freq_neighbor * num_samples / sampling_frequency;

int sample_index=0;

double real_buf[num_samples];
double imag_buf[num_samples];


arduinoFFT FFT = arduinoFFT(real_buf, imag_buf, num_samples, sampling_frequency);

unsigned long start;


void process_fft(){
    // sampling_frequency/num_samples*i = freq

    for(int i=0; i<num_samples/2; i++){
        outfile << real_buf[i] << " ";
    }


    int max = low_index;
    for(int i=low_index+1; i<=high_index; i++){
        if(real_buf[i]>real_buf[max]){
            max = i;
        }
    }



    outfile <<"\n";
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
    std::cout<<"start\n";
    outfile.open("spectrogram.txt");

    std::vector<int16_t> data = read_data("data.txt");
    std::cout<<"low "<<low_index<<" high "<<high_index<<" neighbor "<<neighbor_index<<"\n";

    for(int i=0;i<data.size();i++){
        process_sample(data[i]);
    }
    outfile.close();
    return 0;
}
