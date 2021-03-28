// python generate_data.py -n 1 -l 20 -os "sound.wav" data/A1*
// g++ main.cpp arduinoFFT/arduinoFFT.cpp -o main -Wno-#warnings && ./main && python display_spectrogram.py

#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include "arduinoFFT/arduinoFFT.h"

#define __DESKTOP__

std::ofstream outfile;
#include "process.h"

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

int main(int argc, char const *argv[]) {
    std::cout<<"start\n";
    outfile.open("spectrogram.txt");

    std::vector<int16_t> data = read_data("data.txt");

    int j=0;
    while(j+process::num_samples < data.size()){
        for(int i=0;i<process::num_samples;i++){
            process::real_buf[i] = data[j++];
        }
        process::process_fft();
    }
    outfile.close();
    return 0;
}
